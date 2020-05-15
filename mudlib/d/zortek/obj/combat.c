//	/std/living/combat.c
//	from the Nightmare mudlib
//	code for combat
//	based upon the LOS combat by Buddha@TMI
//	DarkeLIB 0.1
//	Diewarzau 1994
// Modified by Misery 031096 to fix HeartBeat prob
// Modified by Misery 031096 to work with arena

#include <std.h>
#include <party.h>
#include <daemons.h>
#include <council.h>
#include <balance.h>
#include "/wizards/misery/def.h"

#define MAX_MELEE_WC	    20
#define MAX_ATTACK_BONUS    5
#define NEWBIE_LEVEL 3
#define KILL_RANGE 5
#define DEFAULT_PARALYZE_MESSAGE "You are stiff as stone."
#define UNDEAD_RACES ({ "undead", "skeleton", "zombie", "vampire", \
      "ghoul", "ghost" })
#define CRIT_TYPES ({ "A", "B", "C", "D", "E" })

inherit BODY;
inherit SKILLS;

nosave int paralyzed, magic_round;
nosave int hunting;
int wimpy;
private string wimpydir;
private nosave object *attackers;
private nosave int any_attack;
private nosave int casting;
private nosave object *hunters;
private nosave mapping magic_protection;
private nosave string paralyze_message;
private nosave string target_thing;
private nosave object current;

int ok_to_kill(object targ);
void miss(string how, string type);
void set_casting(int x);
void set_paralyzed(int x, string drow);
void remove_paralyzed();
int query_paralyzed();
int query_casting();
int clean_up_attackers();
mapping get_damage(object weap);
void run_away();
void send_messages(string *mesgs);
void set_wimpydir(string str);
void set_wimpy(int x);
string query_wimpydir();
int query_wimpy();
int kill_ob(object victim, int which);
int sight_adjustment(object who);
void continue_attack();
void update_current_skill();
void cease_all_attacks();
private string check_up(string limb);
void execute_attack();
void set_magic_protection(string limb, int x);
void add_magic_protection(mixed *info);
int query_magic_protection(string limb);
int query_magic_round();
string query_paralyze_message();
private void do_criticals(string *criticals);
int check_limbs_for_attack();

//  This function is used to initialize various variables

void init_attack() {
    init_complex_body();
    hunters = ({});
    attackers = ({});
    wimpydir = "out";
}

//  Remove dead or non-existing objects out of the list of attackers
// Return true if there are still attackers around

private int clean_up_attackers() {
    object *hunters_tmp, *attackers_tmp;
    int i;

    attackers_tmp = ({});
    hunters_tmp = ({});
    i = sizeof(attackers);
    while(i--) {
      if(!attackers[i] || !objectp(attackers[i])) continue;
      if(attackers[i]->shadow_form()) {
	hunters_tmp += ({ attackers[i] });
	continue;
      }
      if(attackers[i]->query_ghost())
	continue;
      if(environment(attackers[i]) != environment(this_object()))
	hunters_tmp += ({ attackers[i] });
      else attackers_tmp += ({ attackers[i] });
    }
    i = sizeof(hunters);
    while(i--) {
      if(!hunters[i] || !objectp(hunters[i])) continue;
      if(hunters[i]->query_ghost()) continue;
      if(hunters[i]->shadow_form()) {
	hunters_tmp += ({ hunters[i] });
	continue;
      }
      if(environment(hunters[i]) == environment(this_object())) {
	if(hunters[i]->query_invis() && !this_object()->query("see invis"))
	  continue;
	else if(hunters[i]->query_stealth() &&
	  skill_contest((int)hunters[i]->query_stealth(),
	  (int)this_object()->query_skill("perception"), 1) == 1)
	  continue;
	else attackers_tmp += ({ hunters[i] });
      }
      else hunters_tmp += ({ hunters[i] });
    }
    attackers = attackers_tmp;
    hunters = hunters_tmp;
    any_attack = sizeof(attackers);
    hunting = sizeof(hunters);
    return any_attack;
}

//  This is the kill command.  If the victim is not yet attacked
//  then it is entered first into the list of victims.  If it
//  is already in the list, then it becomes the first entry

int kill_ob(object victim, int which) {
    object *borg_people;
    int i;

    if(victim == this_object()) return 0;
    if(victim->is_pet() && (string)victim->query_owner() == 
	(string)this_object()->query_name()) return 0;
    if(this_object()->is_pet()) {
	if((string)this_object()->query_owner() ==
	  (string)victim->query_name() ||
	  (string)this_object()->query_owner() ==
	  (string)victim->query_owner()) return 0;
    }
    borg_people = (object *)victim->query_attackers();
    if(victim->is_player())
      HUNTING_D->add_hunter((string)victim->query_name(), this_object());
    if(!borg_people) borg_people = ({});
    if(member_array(this_object(), borg_people) == -1)
	if(!ok_to_kill(victim)) {
	    message("my_combat", sprintf("Supernatural forces prevent you "
	      "from attacking %s.", (string)victim->query_cap_name()),
	      this_object());
	   return 0;
	}
    /* changed from 0 to 1 to faciliate PK patch */
if(this_object()->query_attacked()) return 1;
if(attackers)
    i = member_array(victim, attackers);
else i = -1;
if( i>-1 ) {
    if(attackers[0] == victim) {
	if(!which) notify_fail("You are already doing so!\n");
	return 1;
    }
    attackers[i] = attackers[0];
    attackers[0] = victim;
    return 1;
}
attackers = ({ victim }) + attackers;
any_attack = 1;
if(!which) victim->kill_ob(this_object(), 1);
return 1;
}

void set_attackers(object *what) {
    attackers = what;
    any_attack = 1;
}

//  Called from heart_beat
//  Here is where we see if we are dead or in combat

void continue_attack() {
    string attack_limb, lost_limb;
    string *nin;
    object *death;
    int n, xxx;

    if(!environment(this_object()) || this_object()->query_ghost()) return;
    if(this_object()->is_player() && !interactive(this_object())) return;
    if(player_data["general"]["hp"] < 1 && !this_object()->query_ghost() ) {
if(arenap(TO) == 1)
"/wizards/misery/arena/battle_room"->finish_battle(TO);
      if(wizardp(this_object())) {
	player_data["general"]["hp"] = 1;
	message("my_combat", "You are immortal and cannot die.",this_object());
      }
      else {
       if(attackers) {
	if(sizeof(attackers) && attackers[0]) {
	  attackers[0]->add_exp((int)this_object()->query_exp() / DEATH_EXP_MOD);
	  attackers[0]->adj_alignment( (int)this_object()->query_alignment() );
	  attackers[0]->add_kill((string)this_object()->query_name());
	  add_death((string)attackers[0]->query_name());
	  if(this_object()->is_player()) {
	    log_file("kills",
		this_object()->query_name()+" ("+(int)this_object()->query_level()+
		") was killed by "+attackers[0]->query_name()+" ("+
		(int)attackers[0]->query_level()+"): "+ctime(time())+"\n");
	    log_file("kills",
		file_name(attackers[0])+" (uid:"+getuid(attackers[0])+")\n");
	    xxx = sizeof(death = all_inventory(attackers[0]));
	    while(xxx--) death[xxx]->notify_kill(this_object());
	  }
	  this_object()->die((attackers && attackers[0] ? attackers[0] : 0));
	  attackers = ({});
	  return;
	}
       }
     }
    }
    if(!any_attack && !hunting) return;
    if(!clean_up_attackers()) return;
    if(environment(this_object())->query_property("no attack")) return;
    if( (player_data["general"]["hp"] * 100) / player_data["general"]["max_hp"] < wimpy ) {
	run_away();
	return;
    }
    if(!query_paralyzed() && check_limbs_for_attack()) execute_attack();
    else if(!check_limbs_for_attack())
	message("my_combat","You may not attack with no wielding limbs!",	
	    this_object());
    else message("my_combat", query_paralyze_message(),
      this_object());
}

void execute_attack() {
    object *weapons, *a_weapons;
    string this_spell, party, res;
    int protection, hits, pois, i, j, k, num_attacks, x;
    int num_rounds, parry_bonus, has_shield, skill, y;
    mapping damage, tmp_res;
    string *tmp, *tmp2, *criticals;
    mixed w_hit, tmp_mix;

    if(member_array(this_object(), (object *)attackers[0]->
		    query_attackers()) < 0)
      attackers[0]->kill_ob(this_object(), 0);
    party = (string)PARTY_OB->party_member(attackers[0]);
    if(party && present((object)PARTY_OB->query_leader(party),
			environment()) &&
       (object)PARTY_OB->query_leader(party) != attackers[0]) {
      x = ((object)PARTY_OB->query_leader(party))->query_skill("body block");
      if(skill_contest(x, (int)this_object()->query_stats("strength") * 2 / 3)
	 != 2) {
	message("my_combat", (string)((object)PARTY_OB->query_leader(party))->
		query_cap_name() + " prevents you from attacking " +
		(string)attackers[0]->query_cap_name() + ".", this_object());
	message("my_action", "You prevent " + (string)this_object()->
		query_cap_name() + " from attacking "+(string)
		attackers[0]->query_cap_name()+ " with a fierce body block.",
		(object)PARTY_OB->query_leader(party));
	message("other_combat", 
		(string)((object)PARTY_OB->query_leader(party))->
		query_cap_name() + " prevents "+(string)this_object()->
		query_cap_name() + " from attacking "+(string)attackers[0]->
		query_cap_name() + " with a fierce body block.",
		all_inventory(environment()), 
		({ this_object(), (object)PARTY_OB->query_leader(party) }));
	kill_ob((object)PARTY_OB->query_leader(party), 1);
      }
      else {
	remove_call_out("kill_ob");
	call_out("kill_ob", 1, (object)PARTY_OB->query_leader(party), 1);
      }
    }
	if(magic_round) {
	  if(!query("concentrate")) return;
	  if(random(100) >= (int)this_object()->query_skill("concentrate"))
	    return;
	  if(random(100) < (5 - ((int)this_object()->query_skill("concentrate")/20)))
	  {
	    message("info", "%^RED%^%^BOLD%^You accidentally interrupt your spell!",
		  this_object());
	    if(objectp(this_object()->query_casting()))
	      ((object)this_object()->query_casting())->remove();
	    this_object()->set_casting(0);
	  }
	}

	if(!(num_attacks = sizeof(weapons=distinct_array(query_wielded())))) {
	    if(query_skill("martial arts") >= 60) num_attacks = 2;
	    else num_attacks = 1;
	}
    if((i = check_limbs_for_attack()) < num_attacks) num_attacks = i;
    num_rounds = (int)this_object()->query_num_rounds();
    if(!num_rounds) num_rounds = 1;
    if(random(100) < (int)this_object()->query_skill("whirlwind attack"))
      num_rounds++;
    while(num_rounds * num_attacks > 8) num_rounds--;
    for(j=0; j<num_rounds;j++)
    for(i=0; i<num_attacks; i++) {
	target_thing = (string)attackers[0]->return_target_limb();
	if(!sizeof(weapons)) current = 0;
	else current = weapons[i];
	a_weapons = distinct_array(attackers[0]->query_wielded());
	if(current) skill = query_skill((string)current->query_type());
	else skill = query_skill("melee");
	if(skill <= 0) skill -= 25;
	if(current && current->query_hit_bonus())
	  skill += (int)current->query_hit_bonus();
	skill += ((int)this_object()->query_skill("attack")/5 - 15);
	if(this_object()->query_intox())
	  skill -= (int)this_object()->query_intox() / 10;
	if(current && !(int)this_object()->query_property("ambidextry")) {
	    x = 0;
	    tmp = query_wielding_limbs();
	    k = sizeof(tmp);
	    while(k--) if(current == query_weapon(tmp[k]) && tmp[k] ==
		(string)this_object()->query_property("handedness")) x = 1;
	    if(!x) skill -= 30 - (query_skill("offhand training")/4);
	}
	skill -= (int)attackers[0]->sight_adjustment(this_object());
	if(current && num_attacks > 1)
	    skill -= 30-(query_skill("dual attack")/4);
	if(a_weapons) {
	    k = sizeof(a_weapons);
	    parry_bonus = 0;
	    while(k--)
		if(a_weapons[k]->query_parry_bonus() > parry_bonus)
		    parry_bonus = a_weapons[k]->query_parry_bonus();
	}
	has_shield = 0;
	tmp = (string *)attackers[0]->query_limbs();
	k = sizeof(tmp);
	while(k--) {
	    if(member_array("shield",(string *)attackers[0]->
		query_armour(tmp[k])) != -1) has_shield = 1;
	}
	if(wizardp(this_object()) && (string)this_object()->
	   getenv("TRACE") == "on")
	  message("info", "To hit skill: "+skill, this_object());
	if(skill - random(100) <
	    (int)attackers[0]->query_skill("dodge") - to_int(percent(
	    (int)attackers[0]->query_internal_encumbrance(),
	    (int)attackers[0]->query_max_internal_encumbrance())) / 5 -
	    random(100) && !attackers[0]->query_paralyzed()) {
	    miss("dodge",(current)?(string)current->query_type() :
		0);
	    continue;
	} else
	if(check_limbs_for_attack() && skill - random(100) <
	    (int)attackers[0]->query_skill("parry") + parry_bonus - 
	    random(100)) {
	    miss("parry", (current)?(string)current->query_type() :
		0);
	    continue;
	} else if(check_limbs_for_attack() && has_shield && (skill -
	    random(100) < (int)attackers[0]->query_skill("block") -
	    random(100))) {
	    miss("block",(current)?(string)current->query_type() :
		0);
	    continue;
	} else
	  damage = get_damage(current);
	if(!damage || !keys(damage)) continue;
	if(current) {
	    w_hit = current->query_hit();
	    if(functionp(w_hit)) w_hit = (*w_hit)(attackers[0]);
	    else if(stringp(w_hit)) message("my_combat",w_hit,this_object());
	    else if(pointerp(w_hit)) {
	      tmp_res = ([]);
	      x = sizeof(w_hit);
	      while(x--) {
		if(!functionp(w_hit[i])) continue;
		tmp_mix = (*w_hit[i])(attackers[0]);
		if(intp(tmp_mix)) tmp_res += (["crushing" : tmp_mix ]);
		if(mapp(tmp_mix)) tmp_res += tmp_mix;
	      }
	      w_hit = tmp_res;
	    }
	}
	tmp = keys(damage);
	if(w_hit && mapp(w_hit))
	  tmp = distinct_array(tmp + keys(w_hit));
	x = 0;
	criticals = ({});
	for(k=0;k<sizeof(tmp);k++) {
	    if(mapp(w_hit) && !undefinedp(w_hit[tmp[k]])) {
	      if(undefinedp(damage[tmp[k]])) damage[tmp[k]] = w_hit[tmp[k]];
	      else damage[tmp[k]] += w_hit[tmp[k]];
	    }
	    if(current && num_attacks > 1)
		damage[tmp[k]] -= 29-(query_skill("dual attack")/4);
	    if(tmp2 = (string *)attackers[0]->
	       query_property("invulnerability")) {
	      if(current && stringp(current->query("material_name"))
		 &&(!current->query_property("enchantment") ||
		    (int)current->query_property("enchantment") < 0)) {
		if(member_array((string)current->query("material_name"),
				tmp2) >= 0)
		    damage[tmp[k]] -= 1000;
	      }
	      else if(!current && member_array("default", tmp2) >= 0)
		damage[tmp[k]] -= 1000;
	    }
	    tmp_res = (mapping)attackers[0]->
	      query_property("physical resistance");
	    if(tmp_res && current && stringp(current->query("material_name"))
		    && (!current->query_property("enchantment") ||
			(int)current->query_property("enchantment") < 0)) {
		tmp2 = keys(tmp_res);
		if((y = member_array((string)current->query("material_name"),
		    tmp2)) > -1 && damage[tmp[k]] > 0)
		    damage[tmp[k]] = to_int(to_float(damage[tmp[k]]) *
			to_float(100-tmp_res[tmp2[y]]) / 100.0);
	    }
	    if(tmp_res && !current && member_array("default", keys(tmp_res))
	       >= 0) {
	      damage[tmp[k]] = to_int(to_float(damage[tmp[k]]) *
				      to_float(100-tmp_res["default"]) /
				      100.0);
	    }
	    if(w_hit && intp(w_hit)) damage[tmp[k]] += w_hit;
	    hits = random(100)+1;
	    protection = (int)attackers[0]->query_current_protection(
		target_thing, tmp[k]);
	    res = (string)DAMAGE_D->attack_result(sprintf("%d %d %d",
		hits,damage[tmp[k]], protection));
	    switch(explode(res," ")[0]) {
		case "DAMAGE":
		    sscanf(res,"DAMAGE %d",hits);
		    x += hits;
		    break;
		case "CRITICAL":
		    x += 10;
		    hits = member_array(replace_string(res, "CRITICAL ", ""),
			CRIT_TYPES);
		    hits = (hits < 0)? 0 : ((hits > 4)? 4:hits);
		    criticals += ({ lower_case(tmp[k]) +
			" " + CRIT_TYPES[hits] });
		    break;
	    }
	  }
	  if(!criticals || !sizeof(criticals) || x > 0) {
	    send_messages((string *)DAMAGE_D->attack_message(sprintf(
		"%d %s %s:%s", x, (current)?(string)current->query_verb() :
		"hit", (string)this_object()->query_name(), (string)
		attackers[0]->query_name())));
	    if(x > 3 && current && current->query_poisoning()) {
		pois = random((int)current->query_poisoning()+1);
		attackers[0]->add_poisoning(pois);
		current->add_poisoning(-pois); }
	    x = (int)attackers[0]->do_damage(target_thing, x); }
	    if(this_object()->query_property("see damage") &&
		(high_mortalp(this_object()) || wizardp(this_object())))
	    message("my_combat",identify(res),this_object());

	}
	if(x > 0) {
	    if(current) {
		current->decay();
		this_object()->add_exp(x/ATTACK_DAMAGE_EXP_MOD);
	    }
	}
	if(!criticals) criticals = ({});
	if(current && (string *)current->query_auto_critical())
	    do_critical(this_object(), attackers[0],
	      (string *)current->query_auto_critical(),
	      target_thing);
	if(x > 0) attackers[0]->check_on_limb(target_thing);
	if(criticals && sizeof(criticals))
	do_criticals(criticals);
}

private void do_criticals(string *criticals) {
    int x, i, j, amt, dur, roll;
    string *tmp, *crit_words, what, what2, *tmp2;
    object ob;
    object *targ_armour;
    int k, l; 
    mapping tmp_crits;
    function special_func;

    for(i=0; i< sizeof(criticals);i++) {
	amt = 0;
	sscanf(criticals[i],"%s %s", what2, what);
	if(intp(attackers[0]->query_property("enhance criticals")))
	    amt += (int)attackers[0]->query_property("enhance criticals");
	else if(mapp(attackers[0]->query_property("enhance criticals"))) {
	    tmp = keys((mapping)attackers[0]->query_property(
		"enhance criticals"));
	    x = member_array(what2, tmp);
	    tmp_crits = (mapping)attackers[0]->query_property(
		"enhance criticals");
	    if(x >= 0 && intp(tmp_crits[tmp[x]])) amt += 
		tmp_crits[tmp[x]];
	}
	if(current) {
		if(intp(current->query_enh_critical()))
		amt += (int)current->query_enh_critical();
		else if(mapp(current->query_enh_critical())) {
		    tmp = keys((mapping)current->query_enh_critical());
		    x = member_array(what2,tmp);
		    if(x >= 0) {
			tmp_crits = (mapping)current->
			    query_enh_critical();
			amt += tmp_crits[tmp[x]];
		    }
		}
	}
	if(member_array((string)attackers[0]->query_race(), UNDEAD_RACES) >= 0 &&
      what2 == "holy") amt += 2;
	if(target_thing)
	    targ_armour = (object *)attackers[0]->query_armour_ob(target_thing);
	if(pointerp(targ_armour)) {
	    j = sizeof(targ_armour);
	    while(j--) {
		if(intp(targ_armour[j]->query_enh_critical()))
		amt += (int)targ_armour[j]->query_enh_critical();
		else if(mapp(targ_armour[j]->query_enh_critical())) {
		    tmp = keys((mapping)targ_armour[j]->query_enh_critical());
		    x = member_array(what2,tmp);
		    if(x >= 0) {
			tmp_crits = (mapping)targ_armour[j]->
			    query_enh_critical();
			amt += tmp_crits[tmp[x]];
		    }
		}
	    }
	}
	what = capitalize(what);
	dur = member_array(what,CRIT_TYPES);
	dur += amt;
	if(dur > 4) dur = 4;
	if(dur < 0) continue;
	criticals[i] = sprintf("%s %s",what2, CRIT_TYPES[dur]);
	roll = random(100)+1;
      if(random(100) < (roll / 5)*(dur+1)) this_object()->add_dev(1);
	if((string)this_object()->getenv("TRACE") == "on" && wizardp(this_object()))
	  message("info", "Crit type: "+criticals[i]+"  Roll: "+roll, this_object());
	if(attackers[0] && (string)attackers[0]->getenv("TRACE") == "on" && wizardp(attackers[0]))
	  message("info", "Attacker crit type: "+criticals[i]+"  Roll: "+roll, attackers[0]);
	send_messages((string *)DAMAGE_D->query_msg(criticals[i],
	    roll, sprintf("%s:%s",(string)this_object()->query_name(),
	    (string)attackers[0]->query_name())));
	tmp = (string *)DAMAGE_D->query_result(criticals[i], roll);
	if(attackers[0] && (string)attackers[0]->getenv("TRACE") == "on" && wizardp(attackers[0]))
	  message("info", "Attacker critical results: "+implode(tmp, ", "), attackers[0]);
	if((string)this_object()->getenv("TRACE") == "on" && wizardp(this_object()))
	  message("info", "Critical results: "+implode(tmp, ", "), this_object());
	if(!tmp || !sizeof(tmp)) continue;
	else for(j=0;j<sizeof(tmp);j++) {
	    switch(explode(tmp[j]," ")[0]) {
	    case "DAMAGE":
	      if(!sscanf(tmp[j],"DAMAGE %d",amt)) break;
	      if(target_thing) amt = (int)attackers[0]->
		do_damage(target_thing, amt);
	      else amt =(int)attackers[0]->do_damage((target_thing = 
						      (string)attackers[0]->
						      return_target_limb()),amt);
	      if(amt > 0) {
		this_object()->add_exp(amt/ATTACK_DAMAGE_EXP_MOD);
		attackers[0]->check_on_limb(target_thing);
	      }
	      break;
	    case "STUN":
	      if(!sscanf(tmp[j],"STUN %d",dur)) break;
	      attackers[0]->set_paralyzed(dur*2,
					  "%^MAGENTA%^You are stunned and cannot attack!");
	      break;
	    case "KILL":
	      attackers[0]->set_hp(-1);
	      break;
	    case "SPECIAL_RESULT":
	      if(sscanf(tmp[j],"SPECIAL_RESULT %s %s", what, what2) != 2)
		continue;
	if(!this_object() || !attackers[0]) continue;
	      call_other(what, what2, this_object(), attackers[0]);
	      break;
	    case "LIMB_LOSS":
	      crit_words = explode(tmp[j]," ");
	      l = sizeof(crit_words);
	      if(l == 1 || (l == 2 && crit_words[l-1] == "cripple"))
		what = (string)attackers[0]->return_target_limb();
	      else if(l >= 2) {
		if(crit_words[1] == "non_fatal") {
		  k = sizeof((tmp2 = (string *)attackers[0]->
			      query_limbs())) - 1;
		  if(k >= 0) what = tmp2[k];
		  while((string)attackers[0]->query_reference(what) ==
			"FATAL" && k--)
		    what = tmp2[k];
		  if((string)attackers[0]->query_reference(what) ==
		     "FATAL") what = 0;
		} else {
		  if(crit_words[l-1] == "cripple" && l > 2)
		    what = implode(crit_words[1..l-2], " ");
		  else
		    what = implode(crit_words[1..l-1], " ");
		  what = (string)attackers[0]->sub_limb(what);
		}
	      }
	      if(!what) break;
/* Added by Misery 031096 to fix HeartBeat problem */
if(l >= 2 && crit_words[l-1] == "cripple" && attackers && attackers[0] && what) {
    if(attackers[0]->query_crippled(what)) break;
    amt = (int)attackers[0]->query_dam(what);
    if(amt >= (int)attackers[0]->query_max_dam(what)) break;
    attackers[0]->do_damage(what,(int)attackers[0]->query_max_dam(what)+3-amt);
    attackers[0]->add_hp(((int)attackers[0]->query_max_dam(what)+3-amt) / 2);
    attackers[0]->check_on_limb(what);
} else if(attackers && attackers[0] && what) {
    amt = (int)attackers[0]->do_damage(what,(int)attackers[0]->query_max_dam(what)*2+1);
    attackers[0]->add_hp(amt / 2);
    attackers[0]->check_on_limb(what);
}
break;
case "CONT_DAMAGE":
if(sscanf(tmp[j],"CONT_DAMAGE %d %d",amt,dur) != 2)
    break;
if(!target_thing) target_thing = (string)attackers[0]->
    return_target_limb();
DAMAGE_D->init_cont_damage(attackers[0], dur, amt,
  "%^MAGENTA%^You take damage from "+
  "your wounds!");
case "SKILL_MOD":
if(sscanf(tmp[j],"SKILL_MOD %s %d %d", what, amt, dur) != 3)
    break;
attackers[0]->add_skill_bonus(what,amt);
attackers[0]->delay_skill_mod(dur*2,
  -amt, what);
break;
}
}
}
return;
}

void delay_skill_mod(int dur, int amt, string what) {
    call_out("remove_skill_mod", dur, ([ "what" : what,
      "amount" : amt]));
    return;
}

void remove_skill_mod(mapping info) {
    if(!info || !mapp(info)) return;
    this_object()->add_skill_bonus(info["what"], info["amount"]);
    return;
}

void stop_hunting() {
    hunting = 0;
    hunters = ({});
}
void cease_all_attacks() {
    attackers = ({});
    any_attack = 0;
}

object query_current_attacker() {
    if(!sizeof(attackers)) return 0;
    else return attackers[0];
}

void run_away() {
    string *str;

    str = (string *)environment(this_object())->query_exits();
    if(!str || !sizeof(str)) {
	message("my_combat", "There is nowhere to run!!!", this_object());
	return;
    }
    message("my_combat", "You run away.", this_object());
    if(member_array(wimpydir, str) != -1) this_object()->force_me(wimpydir);
    else this_object()->force_me(str[random(sizeof(str))]);
}

void set_wimpydir(string str) {
    if(!stringp(str)) return;
    wimpydir = str;
}

void set_wimpy(int x) { wimpy = x; }

string query_wimpydir() { return wimpydir; }

int query_wimpy() { return wimpy; }

mapping get_damage(object weap) {
    mapping ret_val;
    int attack, melee, i;
    string *tmp, dmg_type;

    ret_val = ([]);
    melee = query_skill("melee");
    attack = query_skill("attack");
    if(!weap) {
	if(mapp(this_object()->query_property("melee damage")))
	    ret_val += (mapping)this_object()->query_property("melee damage");
	else
	    dmg_type = (string)this_object()->query_property("melee damage");
	if(!dmg_type || member_array(dmg_type,DAMAGE_TYPES) < 0)
	    dmg_type = DAMAGE_TYPES[0];
	if(this_object()->query_skill("martial arts")&& (int)
	  this_object()->query_skill("martial arts") > 10)
	    ret_val += ([ "strike" : 10 ]);
	if(mapp(ret_val)) tmp = distinct_array(keys(ret_val) + ({ dmg_type }));
	else tmp = ({ dmg_type });
	i = sizeof(tmp);
	while(i--) {
	    if(undefinedp(ret_val[tmp[i]])) ret_val[tmp[i]] = 0;
	    if(intp(this_object()->query_skill("martial arts")))
		ret_val[tmp[i]] += (int)this_object()->query_skill("martial arts")
		/ 4;
	    ret_val[tmp[i]] += melee/10 + attack/20 + 
	    (int)this_object()->query_physical() + (int)this_object()->query_stats(
	      "strength") / 7;
	}
    }
    else {
	ret_val = weap->query_all_wc();
	tmp = keys(ret_val);
	for(i=0;i<sizeof(tmp);i++) {
	    ret_val[tmp[i]] += attack/15 + query_skill((string)weap->
	      query_type())/10 - 8 + 
	    (int)this_object()->query_physical()
	    + (int)this_object()->query_stats("strength")/7
	    + (int)weap->prop("damage bonus");
	}
    }
    return ret_val;
}

void set_casting(int x) { casting = x; }

int query_casting() { return casting; }

void set_magic_protection(string limb, int x) {
    if(!magic_protection) magic_protection = ([]);
    if(!limb) magic_protection["overall"] = x;
    else magic_protection[limb] = x;
}

void add_magic_protection(mixed *info) {
    string limb;
    int time, x, i;

    if(sizeof(info) != 3) return;
    limb = info[i];
    x= info[1];
    time = info[2];
    if(!magic_protection) magic_protection = ([]);
    if(!limb) magic_protection["overall"] += x;
    else magic_protection[limb] += x;
    if(time > 0) call_out("add_magic_protection", time, ({ limb, -x, 0 }) );
}

int query_magic_protection(string limb) {
    if(!magic_protection) return 0;
    if(!limb) return magic_protection["overall"];
    else return (magic_protection["overall"] + magic_protection[limb]);
}

void send_messages(string *mesgs) {
    if(sizeof(mesgs) != 3) mesgs = ({ "BUG in message sender",
	"BUG in message sender", "BUG in message sender" });
    if(strlen(mesgs[0]))
	message("my_combat",mesgs[0],this_object());
    if(strlen(mesgs[1]))
	message("other_combat",mesgs[1],environment(this_object()),
	  ({ this_object(), attackers[0] }));
    if(strlen(mesgs[2]))
	message("my_combat",mesgs[2],attackers[0]);
    return;
}

object *query_hunted() { return hunters; }

object *query_attackers() {
    if(!attackers) return ({});
    if(!sizeof(attackers)) return ({});
    return attackers;
}

int sight_adjustment(object who) {
    int elight, ret;

    ret = 0;
    if(!pointerp(attackers) || !sizeof(attackers)) return 0;
    elight = effective_light(who);
    if(elight >6 || elight < 1)
	ret = random(20);
    if((int)this_object()->query_property("light") > 0)
	ret -= random(10);
    else if((int)this_object()->query_property("light") < 0)
	ret += random(10);
    if(this_object()->query_invis() && !who->query("see invis")) {
	ret += 60 - ((int)who->query_skill("perception") / 3);
    }
    return ret;
}

void miss(string how, string type) {
    string you, me;
    string *missed;

    you = (string)attackers[0]->query_cap_name();
    me = query_cap_name();
    switch(lower_case(how)) {
    case "block":
	send_messages(({ "Your attack is blocked by "+you+"'s shield.",
	  you+" blocks "+me+"'s attack deftly.",
	  "You skillfully block "+me+"'s attack with your shield." }));
	break;
    case "parry":
	send_messages(({ you+" skillfully parries your feeble attack.",
	  you+" bats "+me+"'s attack aside with a skillful parry.",
	  "You deftly parry "+me+"'s attack." }));
	break;
    case "dodge":
	send_messages(({ you+" dodges your attack.",
	  you+" sidesteps "+me+"'s attack.",
	  "You dodge "+me+"'s attack." }));
	break;
    default:
	send_messages(({ "You miss "+you+".",me+" misses "+you+".",
	  me+" misses you." }));
    }
    return;	
}

void set_paralyzed(int x, string drow) {
    if(wizardp(TO) && !arenap(TO)) {
	message("my_action", "Your Immortality just saved you from being "
	  "paralyzed.", this_object());
	return;
    }
    if(x <= 0) return;
    if(this_object()->query_property("no stun")) return;
    if(!drow)
	paralyze_message = DEFAULT_PARALYZE_MESSAGE;
    else
	paralyze_message = drow;
    paralyzed++;
    call_out("remove_paralyzed", x);
}

string query_paralyze_message() { return paralyze_message; }

int query_paralyzed() { return paralyzed; }

void remove_paralyzed() { 
    if(!this_object()) return;
    if(origin() != 16)
	remove_call_out("remove_paralyzed");
    paralyzed--;
    if(paralyzed < 0) paralyzed = 0;
    message("my_combat", "You can move again.", this_object());
}

int mobility(int magic) {
    int ret;

    if(magic) ret = 70 - (int)this_object()->query_stats("wisdom");
    else {
	ret = to_int(percent(query_internal_encumbrance(),query_max_internal_encumbrance()))/4;
	ret += 35-(int)this_object()->query_stats("dexterity");
    }
    if(ret > 50) ret = 50;
    if(ret < 0) ret = 0;
    return ret;
}

int query_current_protection(string target_thing, string type) {
    int prot, tmp;

    if(!type || member_array(type,DAMAGE_TYPES) < 0)
	type = DAMAGE_TYPES[0];
    prot = (int)this_object()->query_ac(target_thing, type)+query_skill("defense")/15;
    return prot;
}

void set_magic_round(int x) { magic_round = x; }

int query_magic_round() { return magic_round; }

int ok_to_kill(object targ) {
    int vic_level;
    string str, title;

    if(targ->is_invincible()) return 0;
    if(wizardp(this_object())) return 1;
    if(member_array(query_name(), COUNCIL) != -1) return 1;
    if(!this_object()->is_player() || !targ->is_player()) return 1;
    return 1;
}

int check_limbs_for_attack() {
    string *which_limbs;
    string *tmp, ref;
    int i, num;

    if(this_object()->query_ghost()) return 0;
    which_limbs = (string *)this_object()->query_wielding_limbs();
    if(!sizeof(which_limbs)) return 0;
    tmp = (string *)this_object()->query_limbs();
    i = sizeof(tmp);
    while(i--) {
	if(member_array(tmp[i],which_limbs) > -1) {
	    if(this_object()->query_crippled(tmp[i]))
		which_limbs -= ({ tmp[i] });
	    else if(this_object()->query_reference(tmp[i])
	      && this_object()->query_crippled((string)this_object()->
		query_reference(tmp[i])))
		which_limbs -= ({ tmp[i] });
	}
    }
    return sizeof(which_limbs);
}
