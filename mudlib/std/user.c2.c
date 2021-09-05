//	/std/user.c
//	from the Nightmare mudlib
//	the user object
//	based on the user.c created by Sulam and Buddha @TMI
//	rewritten by Descartes of Borg september 1992
//      Pallando (93-05-27) Added multiple character handling.
//      Pallando (93-06-17) Removed call to living::setup()
// Modified by Geldron 031096 to allow 'cpr' command to work
// Modified by Geldron 031096 to fix 'quit' bug
// Modified by Geldron 031096 to fix dying multiple times bug
// Modified by Geldron 031096 to add arena stuff
// Modified by Geldron 031396 to work w/ playertool stuff


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
// WHY IS THIS AN INCLUDE IF IT HAS A .C EXTENSION?
#include "/std/user/ansi_convert.c"
#pragma optimize
#define RESURRECT "/cmds/adm/_resurrect"
#define LOG_PROPS ({ "old exp", "dev points", "dev point base", \
"xp mod", "hp advance", "mp advance" })
,
x
inherit "/std/user/more";

#define OVERRIDE_IGNORE_MSG ({ "broadcast", "info", "more", "room_description", "room_exits","smell","listen","write","say", "system", "prompt", "inanimate_item", "living_item"})
int platinum, gold, electrum, silver, copper;

int level, ghost, crash_money, rolls, verbose_moves;
int birth;
nosave int count, challenged, count2, disable, time_of_login, autosave;
mapping blocked, colours, bank, exp_log;
string *keep;
string snatch;
nosave	int	snoop, earmuffs;
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
string guild_security;
string	*message;
string married;
mixed *current_marriage, *divorced;
nosave string net_died_here;
nosave mapping term_info;
varargs void net_dead(int flag);
protected int finish_quit(object ob);
nosave object died_here, bet;
nosave int bet_allowed;

int query_blocked(string str);
void set_bank(string str, int val);
nomask int query_bank(string str);
nomask mapping query_bank_total();
int set_mini_quest(string str, int x, string desc);
int set_quest(string str);
string *query_mini_quests();
void reset_terminal();
void convert_kills();
string query_default_who();
void set_default_who(string str);
mapping get_mini_quest_map();
string *query_quests();
private register_channels();
private setup_inform();
void set_inf_block(string *new_inf_block);
nomask string *query_inform();
nomask string *query_all_inf_blocked();
nomask int query_inf_blocked(string type);
void set_challenged(int arg);
int query_challenged();
string *query_mysites();
void set_mysites(string *str);
void set_guild(string str);
void set_position(string pos);
void set_level(int x);
void fix_crash_victim();
nomask int query_level();
string query_guild();
void get_email(string e);
void set_colours(mapping tmp);
void set_colour(string tmp1, string tmp2);
void remove_colour(string tmp);
string query_colour(string tmp);
mapping query_colours();
int query_ghost();
void add_message(string str);
void write_messages();
string query_email();
nomask string query_position();
void remove();
string query_catch();
void set_catch(string str);


void set_catch(string str) {
    snatch = str;
    return;
}

string query_catch() { return snatch; }

void receive_snoop(string str) {
    receive("% "+str);
    return;
}

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

int id(string str) {
    return (::id(str) || (str == query_name()));
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
    if(!objectp(env)) {
	move(ROOM_START);
	return;
    }
    if(wizardp(this_object()) || this_object()->query_ghost()) borg = file_name(env)+"\n";
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
    else if(light > 3) borg += "It is very bright.\n";
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
    add_action("accept", "accept");
    add_action("decline", "decline");
}

int accept(string arg) {
    if(!query_challenged()) return 0;
    message("challenge", "You accept the challenge!", TO);
    "/d/damned/arena/booths_room"->accept(this_object());
    return 1;
}

int decline(string arg) {
    if(!query_challenged()) return 0;
    message("challenge", "You decline the challenge!", TO);
    "/d/damned/arena/booths_room"->decline(this_object());
    return 1;
}

int query_verbose() { return verbose_moves; }

int set_brief() {
    verbose_moves = !verbose_moves;
    return 1;
}

void set_bet(object ob) { bet = ob; }

object query_bet() { return bet; }

nomask void set_bet_allowed(int val) {
    if(BN(PO) != "/d/damned/arena/master_room") return;
    bet_allowed = val;
}

int query_bet_allowed() { return bet_allowed; }

varargs void move_player(mixed dest, string msg) {
    object prev;
    object *inv;
    string *my_limbs, here,going,temp1,temp2,temp3;
    int i, illum, bzbd, adj, tmp;
    int flag;

    this_object()->set("error report", 0);
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
    if( this_object()->move(dest) != MOVE_OK ) {
	write("You remain where you are.");
	if(this_object()->query("error report")) {
	    printf("Runtime errors occurred in the execution.\n"
	      "Would you like to view the error report (y/n)? ");
	    input_to((: call_other, "/cmds/system/handler", "handle_error" :), 0, this_object(),
	      (string)this_object()->query("error report"));
	}
	return;
    }
    adj = (int)this_object()->query_stealth();
    if(!intp(adj)) adj = 0;
    if(!hiddenp(this_object())) {
	inv = all_inventory(prev);
	for(i=0, bzbd = sizeof(inv); i<bzbd; i++) {
	    if(this_object()->query_invis() && !inv[i]->query("see invis"))
		continue;
	    if(!living(inv[i]) || inv[i] == this_object()) continue;
	    if(adj) tmp = skill_contest(adj, inv[i]->query_skill(
		    "perception"));
	    else tmp = 2;
	    if(tmp != 2) continue;
	    if(effective_light(inv[i])-(adj/25) < 0) continue;
	    if(!msg || msg == "") message("mmout", query_mmout(),inv[i]);
	    else message("mout", query_mout(msg), inv[i]);
	}
	inv = all_inventory(environment(this_object()));
	for(i=0, bzbd = sizeof(inv); i<bzbd; i++) {
	    if(this_object()->query_invis() && !inv[i]->query("see invis"))
		continue;
	    if(!living(inv[i])) continue;
	    if(inv[i] == this_object()) continue;
	    if(adj) tmp = skill_contest(adj, inv[i]->query_skill(
		    "perception"));
	    else tmp = 2;
	    if(tmp != 2) continue;
	    if(effective_light(inv[i])-(adj/25) < 0) continue;
	    if(!msg || msg == "") message("mmin",query_mmin(),inv[i]);
	    else message("min", query_min(), inv[i]);
	}
	if(query_followers())
	    move_followers(prev);
    }
    describe_current_room(verbose_moves);
    if(this_object()->query("error report")) {
	printf("Runtime errors occurred in the execution.\n"
	  "Would you like to view the error report (y/n)? ");
	input_to((: call_other, "/cmds/system/handler", "handle_error" :), 0, this_object(),
	  (string)this_object()->query("error report"));
    }
}

void reset_euid() {
    seteuid(getuid());
}

void create() {
    more::create();
    seteuid(getuid());
    position = "player";
    wielded = ([]);
    level = 1;
    autosave = 500;
    set_weight(500);
    set_max_internal_encumbrance(2000);
    verbose_moves = 1;
    enable_commands();
}

void remove() {
    object *who_exc;

    who_exc = ({ this_object() });
    if(this_object()->query_invis())
	who_exc += filter_array(users(),"filter_notanarch",this_object());
    if(!count)
	if(previous_object() && previous_object() != this_object() && geteuid(previous_object()) != UID_ROOT && 
	  TP != TO) return;
    destroy_autoload_obj();
    CHAT_D->remove_user();
    INFORM_D->remove_user(this_object());
    if(!hiddenp(this_object()))
	if(count)
	    INFORM_D->do_inform("logins_and_quits","Info: " +
	      capitalize((string)this_object()->query_name()) +
	      " has left the game.",
	      who_exc);
    MULTI_D->quit(query_name());
    this_object()->tsh_cleanup();
    living::remove();
}

protected int quit(string str) {
    if (str) {
	notify_fail("Quit what?\n");
	return 0;
    }
    if(query_level() > 3 && query_current_attacker() && origin() != "call_out") {
	write("%^RED%^%^BOLD%^Quit set on "+
	  sprintf("%d", query_level()*4)+" second combat delay.");
	remove_call_out("quit");
	call_out("finish_quit", query_level() * 4, TP);
	return 1;
    }
    return finish_quit(TP);
}

protected int finish_quit(object ob) {
    crash_money = 0;
    count = 1;
    if(query_challenged())
	"/d/damned/arena/booths_room"->decline(this_object());
    if(arenap(TO) == 1)
	"/d/damned/arena/battle_room"->finish_battle(TO);
    if(ob->query_followers()) ob->clear_followers();
    if(environment(ob)) {
	if(!ob->query_ghost() || environment(ob)->query_property("no quit and save")) {
	    message("Nquit_save", ENV(ob)->query_short() && 
	      (string)ENV(ob)->query_short() != "" ?
	      "Setting start location to " + ENV(ob)->query_short() + "..." : 
	      "Setting start location here...", ob);
	    ob->setenv("start", file_name(environment(ob)));
	    if((string)ob->getenv("start") == BN(ENV(ob)))
		message("quit_save", "%^BLUE%^Successful.%^RESET%^", ob);
	    else message("quit_save", "%^RED%^Unsuccessful.%^RESET%^", ob);
	} else {
	    message("Nquit_save", "Setting start location to Akkad Church...", ob);
	    ob->setenv("start", "/d/standard/square");
	    if((string)ob->getenv("start") == "/d/standard/square")
		message("quit_save", "%^BLUE%^Successful.%^RESET%^", ob);
	    else message("quit_save", "%^RED%^Unsuccessful.%^RESET%^", ob);
	}
    }
    message("quit", "%^CYAN%^Reality suspended.  See you another time!%^RESET%^",
      ob);
 catch(CRASH_D->add_crash_items(this_object(), ({})));
    ob->set_attackers( ({ }) );
    ob->stop_hunting();
    count2 = 1;
    message("Nquit_save", "Saving...", ob);
    ob->save_player(ob->query_name());
    if(count2) {
	count2 = 0;
	message("quit_save", "%^BLUE%^Successful.%^RESET%^", ob);
    } else message("quit_save", "%^RED%^Unsuccessful.%^RESET%^", ob);
    message("quit", ob->query_cap_name() + " is gone from our reality.",
      environment(ob), ({ ob }) );
    log_file("enter", ob->query_name() + " (quit): " + ctime(time()) + "\n");
    PLAYER_D->add_player_info();
    ob->remove();
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
    set_hp(query_max_hp() / 2);
    set_mp(query_max_mp() / 2);
    set_heal_rate(2);
    borg = (mapping)RACE_D->body(this_object());
    for(i=0, max=sizeof(zippo=keys(borg)); i<max; i++) 
	add_limb(zippo[i], borg[zippo[i]]["limb_ref"],borg[zippo[i]]["max_dam"], 0, 0);
    set_wielding_limbs((string *)RACE_D->query_wielding_limbs(tmp));
    set_max_internal_encumbrance(query_stats("strength") * 30);
    set_fingers((int)RACE_D->query_fingers(tmp));
}

void setup() {
    string tmp, *mod_keys;
    mapping mods;
    object *who_exc, join_room, ob;
    int i;

    set_living_name(query_name());
    seteuid(getuid());
    set_heart_beat(1);
    if(!stats) init_stats();
    if(!skills) init_skills(0);
    if(!spells) init_spells();
    if(member_array(query_position(), MORTAL_POSITIONS) == -1) {
        log_file("driver/security", "enable_wizard^"+this_object()->query_name()+"^"+getuid(previous_object()) + "^" + ctime(time()) + "^" +this_object()->query_ip()+"\n");
	enable_wizard();
    }
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
    set_property("light", 0);
    set_max_internal_encumbrance(20*query_stats("strength"));
    if(!sizeof(query_aliases())) {
	message("environment", "Resetting alias data.", this_object());
	init_aliases();
	force_me("alias -reset");
    }
    set_property("light", 0);
    if(race) set("race", race);
    if(!query("race")) {
	sight_bonus = (int)RACE_D->query_sight_bonus(query("race"));
	move(ROOM_SETTER);
    }
    else {
	if(!primary_start) primary_start = getenv("start");
	if(!((tmp = getenv("start")) && stringp(tmp) &&
	    !catch(ob=load_object(tmp)) && move(ob)==MOVE_OK))
	    move(ROOM_START);
	setenv("start", primary_start);
    }
    if(!stringp(tmp = getenv("TERM"))) setenv("TERM", tmp = "unknown");
    term_info = (mapping)TERMINAL_D->query_term_info(tmp);
    write_messages();
    catch(CRASH_D->restore_crash_items(this_object()));
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
      query_money("mithril")+" mi, "+
      query_money("gold")+" gd, "+
      query_money("electrum")+" el, "+
      query_money("silver")+" sl, "+
      query_money("copper")+" cp\n");
    if(query_class() && stringp(query_class()) && query_class() != "child"
      && file_exists("/d/damned/guilds/join_rooms/"+query_class()+"_join.c")) {
	join_room = load_object("/d/damned/guilds/join_rooms/"+
	  query_class()+"_join");
	if(join_room && !join_room->is_member(query_name())) {
	    write("\n--**>> YOU HAVE BEEN KICKED OUT OF YOUR GUILD!!! <<**--\n");
	    mods = (mapping)join_room->query_property("guild mods");
	    if(mods) {
		mod_keys = keys(mods);
		for(i=0;i<sizeof(mod_keys);i++) {
		    if(this_object()->query_base_stats(mod_keys[i]))
			this_object()->set_stats(mod_keys[i], (int)this_object()->
			  query_base_stats(mod_keys[i]) -
			  mods[mod_keys[i]]);
		}
	    }
	    set_class("child");
	}
    }
    more(explode(NEWS_D->get_news(this_object()), "\n") );
    command("look");
    if(platinum || gold || silver || electrum || copper) {
	add_money("electrum", electrum);
	add_money("gold", gold);
	add_money("silver", silver);
	add_money("platinum", platinum);
	add_money("copper", copper);
	platinum = gold = electrum = silver = copper = 0;
    }
    reset_money();
    if(query_exp() < 0) {
	i = (int)query_property("xp mod");
	set_property("xp mod", 0);
	add_exp(-1 * query_exp());
	if(i) set_property("xp mod", i);
    }
    if((string)this_object()->query_race() && replace_string((string)this_object()->query_race(), "were", "") ==
      (string)this_object()->query_race()) {
	remove_property("lycanthrope moon");
	delete_skill("control change");
    }
    set_max_sp(query_stats("dexterity")*7);
    remove_property("reset max");
    HUNTING_D->set_mon_hunting(query_name(), this_object());
    REINC_D->check_reincarnate(this_object());
    convert_kills();
    if(this_object()->query_property("guild watch"))
	GUILD_D->set_last_on(query_class(), time());
}

// Added these lines so wizzes couldn't just call heart_beat() and
// get age.  - Geldron 051296
varargs protected void heart_beat(int recurs_flag) {
    object *inv, lyc_ob;
    string tod, *cns;
    int i, tmp;

    if(!recurs_flag) {
	player_age += 2;
	ok_to_heal++;
	tmp = (int)this_object()->query_property("extra heart beat");
	if(tmp < 0) {
	    tmp += 100;
	    if(tmp <= 0) return;
	    tmp = 20000/tmp;
	    if((100*player_age)%tmp > 10)
		return;
	} else if(tmp > 0) {
	    i = 1;
	    while(tmp > 100*i) {
		i++;
		this_object()->heart_beat(1);
	    }
	    if(tmp%100 != 0) {
		tmp = 20000/(tmp%100);
		if((100*player_age)%tmp <= 10) this_object()->heart_beat(1);
	    }
	}
    }
    else ok_to_heal--;
    if(disable) disable--;
    adjust_exp();
    if( (player_age > autosave) && (!wizardp(this_object())) ) {
	message("environment", "Autosaving.", this_object());
	inv = filter_array(all_inventory(this_object()), 
              (: call_other :), "query_short");
        inv = filter_array(inv, (: ((string)$1->query($2) == $3) :),
                           "protected by", (string)this_object()->
                           query_name());
        if(sizeof(inv) > 3) inv = inv[0..2];
        CRASH_D->add_crash_items(this_object(), inv);
        if(sizeof(inv)) {
          message("environment", "%^CYAN%^Items protected: "+
                  implode(map_array(inv, (: call_other :), "query_short"),
                          ", ")+".", this_object());
        }
	if(environment()) {
	    if(!ghost)
		setenv("start", file_name(environment()));
	    else setenv("start", "/d/standard/square");
	}
	save_player(query_name());
	autosave = player_age + 500;
    }
    if(sizeof(query_attackers()) && getenv("SCORE") != "off")
	message("my_combat", sprintf("hp: %d (%d)  mp: %d (%d)",
	    query_hp(), query_max_hp(), query_mp(), 
	    query_max_mp()), this_object());
    if(stringp(props["lycanthrope moon"]) && !this_object()->
      query("in creation")) {
	tod = (string)EVENTS_D->query_time_of_day();
	if(!this_object()->query_lyc_status() && (((int)ASTRONOMY_D->
	      query_phase(props["lycanthrope moon"]) == 3
	      && (tod == "night" || tod == "twilight")) ||
	    this_object()->query("control change"))) {
	    message("info", "%^RED%^You change into a huge "+
	      capitalize(replace_string(
		  (string)this_object()->query_race(), "were", ""))
	      + "!", this_object());
	    tell_room(environment(this_object()), capitalize(query_name()) +
	      " turns into a huge " +
	      capitalize(replace_string((string)this_object()->
		  query_race(), "were", ""))
	      + "!", ({ this_object() }));
	    lyc_ob = new("/std/spells/shadows/lyc_shadow");
	    lyc_ob->start_shadow(this_object());
	    this_object()->set("lyc ob", lyc_ob);
	    force_me("drop all");
	} else if(this_object()->query_lyc_status() && ((int)ASTRONOMY_D->
	    query_phase(props["lycanthrope moon"]) != 3
	    || (tod != "night" && tod != "twilight")) &&
	  !this_object()->query("control change")) {
	    message("info", "%^RED%^You cange back to normal form."
	      , this_object());
	    tell_room(environment(this_object()), capitalize(query_name())
	      + " changes back to normal form.", ({ this_object() }));
	    lyc_ob = (object)this_object()->query("lyc ob");
	    if(!objectp(lyc_ob)) message(
		  "info", "BUG in your lycanthropy.  "+
		  "Cannot dest shadow.", this_object());
	    else
		lyc_ob->external_destruct(lyc_ob);
	    this_object()->set("lyc ob", 0);
	}
    }
    if(!wizardp(this_object()) && (int)this_object()->query_hp() < 0
      && !this_object()->query_ghost())
	this_object()->die();
    if(!disable) continue_attack();
    if(magic_round > 0) magic_round--; else magic_round = 0;
    if(player_age > ok_to_heal) do_healing(calculate_healing());
    else calculate_healing();
    if(interactive(this_object()) && query_idle(this_object()) >= 3600 &&
      !wizardp(this_object()) ) {
	this_object()->force_me("quit");
    }
    if(!interactive(this_object())) net_dead();
}

void restore_arena_save() {
    if(base_name(previous_object()) != "/adm/daemon/arena_save_d")
	error("Illegal caller.\n");
    if(file_exists("/d/damned/arena/data/" +
	(string)query_name() + ".o"))
	restore_object("/d/damned/arena/data/" +
	  (string)query_name());
}

void set_property(string idx, mixed val) {
    if(!idx) return;
    if(member_array(idx, LOG_PROPS) >= 0 && !wizardp(this_object())
      && origin() != "local" && file_name(previous_object()) !=
      "/d/standard/setter") {
	log_file("set_"+replace_string(idx, " ", "_"),
          sprintf("-*> Property '%s' set in %s\n", idx, (string)this_object()->
	    query_name())+
	  sprintf("Value set to: %O\n", val)+
	  sprintf("Time: %s by: %s (uid: %s)\n", ctime(time()),
	    file_name(previous_object()), getuid(previous_object())));
	if(this_player())
	    log_file("set_"+replace_string(idx, " ", "_"),
	      sprintf("By user: %s\n", (string)this_player()->query_name()));
    }
    ::set_property(idx, val);
}

varargs void net_dead(int flag) {
    object *who_exc;

    if(!flag && query_current_attacker() && query_level() > 3) {
	delayed_call("net_dead", 4*query_level(), 1);
	return;
    }
    else
    if(flag && interactive(this_object())) return;
    catch(CHAT_D->remove_user(channels));
    INFORM_D->remove_user(this_object());
    who_exc = ({ this_object() });
    if(this_object()->query_invis())
	who_exc += filter_array(users(),"filter_notanarch",this_object());
    if(environment() && environment() == load_object(ROOM_FREEZER)) return;
    if(!hiddenp(this_object()))
	INFORM_D->do_inform("net_dead_players","Info: " +
	  capitalize((string)this_object()->query_name()) +
	  " goes net dead.", who_exc);
    if(query_challenged())
	"/d/damned/arena/booths_room"->decline(TO);
    if(arenap(TO) == 1)
	"/d/damned/arena/battle_room"->finish_battle(TO);
    channels = ({});
    net_died_here = file_name( environment(this_object()) );
    message("other_action", sprintf("%s suddenly disappears into a sea of "
	"irreality.", query_cap_name()), environment(this_object()), ({ this_object() }));
    this_object()->move_player(ROOM_FREEZER);
    if(query_snoop(this_object()))
	tell_object(query_snoop(this_object()), capitalize(query_name())+" has gone net-dead.");
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

void restart_heart_beat() {
    set_heart_beat(1);
}

nomask varargs void reduce_exp(int res_spell, int elapsed) {
    int lvl, gain_exp;

    lvl = this_object()->query_level();
    gain_exp = (int)ADVANCE_D->get_exp(lvl+1) - (int)ADVANCE_D->get_exp(lvl);
    if(lvl > 1 && lvl <= 5) gain_exp /= 7;
    else gain_exp /= 5;
    if(res_spell > 0) gain_exp /= 4;
    if(elapsed > 450) elapsed = 450;
    if(res_spell >= 0)
	gain_exp -= (2*(450 - elapsed)*gain_exp) / 1350;
    if(!exp_log) exp_log = ([]);
    exp_log += ([ time() : gain_exp ]);
    this_object()->add_exp(-gain_exp);
    return;
}

nomask void die() {
    object ob;
    object corpse;
    object money_ob;
    object *stuff;
    string *currs;
    int tmp;
    int i;
    function no_death;

    if(functionp(no_death = this_object()->query("death save"))) {
	(*no_death)(this_object(), query_current_attacker());
	return;
    }
    if(arenap(TO)) {
	if(arenap(TO) == 1)
	    "/d/damned/arena/battle_room"->finish_battle(TO);
	else if(arenap(TO) == 2 && TO && ENV(TO)) {
	    message("arena", "%^MAGENTA%^You have lost the battle!%^RESET%^", TO);
	    message("arena", "Transferring to the Arena Booths room...", TO);
	    TO->set_hp(1);
	    TO->set_attackers( ({ }) );
	    TO->stop_hunting();
	    this_object()->move_player("/d/damned/arena/booths_room");
	}
	return;
    }
    if(wizardp(this_object())) {
	message("death", "You are immortal and cannot die.", this_object());
	return;
    } else
      add_hp(1000);
    ob = this_object();
    died_here = environment(ob);
    force_me("unequip");
    message("death", "You die.\nYou feel the sensations of nothingness " +
      "as you rise above your corpse.\nYou arrive at a destination in a reality " +
      "not like your own.", this_object());
    while(this_object()->query_paralyzed()) this_object()->remove_paralyzed();
    DAMAGE_D->stop_cont_damage(this_object());
    message("death", query_cap_name()+ " dies a horrible death.",
      environment());
    if(previous_object()->is_player() && previous_object() != this_object()) {
      reduce_skills();
    }
    corpse = new(OB_CORPSE);
    corpse->set_name(query_cap_name());
    corpse->move(environment(this_object()));
    this_object()->set("corpse ob", corpse);
    money_ob = new("/std/obj/coins");
    currs = query_currencies();
    for(i=0, tmp= sizeof(currs); i<tmp; i++) {
	money_ob->set_money(currs[i], query_money(currs[i]));
	add_money(currs[i], -query_money(currs[i]));
    }
    money_ob->move(corpse);
    stuff = all_inventory(this_object());
    for(i=0;i<sizeof(stuff);i++) {
	if(stuff[i]->id("corpse")) stuff[i]->move(environment(this_object()));
	if((int)stuff[i]->query_always_keep()) {
	    if(!keep) keep = ({});
	    keep += ({ explode(base_name(stuff[i]), "#")[0] });
	    stuff[i]->remove();
	    continue;
	}
	if(!stuff[i]->drop()) stuff[i]->move(corpse);
    }
    catch("/daemon/pk_d"->add_player_kill(query_attackers()[0]));
    cease_all_attacks();
    ghost = 1;
    setenv("start", "/d/standard/square");
    save_player( query_name() );
    PLAYER_D->add_player_info();
}

void set_challenged(int arg) {
    if(BN(PO) != "/d/damned/arena/booths_room")
	error("Illegal caller.\n");
    challenged = arg;
}

int query_challenged() {
    return challenged;
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

    if(query_ghost()) return "The ghost of "+capitalize(query_name());
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
    log_file("driver/security", "set_position^"+this_object()->query_name()+"^"+getuid(previous_object()) + "^" + ctime(time()) + "^" +this_object()->query_ip());
    if(geteuid(previous_object()) != UID_ADVANCE && geteuid(previous_object()) != UID_ROOT) {
      log_file("driver/security", "^FAIL"+"\n");
      return;
    }
    if(member_array(position, MORTAL_POSITIONS) != -1)
	if(member_array(pos, MORTAL_POSITIONS) == -1) enable_wizard();
    if(position == "high mortal" && pos != "high mortal")
	CASTLE_D->disable_high_mortal(this_object());
    init_path();
    position = pos;
    log_file("driver/security", "^SUCCESS"+"\n");
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

   if (!msg) 
      return;
    if(!stringp(str=getenv("SCREEN"))) str = "75";
    x = atoi(str);
    if(msg_class[0] == 'N') msg_class = msg_class[1..sizeof(msg_class)-1];
    else if(msg_class != "prompt") msg = wrap(msg, x);
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
	    msg = "%^RED%^%^BOLD%^"+pre+":%^RESET%^"+post;
	break;
    case "shout":
	if(sscanf(msg, "%s:%s", pre, post) == 2)
	    msg = "%^BLUE%^%^BOLD%^"+pre+":%^RESET%^"+post;
	break;
    case "mmin": case "min": case "mmout": case "mout":
	msg = "%^GREEN%^%^BOLD%^"+msg; break;
    case "living_item": msg = "%^RED%^%^BOLD%^"+msg; break;
    case "inanimate_item": msg = "%^MAGENTA%^%^BOLD%^"+msg; break;
    }
    if(!term_info) reset_terminal();
    if(query_colour(msg_class) &&
      query_colour(msg_class) != "")
	msg = ansi_convert(query_colour(msg_class)) + eliminate_colour(msg);
    for(i=0, max=sizeof(words=explode(msg, "%^")); i<max; i++)
	if(term_info[words[i]]) words[i] = term_info[words[i]];
    receive(implode(words, "")+term_info["RESET"]);
}

void set_guild(string str) {
    guild = str;
    if(!guild) guild = str;
}

string query_guild() { return guild; }

void set_guild_security(string str) {
    if(!str) guild_security = "";
    else guild_security = str;
}

string query_guild_security() {
    if(!guild_security) guild_security = "";
    return guild_security;
}

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

nomask void reset_quests() {
    log_file("reset_quests", sprintf("Quests reset for: %s\nBy:%s UID: %s\n%s\n",
	query_name(), file_name(previous_object()),
	getuid(previous_object()), ctime(time())));
    if(this_player())
	log_file("reset_quests", "By: "+(string)this_player()->query_name()+"\n");
    player_data["general"]["quest points"] = 0;
    quests = ({});
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

void cheap_revive() {
    string *tmp;
    int i;

    if(BN(PO) != RESURRECT) return;
    new_body();
    ghost = 0;
    if(!keep) tmp = ({});
    else tmp = keep;
    keep = ({});
    for(i = 0; i < sizeof(tmp); i++)
	new(tmp[i])->move(TO);
}

void revive() {
    object corpse, *inv, env;
    string *tmp;
    int i, x, flag, elapsed;

    flag = 0;
    elapsed = 450;
    corpse = this_object()->query("corpse ob");
    message("my_action", "You return from the dead!", this_object());
    if(objectp(corpse)) {
	message("my_action", "Your soul is SUCKED back into your body.",
	  this_object());
	env = environment(corpse);
	while(environment(env)) env = environment(env);
	this_object()->move(env);
	elapsed = (int)corpse->query_elapsed();
	inv = all_inventory(corpse);
	i = sizeof(inv);
	while(i--)
	    inv[i]->move(this_object());
	if(env)
	    corpse->move(env);
	corpse->remove();
    } else {
	message("my_action", "Your soul has no corpse to gate into "+
	  "the prime material plane!\nYour soul loses energy in "+
	  "escaping.", this_object());
	flag = 1;
    }
    if(flag) this_object()->reduce_exp(-1, );
    else if(!previous_object() || !previous_object()->res_spell())
	this_object()->reduce_exp();
    else this_object()->reduce_exp(1);
    message("other_action", query_cap_name()+ " returns from the dead.",
      environment());
    //  tell_room(environment(this_object()), query_cap_name()+" returns from the dead.\n", this_object());
    new_body();
    ghost = 0;
    if(!keep) tmp = ({});
    else tmp = keep;
    keep = ({});
    for(x = 0; x < sizeof(tmp); x++)
	new(tmp[x])->move(TO);
}

int query_ghost() { return ghost; }

void add_quest_points(int pts) {
    if(!wizardp(this_object()) && pts > 0 && geteuid(previous_object()) != UID_ROOT)
	return;
    if(!player_data["general"]["quest points"])
	player_data["general"]["quest points"] = 0;
    player_data["general"]["quest points"] += pts;
    if(pts > 0)
	log_file("quests", query_name() + " received "+sprintf("%d quest points ",
	    pts)+"from "+file_name(previous_object()) +
	  " ("+getuid(previous_object())+")\nDate: "+ctime(time())+"\n");
    return;
}

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
    add_money("gold", to_int(crash_money*currency_rate("gold")));
    message("info", "You recover some of your lost money.", this_player());
    crash_money = 0;
}

int query_birthday() { return birth; }


void set_colours(mapping tmp) {
    colours = tmp; 
}

void set_colour(string tmp1, string tmp2) {
    if(!colours) colours = ([]);
    colours[tmp1] = tmp2;
}

void remove_colour(string tmp1) {
    map_delete(colours, tmp1);
}

string query_colour(string tmp) { 
    if(!colours) return "";
    return colours[tmp];
}

mapping query_colours() {
    if(!colours) return ([]);
    return colours;
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

void set_bank(string str, int val) {
    if(BN(PO)[0..28] != "/d/damned/akkad/obj/mon/silas") return;
    if(!bank) bank = ([]);
    if(val < 1) map_delete(bank, str);
    else
	bank[str] = val;
}

nomask int query_bank(string str) {
    if(!bank) return 0;
    return bank[str];
}

nomask mapping query_bank_total() {
    if(!bank) return ([]);
    return bank;
}

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

nomask void reinit_channels() {
    CHAT_D->remove_user(channels);
    register_channels();
    return;
}
private register_channels() {
    string *dir, class_name, join_room, gm;
    int i;

    if(wizardp(this_object())) {
	dir = get_dir("/d/damned/guilds/join_rooms/");
	i = sizeof(dir);
	channels = ({ });
	while(i--)
	    if(sscanf(dir[i], "%s_join.c", class_name)) channels += ({ class_name });
    }
    else
	channels = ((query_class() &&
	    query_class() != "child") ? ({ query_class() }) : ({}));
    if(query_guild()) channels += ({ query_guild() });
    if(query_level() > 1) channels += ({ "arena" });
    gm = "/d/damned/guilds/join_rooms/"+(string)this_player()->query_class()+"_join.c";
      gm = catch( (string)gm->query_master());
    if(this_player()->query_name() == gm) channels += ({ "council" });

    if(wizardp(this_object()) ||
      high_mortalp(this_object())) channels += ({ "newbie", "high-mortal" });
    else
	channels += ({ "newbie" });
    if(wizardp(this_object()) ||
      (int)this_object()->query_level() > 3) channels += ({ "darke" });
    if(wizardp(this_object())) channels += ({ "cre", "intercre"});
    if(archp(this_object())) channels += ({ "admin", "council" });
    if(!wizardp(this_object()) && this_object()->query_class()) {
	join_room = "/d/damned/guilds/join_rooms/"+
	(string)this_object()->query_class()+"_join";
	if(file_exists(join_room+".c") && (string)join_room->query_master() ==
	  (string)this_object()->query_name() && wizardp(this_object())) channels  += ({ "council" });
	if((string)GUILD_D->query_gm(query_class()) ==
	  (string)this_object()->query_name() ) channels += ({ "new-council" });
    }
    CHAT_D->add_user(channels);
}
private setup_inform() {
    if(!inf_block) inf_block = ({});
    inf_heard = ({ "net_dead_players","logins_and_quits","murders" });
    inf_heard += ({ "impending_shutdowns" });
    if(wizardp(this_object())) inf_heard += ({ "auto_removed_users",
	  "typos,_bugs,_etc." });
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

    if(!(ob = load_object(str))) return;
    if(clonep(ob)) return;
    setenv("start", primary_start = str);
}

string query_primary_start() { return primary_start; }

void clean_net_dead() {
    if(base_name(previous_object()) != ROOM_FREEZER) return;
    this_object()->remove();
}

int filter_notanarch(object ob) { return !archp(ob); }

nomask mapping query_exp_log() {
    if(!exp_log) exp_log = ([]);
    return exp_log;
}
