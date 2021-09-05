//	/std/living/body.c
//	from the Nightmare mudlib
//	the code for the limbs system used by the Nightmare mudlib
//	based on the TMI LOS system by Brian with help from Buddha
//	written by Descartes of Borg september 1992
// Modified by Geldron 030996 to reduce bugs in hp
// Modified by Geldron 031096 to work with arena
// Modified by Geldron 031196 for arena limbs and stuff

#include <std.h>
#include <daemons.h>
#include <objects.h>
#define LIMB_NICKS ([ "arm" : ({ "branch", "tentacle", }), "leg" : ({ "root", "tentacle", }), "hand" : ({ "root", "tentacle", "paw", "pincer", }), "foot" : ({ "root", "tentacle", "paw", }), ])

inherit CONTAINER;


mixed *deaths;
mapping player_data, magic, severed, healing, body;
nosave mapping wielded;
nosave string target_limb;
string *limbs;
int all_my_fingers;
nosave int num_wielded;
private int heal_rate;

void set_wielding_limbs(string *str);
int check_on_limb(string limb);
string *query_wielding_limbs();
string type_ok(string type, string limb);
void init_limb_data();
void set_fingers(int x);
int query_fingers();
void set_max_hp(int hp);
void set_hp(int hp);
void add_hp(int x);
void set_max_sp(int x);
void set_sp(int x);
void add_sp(int x);
void add_mp(int x);
void set_max_mp(int x);
void set_mp(int x);
void set_overall_ac(int ac);
void set_ac(string limb_name, int ac);
int add_limb(string limb_name, string limb_ref, int max_dam, int curr_dam, int limb_ac);
int remove_limb(string limb_name);
int query_max_sp();
int query_max_hp();
int query_hp();
int query_sp();
int query_max_mp();
int query_mp();
int query_ac(string limb_name, string type);
int do_damage(string limb_name, int damage);
string query_reference(string limb);
string equip_weapon_to_limb(object weap, string limb1, string limb2);
string equip_armour_to_limb(object arm, string *limb);
object query_weapon(string limb);
string *query_armour(string limb);
object *query_armour_ob(string limb);
int remove_armour_from_limb(object arm, string *limb);
int remove_weapon_from_limb(object ob);
string *query_limbs();
string return_limb();
string return_target_limb();
int query_is_limb(string limb);
void check_crippled(string limb);
void uncripple(string limb);


//  This function initializes the variables
//  This should only be called when there is NO limb data yet

void init_limb_data() {
    if(!player_data)
      player_data = ([ "general":([ "hp":1, "max_hp":50, "sp":1, "max_sp":1, "exp buffer" : 0])]);
    body = ([]);
    limbs = ({});
    wielded = ([]);
    severed = ([]);
    healing = ([]);
    if(!magic) magic = ([]);
}

int query_flying() {
  int i;

  if(!limbs) return 0;
  i = sizeof(limbs);
  while(i--)
    if(member_array("wing", explode(limbs[i], " "), 4) >= 0) return 1;
  return 0;
}

void set_max_hp(int hp) {
    int i;
    string *what_limbs;

    if(!player_data) player_data = ([]);
    if(!player_data["general"]) player_data["general"] = ([]);
    player_data["general"]["max_hp"] = hp;
    what_limbs = query_limbs();
    for(i=0;i<sizeof(what_limbs);i++)
	if(body[what_limbs[i]] && RACE_D->query_max_dam(what_limbs[i],
	    (string)this_object()->query_race()))
	    body[what_limbs[i]]["max_dam"] = hp/(int)RACE_D->query_max_dam(
		what_limbs[i],(string)this_object()->query_race());
}

void set_hp(int hp) {
    if(!query_max_hp()) player_data["general"]["max_hp"] = hp;
    if(hp> query_max_hp()) {
	if(this_object()->is_player()) player_data["general"]["hp"] = query_max_hp();
	else {
	    player_data["general"]["max_hp"] = hp;
	    player_data["general"]["hp"] = hp;
	}
    }
    else player_data["general"]["hp"] = hp;
}

void add_hp(int x) {
    int can_exceed;

    can_exceed = previous_object() && (int)previous_object()->
	can_exceed_max_hp() && previous_object() != this_object();
	  if(!can_exceed && x > 0) {
	    if(x + player_data["general"]["hp"] > player_data["general"]["max_hp"])
	      x = player_data["general"]["max_hp"] - player_data["general"]["hp"];
	    if(x < 0) x = 0;
	  }
    player_data["general"]["hp"] += x;
/*  Added by Geldron 030996 to prevent people to have -hps while still
    alive.  I would have checked to see if it were lower than 1, but if it were
    0, which might happen (dunno maybe the guy had a new char). */
/*
if(player_data["general"]["hp"] < 0) {
    this_object()->die();
    return;
}
*/
}

void set_max_sp(int x) { 
    player_data["general"]["max_sp"] = x;
}

void set_sp(int x) {
    if(!query_max_sp()) set_max_sp(x);
    if(x > query_max_sp()) player_data["general"]["sp"] = query_max_sp();
    else player_data["general"]["sp"] = x;
}

void add_sp(int x) {
    if(!player_data["general"]["sp"])  
	set_sp(x);
    else player_data["general"]["sp"] += x;
    if(player_data["general"]["sp"] > query_max_sp())
	player_data["general"]["sp"] = query_max_sp();
    if(player_data["general"]["sp"] < -200)
	player_data["general"]["sp"] = -200;
}

void set_max_mp(int x) {
    if(!magic) magic = ([]);
    magic["max points"] = x;
}

int query_max_mp() { return magic["max points"]; }

void set_mp(int x) { 
    if(!query_max_mp()) magic["max points"] = x;
    if(x > query_max_mp()) {
	if(this_object()->is_player()) {
	    magic["points"] = magic["max points"];
	}
	else {
	    magic["max points"] = x;
	    magic["points"] = x;
	}
    }
    else magic["points"] = x;
    if(magic["points"] < 0) magic["points"] = 0;
}

void add_mp(int x) {
    int can_exceed;
    can_exceed = previous_object() && previous_object()->can_exceed_max_mp() && previous_object() != this_object();
    if(!can_exceed && x > 0) {
	if(x + magic["points"] > magic["max points"])
	    x = magic["max points"] - magic["points"];
	if(x < 0) x = 0;
    }
    if(!magic) magic = ([ "points":0, "max points":0 ]);
    magic["points"] += x;
    if(magic["points"] < -200) magic["points"] = -200;
}

int query_mp() { return magic["points"]; }

//  This function is only for lazy wizzes to use when creating monsters!!

void set_overall_ac(int x) {
    int i;

    for(i=0; i<sizeof(limbs); i++) {
	if(limbs[i] != "torso") set_ac(limbs[i], x);
	else set_ac("torso", (2*x));
    }
}

void set_ac(string limb_name, int ac) {
    body[limb_name]["ac"] = ac;
}


//  This function adds limbs to a player or monster
//  The values passed to it mean the following:
//  limb_name: The name of the limb being added
//  limb_ref: The name of other limbs that should be lost when this
//      limb is lost, or FATAL is losing it is fatal, or "" if nothing
// 	else should happen.
//  max_dam:  The maximum damage the limb can take before being lost
//  curr_dam:  How much damage has been done to the limb thus far.  This
//	allows you to create wounded monsters.
//  limb_ac:  How much protection the limb has.  By setting it to >0, you
//	set it's natural ac
//  NOTE:  In order to make a creature with 4 arms, each arm must have a
//	distinctive name, like "lower right arm", "right arm", "lower left
//	arm", "lower right arm"

int add_limb(string limb_name, string limb_ref, int max_dam, int curr_dam, int limb_ac) {
    body[limb_name] = ([ "limb_ref": limb_ref, "max_dam": max_dam, "damage": curr_dam, "ac": limb_ac ]);
    if(!limbs) limbs = ({});
    limbs += ({ limb_name });
    body[limb_name]["armour"] = ({});
    body[limb_name]["armour_ob"] = ({});
    body[limb_name]["weapons"] = 0;
    if(!severed) return 1;
    if(severed[limb_name]) map_delete(severed, limb_name);
    return 1;
}

void augment_body(int x) {
    int tmp;
    int i;
    string b_type;

    if((string)this_object()->query_race() == "satyr") {
	if(member_array("left foot", limbs) != -1) {
	  remove_limb("left foot");
	  map_delete(severed, "left foot");
	}
	if(member_array("right foot", limbs) != -1) {
	  remove_limb("right foot");
	  map_delete(severed, "right foot");
	}
	if(severed && severed["left foot"])
	  map_delete(severed, "left foot");
	if(severed && severed["right foot"])
	  map_delete(severed, "right foot");
    }
    b_type = this_object()->query_body_type();
    if(b_type != 0)   // must be a monster  added by Val June 30, 1993
    {
      for(i=0, tmp = sizeof(limbs); i<tmp; i++)
	body[limbs[i]]["max_dam"] =
	  query_max_hp()/((int)RACE_D->query_max_mon_dam(limbs[i],b_type));
      return;
    }
    for(i=0, tmp = sizeof(limbs); i<tmp; i++)
      body[limbs[i]]["max_dam"] =
	query_max_hp()/((int)RACE_D->query_max_dam(limbs[i],
	  (string)this_object()->query_race()));
}

//  This function removes a limb

int remove_limb(string limb_name) {
    if(body[limb_name]) {
	severed[limb_name] = body[limb_name]["limb_ref"];
	map_delete(body, limb_name);
	limbs -= ({ limb_name });
	return 1;
    }
    return 0;
}

//  This function returns a random limb

string return_limb() {
    return limbs[random(sizeof(limbs))];
}

//  This function returns a random limb, giving weight to
//  those limbs that can take the most damage

string return_target_limb() {
    string tmp_lmb;
    int res, i, count;

    if(target_limb && !query_is_limb(target_limb)) target_limb = 0;
    if(target_limb) {
	tmp_lmb = target_limb;
	target_limb = 0;
	if(random(120) > (int)this_object()->query_skill("defense"))
	  return tmp_lmb;
    }
    count = member_array("torso", limbs);
    i = 0;
    while(i++ < ((sizeof(limbs))/2))
      if( (res=random(sizeof(limbs))) == count ) break;
    return limbs[res];
}

string *query_limbs() { return limbs; }

/* Added by Geldron 031196 for arena */
void set_crippled(string limb) {
    if(!body[limb]) return;
    body[limb]["crippled"] = 0;
}

int query_crippled(string limb) { if(!body[limb]) return 0;
    return body[limb]["crippled"];
}

int query_is_limb(string limb) {
    if(member_array(limb, limbs) != -1) return 1;
    else return 0;
}

int query_max_hp() { return player_data["general"]["max_hp"]; }

int query_sp() { 
    return 1000;
}

int query_max_sp() {
    if(!player_data["general"]["max_sp"]) return 0;
    return player_data["general"]["max_sp"];
}

int query_hp() {
    return player_data["general"]["hp"];
}

int query_max_dam(string limb) {
    if(body[limb]) return body[limb]["max_dam"];
}

/* Added by Geldron 031196 for arena */
void set_dam(string limb, int damage) {
    if(body[limb]) body[limb]["damage"] = damage;
}

int query_dam(string limb) {
    if(body[limb]) return body[limb]["damage"];
}

string query_reference(string limb) {
    if(body[limb]) return body[limb]["limb_ref"];
}

int do_damage(string limb, int damage) {
    int amt;

    if(damage < 0) return 0;
    if(body[limb]) {
	amt = body[limb]["max_dam"] *2 + 5 - body[limb]["damage"];
	if(damage > amt) damage  = (amt < 0)? 10:amt;
	body[limb]["damage"] += damage;
	player_data["general"]["hp"] -= damage;
	if(body[limb]["damage"] < 0) body[limb]["damage"] = 0;
	return damage;
    }
    return 0;
}

int query_ac(string limb, string type) {
    int tmp_ac, i;

    if(!type || member_array(type, DAMAGE_TYPES) < 0)
	type = DAMAGE_TYPES[0];
    if(!body) return 0;
    if(!body[limb]) return 0;
    tmp_ac = body[limb]["ac"];
    i = sizeof(body[limb]["armour_ob"]);
    while(i--)
	tmp_ac += (body[limb]["armour_ob"][i] ? 
	  (int)body[limb]["armour_ob"][i]->query_ac(type) : 0);
    if(this_object()->query_property("natural ac"))
	tmp_ac += (int)this_object()->query_property("natural ac");
    return tmp_ac;
}

/* Patched to equip weapons as armours as well to keep query_ac
   in line with new type-specific damage -Diewarzau
*/

string equip_weapon_to_limb(object weap, string limb1, string limb2) {
    int i;

    if(member_array(limb1, limbs) == -1)
	return "You have no "+limb1+"!\n";
    if(wielded[limb1]) return "You are already wielding something in your "+limb1+"!\n";
    if(member_array("shield", body[limb1]["armour"]) != -1)
	return "You cannot wield a weapon while wearing a shield in your "+limb1+".\n";
    if(limb1 == limb2) return "You can only use "+limb1+ " once, silly.\n";
    if(!body[limb1]["wieldable"])
	return "You cannot wield a weapon with a "+limb1+"!\n";
    if(body[limb1]["crippled"])
	return "Your "+limb1+" is crippled!\n";
    i = sizeof(limbs);
    while(i--)
	if(mapp(body[limbs[i]]) && body[limbs[i]]["limb_ref"] == limb1 &&
	  body[limbs[i]]["crippled"])
	    return "Your "+limbs[i]+" is crippled!\n";
    if(limb2) {
	if(!body[limb2])
	    return "You do not have a "+limb2+"!\n";
	if(wielded[limb2])
	    return "You are wielding something with your "+limb2+".\n";
	if(member_array("shield", body[limb2]["armour"]) != -1)
	    return "You cannot wield a weapon where you wear a shield.\n";
	if(!body[limb2]["wieldable"]) 
	    return "You cannot wield anything with your "+limb2+".\n";
	i = sizeof(limbs);
	while(i--)
	    if(mapp(body[limbs[i]]) && body[limbs[i]]["limb_ref"] == limb2 &&
	      body[limbs[i]]["crippled"])
		return "Your "+limbs[i]+" is crippled!\n";
	if(body[limb2]["crippled"])
	    return "Your "+limb2+" is crippled!";
	body[limb2]["weapons"] = weap;
	body[limb2]["armour_ob"] += ({ weap });
	wielded[limb2] = weap;
    }
    body[limb1]["weapons"] = weap;
    body[limb1]["armour_ob"] += ({ weap });
    wielded[limb1] = weap;
    num_wielded ++;
    return 0;
}

string equip_armour_to_limb(object arm, string *limb) {
    int i;
    function f, *funx;
    string y, type;

    type = (string)arm->query_type();
    i = sizeof(limb);
    while(i--) if(y=type_ok(type, limb[i])) return y;
    if(functionp(f = (mixed)arm->query_wear()) && !((*f)())) return "NO";
    if(pointerp(funx = arm->query_wear())) {
	i = sizeof(funx);
	while(i--) {
	    if(!functionp(funx[i])) continue;
	    if(!(*funx[i])()) return "NO";
	}
    }
    i = sizeof(limb);
    while(i--) {
	body[limb[i]]["armour"] += ({type});
	body[limb[i]]["armour_ob"] += ({ arm });
    }
    return 0;
}

string type_ok(string type, string limb) {
    int worn, x, i;

    if(!body[limb]) return "You have no "+limb+"!\n";
    if(member_array(type, body[limb]["armour"]) != -1) worn = 1;
    else worn = 0;
    switch(type) {
    case "shield":
	if(worn) return "You are already wearing a shield!\n";
	else if(wielded[limb])
	    return "You cannot wear a shield while you are wielding a weapon in your "+limb+".\n";
	else return 0;
	break;
    case "body armour":
	if(member_array("armour", body[limb]["armour"]) != -1)
	    return "You cannot wear regular armuur with body armour.\n";
	else if(worn) return "You are already wearing body armour!\n";
	else return 0;
	break;
    case "ring":
	i = 0;
	x = sizeof(body[limb]["armour"]);
	while(x--) 
	    if(body[limb]["armour"][x] == "ring") i++;
	if(i > all_my_fingers-1) 
	    return "You do not have that many fingers on your "+limb+".\n";
	else return 0;
	break;
    case "armour":
	if(member_array("body armour", body[limb]["armour"]) != -1)
	    return "You may not wear body armour and armour at the same time.\n";
	else if(worn) return "You may only wear one piece of body armour at a time.\n";
	else return 0;
	break;
    default:
	if(worn) return "You may only wear one armour of type "+type+" at a time.\n";
	else return 0;
	break;
    }
    return "Error in wearing armour.\n";
}

string *query_armour(string limb) {
    if(body[limb]) return body[limb]["armour"];
    else return 0;
}
object *query_armour_ob(string limb) {
	if(body[limb]) return body[limb]["armour_ob"];
	else return 0;
}

int remove_weapon_from_limb(object ob) {
    int i;
    string *ind;

    ind = keys(wielded);
    for(i=0; i<sizeof(ind); i++) {
	if(wielded[ind[i]] == ob) {
	    map_delete(wielded, ind[i]);
	    body[ind[i]]["armour"] -= ({ "weapon" });
	    if(member_array(ob,body[ind[i]]["armour_ob"]) > -1)
		body[ind[i]]["armour_ob"] -= ({ ob });
	    body[ind[i]]["weapons"] = 0;
	}
    }
    num_wielded --;
    return 1;
}

int remove_armour_from_limb(object arm, string *limb) {
    int i, arm_class;
    string type;

    arm_class = (int)arm->query_ac("crushing");
    type = (string)arm->query_type();
    for(i=0; i<sizeof(limb); i++) {
	if(!body) continue;
	if(!body[limb[i]]) continue;
	if(member_array(type, body[limb[i]]["armour"]) != -1)
	    body[limb[i]]["armour"] -= ({ type });
	body[limb[i]]["armour_ob"] -= ({ arm });
    }
    return 1;
}

void set_fingers(int x) {
    all_my_fingers = x;
}

int query_fingers() { return all_my_fingers; }

void set_wielding_limbs( string *str) {
    int i;

    for(i=0; i<sizeof(str); i++) {
	if(member_array(str[i], limbs) == -1) continue;
	body[str[i]]["wieldable"] = 1;
    }
}

void add_wielding_limb(string str) {
    if(member_array(str, limbs) == -1) return;
    body[str]["wieldable"] = 1;
}

string *query_wielding_limbs() {
    string *tmp;
    int i;
    tmp = ({});

    for(i=0; i<sizeof(limbs); i++) {
	if(body[limbs[i]]["wieldable"]) 
	    tmp += ({ limbs[i] });
    }
    return tmp;
}

void fix_limbs() {
    int i;

    for(i=0; i<sizeof(limbs); i++) {
	body[limbs[i]]["armour"] = ({});
	body[limbs[i]]["armour_ob"] = ({});
	body[limbs[i]]["weapon"] = 0;
	check_crippled(limbs[i]);
    }
    wielded = ([]);
}

void check_crippled(string limb) {
    string *tmp;
    int i, flag;

    if(!body || !body[limb]) return;
    if(body[limb]["crippled"]) {
	flag = 0;
	i = sizeof((tmp = (string *)this_object()->query_wielding_limbs()));
	while(i--)
	    if(body[tmp[i]]["limb_ref"] == limb) flag = 1;
	if(flag) { this_object()->add_stat_bonus("dexterity",-10); return; }
	if(body[limb]["wieldable"]) {
	    this_object()->add_stat_bonus("dexterity", -10);
	    return; }
	if(body[limb]["limb_ref"] != "FATAL")
	    this_object()->add_stat_bonus("strength", -10);
    }
    return;
}


string *query_severed_limbs() {
    if(!severed) return ({});
    else return keys(severed);
}

object *query_wielded() { return (wielded ? values(wielded) : ({})); }

object query_weapon(string limb) {
    if(!wielded) return 0;
    if(!wielded[limb]) return 0;
    return wielded[limb];
}

//  initialize the variables

void init_complex_body() {
    target_limb = "";
    if(!heal_rate) heal_rate = 2;
}

void heal(int x) {
    int i;

    if(!player_data) player_data = ([]);
    if(!player_data["general"]) player_data["general"] = ([]);
    if(player_data["general"]["hp"] < 1) return;
    if(x < 0 && player_data["general"]["hp"] + x <= 0)
	x = 1 + -1 * player_data["general"]["hp"];
    add_hp(x);
    for(i=0; i<sizeof(limbs); i++) {
	body[limbs[i]]["damage"] -= x;
	if(body[limbs[i]]["damage"] < body[limbs[i]]["max_dam"] &&
	    body[limbs[i]]["crippled"]) {
	    message("info","Your "+limbs[i]+" is no longer crippled.",
		this_object());
	    body[limbs[i]]["crippled"] = 0;
	    uncripple(limbs[i]);
	}
	if(body[limbs[i]]["damage"] < 0) body[limbs[i]]["damage"] = 0;
    }
}

void heal_limb(string str, int x) {
    if(member_array(str, limbs) == -1) return;
    if(!body[str]) return;
    body[str]["damage"] -= x;
    if(body[str]["damage"] < 0) body[str]["damage"] = 0;
    if(body[str]["crippled"] && body[str]["damage"] < body[str]["max_dam"]) {
	message("info","Your "+str+" is no longer crippled.",
	    this_object());
	body[str]["crippled"] = 0;
	uncripple(str);
    }	
}

void uncripple(string limb) {
    string *tmp;
    int flag, j;

    if(!body || !body[limb] || !mapp(body[limb])) return;
    if(body[limb]["wieldable"]) { this_object()->add_stat_bonus(
	"dexterity",10); return; }
    tmp = (string *)this_object()->query_wielding_limbs();
    j = sizeof(tmp);
    flag = 0;
    while(j--)
	if(body[tmp[j]]["limb_ref"] == limb) flag = 1;
    if(flag) { this_object()->add_stat_bonus("dexterity",10);
	return; }
    if(!flag && !body[limb]["wieldable"] && 
	body[limb]["limb_ref"] != "FATAL")
	this_object()->add_stat_bonus("strength",10);
    body[limb]["crippled"] = 0;
}

int set_heal_rate(int x) {
    heal_rate = x;
    return x;
}

int query_heal_rate() { return heal_rate; }

int severed_limb(string limb) {
     int temp;
    string childlimb;

    if(!body[limb]) return 0;
    childlimb = body[limb]["limb_ref"];
    if(childlimb == "FATAL") {
	player_data["general"]["hp"] = -1;
	return 2;
    }
    if(childlimb == "") {
	remove_limb(limb);
	return 1;
    }
    if(!body[childlimb]) {
	remove_limb(limb);
	return 1;
    }
    temp = body[childlimb]["damage"];
    if(temp < body[childlimb]["max_dam"]) {
	body[childlimb]["damage"] = body[childlimb]["max_dam"]*2+5;
	check_on_limb(childlimb);
    }
    this_object()->set_severed(limb);
    remove_limb(limb);
    return 1;
}

int check_on_limb(string limb) {
    object weap;
    object *arm;
    string tmp;
    int i;

    object old_limb;

if(!body) body = ([]);
    if(!body[limb]) return 0;
    if(body[limb]["damage"] <= 2*body[limb]["max_dam"] &&
	body[limb]["damage"] > body[limb]["max_dam"] &&
	body[limb]["limb_ref"] != "FATAL") {
	if(body[limb]["crippled"]) return 1;
if(wizardp(this_object())) {
	    message("my_combat",sprintf("Your wizhood saved you from getting "
		"your %s crippled.",limb),this_object());
	    return 1;
	}
	message("my_combat","%^RED%^Your %^RESET%^"+limb+" %^RED%^ is "
		"crippled!", this_object());
	tell_room(environment(this_object()),(string)this_object()->
	    query_cap_name() + "'s " + limb + " is crippled!",
({ this_object() }) );
	body[limb]["crippled"] = 1;
	check_crippled(limb);
	weap = wielded[limb];
	if(weap) {
	    message("my_combat","You drop " + (string)weap->query_short() +".",
		this_object());
	    remove_weapon_from_limb(weap);
	    weap->set_not_equipped();
	    weap->move(environment(this_object()));
	}
	if((tmp = body[limb]["limb_ref"]) != "" && mapp(body[tmp])) {
	    if(body[tmp]["damage"] < body[tmp]["max_dam"])
	    body[tmp]["damage"] = (body[tmp]["max_dam"]/2)*3;
return 0;
/* return check_on_limb(tmp); */
}
return 1;
}
if(body[limb]["damage"] > 2*body[limb]["max_dam"] ||
  (body[limb]["limb_ref"] == "FATAL" && body[limb]["damage"] >
    body[limb]["max_dam"])) {
    if(wizardp(TO)) {
	message("my_combat", sprintf("If you were not immortal, you would "
	    "lose your %s right now!", limb), this_object());
	return 1;
    }
    if(limb == "torso")
	message("my_combat", "%^RED%^A mortal blow is dealt to your body!", this_object());
    else message("my_combat", "%^RED%^%^BOLD%^"+sprintf("Your %s is severed!", limb),
	  this_object());
    if(limb == "torso") tell_object(environment(this_object()),this_object()->query_cap_name()+" is dealt a mortal blow to the torso!", this_object());
    else tell_room(environment(this_object()),this_object()->query_cap_name()+" has "+this_object()->query_possessive()+" "+limb+" severed!", this_object());
    weap = wielded[limb];
    if(weap) {
	remove_weapon_from_limb(weap);
	weap->set_not_equipped();
    }
    arm = body[limb]["armour_ob"];
    if(arm) {
	for(i = 0; i<sizeof(arm); i++) {
	    if(!arm[i] || !objectp(arm[i])) continue;
	    remove_armour_from_limb(arm[i], (string *)arm[i]->query_actual_limbs());
	    arm[i]->set_not_equipped();
	}
    }
    if(limb != "torso") {
	old_limb = new(OB_BODY_PART);
	old_limb->set_limb(query_cap_name(), limb);
	old_limb->move(environment(this_object()));
    }
    if(weap) weap->move(old_limb);
    if(arm) {
	for(i=0; i<sizeof(arm); i++) {
            if(!arm[i]) continue;
	    arm[i]->move(old_limb);
	}
    }
    heal_rate = -3;
    return severed_limb(limb);
}
return 1;
}

void add_kill(string str) {
    int x;

    if(!str) return;
    if(!player_data["kills"]) player_data["kills"] = ({});
    if(member_array((x=(int)PLAYER_D->add_kill(str)), player_data["kills"]) !=
      -1 || x== -1) return;
    player_data["kills"] += ({ x });
}

int *query_kills() { return player_data["kills"]; }

void add_death(string str) {
    if(!str) return;
    if(!deaths) deaths = ({ ({str, time()}) });
    else deaths += ({ ({ str, time() }) });
}
mixed *query_deaths() { return deaths; }


void clear_kills() { player_data["kills"] = ({}); }

void set_target_limb(string str) {
    if(!stringp(str)) return;
    if(member_array(str, limbs) == -1) return;
    target_limb = str;
}

string sub_limb(string str) {
    int i, k, l;
    string *nix, *words;

    if(!stringp(str)) return 0;
    if(member_array(str, limbs) >= 0) return str;
    nix = keys(LIMB_NICKS);
    i = sizeof(nix);
    while(i--) {
	words = explode(str, " ");
	if(member_array(nix[i], words) >= 0) {
	    for(k=0;k<sizeof(limbs);k++) {
		l = sizeof(LIMB_NICKS[nix[i]]);
		if(sizeof(words) == 1 && member_array(words[0], 
		    explode(limbs[k]," ")) >= 0)
		    return limbs[k];
		while(l--)
		    if(member_array(LIMB_NICKS[nix[i]][l], explode(limbs[k], " "))
		      >= 0) return limbs[k];
	    }
	}
    }
    return 0;
}

