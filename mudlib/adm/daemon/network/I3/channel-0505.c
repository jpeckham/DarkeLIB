// drizzt wrote this and I hacked it just a little (Leto)
// Leto@earth, feb 4, 1996. Fixed channel emotes
#include <daemons.h>
#include <channels.h>
#include <net/i3.h>
#include <uid.h>
void send_channel_e(string from, string visname, string channel, string message)
;
 
private void send_channel_m(string from, string visname, string channel, string message){

if(channel=="gwiz") channel = "imud_gossip" ;
  if(channel=="code") channel = "imud_code" ;
    I3_DAEMON->send_packet(PRT_CHANNEL_M, from, 0, 0, ({ channel, visname, message }));
}

int daemon_apply(object user, string channel, string message, int emote){
 
 
   if (!user) return 0 ;
 
if (base_name(previous_object())!=CHANNELS_D ) return 0;
 
  if(channel=="gwiz") channel = "imud_gossip" ;
  if(channel=="code") channel = "imud_code" ;
 
    if(emote) {
        send_channel_e(user->query("name"), user->query("cap_name"), channel, message);
	return 0;
    }
    send_channel_m(user->query("name"),user->query("cap_name"),channel,message);
 
 
  return 1;
}
 
void process_channel_m(mixed *info){

    if(base_name(previous_object()) != I3_DAEMON) return;
    if(sizeof(info) != SIZ_CHANNEL_M) return;
    if(info[2] == MUD_NAME) return; /* This Mud */
    CHANNELS_D->parse_channel(info[6], info[8], info[7]+"@"+info[2],0);
}
 
void send_channel_e(string from, string visname, string channel, string message){

if(channel == "gwiz") channel = "imud_gossip" ;
  if(channel == "code") channel = "imud_code" ;
    message = "$N "+ message;
    I3_DAEMON->send_packet(PRT_CHANNEL_E, from, 0, 0, ({ channel, visname, message }));
}
 
void process_channel_e(mixed *info){
    string message, from;
 
    if(base_name(previous_object()) != I3_DAEMON) return;

    if(sizeof(info) != SIZ_CHANNEL_E) return;
    from = sprintf("%s@%s",info[7],info[2]);
    message = replace_string(info[8],"$N",from);
//    if(message == info[8]) message = from+" "+message;
//  CHANNELS_D->parse_channel(info[6],message,info[2],1);
CHANNELS_D->parse_channel(info[6],message,"",1);
    return;
}
         
void send_channel_t(mixed *info){
/* Info should contain the following 
        originator_username,                               
        channel_name,                                       
        targetted_mudname,                                 
        targetted_username,                                 
        message_others,                                     
        message_target,                                     
        originator_visname,                                 
        target_visname                                      
*/
    if(sizeof(info) != 8) return;
    info[2] = I3_DAEMON->query_network_name(info[2]);
    if(!info[2] || !info[3] || !info[4] || !info[5]) return;
    I3_DAEMON->send_packet(PRT_CHANNEL_T,info[0], 0, 0, info[1..7]);
    return;
}
 
void process_channel_t(mixed *info){
    int directed;
    object ob;
    string from, to, mess_o, mess_t;
 
    if(sizeof(info) != SIZ_CHANNEL_T) return;
    from = info[11]+"@"+info[2];
    to = info[12]+"@"+info[7];
    if(info[7] == MUD_NAME) ob = find_living(info[8]);
    mess_o = replace_string(info[9],"$N",from);
    if(mess_o == info[9]) mess_o = from+" "+info[9];
    mess_o = replace_string(mess_o,"$O",to);
    message("I3", sprintf("[%s] %s\n",info[6], mess_o), users(), ob);
    
    mess_t = replace_string(info[10],"$N",from);
    if(mess_t == info[10]) mess_t = from+" "+info[10];
    mess_t = replace_string(mess_t,"$O","You");
    message("I3",sprintf("[%s] %s\n",info[6], mess_t), ob);
    return;
}
       

