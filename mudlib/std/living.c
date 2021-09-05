//	/std/living.c
//	from the Nightmare mudlib
//	inheritable code for living things
//	created by Descartes of Borg september 1992
// Modified by Geldron 030696 to add did_command(x) and fix add_exp2() so
// it actually _will_ log
// Modified by Geldron 031096 to work with arena
// Modified by Geldron 031096 to work with equip/unequip


#include <security.h>
#include <daemons.h>
#include <party.h>
#include <ritual.h>
#include <dirs.h>
#include <living.h>

#define TIME_TO_HEAL 12
#define HEALING_FORMULA (stats["strength"]+stats["constitution"]+stats["charisma"])*2

int invis, ok_to_heal, player_age;
nosave int forced, spiritual, physical;
nosave int next_lev_exp;
int sight_bonus;
string description;
nosave string ritual;
nosave string party;
nosave string *search_path;
nosave int login_flag;
private string gender;
mapping stats;
nosave mapping stat_bonus;
mapping languages;
string primary_language;
mapping language_exp;
nosave int dev_rate_int;

private int xp_to_next_dev;

void did_command(string str);
void add_exp2(int x);
void set_stats(string str, int x);
void set_alignment(int x);
void add_alignment(int x);
void adj_alignment(int x);
void adjust_biorhythms();
void set_invis();
int query_physical();
int query_spiritual();
int query_alignment();
int query_stats(string str);
int query_stat_bonus(string stat);
int query_base_stats(string stat);
int query_poisoning();
int query_potion_healing();
void add_poisoning(int x);
void add_potion_healing(int x);
string query_gender();
void set_gender(string str);
string query_objective();
string query_possessive();
string query_subjective();
int query_stoned();
int query_intox();
int query_stuffed();
int query_quenched();
int query_invis();
void adjust_exp();
int max_buffer_size();
string query_ritual();
string query_party();
string query_long(string unused);
protected void init_path();
int get_dev_rate();
void remove();

protected void init_living() {
    add_action("cmd_hook", "", 1);
    init_path();
    init_attack();
}

void override_add_exp(int exp) {
    object ob;

    ob = this_object();
    if(!this_object()->is_player()) 
        return;
    player_data["general"]["experience"] += exp;
    if(player_data["general"]["experience"] > player_data["general"]["max exp"] || !player_data["general"]["max exp"])
	{
	    player_data["general"]["max exp"] =	(player_data["general"]["experience"] - 1);
//ADDED NEW DEV SYS TO HERE (parnell feb99)
       if(!query("no add dev") && dev_rate_int = get_dev_rate())
		{
            if(!xp_to_next_dev) 
                xp_to_next_dev = player_data["general"]["max exp"] + dev_rate_int;
            while( player_data["general"]["max exp"] > xp_to_next_dev )
            {
                xp_to_next_dev += dev_rate_int;
                this_object()->add_dev(1);
            }
        }

	}
    ob = previous_object();
    if(ob && file_name(ob) != "/cmds/mortal/_advance")
	log_file("override_exp", "Override exp: "+exp+" to "+query_name()+".\n"+ "Object: "+file_name(ob)+"  Uid: "+getuid(ob)+"\n");
    if(this_player() && this_player() != this_object())
	    log_file("override_exp", sprintf("By: %s  Euid: %s\n", this_player()->query_name(), geteuid(this_player())));
    this_object()->add_exp2(0);
    return;
}

int buffer_full() {
    if((player_data["general"]["exp buffer"] * 100 / max_buffer_size()) < 95) return 0;
    return 1;
}

void adjust_exp() {
    int buff, rate;
    if(!this_object()->is_player()) return;
    buff = player_data["general"]["exp buffer"];
    if(!buff) return;
    if(!next_lev_exp)
	next_lev_exp = (int)ADVANCE_D->get_exp((int)this_object()->query_level() + 1) -
	(int)ADVANCE_D->get_exp((int)this_object()->query_level());
    rate = 2+next_lev_exp/11000;
    if((int)this_object()->query_level() > 19) rate /= 2;
    rate += rate * query_skill("quick study") / 200;
    if(environment() && environment()->query_property("quick study"))
	rate *= 2;
    if(!login_flag && player_data["general"]["last adjust"]) {
	rate *= (time() - player_data["general"]["last adjust"]) / 10;
	login_flag = 1;
    }
    player_data["general"]["last adjust"] = time();
    if(rate > buff) rate = buff;
    buff -= rate;
    player_data["general"]["exp buffer"] = buff;
    player_data["general"]["experience"] += rate;

    if(player_data["general"]["experience"] >
      player_data["general"]["max exp"] ||
      !player_data["general"]["max exp"])
	{
	player_data["general"]["max exp"] =
	(player_data["general"]["experience"] - 1);
//GOING TO ADD DEV CODE HERE (did add it - Parnell feb99)
//changed to fix various HB bugs
       if(!query("no add dev") && dev_rate_int = get_dev_rate())
		{
if(!xp_to_next_dev) xp_to_next_dev = player_data["general"]["max exp"] + dev_rate_int;
		while( player_data["general"]["max exp"] > xp_to_next_dev )
		{
		xp_to_next_dev += dev_rate_int;
		this_object()->add_dev(1);
		}
                }
	}
    return;
}

void reset_xp_to_dev()
{
 if(previous_object()->is_player()) return;
 xp_to_next_dev = player_data["general"]["max exp"] + dev_rate_int;
 return;
}

void reset_max_exp() {
    if(!this_player() || !archp(this_player())) return;
    player_data["general"]["max exp"] = player_data["general"]["experience"] - 1;
    return;
}

nomask int at_max_exp() {
    if(player_data["general"]["experience"] >=
      player_data["general"]["max exp"]) return 1;
    return 0;
}

int percent_buffer() {
    return 100 * player_data["general"]["exp buffer"] /
    max_buffer_size();
}

int max_buffer_size() {
    int gain_exp, lev;

    if((lev=(int)this_object()->query_level()) <= 1) return 500;
    gain_exp = (int)ADVANCE_D->get_exp(lev+1) -
    (int)ADVANCE_D->get_exp(lev);
    if(lev < 5) gain_exp /= 10;
    else
	gain_exp /= 20;
    gain_exp += gain_exp * query_skill("quick study") / 100;
    return gain_exp;
}

protected void init_path() {
    string tmp;

    search_path = ({ DIR_MORTAL_CMDS, DIR_CLASS_CMDS });
    if(tmp = (string)this_object()->query_guild())
	search_path += ({ DIR_GUILD_CMDS+"/"+tmp });
    if(ambassadorp(this_object()) ||
      wizardp(this_object()))
	search_path += ({ DIR_AMBASSADOR_CMDS, DIR_SYSTEM_CMDS });
    if(high_mortalp(this_object()) || wizardp(this_object()))
	search_path += ({ DIR_HM_CMDS });
      if(legendp(this_object()) || wizardp(this_object()))
          search_path += ({ DIR_LEGEND_CMDS });
    if(wizardp(this_object())) {
	search_path += ({ DIR_CREATOR_CMDS });
	if(file_size(user_path(query_name()) + "bin") == -2)
	    search_path += ({ user_path(query_name())+"bin" });
        if(member_group( (string)this_object()->query_name(), "mentor") ||
           member_group( (string)this_object()->query_name(), "superuser") )
	    search_path += ({ DIR_MENTOR_CMDS });
	if(archp(this_object()))
	    search_path += ({ DIR_ADMIN_CMDS });
    }
}

protected void init_stats() { stats = ([]); }

/* Added did_command(cmd); so shadowing would work more efficiently
 * - Geldron 030696
 */
nomask protected int cmd_hook(string cmd) {
    string file, verb, abcmd;

    verb = query_verb();
    did_command(cmd);
    if(!cmd) abcmd = 0;
    else if(sizeof(explode(cmd, " ")) > 4)
      abcmd = implode(explode(cmd, " ")[0..3], " ");
    else abcmd = cmd;
    if( verb != "quit" && query_paralyzed()) {
	message("my_action", sprintf("%s", (string)this_player()->query_paralyze_message()),
	  this_player());
	return 1;
    }
    if(!(file = (string)CMD_D->find_cmd(verb, search_path))) {
	if(!((int)SOUL_D->do_cmd(verb, abcmd))) 
	    return (int)CHAT_D->do_chat(verb, cmd);
	else return 1;
    }
    return (int)call_other(file, "cmd_"+verb, cmd);
}

void did_command(string str) {
}

int force_me(string cmd) {
    string tmp;
    int res;

    if(previous_object())
	tmp = geteuid(previous_object());
    else tmp = "";
    /*  Added by Geldron 031096 to work with equip/unequip */
    if(PO && BN(PO) && (BN(PO) == "/cmds/mortal/_equip" || 
	BN(PO) == "/cmds/mortal/_unequip" || BN(PO) ==
	"/d/damned/arena/booths_room")) {
	res = command(cmd);
	return res;
    }
    if(this_object()->is_player() && origin() != "local" &&
      (tmp != UID_ROOT && tmp != UID_FORCE && tmp != geteuid()) &&
      (!previous_object() || (object)previous_object()->query_caster() != this_object()))
	return 0;
    forced = 1;
    res = command(cmd);
    forced = 0;
    return res;
}

void reduce_stats() {
    string *ind;
    int i;

    if(!stats) return;
    ind = keys(stats);
    for(i=0; i<sizeof(ind); i++)
	if(random(100) < (int)this_object()->query_level())
	    set_stats(ind[i], query_base_stats(ind[i])-1);
}

int sort_limbs(string one, string two) {
    string a_one, a_two;
    mapping tmp;

    tmp = (mapping)RACE_D->query_limb_info(one, query("race"));
    if(!mapp(tmp))
	a_one = "";
    else 
	a_one = tmp["attach"];
    tmp = (mapping)RACE_D->query_limb_info(two, query("race"));
    if(!mapp(tmp))
	a_two = "";
    else
	a_two = tmp["attach"];
    if(a_one == two) return -1;
    if(a_two == one) return 1;
    return strcmp(one, two);
}

void do_healing(int x) {
    int tmp, i;
    mapping limb_info;
    string *severed;

    if(this_object() && this_object()->is_player()) {
	this_object()->add_exp2(0);
	//     ^ Checks for level advancement. (Diewarzau 4/9/96)
	tmp = query_physical();
	if(tmp < 0) tmp = 0;
	else if(tmp > 2) tmp = 2;
	add_hp(tmp);
	tmp = query_spiritual();
	if(tmp < -2) tmp = -2;
	else if(tmp > 2) tmp = 2;
	add_mp(tmp);
    }
    if(!query_poisoning())
	heal(query_heal_rate());
    if(intp(props["base hp regen"])) tmp = props["base hp regen"];
    else tmp = 3;
    tmp += x / 30;
    if(intp(props["extra hp regen"]))
	tmp += props["extra hp regen"] * tmp / 100;
    if(x < 20 && this_object()->is_player()) tmp /= 3;
    set_heal_rate(tmp);
    if(intp(props["base mp regen"])) tmp = props["base mp regen"];
    else tmp = 3;
    tmp += x/50;
    tmp += query_stoned() / 3;
    if(intp(props["extra mp regen"]))
	tmp += props["extra mp regen"] * tmp / 100;
    if(x < 20 && query_stoned() < 10 && this_object()->is_player()) tmp /= 3;
    add_mp(tmp);
    adjust_biorhythms();
    if(query_poisoning()) {
	message("info","%^GREEN%^You take poison damage.",this_object());
	this_object()->add_hp(-(1 + (int)this_object()->query_poisoning() / 5));
    }
      if(query_potion_healing()) {
          message("info","%^WHITE%^%^BOLD%^You feel much better.",this_object());
          this_object()->add_hp((1 + (int)this_object()->query_potion_healing() / 5));
      }
    if(this_object()->is_player() && (string)this_object()->
      getenv("SCORE") != "off" && !(query_hp() >= query_max_hp() &&
	query_mp() >= query_max_mp()))
	message("info","hp: "+query_hp()+" ("+query_max_hp()+")  mp: "+
	  query_mp() + " ("+query_max_mp()+")", this_object());
    if(this_object()->query_property("limb regen") &&
      !this_object()->query_ghost()) {
	severed = (string *)this_object()->query_severed_limbs();
	i = sizeof(severed);
	if(!healing) healing = ([]);
	while(i--) {
	    if(member_array(severed[i], (string *)this_object()->query_limbs())
	      >= 0) continue;
	    if(undefinedp(healing[severed[i]+" regen"]))
		healing[severed[i]+" regen"] = 5 + random(10);
	    if(healing[severed[i]+" regen"] <= 0) {
		limb_info=
		(mapping)RACE_D->query_limb_info(severed[i],(string)this_object()->
		  query("race"));
		if(!limb_info) continue;
		if(limb_info["attach"] && limb_info["attach"] != "0" &&
		  member_array(limb_info["attach"], (string *)this_object()->
		    query_limbs()) < 0) {
		    healing[severed[i]+" regen"] = 5 + random(10);
		    continue;
		}
		this_object()->add_limb(severed[i], limb_info["ref"],
		  query_max_hp() / limb_info["max"], 0, 0);
		if(member_array(severed[i], (string *)RACE_D->
		    query_wielding_limbs((string)this_object()->query("race"))) != -1)
		    this_object()->add_wielding_limb(severed[i]);
		message("info", "%^CYAN%^%^BOLD%^Your "+severed[i]+" grows back.",
		  this_object());
	    } else
		healing[severed[i]+" regen"]--;
	}
    }
    ok_to_heal = player_age + TIME_TO_HEAL;
}

void set_severed(string limb) {
    if(!this_object()->query_property("limb regen")) return;
    if(!healing) healing = ([]);
    healing[limb + " regen"] = 5 + random(10);
    return;
}

int calculate_healing() {
    int borg;
    string msg;

    if(query_intox()) {
	healing["intox"] --;
	if(healing["intox"] < 0) healing["intox"] = 0;
	if(!healing["intox"]) {
	    write("Suddenly you get a bad headache.");
	}
	else if(3> random(101)) {
	    borg = random(4);
	    switch(borg) {
	    case 0: msg = "stumble"; break;
	    case 1: msg = "hiccup"; break;
	    case 2: msg = "look"; break;
	    case 3: msg = "burp"; break;
	    }
	    write("You "+msg+(msg=="look" ? " drunk." : "."));
           say(query_cap_name()+" "+msg+"s"+(msg == "look" ? " drunk." : "."));
	}
    }
    if(query_stoned()) {
	healing["stoned"]--;
	if(healing["stoned"] <= 0) healing["stoned"] = 0;
	else if(3 > random(101)) {
	    borg = random(4);
	    switch(borg) {
	    case 0: msg = "stares at the clouds, even if there are none."; break;
	    case 1: msg = "says: Woooooooah, I am sooooo wasted."; break;
	    case 2: msg = "contemplates: what do stupid people think when "+
		"they are stoned?"; break;
	    case 3: msg = "dances about in a Jim Morrison-esque fashion."; break;
	    }
	    say(query_cap_name() + " "+msg);
	}
    }
    if(query_stuffed()) {
	healing["stuffed"]--;
	if(healing["stuffed"] < 0) healing["stuffed"] = 0;
    }
    if(query_quenched()) {
	healing["quenched"]--;
	if(healing["quenched"] < 0) healing["quenched"] = 0;
    }
    if(query_poisoning() > 0) add_poisoning(-1);
      if(query_potion_healing() > 0) add_potion_healing(-1);
    return query_intox()+query_stuffed()+query_quenched();
}

void set_ritual(string str) {
      if(getuid(previous_object()) != UID_ROOT) return;
      ritual = str;
}

void set_party(string str) {
    if(getuid(previous_object()) != UID_ROOT) return;
    party = str;
}

int add_stoned(int x) {
    if(!healing) healing = ([]);
    if(!healing["stoned"]) healing["stoned"] = 0;
    if((healing["stoned"] + x) > 300) {
	healing["stoned"] = 300;
	return 0;
    }
    healing["stoned"] += x;
    return 1;
}

int query_stoned() {
    if(!healing || undefinedp(healing["stoned"])) return 0;
    return healing["stoned"];
}


void add_poisoning(int x) {
    if(!healing) healing = ([]);
    healing["poisoning"] += x;
    if(healing["poisoning"] < 0) healing["poisoning"] = 0;
}

  void add_potion_healing(int x) {
      if(!healing) healing = ([]);
      healing["potion"] += x;
      if(healing["potion"] < 0) healing["potion"] = 0;
  }

void set_stats(string str, int x) {
    if(!stats) stats = ([]);
    if(stats[str] && stats[str] != x) {
	log_file("stats", query_name()+" went from "+stats[str]+" to "+x+
	  " in "+str+" ("+ctime(time())+")\n");
	log_file("stats", "uid: "+getuid(previous_object())+" ("+
	  file_name(previous_object())+")\n");
    }
    if(str == "strength")
	this_object()->set_max_internal_encumbrance(30 * x);
    stats[str] = x;
}

void set_invis() {
    if(invis) {
	invis = 0;
	write("You step out of the shadows.");
	say(query_mvis());
    }
    else {
	write("You fade into the shadows.");
	say(query_minvis()+"");
	invis = 1;
    }
}

void set_description(string str) { description = str; }

void add_sight_bonus(int x) { sight_bonus += x; }

void add_search_path(string dir) {
    log_file("paths", query_name()+": "+dir+" ("+ctime(time())+")\n");
    if(member_array(dir, search_path) == -1) search_path += ({ dir });
}

void delete_search_path(string dir) {
    if(getuid(previous_object()) != UID_ROOT) return;
    if(member_array(dir, search_path) != -1) search_path -= ({ dir });
}

void add_exp(int x) {
    if(x>0 && query_party()) {
	PARTY_OB->calculate_exp(party, x, previous_object());
	return;
    }
    add_exp2(x);
    return;
}

void add_exp2(int x) {
    int i, new_lev, flag;

    flag = 0;
    if( x > 0 && intp(this_object()->query_property("xp mod")))
	x += x * (int)this_object()->query_property("xp mod") / 100;
    if(x >= 0 && this_object()->is_player()) {
	player_data["general"]["exp buffer"] += x;
	if(player_data["general"]["exp buffer"] >=
	  max_buffer_size()) {
	    player_data["general"]["exp buffer"] =
	    max_buffer_size();
	    flag = 1;
	}
    }
    else {
	player_data["general"]["experience"] += x;
	if(player_data["general"]["experience"] < 0)
	    player_data["general"]["experience"] = 0;
    }
    if(wizardp(this_object()) || !this_object()->is_player()) return;
    new_lev = (int)ADVANCE_D->advance(this_object());
    if(new_lev > (int)this_object()->query_level())
    {
	this_object()->set_level(new_lev);
	next_lev_exp = ((int)ADVANCE_D->get_exp(new_lev + 1) -
	  (int)ADVANCE_D->get_exp(new_lev));
    }
    if(x < 0) return;
    i = x/2000;
    if(i>5) i = 5 + (i-5)/15;
    if(i>8) i = 8 + (i-8)/30;
    if(i>15) i = 15;
    if(!query("no add dev") && !flag && this_object()->at_max_exp() && x > 0) {
	while(i--)
	    if(random(100) < 80) this_object()->add_dev(1);
	i = (x%2000) / 20 + 1;
	if(random(100) < i)
	    this_object()->add_dev(1);
    }
    if(x >= 15000) {
	log_file("exp",
	  query_name()+" received "+x+" exp from "+ (string)previous_object()->query_name()+"\n");
	log_file("exp",
	  "(uid: "+getuid(previous_object())+" "+file_name(previous_object())+": "+
	  ctime(time())+"\n");
    }
}

void add_dev(int x) {
    int old_dev;

if(query("no add dev")) return;
    if(!query_property("dev points")) set_property("dev points", 0);
    old_dev = query_property("dev points");
    set_property("dev points", old_dev + x);
    if(wizardp(this_object()) || !this_object()->is_player()) return;
    if(x > 10 && previous_object() != this_object()) {
	log_file("dev", query_name() + " received " + x + " dev points from "+
	  (string)previous_object()->query_name()+"\n");
	log_file("dev", "Uid: "+geteuid(previous_object())+" "+
	  file_name(previous_object())+" "+ctime(time())+"\n");
    }
    return;
}

void fix_exp(int x, object tmp) {
    if(getuid(previous_object()) != UID_ROOT) return;
    add_exp2(x);
    if(tmp && living(tmp) && member_array(this_object(),
	(object *)tmp->query_hunted()) < 0) return;
    if(x> 15000) {
	log_file("exp",
	  query_name()+" received "+x+" exp in party: "+party+" from "+
	  (string)tmp->query_short()+"\n");
	log_file("exp", 
	  "(uid: "+getuid(tmp)+" "+file_name(tmp)+"): "+ctime(time())+"\n");
    }
    if(wizardp(this_object()) || !this_object()->is_player()) return;
}

void add_alignment(int x) {
    if(x>40) x = 40;
    else if(x<-40) x = -40;
    player_data["general"]["alignment"] += x;
    if(query_alignment() > 1500) player_data["general"]["alignment"] = 1500;
    if(query_alignment()< -1500) player_data["general"]["alignment"] = -1500;
}

void adj_alignment(int x) {
    add_alignment(x/200-x/50);
}

int add_intox(int x) {
    if(x>0) x = x*3 + x/2;
    if(x+healing["intox"] > HEALING_FORMULA) return 0;
    else healing["intox"] += x;
    if(healing["intox"] < 0) healing["intox"] = 0;
    return 1;
}

int add_stuffed(int x) {
    if(x>0) x = x*3;
    if(x+healing["stuffed"] > HEALING_FORMULA) return 0;
    else healing["stuffed"] += x;
    if(healing["stuffed"] < 0) healing["stuffed"] = 0;
    return 1;
}

int add_quenched(int x) {
    if(x>0) x = x*3;
    if(x+healing["quenched"] > HEALING_FORMULA) return 0;
    else healing["quenched"] += x;
    if(healing["quenched"] < 0) healing["quenched"] = 0;
    return 1;
}

void add_stat_bonus(string stat, int amount) {
    if(!stat_bonus) stat_bonus = ([]);
    if(stat_bonus[stat]) stat_bonus[stat] += amount;
    else stat_bonus[stat] = amount;
    if(!stat_bonus[stat]) map_delete(stat_bonus, stat);
}

string query_long(string unused) {
    object *inv;
    string *tmp;
    string pre, stuff, extra, reg, short;
    int i, x;

    if(this_object()->query_ghost()) return "An ethereal presence.\n";
    reg = "";
    pre = "You look over the "+query_gender()+" "+query("race")+".\n";
    if(::query_long("junk")) pre += ::query_long("junk")+"\n";
    if(description) pre += query_cap_name()+" "+description+"\n";
    if(severed) tmp = keys(severed);
    if(tmp && sizeof(tmp)) {
	reg += query_cap_name()+" is missing a "+tmp[0];
	if(sizeof(tmp) > 1) {
	    if(sizeof(tmp) != 2) reg += ",";
	    for(i=1; i<sizeof(tmp); i++) {
		if(i== sizeof(tmp)-1) reg += " and a ";
		reg += " " +tmp[i];
		if(i != sizeof(tmp)-1) reg +=",";
	    }
	}
	reg += ".\n";
    }
    else reg += query_cap_name() + " has no missing limbs.\n";
    x = ((player_data["general"]["hp"]*100)/player_data["general"]["max_hp"]);
    reg += "%^CYAN%^%^BOLD%^" + capitalize(query_subjective());
    if(x>90) reg += "%^CYAN%^%^BOLD%^ is in top shape.%^RESET%^\n";
    else if(x>75) reg += "%^CYAN%^%^BOLD%^ is in decent shape.%^RESET%^\n";
    else if(x>60) reg += "%^CYAN%^%^BOLD%^ is slightly injured.%^RESET%^\n";
    else if(x>45) reg += "%^CYAN%^%^BOLD%^ is hurting.%^RESET%^\n";
    else if(x>30) reg += "%^CYAN%^%^BOLD%^ is badly injured.%^RESET%^\n";
    else if(x>15) reg += "%^CYAN%^%^BOLD%^ is terribly injured.%^RESET%^\n";
    else reg += "%^CYAN%^%^BOLD%^ is near death.%^RESET%^\n";
    inv = filter_array(all_inventory(this_object()),
      (: (stringp(call_other($1, $2)) && !living($1)) :),
      "query_short");
    if(inv && sizeof(inv)) {
	reg += capitalize(nominative(this_object())) + " is carrying:\n";
	reg += implode(map_array(inv, (: call_other :), "query_short"),
	  "\n")+"\n";
    }
    else reg += capitalize(nominative(this_object())) + " is carrying nothing.\n";
    return pre+reg;
}

int query_stat_bonus(string stat) {
   int x;
   if(!stats) return 0;
   if(stat_bonus) x=stat_bonus[stat];
   else x=0;
   return x;
}

int query_stats(string stat) {
    int x;

    if(!stats) return 0;
    if(stat_bonus) x= stat_bonus[stat];
    else x = 0;
    return stats[stat] + x;
}

int query_base_stats(string stat) {
    if(!stats || !stats[stat]) return 0;
    else return stats[stat];
}


nomask int query_forced() { return forced; }

string *query_search_path() {
    if(previous_object() != this_object() && geteuid(previous_object()) != UID_ROOT) return search_path + ({});
    else return search_path;
}

int query_invis() { return invis; }

int query_exp() { return player_data["general"]["experience"]; }


int query_alignment() { return player_data["general"]["alignment"]; }

int query_intox() {
    if(healing) return healing["intox"];
    else return 0;
}

int query_stuffed() {
    if(healing) return healing["stuffed"];
    else return 0;
}

int query_quenched() {
    if(healing) return healing["quenched"];
    else return 0;
}

int query_poisoning() {
    if(healing) return healing["poisoning"];
    else return 0;
}

    int query_potion_healing() {
      if(healing) return healing["potion"];
      else return 0;
  }

string query_ritual() { return ritual; }

string query_party() { return party; }

string query_al_title() {
    int al;

    al = player_data["general"]["alignment"];
    if(al > 1000) return "saintly";
    else if(al >750) return "righteous";
    else if(al >500) return "good";
    else if(al > 280) return "benevolent";
    else if(al > 135) return "nice";
    else if(al > -135) return "neutral";
    else if(al > -280) return "mean";
    else if(al > -500) return "malevolent";
    else if(al > -750) return "bad";
    else if(al > -1000) return "evil";
    else return "demonic";
}

int query_sight_bonus() { return sight_bonus; }

int query_age() { return player_age; }
string *query_all_stats() { return keys(stats); }

string query_description() { return description; }

void remove() {
    int i;
    object *inv;

    inv = all_inventory(this_object());
    for(i=0; i<sizeof(inv); i++) {
	if(!inv[i]) continue;
	if(inv[i]->drop() && inv[i]) inv[i]->remove();
	/* Some objects call remove() in drop() */
    }
    ::remove();
}

void adjust_biorhythms() {
    float omega1, omega2, temps;

    temps = to_float(player_age)/ 400.0;
    if(!intp(this_object()->query_stats("wisdom")) ||
      !intp(this_object()->query_stats("intelligence"))) return;
    omega1 = to_float((int)this_object()->query_stats("wisdom"));
    omega2 = to_float((int)this_object()->query_stats("intelligence"));
    spiritual = to_int(5.0*sin(temps*omega1) + 5.0*cos(temps*omega2));
    omega1 = to_float((int)this_object()->query_stats("strength"));
    omega2 = to_float((int)this_object()->query_stats("dexterity"));
    physical = to_int(5.0*sin(temps*omega1) + 5.0*cos(temps*omega2));
}

int query_spiritual() { return spiritual; }

int query_physical() { return physical; }

string query_primary_lang() {
    if(!primary_language && living(this_object()) &&
      !this_object()->is_player()) return "common";
    return primary_language;
}

void set_primary_lang(string str) { primary_language = str; }


// language system added by Valodin in August 1993

int query_lang_prof(string lang) 
{ 
    if(wizardp(this_object())) return 10;
    if(!languages)
	return 0;
    if(!languages[lang]) 
	return 0;
    return languages[lang];
}

int set_lang_prof(string lang, int i)
{
    if(!languages)
	languages = ([]);
    if(i > 10)
	i = 10;
    languages[lang] = i;
    return languages[lang];
}

string *query_all_languages()
{
    if(!languages)
	return ({});
    return keys(languages);
}

int remove_language(string lang)
{
    if(!languages || !languages[lang])
	return 0;
    map_delete(languages, lang);
    return 1;
}


// each mapping element is an array of two ints
// The first is the number of language points amassed.  When this reaches
// (lang_prof + 1) ^ 4, the language prof advances and the language points
// are decreased.  The second number is the number of excess exp points
// have been spent.  When exp is spent, it is converted to lang_pts based
// on intelligence.  it takes (40 - int) exp pts to make one lang_pt
// -Valodin

void learn_language(string lang, int exp)
{
    int tot_exp, tmp, goal, lang_pts, intel_fac;

    if(lang == "coderish" && !wizardp(this_object()))
	return;
    if(mapp(languages) && languages[lang] >= 10) return;
    if(!language_exp)
	language_exp = ([]);
    if(!language_exp[lang] || !pointerp(language_exp[lang]))
	language_exp[lang] = ({ 0, 0});
    tot_exp = (language_exp[lang][0] += exp);
    intel_fac = (110 - query_stats("intelligence"));
    if(intel_fac > 25) intel_fac = 20;
    if(intel_fac <= 0) intel_fac = 5;
    language_exp[lang][1] = tot_exp / intel_fac;
    language_exp[lang][0] = tot_exp % intel_fac;
    if(language_exp[lang][1] >= (to_int(pow(to_float(languages[lang]),1.1)) +1)) {
	language_exp[lang][1] = 0;
	languages[lang] += (languages[lang] >= 10)?0:1;
    }
    return;
}

int *query_lang_exp(string lang)
{
    if(!language_exp)
	return ({ 0, 0 });
    if(!language_exp[lang])
	return ({ 0, 0});
    return language_exp[lang];
}

void set_gender(string str) { 
    if(str != "male" && str != "female" && str != "neuter") return;
    gender = str;
}

string query_gender() { return (gender ? gender : "neuter"); }

string query_subjective() { return nominative(gender); }

string query_possessive() { return possessive(gender); }

string query_objective() { return objective(gender); }

int get_dev_rate()
{
int GDlev;
GDlev = (int)this_object()->query_level();
if(!GDlev) return 0;
if(GDlev == 1) return 250;
	
		next_lev_exp = (int)ADVANCE_D->get_exp(GDlev + 1) -
		(int)ADVANCE_D->get_exp(GDlev);
	
	  if(GDlev < 10) {
	 return  (next_lev_exp / (GDlev*20));
	  }  
	  else {
	    if(GDlev <18) { 
	 return (next_lev_exp / (GDlev*50));
	      }
	    else {
	     return (next_lev_exp / (GDlev*100));
	    }
	  }
	

}
