/*
    InterMud 3 client
        by Drizzt@Tmi-2
// Leto july 6th 95 - Tried to add some security :)
*/
                     
#include <i3.h>
 
#define I3_DEBUG_OUT 0x01
#define I3_DEBUG_IN  0x02
#define I3_DEBUG_LST 0x04
 
nosave object router_socket, oob_socket, debugger;
nosave mapping services;
nosave int debug_level;
int router_password, mudlist_id, chanlist_id;
mapping mudlist, chanlist;
 
// Prototypes :)
void create();
void check_router();

void set_debugging(int level, object person){

// Prevent people from snooping private stuff
   if(!adminp(person)) return;

    debugger = person;
    debug_level = level;
}
 
mixed *query_debugging(){
    return ({ debugger, debug_level });
}
 
private void debug_stuff(mixed message){

  if(debugger)
    message("I3",sprintf("%c[1;32m[I3]: %O%c[0m\n", 27, message, 27), debugger);    
}
 
string query_network_name(string mud){
    string *names;
    int found,i;
    
    if(mud == "global") return 0;
    if(mud && mudlist){
        mud = replace_string(mud,"."," ");
        /* Match in the Mudlist for a correct name */
        mud = lower_case(mud);
        names = keys(mudlist);
        found = 0;
        for(i=0;i<sizeof(names);i++){
            if(lower_case(names[i]) == mud) {
                mud = names[i];
                found = 1;
                break;
            }
        }
        if(!found) return 0;
    }        
    return mud;
}    
 
int read_callback(object socket, mixed info){
    string func, fname;
 
// Leto: We should add an origin() and/or prev_obj==SOCKET_OBJ
// check for security.
    if(!sizeof(info)) return 0;
    if(debug_level & I3_DEBUG_IN) debug_stuff(info);
    func = "process_"+info[0];
    func = replace_string(func,"-","_");
    if(function_exists(func, this_object())) {
        /* Internal function (Mudlists, etc) */
        call_other(this_object(), func, info);
        return 0;
    }    
    /* External Module */
    if(!sscanf(info[0], "%s-%*s",fname)) fname = info[0];
    call_other(I3_MODULES+fname, func, info);
    return 1;
}
 
int close_callback(object socket){

// Leto: add a check here too, same reason.

    socket->remove();
    create();
    return 1;
}
  
int send_packet(string type, string user, string mud, string target, mixed *data){
    mixed *send, *names;
    int i,found;
 
// Leto: Add origin()==LOCAL_ORGIN || base_name(prev_obj)[0..x]
// == "/adm/daemons/network/I3/" check here.

    if(!query_network_name(mud) && mud != "global" && mud != 0) return 0;
    mud = query_network_name(mud);
    send = ({ type, I3_TTL, MUD_NAME, user, mud, target }) + data;
    if(debug_level & I3_DEBUG_OUT) debug_stuff(send);
    router_socket->send(send);
    return 1;
}    
 
private int send_chanlist_req(){
    send_packet(PRT_CHANLIST_REQ, 0, ROUTER_NAME, 0, ({ chanlist_id }) );
}    
 
// Leto: I can't find anything calling this function.
void process_chanlist_reply(mixed *info){
    int i;
    string *kys;
 
    if(sizeof(info) != SIZ_CHANLIST_REPLY) return;
    chanlist_id = info[6];
    if(!chanlist) chanlist = ([]);
    chanlist += info[7];
    kys = keys(chanlist);
    /* Clean up deleted channels */
    for(i=0;i<sizeof(kys);i++){
        if(!chanlist[kys[i]]) map_delete(chanlist, kys[i]);
    }
    save_object(I3_SAVE_FILE);
}    
    
private int send_startup_req_1() {
    mixed *packet;
 
    packet = ({ router_password, 
                mudlist_id,
                chanlist_id,
                __PORT__,
                OOB_TCP,
                OOB_UDP,
                I3_MUDLIB,
                I3_BASE_LIB,
                __VERSION__,
                "LPmud",
                MUD_STAGE,
                services });
    send_packet(PRT_STARTUP_REQ_1,0,ROUTER_NAME,0,packet);
}    
 
// Leto: This function isn't called either
void process_startup_reply(mixed *info){
    if(sizeof(info) != SIZ_STARTUP_REPLY) return;
    router_password = info[7];
    save_object(I3_SAVE_FILE);
}
 
 
/*
    Internal Mudlist Routines:
        We handle all of them in here because this way we can deal with
        only 1 object to save, instead of saving a module, and this object
*/
 
void process_mudlist(mixed *info){
    string *kys;
    int i;
 
    if(sizeof(info) != SIZ_MUDLIST) return;
    mudlist_id = info[6];
    if(!mudlist) mudlist = ([]);
    mudlist += info[7];
    kys = keys(mudlist);
    /* Clean up deleted channels */
    for(i=0;i<sizeof(kys);i++){
        if(!mudlist[kys[i]]) {
            map_delete(mudlist, kys[i]);
            if(debug_level & I3_DEBUG_LST) debug_stuff("Removing Mud \""+kys[i]+"\"\n");
            continue;
        }
    }
    save_object(I3_SAVE_FILE);
}
 
mixed query_mud(string arg){
    return mudlist[arg];
}
mapping query_mudlist(){
    return mudlist;
}
/* End of Mudlist Routines */
 
void send_error(string mud, string user, string error_code, string message, mixed *packet){
    send_packet(PRT_ERROR, 0, mud, user, ({ error_code, message, packet }));
}
 
void process_error(mixed *info){
    object ob;
 
    ob = find_living(info[5]);
if (!ob && debugger) ob = debugger;
if (!ob) log_file("I3","I3 Error from "+info[2]+": "+info[7]+"\n");
    else message("I3","I3 Error from "+info[2]+": "+info[7]+"\n", ob);
}    
 
void setup_services(){
    mixed dir;
    string name;
    int i;
 
    services = ([]);
#ifdef I3_SERVICE_DETECT
    dir = get_dir(I3_MODULES);
    for(i=0;i<sizeof(dir);i++){
        sscanf(dir[i],"%s.c", name);
        services += ([ name : 1 ]);
    }
#endif
    services += I3_SERVICES;
    return;
}
 
 
void create(){ 
    if(sizeof(get_dir(I3_SAVE_FILE))) restore_object(I3_SAVE_FILE);
    setup_services();
    router_socket = clone_object(SOCKET, SKT_STYLE_CONNECT_M, 
                                 I3_ROUTER, (: read_callback :), 
                                 (: close_callback :) );
    oob_socket = clone_object(SOCKET, SKT_STYLE_LISTEN_M, OOB_TCP,
		 		(: read_callback :), (: close_callback :) );
    send_startup_req_1();
// [Deathblade] commented this damn thing out :-).  There is
// no such packet.
//    send_chanlist_req();

call_out( (: check_router :) , 900 );
}
 
void remove(){  
    if(oob_socket) oob_socket->remove();
    if(router_socket) router_socket->remove();
    destruct(this_object());
}
 

void check_router() {

if(!router_socket || !oob_socket) 
 {
  log_file("I3","Lost I3 router, reconnecting at "+ctime(time())+".\n");
  create();
  return;
 }
 call_out( (: check_router :) , 900 );
}
