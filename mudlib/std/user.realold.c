//	/std/user.c
//	from the Nightmare mudlib
//	the user object
//	based on the user.c created by Sulam and Buddha @TMI
//	rewritten by Descartes of Borg september 1992
//      Pallando (93-05-27) Added multiple character handling.
//      Pallando (93-06-17) Removed call to living::setup()


#include <config.h>
#include <rooms.h>
#include <security.h>
#include <daemons.h>
#include <move.h>
#include <objects.h>
#include <money.h>
#include <flags.h>
#include <user.h>
#include <dirs.h>
#include <council.h>

inherit "/std/user/more";

#define OVERRIDE_IGNORE_MSG ({ "broadcast", "info", "more", "room_description", "room_exits","smell","listen","write","say", "system", "prompt", "inanimate_item", "living_item"})
int platinum, gold, electrum, silver, copper;

int level, ghost, crash_money, rolls, verbose_moves;
int birth;
nosave int disable, time_of_login, autosave;
mapping blocked, news;
nosave	status	snoop, earmuffs;
nosave string *inf_heard;
string default_who;
string *inf_block;
string	real_name, email, ip, last_on, password, cpath, race, original_site;
private string position, primary_start;
private nosave string *channels;
mapping mini_quests;
string *quests;
string *mysites;
string guild;
string	*message;
string married;
mixed *current_marriage, *divorced;
nosave string net_died_here;
nosave mapping term_info;
nosave object died_here;

int query_blocked(string str);
int set_mini_quest(string str, int x, string desc);
int set_quest(string str);
string *query_mini_quests();
void reset_terminal();
void convert_kills();
string query_default_who();
void set_default_who(string str);
mapping get_mini_quest_map();
string *query_quests();
private protected register_channels();
private protected setup_inform();
void set_inf_block(string *new_inf_block);
nomask string *query_inform();
nomask string *query_all_inf_blocked();
nomask int query_inf_blocked(string type);
string *query_mysites();
void set_mysites(string *str);
void set_guild(string str);
void set_position(string pos);
void set_level(int x);
void fix_crash_victim();
nomask int query_level();
string query_guild();
void get_email(string e);
int query_ghost();
void add_message(string str);
void write_messages();
string query_email();
nomask string query_position();


    

void get_email(string e) {
    string who, where;

    write("\n");
    if( sscanf(e, "%s@%s", who, where) != 2 ) {
	write("Sorry, email must be in the form of user@host");
        message("prompt", "Please reenter your email address: ", this_player());
	input_to("get_email");
	return;
    }
    email = e;
}


void set_default_who(string str) {
    if(!stringp(str)) return;
    default_who = str;
}

string query_default_who() {
    return (stringp(default_who)? default_who : "normal");
}

void describe_current_room(int verbose) {
    object env;
    string borg;
    mixed tmp;
    int light;

    env = environment(this_object());
    if(wizardp(this_object())) borg = file_name(env)+"\n";
    else borg = "";
    if((light=effective_light(this_object())) > 6 || light < 1) {
        if(light > 6) borg += "It is too bright to see.";
        else if(light > -2) borg += "It is dark.";
        else if(light > -4) borg += "It is quite dark.";
        else if(light > -6) borg += "It is very dark.";
        else borg += "It is completely dark.";
        message("room_description", borg, this_object());
        if(stringp(tmp=(string)env->query_smell("default")))
         message("smell", tmp, this_object());
        else if(functionp(tmp))
          message("smell",(string)((*tmp)("default")), this_object());
        if(stringp(tmp=(mixed)env->query_listen("default")))
          message("listen", tmp, this_object());
        else if(functionp(tmp)) message("listen", (string)((*tmp)("default")), this_object());
        return;
    }
    else if(light > 3) borg += "It is really bright.\n";
    borg += (verbose ? (string)env->query_long(0)+" " :
      (string)env->query_short());
    message("room_description", borg, this_object());
    if(!verbose)
      message("room_exits", (string)env->query_short_exits(), this_object());
    if(verbose && stringp(tmp=(mixed)env->query_smell("default")))
      message("smell", tmp, this_object());
    else if(verbose && functionp(tmp))
      message("smell",(string)((*tmp)("default")), this_object());
    if(verbose && stringp(tmp=(mixed)env->query_listen("default")))
        message("listen", tmp, this_object());
    else if(verbose && functionp(tmp))
      message("listen", (string)((*tmp)("default")), this_object());
    if(verbose && (tmp=(string)env->query_long_exits()) && tmp != "")
      message("room_exits", sprintf("\n%s\n", tmp), this_object());
    if((tmp=(string)env->describe_living_contents(({this_object()})))!="")
      message("living_item", tmp, this_object());
    if((tmp=(string)env->describe_item_contents(({})))!="")
      message("inanimate_item", tmp, this_object());
}

void basic_commands() {
    add_action("quit", "quit");
}

int query_verbose() { return verbose_moves; }

int set_brief() {
    verbose_moves = !verbose_moves;
    return 1;
}

varargs void move_player(mixed dest, string msg) {
    object prev;
    object *inv;
    string here,going,temp1,temp2,temp3;
    int i, illum, bzbd, adj, tmp;

    prev = environment( this_object() );
    here = file_name(prev);
    if (stringp(dest)) {
      if (sscanf(dest,"/%s",temp1)!=1) {
        sscanf(here,"/%s",temp1);
        going = "";
        while(sscanf(temp1,"%s/%s",temp2,temp3)==2) {
          going = going + "/"+temp2;
          temp1 = temp3;
        }
        temp1 = dest;
        if (file_size(going+"/"+temp1+".c") != -1) {
          dest = going +"/"+temp1;
	}
      }
    }
    if( move(dest) != MOVE_OK ) {
	write("You remain where you are.");
	return;
    }
    if(query_property("stealth")) set_property("stealth", -1);
    if(adj = query_property("stealth")) {
        add_skill_points("stealth", adj);
        adj += query_stats("dexterity");
        add_sp(-1);
    }
    if(!query_invis() && !hiddenp(this_object())) {
      inv = all_inventory(prev);
      for(i=0, bzbd = sizeof(inv); i<bzbd; i++) {
        if(!living(inv[i]) || inv[i] == this_object()) continue;
        if(adj) tmp = (adj-(int)inv[i]->query_stats("wisdom"));
        else tmp = 0;
        if(tmp > random(101)) continue;
        if(effective_light(inv[i])-(query_skill("stealth")/17) < 0) continue;
        if(!msg || msg == "") message("mmout", query_mmout(),inv[i]);
        else message("mout", query_mout(msg), inv[i]);
      }
      inv = all_inventory(environment(this_object()));
      for(i=0, bzbd = sizeof(inv); i<bzbd; i++) {
        if(!living(inv[i])) continue;
        if(inv[i] == this_object()) continue;
        if(adj) tmp = (adj-(int)inv[i]->query_stats("wisdom"));
        else tmp = 0;
        if(tmp > random(101)) continue;
        if(effective_light(inv[i])-(query_skill("stealth")/17) < 0) continue;
        if(!msg || msg == "") message("mmin",query_mmin(),inv[i]);
        else message("min", query_min(), inv[i]);
      }
	if(query_followers())
	    move_followers(prev);
    }
    add_sp(-1);
    describe_current_room(verbose_moves);
}

void create() {
    more::create();
    position = "player";
    wielded = ([]);
    level = 1;
    autosave = 500;
    set_weight(500);
    set_max_internal_encumbrance(5000);
    verbose_moves = 1;
    enable_commands();
}

void remove() {
    object *who_exc;

    who_exc = ({ this_object() });
    if(this_object()->query_invis())
    who_exc += filter_array(users(),"filter_notanarch",this_object());
    if(geteuid(previous_object()) != UID_ROOT && 
      previous_object() != this_object() &&
      (query_verb() != "quit" && this_player() != this_object())) return;
    destroy_autoload_obj();
    CHAT_D->remove_user();
    INFORM_D->remove_user(this_object());
    if(!hiddenp(this_object()))
    if(query_verb() == "quit")
    INFORM_D->do_inform("logins_and_quits","Info: " +
	capitalize((string)this_object()->query_name()) +
	" has left the game.",
	who_exc);
    MULTI_D->quit(query_name());
    this_object()->tsh_cleanup();
    living::remove();
}

int quit(string str) {
    if (str) {
	notify_fail("Quit what ?\n");
	return 0;
    }
    crash_money = 0;
    if(query_followers()) clear_followers();
    message("environment",
      "Reality suspended.  See you another time!", this_object());
    // tell_object(this_object(),"Reality suspended.  See you another time!\n");
    save_player( query_name() );
    say(query_cap_name() + " is gone from our reality.");
    log_file("enter", query_name()+" (quit): "+ctime(time())+"\n");
    PLAYER_D->add_player_info();
    remove();
    return 1;
}

void new_body() {
    mapping borg;
    string *zippo;
    int i, max;
    string tmp;

	init_limb_data();
    if(!race) return;
    tmp = race;
	set_max_hp(50 + stats["constitution"] * 10);
	set_hp( (50 + stats["constitution"]*8)/2 );
	set_max_mp( (50 + stats["intelligence"]*10) );
	set_mp( 0 );
        set_max_sp(7*query_stats("dexterity"));
        set_sp(query_stats("dexterity"));
	set_heal_rate(2);
    borg = (mapping)RACE_D->body(this_object());
    for(i=0, max=sizeof(zippo=keys(borg)); i<max; i++) 
      add_limb(zippo[i], borg[zippo[i]]["limb_ref"],borg[zippo[i]]["max_dam"], 0, 0);
    set_wielding_limbs((string *)RACE_D->query_wielding_limbs(tmp));
    set_fingers((int)RACE_D->query_fingers(tmp));
	set_max_internal_encumbrance(stats["strength"] * 200);
}

void setup() {
    string tmp;
    object *who_exc;

    set_living_name(query_name());
    seteuid(getuid());
    set_heart_beat(1);
    if(!stats) init_stats();
    if(!skills) init_skills(0);
    if(member_array(query_position(), MORTAL_POSITIONS) == -1)
      enable_wizard();
    init_living();
    basic_commands();
    ip = query_ip_name(this_object());
    last_on = ctime(time());
    time_of_login = time();
    if(!body) new_body();
    if(!birth) birth = time();
    fix_limbs();
    tsh::initialize();
    register_channels();
    setup_inform();
    if(!original_site) original_site = query_ip_number(this_object());
    if(!wizardp(this_object()) && query_level() == 1 &&
      original_site != query_ip_number(this_object())) {
        message("info", "Level 1 players may only login from their original site.", this_object());
        destruct(this_object());
        return;
    }
    set_max_internal_encumbrance(200*query_stats("strength"));
    if(!sizeof(query_aliases())) {
        message("environment", "Resetting alias data.", this_object());
        init_aliases();
        force_me("alias -reset");
    }
    set_property("light", -query_property("light"));
    if(race) set("race", race);
    if(!query("race")) move(ROOM_SETTER);
    else {
        sight_bonus = (int)RACE_D->query_sight_bonus(query("race"));
        if(!primary_start) primary_start = getenv("start");
        if(!((tmp = getenv("start")) && stringp(tmp) && move(tmp)==MOVE_OK))
          move(ROOM_START);
        setenv("start", primary_start);
    }
    if(!stringp(tmp = getenv("TERM"))) setenv("TERM", tmp = "unknown");
    term_info = (mapping)TERMINAL_D->query_term_info(tmp);
    write_messages();
    if(crash_money) fix_crash_victim();
    load_autoload_obj(); /* Truilkan@TMI 01/18/92 */
    call_out("save_player", 2, query_name());
    PLAYER_D->add_player_info();
    who_exc = ({ this_object() });
    if(this_player()->query_invis())
	who_exc += filter_array(users(),"filter_notanarch",this_object());
    if(!hiddenp(this_object()))
    INFORM_D->do_inform("logins_and_quits","Info: " +
        capitalize((string)this_object()->query_name()) +
	" enters the game.",
	who_exc);
    log_file("enter", query_name()+" (enter): "+ctime(time())+" from "+
	query_ip_name()+" with\n"+
        query_exp()+" exp, "+
        query_money("platinum")+" pl, "+
        query_money("gold")+" gd, "+
	query_money("electrum")+" el, "+
	query_money("silver")+" sl, "+
	query_money("copper")+" cp\n");
    NEWS_D->read_news();
    if(platinum || gold || silver || electrum || copper) {
        add_money("electrum", electrum);
        add_money("gold", gold);
        add_money("silver", silver);
        add_money("platinum", platinum);
        add_money("copper", copper);
        platinum = gold = electrum = silver = copper = 0;
    }
    set_max_sp(query_stats("dexterity")*7);
    convert_kills();
}

void heart_beat() {
    object *inv;
    string *cns;
    int i;
    player_age += 2;
    ok_to_heal ++;
    if(disable) disable--;
    if( (player_age > autosave) && (!wizardp(this_object())) ) {
	message("environment", "Autosaving.", this_object());
	inv = all_inventory(this_object());
        for(i=0, crash_money =0; i<sizeof(inv); i++)
	    crash_money += (int)inv[i]->query_value()/2;
	save_player(query_name());
	autosave = player_age + 500;
    }
    if(!disable) continue_attack();
    magic_round = 0;
    if(player_age > ok_to_heal) do_healing(calculate_healing());
    else calculate_healing();
    if(query_idle(this_object()) >= 3600 &&
       !wizardp(this_object()) ) {
	this_object()->move_player(ROOM_IDLE_SHOP);
        this_object()->force_me("sell all");
        this_object()->force_me("quit");
    }
    if(query_invis() && query_sp()<0 && !wizardp(this_player()) ) set_invis();
}

void net_dead() {
    object *who_exc;

    CHAT_D->remove_user(channels);
    INFORM_D->remove_user(this_object());
    who_exc = ({ this_object() });
    if(this_player()->query_invis())
	who_exc += filter_array(users(),"filter_notanarch",this_object());
    if(!hiddenp(this_object()))
    INFORM_D->do_inform("net_dead_players","Info: " +
        capitalize((string)this_object()->query_name()) +
	" goes net dead.",
	who_exc);
    channels = ({});
    net_died_here = file_name( environment(this_object()) );
    message("other_action", sprintf("%s suddenly disappears into a sea of "
      "irreality.", query_cap_name()), environment(this_object()), ({ this_object() }));
    this_object()->move_player(ROOM_FREEZER);
    if(query_snoop(this_object()))
      tell_object(query_snoop(this_object()), capitalize(query_name())+
        " has gone net-dead.");
    set_heart_beat(0);
}

void restart_heart() {
    string dead_ed;
    object *who_exc;

    message("write", (wizardp(this_object()) &&
      file_size(dead_ed = user_path(getuid())+"dead.edit") > -1 ?
      "\nYour edit file was saved as: "+dead_ed+"\n" :
      "Reconnected."), this_object());
    set_heart_beat(1);
    if(net_died_here) this_object()->move_player(net_died_here);
    else this_object()->move_player(ROOM_START);
    net_died_here = 0;
    say(query_cap_name()+" has rejoined our reality.");
    setup_inform();
    register_channels();
    who_exc = ({ this_object() });
    if(this_player()->query_invis())
	who_exc += filter_array(users(),"filter_notanarch",this_object());
    if(!hiddenp(this_object()))
    INFORM_D->do_inform("net_dead_players","Info: "+
        capitalize((string)this_object()->query_name()) +
	" recovers from net death.",
	who_exc);
}

nomask void die() {
    object ob;
    object corpse;
    object money_ob;
    object *stuff;
   string *currs;
    int tmp;
    int i;

    if(previous_object() != this_object())  /* prevent practical jokes */
      message("death", "You were killed by " +
        file_name(previous_object()) + "!", this_object());
    if(wizardp(this_object())) {
      message("death", "You are immortal and cannot die.", this_object());
      return;
    }
  ob = this_object();
  died_here = environment(ob);
  message("death", "You die.\nYou feel the sensations of nothingness " +
   "as you rise above your corpse.\nYou arrive at a destination in a reality " +
   "not like your own.", this_object());
//  tell_object(this_object(), "You die.\nYou feel the sensations of nothingness
// as you rise above your corpse.\nYou arrive at a destination in a reality not
// like your own.\n");
  message("death", query_cap_name()+ " dies a horrible death.",
    environment());
//  tell_room(environment(this_object()), query_cap_name()+" dies a horrible death.\n", ob);
  if((int)this_object()->query_level() > 1) {
    this_object()->add_exp(-((int)this_object()->query_exp()/4));
    this_object()->reduce_stats();
    reduce_skills();
    while((int)ADVANCE_D->get_exp(query_level()) > query_exp())
	set_level(query_level()-1);
    setenv("TITLE", (string)ADVANCE_D->get_new_title(this_object()));
   }
    corpse = new(OB_CORPSE);
    corpse->set_name(query_cap_name());
    corpse->move(environment(this_object()));
    money_ob = new("/std/obj/coins");
    currs = query_currencies();
    for(i=0, tmp= sizeof(currs); i<tmp; i++) {
        money_ob->set_money(currs[i], query_money(currs[i]));
        add_money(currs[i], -query_money(currs[i]));
    }
    money_ob->move(corpse);
    stuff = all_inventory(this_object());
    for(i=0;i<sizeof(stuff);i++)
    if(!stuff[i]->drop()) stuff[i]->move(corpse);
    this_object()->move("/d/standard/death/death_room");
    cease_all_attacks();
    ghost = 1;
    save_player( query_name() );
    PLAYER_D->add_player_info();
}

void set_rname(string rname) {
    if(geteuid(previous_object()) != UID_ROOT && 
      geteuid(previous_object()) != UID_USERACCESS) return;
    real_name = rname;
}

int is_player() { return 1; }

string query_ip() { return ip; }

string query_email() { if (email) return email; return "???@" + ip; }

string query_rname() { return real_name ? real_name : "???"; }

string query_password() { return password; }

void set_password(string pass) {
    if(geteuid(previous_object()) != UID_ROOT &&
      file_name(previous_object()) != PASSWD_D) return 0;
    password = pass;
    save_player( query_name() );
}

void set_email(string e) {
    if(geteuid(previous_object()) != UID_ROOT && 
      geteuid(previous_object()) != UID_USERACCESS) return 0;
    if (this_player(1) != this_player()) return 0;
    email = e;
    save_player( query_name() );
}

int set_snoop() {
    if (getuid(previous_object()) != UID_ROOT) return 0;
    if(snoop) {
	message("my_action", "You are now secure from being snooped.",
                this_object());
	snoop = 0;
	return 1;
    }
    else {
	message("my_action", "You can now be snooped by anyone.",
                this_object());
	snoop = 1;
	return 1;
    }
}

int query_snoop() { return snoop; }

string get_path() { return cpath; }

void set_path(string path) {
    int foo;

    if(geteuid(previous_object()) != geteuid(this_object()))
	return;
    foo = strlen(path) - 1;
    if(path[foo] == '/') path = path[0..foo-1];
    cpath = path;
}

void write_messages() {
	mapping mail_stat;
	int i;

    message("login", sprintf("\n        >>> Terminal currently set to %s <<<",
      getenv("TERM")), this_object());
    mail_stat = (mapping)LOCALPOST_D->mail_status(query_name());
	if(mail_stat["unread"]) {
          message("login", sprintf("\n        >>> %d of your %d %s are "
            "unread! <<<", mail_stat["unread"], mail_stat["total"],
            (mail_stat["total"]>1 ? "letters" : "letter")), this_object());
	}
        if(query_invis()) message("login", "\n        You are currently "
          "invisible.", this_object());
	if(wizardp(this_object())) {
            if(file_exists("/log/errors/"+query_name()))
              message("login", "\n        >>> You have errors in /log/errors/"+
                query_name()+" <<<", this_object());
            if(file_exists("/log/reports/"+query_name()))
              message("login", "\n        >>> You have reports in "
                "/log/reports/"+query_name()+" <<<", this_object());
	}
	if( !message ) return;
	for( i=0; i<sizeof(message); i++ )
		tell_object(this_object(), message[i]);
	message = ({ });
}

string query_title() {
     string str;
     string foo,fii;

    if(query_ghost()) return "A ghost";
     if(invis) return 0;
     str = getenv( "TITLE" );
     if( !str ) str = query_cap_name();
     else if( !sscanf(str, "%s$N%s", foo,fii) )
     	str = query_cap_name() + " " + str;
     else
     	str = substr( str, "$N", query_cap_name() );
     return str;
}

string query_short() {
    if (interactive(this_object()))
        return query_title();
    else
	return (query_title() + " (link-dead)");
}

mixed *local_commands() {
    if(geteuid(previous_object()) != UID_ROOT &&
      geteuid(previous_object()) != UID_USERACCESS) {
     message("my_action", "You aren't authorized to check this information.",
       this_object());
     return ({});
   }
  return commands();
}
 
nomask string query_position() { return position; }

nomask int query_level() { return level; }

void set_position(string pos) {
    if(geteuid(previous_object()) != UID_ADVANCE &&
      geteuid(previous_object()) != UID_ROOT) return;
    if(member_array(position, MORTAL_POSITIONS) != -1)
	if(member_array(pos, MORTAL_POSITIONS) == -1) enable_wizard();
    if(position == "high mortal" && pos != "high mortal")
	CASTLE_D->disable_high_mortal(this_object());
    init_path();
    position = pos;
    return;
} 

void set_level(int lev) {
    level = lev;
    PLAYER_D->add_player_info();
    log_file("advance", this_object()->query_name()+" advanced to level "+lev+": "+ctime(time())+"\n");
    if(position == "high mortal" && level < 20) {
	position = "player";
	search_path -= ({ DIR_HM_CMDS });
	CASTLE_D->disable_high_mortal(this_object());
	set_env("start", ROOM_START);
	set_env("TITLE", "Mortal $N the fallen high mortal");
    }
    return;
}


int set_earmuffs() {
    if( earmuffs )
    {
	earmuffs = 0;
	message("my_action", "Your earmuffs are now off.",
          this_object());
    }
    else
    {
	message("my_action", "Your earmuffs are now on.", this_object());
	earmuffs = 1;
    }
    return earmuffs;
}

void receive_message(string msg_class, string msg) {
    string *words;
    int i, max;
    string str, pre, post;
    int x, do_wrap;

    if(!stringp(str=getenv("SCREEN"))) str = "75";
    x = atoi(str);
    if(msg_class[0] == 'N') msg_class = msg_class[1..sizeof(msg_class)-1];
    else if(msg_class != "prompt") msg = wrap(msg, x);  // prompt fix -Val
    if(msg_class == "system" || msg_class == "help" || msg_class == "more"){
      receive(msg);
      return;
    }
    if(query_blocked(msg_class) &&
      member_array(msg_class, OVERRIDE_IGNORE_MSG) == -1) return;
    switch(msg_class) {
      case "room_exits": msg = "%^GREEN%^"+msg; break;
      case "smell": msg = "%^ORANGE%^"+msg; break;
      case "listen": msg = "%^CYAN%^"+msg; break;
      case "tell": 
        if(sscanf(msg, "%s:%s", pre, post) == 2)
          msg = "%^BOLD%^RED%^"+pre+":%^RESET%^"+post;
        break;
      case "shout":
        if(sscanf(msg, "%s:%s", pre, post) == 2)
          msg = "%^BOLD%^BLUE%^"+pre+":%^RESET%^"+post;
        break;
      case "mmin": case "min": case "mmout": case "mout":
        msg = "%^BOLD%^GREEN%^"+msg; break;
      case "living_item": msg = "%^BOLD%^RED%^"+msg; break;
      case "inanimate_item": msg = "%^BOLD%^MAGENTA%^"+msg; break;
    }
    if(!term_info) reset_terminal();
    for(i=0, max=sizeof(words=explode(msg, "%^")); i<max; i++)
      if(term_info[words[i]]) words[i] = term_info[words[i]];
    receive(implode(words, "")+term_info["RESET"]);
}

void catch_tell(string str) {
    if(earmuffs) {
      if(sscanf(str, "%*s shouts %*s")) return;
    }
    receive(str);
}

void set_guild(string str) {
    guild = str;
    if(!guild) guild = str;
}

string query_guild() { return guild; }

void set_rolls(int x) { rolls = x; }

int query_rolls() { return rolls; }

void set_blocked(string str) {
    if(!blocked) blocked = ([]);
    blocked[str] = !blocked[str];
    if(blocked[str]) message("info", "You are now blocking "+str+".",
      this_object());
    else message("info", "You are no longer blocking "+str+".",this_object());
}

int query_blocked(string str) {
    if(!blocked) blocked = ([]);
    if(blocked["all"]) return blocked["all"];
    else return blocked[str];
}

string *query_mysites() {
    if(!mysites) mysites = ({});
        return mysites;
}

void set_mysites(string *borg) {
    if(!borg) return;
    if (getuid(previous_object()) != UID_ROOT) return;
    mysites = borg;
    return;
}

string *query_quests() {
    if(!quests) quests = ({});
    return quests;
}

int set_quest(string str) {
    if(!quests) quests = ({});
    if(member_array(str, quests) != -1) return 0;
    player_data["general"]["quest points"] += (int)call_other(ROOM_QUEST, "query_quest_points", str);
    quests += ({ str });
    log_file("quests", query_name()+" completed "+str+": "+ctime(time())+"\n");
    return 1;
}

void return_to_death_site() {
    if(!died_here) this_object()->move("/d/standard/square");
    else this_object()->move(died_here);
}

void revive() {
    message("my_action", "You return from the dead!", this_object());
    message("other_action", query_cap_name()+ " returns from the dead.",
      environment());
//  tell_room(environment(this_object()), query_cap_name()+" returns from the dead.\n", this_object());
    new_body();
    ghost = 0;
}

int query_ghost() { return ghost; }

int query_quest_points() {
    if(!player_data["general"]["quest points"]) return 0;
    else return player_data["general"]["quest points"];
}

void set_disable(int x) { 
    if(x) disable = x;
    else disable = 1;
}

int query_disable() { return disable; }

void set_married(string str) {
    if(current_marriage) return;
    current_marriage = ({ lower_case(str), time() });
}

void divorce_me() {
    if(!current_marriage) return;
    if(!divorced) divorced = ({ ({ current_marriage[0], current_marriage[1], time() }) });
    else divorced += ({ ({current_marriage[0], current_marriage[1], time() }) });
    current_marriage = 0;
}

string query_married() {
    return (current_marriage && sizeof(current_marriage) ? 
      current_marriage[0] : 0);
}

mixed *query_current_marriage() { return current_marriage; }

mixed *query_divorced() { return divorced; }


void  fix_crash_victim() {
    int i;

    i= random(5);
    add_money(HARD_CURRENCIES[i], to_int(crash_money*currency_rate(HARD_CURRENCIES[i])));
    message("info", "You recover some of your lost money.", this_player());
    crash_money = 0;
}

int query_birthday() { return birth; }


void set_news(string which, int size) {
    if(!news) news = ([]);
    news[which] = size;
}

int query_news(string which) {
    if(!news) news = ([]);
    return news[which];
}


void hide(int x) {
    if(file_name(previous_object()) != "/cmds/adm/_haven") return;
    set_hide(x);
}

void set_race(string str) { 
    race = str;
    set("race", str);
    LANG_D->init_languages(this_object());
}

string query_race() { return race; }

int set_mini_quest(string str, int x, string desc) {
    if(!mini_quests) mini_quests = ([]);
    if(mini_quests[str]) return 0;
    else mini_quests[str] = ({ time(), desc });
    add_exp(x);
    log_file("quests", query_name()+" completed mini-quest "+str+" "+ctime(time())+" for "+x+" exp.\n");
    return 1;
}

string *query_mini_quests() {
    if(!mini_quests) return ({});
    else return keys(mini_quests);
}

mapping query_mini_quest_map() { return (mini_quests ? mini_quests : ([])); }

int query_login_time() { return time_of_login; }

void convert_kills() {
    int *tmp;
    int i;

    if(!player_data["kills"] || !sizeof(player_data["kills"]) ||
      intp(player_data["kills"][0])) return;
    tmp = ({});
    i = sizeof(player_data["kills"]);
    while(i--) tmp += ({ (int)PLAYER_D->add_kill(player_data["kills"][i]) });
    player_data["kills"] = tmp;
}

void reset_terminal() {
    term_info = (mapping)TERMINAL_D->query_term_info(getenv("TERM"));
}

void set_name(string str) {
    if(geteuid(previous_object()) != UID_ROOT) return;
    ::set_name(str);
}

nomask string *query_channels() { return channels; }

protected private register_channels() {
    if(wizardp(this_object()))
      channels = ({ "cleric","fighter","kataan","mage","monk","rogue"});
    else channels = (query_class() ? ({ query_class() }) : ({}));
    if(query_guild()) channels += ({ query_guild() });
    if(wizardp(this_object()) || high_mortalp(this_object())) channels += ({ "hm", "newbie" });
    else if(query_level() < 6) channels += ({ "newbie" });
    if(wizardp(this_object())) channels += ({ "cre", "gossip", "intercre"});
    if(ambassadorp(this_object())) channels += ({ "gossip" });
    if(archp(this_object())) channels += ({ "admin" });
    if(wizardp(this_object()) || ambassadorp(this_object())) channels += ({ "gossip" });
    if(wizardp(this_object()) || leaderp(this_object()))
          channels += ({ "council" });
    CHAT_D->add_user(channels);
}

protected private setup_inform() {
    if(!inf_block) inf_block = ({});
    inf_heard = ({ "net_dead_players","logins_and_quits" });
    inf_heard += ({ "impending_shutdowns" });
    if(archp(this_object()))
	inf_heard += ({ "external_edits","user_shadows" });
    INFORM_D->add_user(this_object(),inf_heard);
}

nomask string *query_inform() { return inf_heard; }

void set_inf_block(string *new_inf_block) {
	if(new_inf_block) inf_block = new_inf_block;
}

nomask string *query_all_inf_blocked() { return inf_block; }

nomask int query_inf_blocked(string type) {
    if(!inf_block || !sizeof(inf_block)) return 0;
    return (member_array(type,inf_block) > -1);
}

string query_first_site() { return original_site; }

void set_primary_start(string str) {
    object ob;

    if(!(ob = find_object_or_load(str))) return;
    if(clonep(ob) || virtualp(ob)) return;
    setenv("start", primary_start = str);
}

string query_primary_start() { return primary_start; }

void clean_net_dead() {
    if(base_name(previous_object()) != ROOM_FREEZER) return;
    this_object()->remove();
}

int filter_notanarch(object ob) { return !archp(ob); }

