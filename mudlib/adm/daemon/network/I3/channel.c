// drizzt wrote this and I hacked it just a little (Leto)
// Leto@earth, feb 4, 1996. Fixed channel emotes
// Support for I3 channels and better security - Brainstorm 1.5.96
#include <daemons.h>
#include <channels.h>
#include <net/i3.h>
#include <uid.h>
void send_channel_e(string from, string visname, string channel, string message)
;

// Inserted channel things here.
private
void send_chan_who_req( string from, string target, string channel ) {
  target=I3_DAEMON->query_network_name(target);
  I3_DAEMON->send_packet( PRT_CHAN_WHO_REQ, from, target, 0, 
			({ channel }) );
}

private
void send_channel_listen( string channel, int on_or_off ) {
  I3_DAEMON->send_packet( PRT_CHANNEL_LISTEN, 0, ROUTER_NAME, 0,
			({ channel, on_or_off }) );
}

private
void send_channel_add( string from, string channel, int type )
{ 
  I3_DAEMON->send_packet( PRT_CHANNEL_ADD, from, ROUTER_NAME, 0,
			  ({ channel, type }) );
}

private
void send_channel_remove( string from, string channel )
{ 
  I3_DAEMON->send_packet( PRT_CHANNEL_REMOVE, from, ROUTER_NAME, 0,
                          ({ channel }) );
}

private
void send_channel_admin( string from, string channel, string *add_to_list,
						      string *remove_from_list )
{ int i;
  if( sizeof( add_to_list ) )
    for( i=0; i<sizeof(add_to_list); i++ )
      add_to_list[i]=I3_DAEMON->query_network_name( add_to_list[i] );
  if( sizeof( remove_from_list ) )
    for( i=0; i<sizeof(remove_from_list); i++ )
      remove_from_list[i]=I3_DAEMON->query_network_name( remove_from_list[i] );

  I3_DAEMON->send_packet( PRT_CHANNEL_ADMIN, from, ROUTER_NAME, 0,
                          ({ channel, add_to_list, remove_from_list }) );
}

			
private void send_channel_m(string from, string visname, string channel, string message){

    I3_DAEMON->send_packet(PRT_CHANNEL_M, from, 0, 0, ({ channel, visname, message }));
}

int daemon_apply(object user, int type, mixed *action )
{ string channel, message; 
  int emote;
 
 
  if (!user) return 0 ;
  if (base_name(previous_object())!=CHANNELS_D ) return 0;
 
  switch( type ) {
  case MSG: 
	if( sizeof( action ) != 3 ) return 0;
	channel=action[0]; 
	message=action[1];
	emote=action[2];
 
 	if(emote) {
          send_channel_e(user->query("name"), user->query("cap_name"), 
					channel, message);
	  return 0;
    	}
    	send_channel_m(user->query("name"),user->query("cap_name"),
			channel,message);
 
 
  	return 1;
  case ADM:
	if( sizeof( action ) != 4 ) return 0;
	send_channel_admin( action[0], action[1], action[2], action[3] );
	return 1;
  case LIS:
	if( sizeof( action ) != 2 ) return 0;
	send_channel_listen( action[0], action[1] );
	return 1;
  case CRE:
	if( sizeof( action ) != 3 ) return 0;
	send_channel_add( action[0], action[1], action[2] );
	return 1;
  case DEL:
	if( sizeof( action ) != 2 ) return 9;
	send_channel_remove( action[0], action[1] );
	return 1;
  case WHOREQ:
	if( sizeof( action ) != 3 ) return 0;
        send_chan_who_req( action[0], action[1], action[2] );
        return 1;
  default:
	return 0;
  }
}

private
void send_chan_who_reply( mixed *info ) {
/*      (string)   "chan-who-reply",
        (int)      5,
        (string)   originator_mudname,
        (string)   0,
        (string)   target_mudname,
        (string)   target_username,
        (string)   channel_name,
        (string *) user_list
*/
    if(sizeof(info) != 4) return;
    info[0] = I3_DAEMON->query_network_name(info[0]);
    if(!info[0] || !info[1] || !info[2] || !info[3]) return;
    I3_DAEMON->send_packet(PRT_CHAN_WHO_REPLY, 0, info[0], info[1], info[2..3] );
    return; 

}

private
void send_chan_filter_reply( mixed *info )
{ 
/*      (string)   "chan-filter-reply",
        (int)      5,
        (string)   originator_mudname,    // The channel host/owner mudname
        (string)   0,
        (string)   target_mudname,        // the router
        (string)   0,
        (string)   channel_name,
        (mixed *)  filtered_packet,
*/
	if( sizeof( info ) != 2 ) return 0;
	I3_DAEMON->send_packet( PRT_CHAN_FILTER_REPLY, 0, ROUTER_NAME, 0,
		 info );
}

void process_chan_filter_req( mixed *info )
{
/*      (string)   "chan-filter-req",
        (int)      5,
        (string)   originator_mudname,     // the router
        (string)   0,
        (string)   target_mudname,         // the owner/host mud
        (string)   0,
        (string)   channel_name,
        (mixed *)  packet_to_filter,
*/
 mixed *output, *filterd;
 if(base_name(previous_object()) != I3_DAEMON) return;
 if( sizeof( info ) != SIZ_CHAN_FILTER_REQ ) return 0;
 if( function_exists( "filter_packet", (object)(I3_MODULES+"filters/"+info[6]) ) )
	filterd=(I3_MODULES+"filters/"+info[6])->filter_packet(info[7]);
 else filterd=info[7];

 output=({ info[6], filterd });
 send_chan_filter_reply( output );

}

void process_chan_who_req( mixed *info )
{
/*      (string) "chan-who-req",
        (int)    5,
        (string) originator_mudname,
        (string) originator_username,
        (string) target_mudname,
        (string) 0,
        (string) channel_name
*/
 mixed *output;
 if(base_name(previous_object()) != I3_DAEMON) return;
 if( sizeof( info ) != SIZ_CHAN_WHO_REQ ) return 0;
 output=({ info[2],info[3], info[6], 
	explode( CHANNELS_D->display_channel( info[6]), "," ) });
 send_chan_who_reply( output );

} 

// Show which users are listening to this channel
void process_chan_who_reply(mixed *info){

   string msg, which;
   object target;

   if(base_name(previous_object()) != I3_DAEMON) return;
   if( sizeof(info) != SIZ_CHAN_WHO_REPLY) return;
   target=find_living(info[5]);
   if( target ) { 
     if(sizeof(info[7])) {
       msg = "The following users from "+info[2]+" are tuned into "+info[6]+":\n";
       msg += ( implode( info[7], "," ) +"\n" ); // BrLeto + "   <End>\n" );
       message("I3", msg, target );
     } else
       message("I3", "Nobody from "+info[2]+" is tuned into "+info[6]+".\n",
		target );
   }
}          
 
void process_channel_m(mixed *info){

    if(base_name(previous_object()) != I3_DAEMON) return;
    if(sizeof(info) != SIZ_CHANNEL_M) return;
    if(info[2] == MUD_NAME) return; /* This Mud */
    CHANNELS_D->parse_channel(info[6], info[8], info[7]+"@"+info[2],0);
}
 
private
void send_channel_e(string from, string visname, string channel, string message){

// Lousy hack to fix emote problem ...
  sscanf( message, "%*s %s", message );
  message =  "$N "  + message;
    
    I3_DAEMON->send_packet(PRT_CHANNEL_E, from, 0, 0, ({ channel, visname, message }));
}
 
void process_channel_e(mixed *info){
    string message, from;
 
    if(base_name(previous_object()) != I3_DAEMON) return;

    if(sizeof(info) != SIZ_CHANNEL_E) return;
    if(info[2] == MUD_NAME) return;
    from = sprintf("%s@%s",info[7],info[2]);
    message = replace_string(info[8],"$N",from);
//    if(message == info[8]) message = from+" "+message;
//  CHANNELS_D->parse_channel(info[6],message,info[2],1);
CHANNELS_D->parse_channel(info[6],message,"",1);
    return;
}
 
private
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

    if(base_name(previous_object()) != I3_DAEMON) return;
 
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
