#include <daemons.h>
#include <clock.h>

inherit "/std/skills/skill";

object weap;
int bonus;
nosave int rev_flag;

void do_stab(object from, object at, mapping wc, int skill);

void create() {
    ::create();
    set_stat("dexterity");
    set_dev_cost(25);
    set_fast_dev_cost(73);
    set_property("target type", "living");
    set_property("must be present",1);
    rev_flag = 0;
}

void info() {
message("help",
"Using this skill at a creature allows you to get an excellent deadly "
"strike.  It requires much concentration, but can have extremely "
"satisfactory results.  It cannot be used while in combat.  Watch out "
"for fighters with the 'reverse stroke' skill....  You MUST be wielding "
"a stabbing weapon for this endeavor.  Repeated stab attempts at the "
"same opponent result in a perception bonus for the opponent.",
this_player());
}

int stab_filter(object weapon) {
  string *wcs, type;

  if(!weapon || !objectp(weapon) || !weapon->is_weapon()) return 0;
  if(!mapp(weapon->query_all_wc())) return 0;
  type = (string)weapon->query_type();
  if(!type || (strlen(type) >= 10 && type[0..9] == "two handed")) return 0;
  wcs = keys((mapping)weapon->query_all_wc());
  if(member_array("impaling", wcs) >= 0) return 1;
  else return 0;
}

void skill_func(object from, object at, string arg) {
  object *tmp;
  mapping wc;
  mixed hit_res;
  function w_hit;
  int roll, i, *tmp_pi;

  if(environment(from) && environment(from)->query_property("no attack")) {
    message("info", "Divine forces prevent your action.", from);
    remove();
    return;
  }
  if(at->shadow_form() || at->query_ghost()) {
    message("info", "You may not back-stab a target which is immaterial.",
	    from);
    remove();
    return;
  }
  if(from->query_current_attacker()) {
    message("info", "You may not use this skill in combat.", from);
    remove();
    return;
  }
  tmp = (object *)from->query_wielded();
  if(!tmp || !sizeof(tmp)) {
    message("info", "You must have a weapon wielded to back-stab.", from);
    remove();
    return;
  }
  tmp = filter_array(tmp, "stab_filter", this_object());
  if(!sizeof(tmp)) {
    message("info", "You must wield a one-handed stabbing weapon.", from);
    remove();
    return;
  }
  tmp_pi = (int *)at->query("backstab bonus:"+(string)from->query_name());
  if(!tmp_pi) bonus = 0;
  else if(sizeof(tmp_pi) != 2) bonus = 0;
  else if((time() - tmp_pi[1]) > HOUR) bonus = 0;
  else bonus = tmp_pi[0];
  at->set("backstab bonus:"+(string)from->query_name(),
         ({ bonus + 7 + random(10), time() }));
  weap = tmp[0];
  roll = skill_contest(props["skill level"],
		       (int)at->query_skill("reverse stroke")+(bonus/2), 1);
  if(roll == 2) {
    wc = 0;
    tmp = (object *)at->query_wielded();
    if(tmp && sizeof(tmp)) {
      wc = (mapping)at->get_damage(tmp[0]);
      weap = tmp[0];
    }
    tmp = all_inventory(at);
    i = sizeof(at);
    while(!wc && i--) {
      if(tmp[i]->is_weapon()) {
        wc = (mapping)at->get_damage(tmp[i]);
	weap = tmp[i];
      }
      if(wc && !tmp[i]->query_wielded()) {
	message("info", "You wield "+(string)tmp[i]->query_short()+".",
		at);
	message("info", (string)at->query_cap_name() +
		" calmly wields " +(string)tmp[i]->query_short()+".",
		environment(at), ({ at }) );
      }
    }
    if(wc) {
      message("info", "Before you can attack, "+
	      (string)at->query_cap_name() + " inverts "+possessive(at)+
	      " weapon and strikes you with it!", from);
      message("info", "You spy "+(string)from->query_cap_name() +
	      " about to stab you in the back, so you decide to beat "+
              objective(from)+" to it, striking "+objective(from)+" skillfully with your "+
	      "weapon.", at);
      message("info", "Without even turning around, "+
	      (string)at->query_cap_name() + " inverts "+possessive(at)+
	      " weapon and delivers a deadly strike to "+
	      (string)from->query_cap_name() + ", fouling "+
	      possessive(from) + " backstabbing attempt.",
	      environment(at), ({ from, at }) );
      from->kill_ob(at, 0);
      rev_flag = 1;
      do_stab(at, from, wc, 2*(int)at->query_skill((string)weap->
						 query_type())/3);
      remove();
      return;
    }
  }
  roll = skill_contest(props["skill level"],
		       (int)at->query_skill("perception")+bonus-38, 1);
  if(roll == 2) {
    message("info", (string)at->query_cap_name() + " has spotted you!",
	    from);
    message("other_action", "You spy "+(string)from->query_cap_name() +
	    " just as "+nominative(from)+" is about to stab you in the back!",
	    at);
    message("other_action", (string)from->query_cap_name() +
	    " attempts to stab "+(string)at->query_cap_name() +
	    " in the back, but is thwarted.", environment(from),
	    ({ from, at }));
    at->kill_ob(from, 0);
    from->kill_ob(at, 1);
    remove();
    return;
  }
  message("info", "You bury your weapon deep in "+
	  (string)at->query_cap_name() + "'s back.", from);
  message("info", "You feel a sudden, awful pain as "+
	  (string)from->query_cap_name() + " buries "+
	  possessive(from) + " weapon deep in your back!", at);
  message("info", "Without warning, "+(string)from->query_cap_name() +
	  " leaps in the air, bringing "+possessive(from) +
	  " weapon down violently on "+(string)at->query_cap_name() + 
	  "'s back!", environment(from), ({ from, at }) );
  at->kill_ob(from, 0);
   do_stab(from, at, (mapping)from->get_damage(weap), props["skill level"]);
  remove();
  return;
}

void do_stab(object from, object at, mapping wc, int skill) {
  string *mesgs, *crits, *wc_keys;
  int i, j, dmg, pierce, prot;
  mapping tmpmap;

  wc_keys = keys(wc);
  i = sizeof(wc_keys);
  crits = ({});
  pierce = skill / 3;
  while(i--) {
    if(wc[wc_keys[i]] <= 0) continue;
    prot = (int)at->query_current_protection("torso", wc_keys[i]) - pierce;
    if(prot < 0) prot = 0;
    prot = prot / 3 + random(prot);
    if(prot >= 100) continue;
    dmg = 0;
    for(j=1;j <= 10;j++) dmg += 1+random(6+skill/7);
    dmg -= prot*dmg/100;
    if(wc_keys[i] != "impaling") {
      if(!rev_flag)
        dmg /= 10;
      else
        dmg /= 5;
    }
    if(dmg < 1) dmg = 1;
    dmg = (int)at->do_damage("torso", dmg);
    if(dmg >= 0 && wc_keys[i] == "impaling") {
      mesgs = (string *)DAMAGE_D->attack_message(sprintf(
                "%d %s %s:%s", dmg, ((rev_flag)?"reverse-strike":"back-stab"),
                (string)from->query_name(), (string)
                at->query_name()));
      if(sizeof(mesgs) != 3) mesgs = ({ "BUG in message sender",
        "BUG in message sender", "BUG in message sender" });
      if(strlen(mesgs[0]))
        message("my_combat",mesgs[0],from);
      if(strlen(mesgs[1]))
        message("other_combat",mesgs[1],environment(from),
          ({ from, at }));
      if(strlen(mesgs[2]))
        message("my_combat",mesgs[2],at);
    }
    if(wc_keys[i] != "impaling") {
      if(!rev_flag)
        continue;
      else
        wc[wc_keys[i]] /= 4;
    }
    if(intp(at->query_property("enhance criticals")))
      wc[wc_keys[i]] += 20 * (int)at->query_property("enhance criticals");
    else if(mapp(at->query_property("enhance criticals"))) {
      tmpmap = (mapping)at->query_property("enhance criticals");
      if(tmpmap["impaling"])
        wc[wc_keys[i]] += tmpmap["impaling"] * 20;
    }
    switch(wc[wc_keys[i]] + skill - random(80) - random(80)) {
    case -50..0:
      crits += ({ wc_keys[i] + " A" });
      break;
    case 1..20:
      crits += ({ wc_keys[i] + " B" });
      break;
    case 21..45:
      crits += ({ wc_keys[i] + " C" });
      break;
    case 46..70:
      crits += ({ wc_keys[i] + " C", wc_keys[i] + " A"});
      break;
    case 71..100:
      crits += ({ wc_keys[i] + " C", wc_keys[i] + " B" });
      break;
    case 101..140:
      crits += ({ wc_keys[i] + " C", wc_keys[i] + " C" });
      break;
    case 141..10000:
      crits += ({ wc_keys[i] + " C", wc_keys[i] + " C", wc_keys[i] + " A" });
    }
  }
  if(objectp(weap) && pointerp(weap->query_auto_critical()))
    crits += (string *)weap->query_auto_critical();
  do_critical(from, at, crits, "torso");
  if((int)at->query_hp() <= 0 && !at->is_player())
    at->set_exp((int)at->query_exp() / 2);
  return;
}











