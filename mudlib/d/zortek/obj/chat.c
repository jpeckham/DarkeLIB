// Daemon which handles chat channels accessed with "review"
// mod: Diewarzau, 3/16/94
// (darkemud)
// Modified by Misery 031096 to add arena stuff

#include <udp.h>
#include <daemons.h>
#include <objects.h>
#include "/wizards/misery/def.h"

nosave private mapping channels;

int list_channel(string str);

void create() {
    string *chans;
    object *who;
    int i, j;

    seteuid(getuid());
    channels = ([]);
    i = sizeof(who = users());
    while(i--) {
      j = sizeof(chans = (string *)who[i]->query_channels());
	  while(j--) {
        if(!channels[chans[j]]) channels[chans[j]] = ({});
	    channels[chans[j]] = distinct_array(channels[chans[j]]+({who[i]}));
      }
    }
}

void add_user(string *chans) {
    object ob;
    int i;

    if(base_name(ob = previous_object()) != OB_USER) return;
    i = sizeof(chans);
    while(i--) { 
	if(!channels[chans[i]]) channels[chans[i]] = ({});
	  channels[chans[i]] = distinct_array(channels[chans[i]]+({ob}));
    }
}

void remove_user(string *chans) {
    object ob;
    int i;

    if(base_name(ob = previous_object()) != OB_USER) return;
    i = sizeof(chans);
    while(i--) {
	  if(!channels[chans[i]]) continue;
	  else channels[chans[i]] -= ({ ob });
	  if(!sizeof(channels[chans[i]])) map_delete(channels, chans[i]);
    }
}

int do_chat(string verb, string str) {
    string msg, col, name;
    int emote;

    if(verb == "list") return list_channel(str);
    if(!channels[verb]) {
      if(sscanf(verb, "%semote", verb)) emote = 1;
	  else return 0;
	  if(!channels[verb]) return 0;
    }
    if(member_array(this_player(), channels[verb]) == -1) return 0;
    if(!str) {
	  this_player()->set_blocked(verb);
	  return 1;
    }
    if((int)this_player()->query_blocked(verb)) {
	  if((int)this_player()->query_blocked("all")) {
	    message(verb, "You cannot chat while totally blocked.", this_player());
	    return 1;
	  }
	  this_player()->set_blocked(verb);
    }
    switch(verb) {
      case "newbie": col = "%^BOLD%^MAGENTA%^"; break;
      case "darke":  col = "%^MAGENTA%^"; break;
      case "cre":    col = "%^BOLD%^BLUE%^"; break;
      case "intercre": col = "%^BOLD%^CYAN%^"; break;
      case "admin":  col = "%^YELLOW%^"; break;
      case "arena":  col = "%^BOLD%^RED%^"; break;
      default: col = "%^GREEN%^"; break;
    }
    if(emote) msg = sprintf("[%s%s%s] %s %s", col, verb, "%^RESET%^",
      (string)this_player()->query_cap_name(), str);
    else msg = sprintf("[%s%s%s : %s] %s",col,verb,"%^RESET%^",
      (string)this_player()->query_cap_name(),str);
    message(verb, msg, channels[verb]);
    if(member_array(verb, INTERMUD_CHANNELS) != -1)
      SERVICES_D->send_gwizmsg(str, emote);
    return 1;
}

void send_arena_message(string str) {
   if(FN(PO) != "/wizards/misery/arena/booths_room") return;
   message("arena", sprintf("[%s : %s%s%s] %s", "Announcer",
      "%^BOLD%^RED%^", "arena", "%^RESET%^", str), channels["arena"]);
   return;
}

void inter_channels(string who, string verb, string str, int emote) {
    string msg;

    if(!channels[verb] || base_name(previous_object()) != SERVICES_D) return;
    if(emote)
       msg = sprintf("[%s%s%s] %s %s", "%^BOLD%^", verb, "%^RESET%^", who, str);
    else msg = sprintf("[%s : %s%s%s] %s", who, "%^BOLD%^", verb, "%^RESET%^", str);
    message(verb, msg, channels[verb]);
}

int list_channel(string str) {
    string list, col;
    int i;

    if(!channels[str]) return 0;
    if(member_array(this_player(), channels[str]) == -1) return 0;
    switch(str) {
      case "newbie": col = "%^BOLD%^MAGENTA%^"; break;
      case "darke":  col = "%^MAGENTA%^"; break;
      case "cre":    col = "%^BOLD%^BLUE%^"; break;
      case "intercre": col = "%^BOLD%^CYAN%^"; break;
      case "admin":  col = "%^YELLOW%^"; break;
      case "arena":  col = "%^BOLD%^RED%^"; break;
      default: col = "%^GREEN%^"; break;
    }
    list = col+"+------------------------------------------------------------------+%^RESET%^";
    list += "\n%^BOLD%^People active on %^RESET%^["+col+str+"%^RESET%^]\n";
    i =sizeof(channels[str]);
    while(i--) {
       if(!channels[str][i]) continue;
	   if(hiddenp(channels[str][i]) || (int)channels[str][i]->query_invis() ||
	     (int)channels[str][i]->query_blocked(str)) continue;
	   list += sprintf("%-10s  ", (string)channels[str][i]->query_cap_name());
    }
    list += "\n"+col+"+------------------------------------------------------------------+%^RESET%^";
    message("info", list, this_player());
    return 1;
}
