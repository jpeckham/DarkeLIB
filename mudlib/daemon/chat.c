// Daemon which handles chat channels accessed with "review"
// mod: Diewarzau, 3/16/94
// (darkemud)
// Modified by Geldron 031096 to add arena stuff

#include <udp.h>
#include <daemons.h>
#include <objects.h>
#include <security.h>
#include <ansi.h>

nosave private mapping channels;

int list_channel(string str);
int hist_channel(string str);

protected int compare(string one, string two) {
    return strcmp(one, two);
}
mapping query_channel(string str) {return channels[str];}
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

int send_chat(string verb, string who, string str) {
    string msg, col;
    int emote;

    if(!channels[verb]) {
	if(sscanf(verb, "%semote", verb)) emote = 1;
	else return 0;
	if(!channels[verb]) return 0;
    }
    switch(verb) {
    case "newbie": col = "%^BOLD%^%^MAGENTA%^"; break;
    case "bid": col = "%^B_GREEN%^%^BLACK%^"; break;
    case "legend": col = "%^YELLOW%^"; break;
    case "chat": col = "%^CYAN%^"; break;
      case "daybreak":  col = "%^ORANGE%^"; break;
    case "cre":    col = "%^BOLD%^%^BLUE%^"; break;
    case "intercre": col = "%^BOLD%^%^CYAN%^"; break;
    case "admin":  col = "%^YELLOW%^"; break;
    case "arena":  col = "%^BOLD%^%^RED%^"; break;
    case "high-mortal": col = "%^YELLOW%^%^B_BLUE%^"; break;
    case "council": col = "%^B_RED%^%^YELLOW%^"; break;
    case "arch": col = "%^B_MAGENTA%^%^WHITE%^"; break;
    case "darkechat": col = "%^BOLD%^%^BLUE%^";break;
    case "darkelib" : col = "%^MAGENTA%^";break;
    default: col = "%^GREEN%^"; break;
    }
    if(emote)
	msg = sprintf("[%s%s%s] %s %s", col, verb, "%^RESET%^", who, str);
    else
	msg = sprintf("[%s%s%s : %s] %s",col,verb,"%^RESET%^", who ,str);
    message(verb, msg, channels[verb]);
    seteuid(UID_LOG);
    log_file("channels/"+verb, "\t\t\t\t%^RED%^"+ctime(time())+" %^BOLD%^##%^RESET%^\n"+msg+"\n");
    seteuid(getuid());
    return 1;
}

int do_chat(string verb, string str) {
    string msg, col, person;
    int emote;

 if (wizardp(this_player())){
    person = (string)this_player()->query_cap_name();
 }
 if (!wizardp(this_player())){
    person = capitalize((string)this_player()->query_true_name());
}
if (this_player()->query_property("no chat")==1){
    message("info", "You may not chat, you have been banned from all lines", this_player());
    return 1;
}
    if(verb == "list") return list_channel(str);
    if(verb == "hist") return hist_channel(str);
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
    case "newbie": col = "%^BOLD%^%^MAGENTA%^"; break;
    case "legend": col = "%^YELLOW%^"; break;
    case "bid": col = "%^B_GREEN%^%^BLACK%^"; break;
    case "chat": col = "%^CYAN%^"; break;
      case "daybreak":  col = "%^ORANGE%^"; break;
    case "cre":    col = "%^BOLD%^%^BLUE%^"; break;
    case "intercre": col = "%^BOLD%^%^CYAN%^"; break;
    case "admin":  col = "%^YELLOW%^"; break;
    case "arena":  col = "%^BOLD%^%^RED%^"; break;
    case "high-mortal": col = "%^YELLOW%^%^B_BLUE%^"; break;
    case "council": col = "%^B_RED%^%^YELLOW%^"; break;
    case "arch": col = "%^B_MAGENTA%^%^WHITE%^"; break;
    case "darkelib"   : col = "%^MAGENTA%^"; break;
    case "darkechat"  : col = "%^BOLD%^%^BLUE%^";break;
    default: col = "%^GREEN%^"; break;
    }
    if(emote) msg = sprintf("[%s%s%s] %s %s", col, verb, "%^RESET%^",
          person, str);
    else msg = sprintf("[%s%s%s : %s] %s",col,verb,"%^RESET%^",
          person, str);
    message(verb, msg, channels[verb]);
    if(member_array(verb, INTERMUD_CHANNELS) != -1)
	   SERVICES_D->send_gwizmsg(str, emote, verb);
    seteuid(UID_LOG);
    log_file("channels/"+verb, "\t\t\t\t%^RED%^"+ctime(time())+" **\n"+msg+"\n");
    seteuid(getuid());
    return 1;
}

void send_arena_message(string str) {
    if(BN(PO) != "/d/damned/arena/booths_room") return;
    message("arena", sprintf("[%s : %s%s%s] %s", "Announcer",
	"%^BOLD%^RED%^", "arena", "%^RESET%^", str), channels["arena"]);
    return;
}

void inter_channels(string who, string verb, string str, int emote) {
    string msg;

    if(!channels[verb] || base_name(previous_object()) != SERVICES_D)
      return;
    switch(verb){
       case "darkelib"  :  if(emote)
	                           msg = sprintf("[%s%s%s] %s %s", "%^MAGENTA%^", verb, "%^RESET%^", who, str);
                           else
                              msg = sprintf("[%s%s%s : %s] %s", "%^MAGENTA%^", verb, "%^RESET%^",who , str);
                           break;
       case "darkechat" :  if(emote)
                              msg = sprintf("[%s%s%s] %s %s", "%^BOLD%^%^BLUE%^", verb, "%^RESET%^", who, str);
                           else
                              msg = sprintf("[%s%s%s : %s] %s", "%^BOLD%^%^BLUE%^", verb, "%^RESET%^", who, str);
                           break;
            default     :  if(emote)
                              msg = sprintf("[%s%s%s] %s %s", "%^BOLD%^", verb, "%^RESET%^", who, str);
                           else
                              msg = sprintf("[%s%s%s : %s] %s", "%^BOLD%^", verb, "%^RESET%^", who, str);
                           break;
    }
    message(verb, msg, channels[verb]);
    seteuid(UID_LOG);
    log_file("channels/"+verb, "\t\t\t\t%^RED%^"+ctime(time())+" !!\n"+msg+"\n");
    seteuid(getuid());
}

string *query_channel_listeners(string str) { return channels[str]; }

int list_channel(string str) {
    string list, col;
    string *foo;
    int i;

    if(!channels[str]) return 0;
    if(member_array(this_player(), channels[str]) == -1) return 0;
    switch(str) {
    case "newbie": col = "%^BOLD%^%^MAGENTA%^"; break;
    case "bid": col = "%^B_GREEN%^%^BLACK%^"; break;
      case "daybreak":  col = "%^ORANGE%^"; break;
    case "chat": col = "%^CYAN%^"; break;
    case "legend": col = "%^YELLOW%^"; break;
    case "cre":    col = "%^BOLD%^%^BLUE%^"; break;
    case "intercre": col = "%^BOLD%^%^CYAN%^"; break;
    case "admin":  col = "%^YELLOW%^"; break;
    case "arena":  col = "%^BOLD%^%^RED%^"; break;
    case "high-mortal": col = "%^YELLOW%^%^B_BLUE%^"; break;
    case "council": col = "%^B_RED%^%^YELLOW%^"; break;
    case "arch": col = "%^B_MAGENTA%^%^WHITE%^"; break;
    case "darkelib"   : col = "%^MAGENTA%^"; break;
    case "darkechat"  : col = "%^BOLD%^%^BLUE%^";break;
    default: col = "%^GREEN%^"; break;
    }
    list = col+"+------------------------------------------------------------------+%^RESET%^";
    list += "\n%^BOLD%^People active on %^RESET%^["+col+str+"%^RESET%^]\n";
    message("info", list, this_player());
    foo = ({});
    if(wizardp(this_player())) {
	i =sizeof(channels[str]);
	while(i--) {
	    if(!channels[str][i]) continue;
	    if( (int)channels[str][i]->query_blocked(str)) continue;
	    if(hiddenp(channels[str][i]) ) {
#if 0
		if(!archp(this_player())) continue;
		foo += ({"!"+capitalize(getuid(channels[str][i]))+"!"});
#endif
		continue;
	    }
	    if( (int)channels[str][i]->query_invis() ) {
		if(!wizardp(this_player())) continue;
		foo += ({"{"+capitalize(getuid(channels[str][i]))+"}"});
		continue;
	    }
	    foo += ({ (string)channels[str][i]->query_cap_name() });
	}
    }
    else {
	i =sizeof(channels[str]);
	while(i--) {
	    if(!channels[str][i]) continue;
	    if(hiddenp(channels[str][i]) || (int)channels[str][i]->query_invis() ||
	      (int)channels[str][i]->query_blocked(str)) continue;
	    foo += ({ (string)channels[str][i]->query_cap_name() });
	}
    }
    foo = (sort_array(foo, "compare", this_object()));
    i = sizeof(foo);
    message("info", format_page(foo,4), this_player() );
    list = "\n"+col+"+------------------------------------------------------------------+%^RESET%^";
    message("info", list, this_player());
    return 1;
}

int hist_channel(string str) {
    string hist, gm;
    string col, *foo;
    int  x, allow;

    if(!channels[str]) return 0;
    if(member_array(this_player(), channels[str]) == -1) {
	write("You are unaware of a "+str+" channel (much less a hist for it)!");
	return 1;
    }
    allow = 0;
    switch(str) {
    case "newbie":
	col = "%^BOLD%^%^MAGENTA%^";
	allow = 1;
	break;
    case "chat":
        col = "%^CYAN%^";
        allow = 1;
        break;
    case "bid":
        col = "%^B_GREEN%^%^BLACK%^";
	allow = 1;
	break;
      case "daybreak":
          col = "%^ORANGE%^";
          if(member_array(this_player(), channels["daybreak"]) > -1) allow = 1;
	break;
    case "legend":
        col = "%^YELLOW%^";
        if(wizardp(this_player()) || legendp(this_player())) allow = 1;
        break;
    case "cre":
	col = "%^BOLD%^%^BLUE%^";
	if(wizardp(this_player())) allow = 1;
	break;
    case "intercre":
	col = "%^BOLD%^%^CYAN%^";
	if(wizardp(this_player())) allow = 1;
	break;
    case "admin":
	col = "%^YELLOW%^";
	if(archp(this_player())) allow = 1;
	break;
    case "arena":
	col = "%^BOLD%^%^RED%^";
	allow = 1;
	break;
    case "council":
	col = "%^B_RED%^%^YELLOW%^";
      gm = "/d/damned/guilds/join_rooms/"+ (string)this_player()->query_class()+"_join.c";
      gm = gm->query_master();        
      if(this_player()->query_name() == gm || wizardp(this_player())) allow = 1;
// 	if(member_array(this_player(), channels["council"]) > -1) allow = 1;
	break;
    case "high-mortal":
	col = "%^B_BLUE%^%^YELLOW%^";
	if(member_array(this_player(), channels["high-mortal"]) > -1) allow = 1;
	break;
    case "arch":
	col = "%^B_MAGENTA%^%^WHITE%^";
	if(member_array(this_player(), channels["arch"]) > -1) allow = 1;
	break;
    case "darkelib":
   col = "%^MAGENTA%^";
   if(member_array(this_player(), channels["darkelib"]) > -1) allow = 1;
   break;
    case "darkechat":
   col = "%^BOLD%^%^BLUE%^";
   if(member_array(this_player(), channels["darkechat"]) > -1) allow = 1;
   break;
   default:
	col = "%^GREEN%^";
	allow = 1;
	break;
    }
    if(allow) {
	seteuid(UID_LOG);
	if(!file_exists("/log/channels/"+str)) {
	    write("No history for that channel.");
	    return 1;
	}
	hist = read_file("/log/channels/"+str);
	seteuid(getuid());
	foo = explode(hist, "\n");
	x = sizeof(foo) - 22;
	if(x > 0)
	    this_player()->more(foo[x..]);
	else
	    this_player()->more(foo);
	if(x > 100) {
	    seteuid(UID_LOG);
	    rm("/log/channels/"+str);
	    write_file("/log/channels/"+str, implode(foo[x..], "\n"));
	    seteuid(getuid());
	}
	return 1;
    }
    write("You do not have access to "+str+"'s hist log!");
    return 1;
}





