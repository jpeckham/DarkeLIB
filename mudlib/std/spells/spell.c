//	Generic spell.c for use with darkeMUD spells system
//	DarkeLIB 0.1
//	Diewarzau 4/2/94

#include <std.h>
#include <daemons.h>
#include <balance.h>
#include <party.h>
#include <ritual.h>

#define UNDEAD_RACES ({ "undead", "skeleton", "zombie", "vampire", \
      "ghoul", "ghost", "lich" })
#define CRIT_TYPES ({ "A", "B", "C", "D", "E" })
#define NO_CRIT_MSG ({ })
// #define NO_CRIT_MSG ({ "impaling", "crushing", "cutting" })
// You can use this define to set damage types for which the critical
//	result message is not sent.  -- Diewarzau 7/7/95

inherit OBJECT;

string ritual;

nosave int skill;
nosave int resist_flag;
nosave string *bonus;
private object who_casting;

object *casters;
string casters_names;
int b;

void spell_func(object caster, object at, int power, string args, int flag);
void do_criticals(string *criticals, string target_thing, object caster, object targ);
void remove_skill_mod(mapping info);
int filter_area(object who);
void send_messages(string *mesgs,object caster,object at);
void set_element();
int is_spell() { return 1; }

int clean_up() {
  mixed *info;
  int i;

  info = call_out_info();
  info += delayed_call_info();
  i = sizeof(info);
  while(i--)
    if(info[i][0] == this_object() && intp(info[i][2]) && info[i][2] > 0 &&
       info[i][1] != "do_spell") {
      delayed_call("remove",info[i][2]+1);
	    return 1;
    }
  remove();
  return 0;
}

void remove() {
    mixed *info;
    int i;

    remove_delayed_call("do_spell");
    info = (mixed *)call_out_info() + (mixed *)delayed_call_info();
    i = sizeof(info);
    while(i--)
      if(info[i][0] == this_object() && intp(info[i][2]) && info[i][2] > 0) {
	      delayed_call("remove",info[i][2]+1);
	      return;
      }
    destruct(this_object());
}

int move_while_casting(object caster) {
    if((int)caster->query_skill(props["skill"]) < 95)
      return 0;
    if(random(100) < ((int)caster->query_skill(props["skill"])-60))
      return 1;
    return 0;
}


int not_party(object who, object caster) {
    object owner, owner2;
    string cparty;

    if(who == caster) return 0;
    if(!filter_area(who)) return 0;
    cparty = (string)PARTY_OB->party_member(caster);
    if(caster->is_player()) {
      if(who->is_pet() &&
	 (string)who->query_owner() == (string)caster->query_name())
	return 0;
      if(cparty) {
	if(who->is_player() && (string)PARTY_OB->party_member(who) ==
	   cparty) return 0;
	if(who->is_pet() && stringp(who->query_owner()) &&
	   (owner=find_player((string)who->query_owner()))) {
	  if((string)PARTY_OB->party_member(owner) ==
	     (string)PARTY_OB->party_member(caster)) return 0;
	}
      }
    }
    else if(caster->is_pet()) {
      if((string)caster->query_owner() == (string)who->query_name())
	return 0;
      if(stringp(caster->query_owner()) &&
	 (owner=find_player((string)caster->query_owner()))) {
	if(cparty=(string)PARTY_OB->party_member(owner)) {
	  if(who->is_player() && (string)PARTY_OB->party_member(who) ==
	     (string)PARTY_OB->party_member(owner)) return 0;
	  if(who->is_pet() && stringp(who->query_owner()) &&
	     (owner2=find_player((string)who->query_owner()))) {
	    if((string)PARTY_OB->party_member(owner) == 
	       (string)PARTY_OB->party_member(owner2)) return 0;
	  }
	}
      }
    }
    return 1;
}

int not_attacker(object who, object caster) {
    return (member_array(who,(object *)caster->query_attackers()) > -1)?
	0:filter_area(who);
}

int random_filter(object who) { return random(2) && filter_area(who); }

int filter_area(object who) {
    switch(props["target type"]) {
	case "player":
	    return (int)who->is_player();
	case "any":
	    return objectp(who);
	case "living":
	default:
	    return living(who);
    }
}

void do_criticals(string *criticals, string target_thing, object caster, object targ) {
    int i,j, amt, roll, dur;
    string *tmp, what, what2, *tmp2;
    object ob;
    object *targ_armour;
    int k, x, l;
    mixed tmp_crits;
    string *crit_words;
//    function special_func;

    if(!target_thing) target_thing = (string)targ->return_target_limb();
    for(i=0; i< sizeof(criticals);i++) {
	sscanf(criticals[i],"%s %s",what2,what);
	amt = 0;
	tmp_crits = targ->query_property("enhance criticals");
	if(tmp_crits && intp(tmp_crits))
	    amt += tmp_crits;
	else if(tmp_crits && mapp(tmp_crits)) {
	    tmp = keys(tmp_crits);
	    x = member_array(what2, tmp);
	    if(x >= 0 && intp(tmp_crits[tmp[x]]))
		amt += tmp_crits[tmp[x]];
	}
  if(member_array((string)targ->query_race(), UNDEAD_RACES) >= 0 &&
      what2 == "holy") amt += 2;
	if(target_thing)
	    targ_armour = (object *)targ->query_armour_ob(target_thing);
	if(pointerp(targ_armour)) {
	    j = sizeof(targ_armour);
	    while(j--) {
	      tmp_crits = targ_armour[j]->query_enh_critical();
		if(tmp_crits && intp(tmp_crits))
		amt += tmp_crits;
		else if(tmp_crits && mapp(tmp_crits)) {
		    tmp = keys(tmp_crits);
		    x = member_array(what2,tmp);
		    if(x >= 0) {
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
	criticals[i] = replace_string(criticals[i],what,CRIT_TYPES[dur]);
	roll = random(100)+1;
       if(member_array(what2, DAMAGE_TYPES) >= 0) 
      if(random(100) < (dur+1)*(roll/5) && living(caster) && caster->at_max_exp() &&
        !caster->buffer_full() && ((int)targ->query_hp() > 5)) caster->add_dev(1);
	if(member_array(what2,NO_CRIT_MSG) < 0)
	send_messages((string *)DAMAGE_D->query_msg(criticals[i],
	    roll, sprintf("%s:%s",(string)caster->query_name(),
	    (string)targ->query_name())),caster,targ);
	tmp = (string *)DAMAGE_D->query_result(criticals[i], roll);
	if(!tmp || !sizeof(tmp)) continue;
	else for(j=0;j<sizeof(tmp);j++) {
	    switch(explode(tmp[j]," ")[0]) {
		case "DAMAGE":
		    if(!sscanf(tmp[j],"DAMAGE %d",amt)) break;
		    if(target_thing) amt = (int)targ->
			do_damage(target_thing, amt);
		    else amt =(int)targ->do_damage((target_thing = 
			(string)targ->
			return_target_limb()),amt);
		    if(amt > 0) {
			caster->add_exp(amt/ATTACK_DAMAGE_EXP_MOD);
			targ->check_on_limb(target_thing);
		    }
			break;
		case "STUN":
		    if(!sscanf(tmp[j],"STUN %d",dur)) break;
		    targ->set_paralyzed(dur*2,
			"%^MAGENTA%^You are stunned and cannot attack!");
		    break;
		case "KILL":
		    targ->set_hp(-1);
		    break;
		  case "SPECIAL_RESULT":
		    if(sscanf(tmp[j],"SPECIAL_RESULT %s %s", what, what2) != 2)
		      continue;
		    if(!caster || !targ) continue;
		    call_other(what, what2, caster, targ);
		case "LIMB_LOSS":
		  crit_words = explode(tmp[j]," ");
		    l = sizeof(crit_words);
		    if(l == 1 || (l == 2 && crit_words[l-1] == "cripple"))
		      what = (string)targ->return_target_limb();
		    else if(l >= 2) {
		      if(crit_words[1] == "non_fatal") {
			k = sizeof((tmp2 = (string *)targ->
				    query_limbs()));
                        if(k >= 0) what = tmp2[k-1];
			while((string)targ->query_reference(what) ==
			      "FATAL" && k--)
			  what = tmp2[k];
			if((string)targ->query_reference(what) ==
			      "FATAL") what = 0;
		      } else {
			if(crit_words[l-1] == "cripple" && l > 2)
			  what = implode(crit_words[1..l-2], " ");
			else
			  what = implode(crit_words[1..l-1], " ");
			what = (string)targ->sub_limb(what);
		      }
		    }
		    if(!what) break;
		    if(l >= 2 && crit_words[l-1] == "cripple") {
		      if(targ->query_crippled(what)) break;
		      amt = (int)targ->query_dam(what);
		      if(amt >= (int)targ->query_max_dam(what)) break;
		      targ->do_damage(what,(int)targ->query_max_dam(what)+3-amt);
		      targ->add_hp(((int)targ->query_max_dam(what)+3-amt) / 2);
		      targ->check_on_limb(what);
} else if(targ) {
		      amt = (int)targ->do_damage(what,(int)targ->query_max_dam(what)*2+1);
		      targ->add_hp(amt / 2);
		      targ->check_on_limb(what);
		    }
		    break;
		case "STAT_MOD":
		    if(sscanf(tmp[j],"STAT_MOD %s %d %d",what, amt, dur) !=3)
			break;
		    ob = clone_object("/std/spells/shadows/stat_shadow");
		    ob->start_shadow(targ, dur*2, "%^GREEN%^You feel a "+
			"little better.");
		    ob->set_stat(what);
		    ob->set_mod(amt);
		    break;
		case "CONT_DAMAGEOB":
		    if(sscanf(tmp[j],"CONT_DAMAGE %d %d",amt,dur) != 2)
			break;
		    if(!target_thing) target_thing = (string)targ->
			return_target_limb();
		    DAMAGE_D->init_cont_damage(targ, dur, amt,
					       "%^MAGENTA%^Your body is "+
					       "racked with pain!");
		    break;
		case "SKILL_MOD":
		    if(sscanf(tmp[j],"SKILL_MOD %s %d %d", what, amt, dur) != 3)
			break;
		    targ->add_skill_bonus(what,amt);
		    delayed_call("remove_skill_mod",dur*2,([ "who" : targ,
			"amount" : -amt, "what" : what ]));
		    break;
	    }
	}
    }
    return;
}

void send_messages(string *mesgs,object caster, object at) {
    if(sizeof(mesgs) != 3) mesgs = ({ "BUG in message sender",
	"BUG in message sender", "BUG in message sender" });
    if(strlen(mesgs[0]))
      message("my_combat",mesgs[0],caster);
    if(strlen(mesgs[1]))
      message("other_combat",mesgs[1],environment(caster),
	({ caster, at }));
    if(strlen(mesgs[2]))
      message("my_combat",mesgs[2],at);
    return;
}

void remove_skill_mod(mapping info) {
    if(!info || !mapp(info) || !info["who"]) return;
    info["who"]->add_skill_bonus(info["what"], info["amount"]);
    return;
}

void remove_stat_mod(mapping info) {
    if(!info || !mapp(info) || !info["who"]) return;
    info["who"]->add_stat_bonus(info["what"], info["amount"]);
    return;
}

object query_caster() { return who_casting; }

void begin_casting(object caster, string arg, int power) {
    mapping call_value;
    object current_spell;
    string xtra_arg, target;
    int i;

    who_casting = caster;
    if(!props["locked ench"] && caster->query_stoned() &&
	(int)caster->query_stoned() > 240) {
	message("info", "You are WAY too baked to cast a damn thing.",
	  caster);
	return;
    }
    if(environment(caster)->query_property("no spell")) {
	message("info","Mystic forces prevent casting.",caster);
	remove();
	return;
    }
    if(props["combat spell"] && environment(caster)->query_property(
	"no attack")) {
	message("info","Greater powers prevent your malice.",caster);
	remove();
	return;
    }
    if(!props["no target"] && !arg) {
	message("info","You must supply a target.",caster);
	remove();
	return;
    }
    if(props["no target"]) xtra_arg = arg;
    else if(sscanf(arg, "%s with %s", target, xtra_arg) != 2)
      target = arg;
      if(((int)caster->query_spell_level(props["name"]) < power || power == 0) && !props["ritual final"] && !props["ritual spell"] && !props["ritual dud"]) {
	message("info","You do not know that spell at that power level.",
	    caster);
	remove();
	return;
    }
if(!caster->query_skill(props["skill"]) && !props["ritual final"] && !props["ritual spell"] && !props["ritual dud"]) {
	message("info","You do not know the skill "+props["skill"]+", "
	    "which is required to cast this spell.",caster);
	remove();
	return;
    }
    if((current_spell = caster->query_casting())) {
	message("info","%^RED%^%^BOLD%^You interrupt your other spell.",
	    caster);
	current_spell->remove();
    }
    if(props["elemental spell"]) {
      if(props["elemental spell"] && !caster->query_element()) {
	  message("info", "You must choose an "+
	    "element in which to cast this spell.", caster);
	  remove();
	  return;
	}
	if(props["elemental spell"])
	  set_property("element", (string)caster->query_element());
	write("%^RED%^Using the element: "+props["element"] + ".");
	set_element();
    }

// Setting up the ritual variables
	if(caster->query_ritual()){
	ritual = caster->query_ritual();
	b = sizeof(casters = RITUAL_D->query_members(ritual));
	while(b--){
	casters_names = casters[b]->query_cap_name();
}
	b = sizeof(casters);
}

    call_value = ([ "caster" : caster, "target" : target,
	"power" : power, "arg" : xtra_arg ]);
    if(!props["casting time"]) props["casting time"] = 1;

	if(props["ritual dud"] || props["ritual spell"]){
	message("info","%^GREEN%^You begin casting your part of the ritual spell.", caster);
	message("info","%^GREEN%^" +caster->query_cap_name()+" begins casting a ritual spell with "+caster->query_possessive()+" group.", environment(caster), ({caster}));
}

else{
    message("info","%^CYAN%^You begin your casting.",caster);
    message("info","%^CYAN%^" + (string)caster->query_cap_name() +
	" begins casting a spell.",
	filter_array(all_inventory(environment(caster)), "silent_cast",
		     this_object(), (int)caster->query_skill(props["skill"])),
	({ caster }));
}

    bonus = ({});
	if(props["ritual bonus"] || props["ritual bonus"] > 0){
        props["extra power"] += props["ritual bonus"];
        call_value["power"] += props["ritual bonus"];
}
    if((int)caster->query_skill(props["skill"]) > 100) {
      i = (int)caster->query_skill(props["skill"]) / 100;
      if(i > 0) set_property("extra power", 0);
      while(i--) {
	if(props["ritual final"])
	set("instant cast", 1);
	if(i==0 && random(100) > ((int)caster->query_skill(props["skill"])%100) || props["ritual spell"] || props["ritual dud"] || props["ritual final"])
          continue;
        switch(random(10)) {
        case 0..2:
          bonus += ({ "%^CYAN%^Bonus:%^RESET%^ Half casting time" });
          props["casting time"] /= 2;
          if(props["casting time"] < 1) props["casting time"] = 1;
          break;
        case 3..5:
          bonus += ({ "%^CYAN%^Bonus:%^RESET%^ Half mp cost" });
          props["base mp cost"] /= 2;
          if(props["base mp cost"] < 1) props["base mp cost"] = 1;
          break;
        case 6..7:
          bonus += ({ "%^CYAN%^Bonus:%^RESET%^ Power +1" });
          props["extra power"]++;
          call_value["power"]++;
          break;
        case 8:
          bonus += ({ "%^CYAN%^Bonus:%^RESET%^ Power +2" });
          call_value["power"] += 2;
          props["extra power"] += 2;
          break;
        case 9:
          bonus += ({ "%^CYAN%^Bonus:%^RESET%^ NO mp cost!" });
          props["base mp cost"] = 0;
          break;
/*        case 10:
          bonus += ({ "%^CYAN%^Bonus:%^RESET%^ Instant cast!" });
          set("instant cast", 1);
          break;
*/
//like paladins need that BS ^^^^^ or cLS - parnell 99
        }
      }
    }
    caster->set_casting(this_object());
    caster->set_magic_round(props["casting time"]);
    if(query("instant cast") || !props["casting time"])
      this_object()->do_spell(call_value);
    else
      delayed_call("do_spell",props["casting time"]*2,call_value);
    return;
}

void set_element() {
    int i, j;
    string tmp_str;

    switch(props["element"]) {
	case "air":
	  props["element types"] = ({ "vacuum" });
	  break;
	case "cold":
	  props["element types"] = ({ "cold" });
	  break;
	case "water":
	  props["element types"] = ({ "impact" });
	  break;
	case "gravity":
	  props["element types"] = ({ "stress", "crushing" });
	  break;
	default:
	case "fire":
	  props["element types"] = ({ "fire", "stress" });
	  break;
	case "dark":
	  props["element types"] = ({ "cold", "depression" });
	  break;
	case "inertia":
	  props["element types"] = ({ "impact", "stress" });
	  break;
	case "earth":
	  props["element types"] = ({ "impact", "crushing" });
	  break;
	case "electricity":
	  props["element types"] = ({ "electricity", "fire", "impact" });
	  break;
	case "divine":
	  props["element types"] = ({ "holy" });
	  break;
	case "sinister":
	  props["element types"] = ({ "vacuum" });
	  break;
	case "balance":
	  props["element types"] = ({ "vacuum", "holy" });
	  break;
	case "ice":
	  props["element types"] = ({ "impact", "cold" });
	  break;
	case "wind":
	  props["element types"] = ({ "impact", "vacuum" });
	  break;
	case "plasma":
	  props["element types"] = ({ "plasma", "fire", "electricity" });
	  break;
	case "vacid":
	  props["element types"] = ({ "cold", "stress", "impact" });
	  break;
	case "aether":
	  props["element types"] = ({ "aether", "stress" });
	  break;
	case "chaos":
	  props["element types"] = ({ "stress", "disruption" });
	  break;
	case "time":
	  props["element types"] = ({ "time" , "stress" });
	  break;
	case "nexus":
	  props["element types"] = ({ "disruption", "aether", "stress" });
	  break;
	}
	if(props["ele damage"]) {
	  if(intp(props["ele damage"]))
	    props["ele damage"] = ({ props["ele damage"] });
	  if(!props["damage types"]) props["damage types"] = ([]);
	  i = sizeof(props["ele damage"]);
	  while(i--) {
	   for(j = 0; j < sizeof(props["element types"]); j++) {
	    if(i==0) tmp_str = props["element types"][j];
	    else tmp_str = sprintf("%s #%d", props["element types"][j],
				   (i+1));
	    props["damage types"] += ([ tmp_str : props["ele damage"][i] ]);
	   }
	  }
	}
	if(props["ele protection"]) {
	  if(!props["protection types"]) props["protection types"] = ([]);
	  for(j = 0; j < sizeof(props["element types"]); j++) {
	    tmp_str = props["element types"][j];
	    props["protection types"] += ([ tmp_str : props["ele protection"] ]);
	  }
	}
  return;
}

int silent_cast(object who, int skill) {
  if(living(who) && random(100) < (int)who->query_skill("spell sense")) {
    if(random(100) < ((int)who->query_skill("spell sense") - 20))
      message("other_action", "%^CYAN%^You sense the aura of "+
	      article(props["name"]) + " spell.", who);
    return 1;
  }
  if(skill < 80) return 1;
  return 0;
}

void do_spell(mapping info) {
    string target, args;
    object caster, at, caster_env;
    int power, roll, fumble_flag, resist, i;
    int mp_cost;
	object leader;

    caster = info["caster"]; target = info["target"];
	 power = info["power"]; args = info["arg"];
   if(!caster){ TO->remove();return;}
    caster->set_casting(0);
    caster->set_magic_round(0);
    caster_env = environment(caster);
        if(environment(caster)->query_property("no spell")) {
	message("info","Mystic forces prevent casting.",caster);
	remove();
	return;
    }
    if(props["combat spell"] && environment(caster)->query_property(
	"no attack")) {
	message("info","Greater powers prevent your malice.",caster);
	remove();
	return;
    }
    message("info","%^CYAN%^You cast your spell.",caster);
    i = sizeof(bonus);
    while(i--) message("info", bonus[i], caster);
    message("other_action", "%^CYAN%^%^BOLD%^"+
	    (string)caster->query_cap_name() + " casts a spell.",
	    environment(caster), ({ caster }) );
    if(!props["no target"]) {
	switch(props["target type"]) {
	    case "any":
	  if(query_property("can be in room"))
		at = present(target, environment(caster));
		else at = 0;
		if(!at) at = present(target,caster);
		break;
	    case "player":
		if(!props["must be present"])
		at = find_player(target);
		else at = present(target,caster_env);
		if(at && !at->is_player()) at = 0;
		break;
	    case "living":
	    default:
		if(!props["must be present"])
		at = find_living(target);
		else at = present(target,caster_env);
		if(at && !living(at))at=0;
	}
	if(!at) { message("info","Can't find "+target+" here!  "
		"Your spell fails.",caster); return; }
    }
    mp_cost = props["base mp cost"];

// Ritual skill settings
	if(props["ritual spell"] || props["ritual dud"] || props["ritual final"])
	skill = props["original skill"];
	else
	skill = (int)caster->query_skill(props["skill"]);

   if(caster->query_stoned())
	  skill -= (int)caster->query_stoned() / 7;
   if((string)caster->query_class() == "moon mage" && props["moon"]) {
	  switch((int)ASTRONOMY_D->query_phase(props["moon"])) {
	    case 2:
	    case 4:
		skill = to_int(0.8 * to_float(skill));
		mp_cost = to_int(1.2 * to_float(mp_cost));
		message("info","%^YELLOW%^The moons are indifferent to you.",
		    caster);
		break;
	    case 3:
		skill = to_int(1.3 * to_float(skill));
		mp_cost = to_int(0.6 * to_float(mp_cost));
		message("info","%^YELLOW%^The moons smile upon you.",
		    caster);
		break;
	    case 1:
	    default:
		skill = to_int(0.5 * to_float(skill));
		mp_cost = to_int(1.7 * to_float(mp_cost));
		message("info","%^YELLOW%^The moons frown upon you.",
		    caster);
	}
	if((string)EVENTS_D->query_time_of_day() == "day") {
	    skill = to_int(0.8 * to_float(skill));
	    mp_cost = to_int(1.2 * to_float(mp_cost));
	}
    }
    mp_cost *= power;
    if(mp_cost >(int)caster->query_mp() && !props["can overcast"]) {
	message("info","You do not have enough magic points to cast this "
	    "spell.",caster);
	return;
    }
    if(at && living(at)) {
      props["caster message"] = replace_string(props["caster message"],"$T",
	(string)at->query_cap_name());
      props["observer message"] = replace_string(props["observer message"],"$T",
	(string)at->query_cap_name());
    } else if(objectp(at)) {
      props["caster message"] = replace_string(props["caster message"],"$T",
	(string)at->query_short());
      props["observer message"] = replace_string(props["observer message"],"$T",
	(string)at->query_short());
    }
    if(props["target observer message"]) {
    if(at && living(at))
      props["target observer message"] = replace_string(props["target observer message"],"$T",
	(string)at->query_cap_name());
	  else if(objectp(at))
	    props["target observer message"] = replace_string(props["target observer message"],"$T",
	(string)at->query_short());
    props["target observer message"] = replace_string(props["target observer message"],"$C",
	(string)caster->query_cap_name()); }
    props["target message"] = replace_string(props["target message"],"$C",
	(string)caster->query_cap_name());
    props["observer message"] = replace_string(props["observer message"],"$C",
	(string)caster->query_cap_name());
    skill += caster->query_spiritual();
	if(wizardp(caster) || caster->query_property("test ritual"))
      message("info", sprintf("Casting skill level: %d", skill), caster);
    if(mp_cost < 0) mp_cost = 0;
    props["mp cost"] = mp_cost;
    caster->add_mp(-mp_cost);
    roll = random(100) + 1;
    if(roll > (95 + skill / 20) || roll == 100) {

	if(!caster->query("no fumble") && !caster->query_property("no fumble") && !props["ritual final"]) {
// Ritual fumbling
	if(props["ritual dud"]){
	message("info","%^RED%^%^BOLD%^You fumble your spell ruining the entire group's efforts!",caster);
	message("info","%^RED%^%^BOLD%^"+(string)caster->query_cap_name()+" fumbles "+possessive(caster)+" spell ruining the groups efforts!", environment(caster),({ caster }));
	leader = RITUAL_D->query_leader(ritual);
	leader = leader->query_casting();
	leader->set_property("no cast", 1);
}
	else{
	message("info","%^RED%^%^BOLD%^You fumble your spell!",caster);
	message("info","%^RED%^%^BOLD%^"+(string)caster->query_cap_name()+" fumbles "+possessive(caster)+" spell!", environment(caster),({ caster }));
		fumble_flag = 1;
}
}
	} else if(roll > skill && !props["ritual final"]) {
// Ritual sputtering
	if(props["ritual dud"] || props["ritual spell"]){
	message("info","%^CYAN%^%^BOLD%^You sputter your spell ruining the entire group's efforts!",caster);
	message("info","%^CYAN%^%^BOLD%^"+(string)caster->query_cap_name()+" sputters "+possessive(caster)+" spell ruining the groups efforts!", environment(caster),({ caster }));
	leader = RITUAL_D->query_leader(ritual);
	leader = leader->query_casting();
	leader->set_property("no cast", 1);
}
	else{
	message("info","%^CYAN%^Your spell sputters ineffectively.", caster);
	message("info","%^CYAN%^%^BOLD%^"+(string)caster->query_cap_name()+"'s spell fails.", environment(caster),({ caster }));
}

	    caster->add_mp(mp_cost/2);
	    if(props["combat spell"] && at && member_array(caster,(object *)
		at->query_attackers()) < 0 && at != caster)
		at->kill_ob(caster,0);
      caster->add_exp2(13*props["spell level"]);
	    remove();
	    return;
    } else {
	message("info",props["caster message"], caster);
	if(objectp(at))
	    message("info",props["target message"], at);
	message("info",props["observer message"], environment(caster),
	    objectp(at)?({ at, caster }) : ({ caster }));
	if(props["target observer message"] && !props["no target"] &&
	    objectp(at))
	    message("info",props["target observer message"],
		environment(at), ({ caster, at }));
	fumble_flag = 0;
    caster->add_exp2(25 * props["spell level"]);
    if(props["spell level"] > 10)
        caster->add_exp2((props["spell level"] - 10) * 50 / 3);
    }
    resist_flag = 0;
    if(!fumble_flag && props["can resist"] && objectp(at)) {
	resist = 50 + (int)at->query_property("magic resistance") +
	    (stringp(props["save mod"]))?
	    ((int)at->query_stats(props["save mod"])/5):0;
	resist -= 5 * (power-1) ;
	resist -= ((int)caster->query_level() - (int)at->query_level()) * 5;
	if(resist < 0) resist = 0;
	if(random(100) < resist) {
	  if(props["can resist"] ) {
	    message("info","You resist "+(string)caster->query_cap_name()+
		"'s spell.",at);
	    if(props["combat spell"] && at && member_array(caster,(object *)at->
		query_attackers()) < 0 && at != caster)
		at->kill_ob(caster,0);
	    message("info",(string)at->query_cap_name()+" resists your spell.",
		caster);
	    message("info",(string)at->query_cap_name()+" resists "+(string)
		caster->query_cap_name() + "'s spell.",
		environment(caster),({ at, caster }));
	    remove();
	    return;
	  } else resist_flag = 1;
	}
    }
    if(intp(props["duration"])) props["duration"] *= power;
    if(props["combat spell"] && at) {
      if(member_array(caster,(object *)at->query_attackers()) < 0
	  && at != caster)
	at->kill_ob(caster,0);
    }


    this_object()->spell_func(caster, at, power, args, fumble_flag);
    return;
}

void spell_func(object caster, object at, int power, string args, int flag) {
    int roll, i, j, k, dice, tmp_dice, sides;
    int resist;
    string dam_type, dumb_str, mesarg;
    string *tmp, targ_limb, tmp_str, res;
    object *ob, *inv, tmp_ob;

    if(stringp(props["spell type"]) && !pointerp(props["spell type"])) {
      tmp = ({ props["spell type"] });
      props["spell type"] = tmp;
    }
    else tmp = props["spell type"];
    for(i=0;i<sizeof(props["spell type"]);i++)
    switch(props["spell type"][i]) {
	case "damage":
	case "area damage":
	    if(!mapp(props["damage types"])) return;
	    inv = ({});
	    j = sizeof((tmp = keys(props["damage types"])));
	    ob = all_inventory(environment(caster));
	    if(props["spell type"][i] == "area damage" &&
		!flag)
		inv = filter_array(all_inventory(
		    environment(caster)),"not_party",
		    this_object(),caster);
	    else if(!flag)
		inv = ({ at });
	    else if(flag && props["spell type"][i] == "area damage")
		inv = filter_array(all_inventory(environment(caster)),
		    "random_filter",this_object());
	    else { 
	      inv = filter_array(all_inventory(environment(caster)),
				"filter_area", this_object());
	      if(sizeof(inv)) inv = ({ inv[random(sizeof(inv))] });
		else inv = ({ caster });
	    }
	    message("info",props["target message"],inv,
		(flag && sizeof(inv) && inv[0] != at)?({}):({ at }));
      if(inv && sizeof(inv))
	call_other(inv, "kill_ob", caster, 0);
	    k = sizeof(inv);
	    dumb_str = "";
	    if(!k) dumb_str = " nobody.";
	    else {
	      while(k--) {
	  if(stringp(inv[k]->query_name())) {
		dumb_str += " "+(string)inv[k]->query_name();
	  if(k == 0) dumb_str += "."; else dumb_str += ",";
		}
	      }
	    }
	    message("info","%^CYAN%^Your spell hits"+dumb_str,caster);
	    message("info",capitalize((string)caster->query_name()) +
	"'s spell hits"+dumb_str,all_inventory(environment(caster)),
	({ caster }));
	    k = sizeof(inv);
	    while(k--) {
		  inv[k]->kill_ob(caster,1);
	    if(props["can resist"] && inv[k] != at) {
		if(!(resist = (int)inv[k]->query_property("magic resistance")
		    )) resist = 50;
		if(props["save mod"])
		    resist += (int)inv[k]->query_stats(props["save mod"]) / 5;
		resist -= 5 * (power - 1) ;
		resist -= ((int)caster->query_level() - (int)inv[k]->query_level())*5;
		resist_flag = 0;
		if(random(100) < resist) {
		  message("info","%^CYAN%^You resist a spell!",inv[k]);
		  message("info", "%^CYAN%^%^BOLD%^"+(string)inv[k]->query_cap_name()+
		     " resists your spell.", caster);
		  if(props["can resist"])
		    continue;
		  else
		    resist_flag = 1;
		}
	    }
	    tmp = keys(props["damage types"]);
	    j = sizeof(tmp);
	    while(j--) {
		if(!pointerp(tmp)) {
		    message("info","There is a bug in the spells system.\n",
			({ caster }));
		    continue;
		}
		if(sscanf(tmp[j],"%s #%s",dam_type,dumb_str) == 2) {
		    tmp[j] = dam_type;
		}
		if(member_array(tmp[j],DAMAGE_TYPES) < 0) continue;
		if(!living(inv[k])) continue;
		if(stringp(args) && random(100)+1 <= skill)
		    inv[k]->set_target_limb(args);
		targ_limb = (string)inv[k]->return_target_limb();
		res = (string)DAMAGE_D->attack_result(sprintf("%d %d %d",
		     random(100)+1,(props["damage types"][tmp[j]] *
		     (power + 2)/2) / (resist_flag?2:1), (int)inv[k]->
			query_current_protection(targ_limb,tmp[j])));
		switch(explode(res," ")[0]) {
		    case "DAMAGE":
			sscanf(res,"DAMAGE %d",roll);
			if(query_property("damage multiplier")) {
			  if(floatp(query_property("damage multiplier")))
			    roll = to_int(to_float(roll) * (float)query_property("damage multiplier"));
			  else if(intp(query_property("damage multiplier")))
			    roll *= (int)query_property("damage multiplier");
			}
			roll *= power;
			roll /= (resist_flag?2:1);
			inv[k]->do_damage(targ_limb, roll);
			inv[k]->check_on_limb(targ_limb);
			caster->add_exp(roll/ATTACK_DAMAGE_EXP_MOD);
			break;
		    case "CRITICAL":
		  if(query_property("damage multiplier")) {
			  if(floatp(query_property("damage multiplier")))
			    roll = to_int(10.0 * (float)query_property("damage multiplier"));
			  else if(intp(query_property("damage multiplier")))
			    roll = 10 * (int)query_property("damage multiplier");
			}
			roll *= power;
			roll /= (resist_flag?2:1);
			inv[k]->do_damage(targ_limb, roll);
			inv[k]->check_on_limb(targ_limb);			
			caster->add_exp(roll/ATTACK_DAMAGE_EXP_MOD);
			do_criticals(({ tmp[j] + replace_string(res,
			     "CRITICAL","") }), targ_limb,caster, inv[k]);
			break;
		}
	    } }
	    break;
	case "healing":
	case "area healing":
	    if(!props["heal dice"]) break;
	    if(props["spell type"][i] == "area healing")
		inv = filter_array(all_inventory(
		    environment(caster)),"not_attacker",
		    this_object(),caster);
	    else
		inv = ({ at });
	    if(sscanf(props["heal dice"],"%dD%d",dice, sides) != 2) break;
	    j = sizeof(inv);
	    if(flag && props["fumble message"])
		message("info",props["fumble message"],inv);
	    else
		message("info",props["target message"],inv,({ at }));
		roll = 0;
		if(dice < 0) { dice *= -1; power *= -1; }
		while(dice--) roll += random(sides)+1;
		roll *= power;
	    while(j--) {
		if(!flag && props["can resist"]) {
		    if(!intp(resist = (int)inv[j]->query_property(
			"magic resistance"))) resist = 0;
		    if(stringp(props["save mod"])) resist += (int)inv[j]->
			query_stats(props["save mod"]) / 7;
		    if(random(100) < resist) {
			message("info","%^CYAN%^You resist a spell.",inv[j]);
			continue;
		    }
		}
		if(!flag){
		    inv[j]->heal(roll/(resist_flag?2:1));
		    caster->add_exp2(roll);
		}
		else {
		   inv[j]->do_damage((targ_limb = 
			(string)inv[i]->return_target_limb()),
			roll/(resist_flag?4:2));
		   inv[j]->check_on_limb(targ_limb);
                   caster->add_exp2(-roll);
		}
	    }
	    break;
	case "stat mod":
	    if(!props["stat dice"] || sscanf(props["stat dice"],"%dD%d",
		dice,sides) != 2 || !props["stats"])
		break;
	    if(stringp(props["stats"]) && !pointerp(props["stats"])) {
		tmp_str = props["stats"];
		map_delete(props,"stats");
		props["stats"] = ({ tmp_str });
	    }
	    j = sizeof(props["stats"]);
            if(dice < 0) { dice *= -1; power *= -1; }
            tmp_dice = dice;
	    while(j--) {
		roll = 0;
            dice = tmp_dice;
		while(dice--) roll += random(sides)+1;
		roll *= power;
		roll *= (flag)? -1 : 1;
		if(stringp(props["duration"]) && props["duration"] == "permanent") at->set_stats(props["stats"][j],
		  (int)at->query_base_stats(props["stats"][j])+roll/(resist_flag?2:1));
		else {
		    roll /= resist_flag?2:1;
		    at->add_stat_bonus(props["stats"][j],roll);
                switch(absolute_value(roll)) {
                  case 0..3:  mesarg = "mildly."; break;
                  case 4..8: mesarg = "moderately."; break;
                  case 9..13:mesarg = "respectably."; break;
                  case 14..18:mesarg = "significantly."; break;
                  case 19..23:mesarg = "%^YELLOW%^enormousely."; break;
                  case 24..28:mesarg = "%^YELLOW%^sTaGgeRiNglY!"; break;
                  case 29..33:mesarg = "%^BOLD%^YELLOW%^UNBELIEVABLY!!!"; break;
                  default: mesarg = "%^BOLD%^RED%^U N F A T H O M A B L Y!!!";
                }
                message("info", "Your "+props["stats"][j]+" has been affected "+mesarg, at);
		    delayed_call("remove_stat_mod",props["duration"],
			([ "who" : at, "what" : props["stats"][j],
			"amount" : -roll ]));
		}
	    }
	    break;
	case "skill mod":
	    if(!props["skill dice"] || sscanf(props["skill dice"],"%dD%d",
		dice,sides) != 2 || !props["skills"])
		break;
	    if(stringp(props["skills"]) && !pointerp(props["skills"])) {
		tmp_str = props["skills"];
		map_delete(props,"skills");
		props["skills"] = ({ tmp_str });
	    }
	    j = sizeof(props["skills"]);
	    while(j--) {
		roll = 0;
		if(dice < 0) { dice *= -1; power *= -1; }
		for(k=1;k <= dice;k++) roll += random(sides)+1;
		roll *= power;
		roll *= (flag)? -1 : 1;
		if(stringp(props["duration"]) && props["duration"] ==
		    "permanent") at->set_skill(props["skills"][j],
			(int)at->query_base_skill(props["skills"][j])+roll
			/ (resist_flag?2:1));
		else {
		    roll /= resist_flag?2:1;
		    at->add_skill_bonus(props["skills"][j],roll);
		    delayed_call("remove_skill_mod",props["duration"],
			([ "who" : at, "what" : props["skills"][j],
			"amount" : -roll ]));
		}
	    }
	    break;
	case "protection":
	    if(!mapp(props["protection types"]) || !props["duration"]) break;
	    if(props["duration"] == "permanent") {
		at->set_overall_ac(values(props["protection types"])[0]*power);
		break;
	    }
	    if(props["stack key"]) {
	      if((int)at->query_stack(props["stack key"]) >= props["stack num"]) {
		message("info", "You may not cast another spell of this type on "+
		    (string)at->query_cap_name()+ ".  Your spell fails.", caster);
		message("info", "%^CYAN%^%^BOLD%^"+(string)caster->query_cap_name() +
		    "'s spell fails.", environment(caster), ({ caster }) );
		if(!props["must be present"])
		  message("info", "%^CYAN%^%^BOLD%^The protection spell has no effect.",
			caster);
			  remove();
			  return;
	      }
	    }
	    tmp_ob = clone_object("/std/spells/shadows/ac_shadow");
	    if(props["stack key"])
	      tmp_ob->set_stack_key(props["stack key"]);
	    tmp_ob->start_shadow(at,props["duration"]);
	    j = sizeof((tmp = keys(props["protection types"])));
	    while(j--)
		if(member_array(tmp[j],DAMAGE_TYPES) > -1)
		    tmp_ob->set_shad_ac(props["protection types"][tmp[j]]*power
			/ (resist_flag?2:1),tmp[j]);
	    tmp_ob->set_expire_message("%^CYAN%^A protection spell wears off.");
	    break;	    
    }
    remove_delayed_call("remove");
    delayed_call("remove", 4);
}

