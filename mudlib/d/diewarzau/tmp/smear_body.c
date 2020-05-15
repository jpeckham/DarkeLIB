#include <std.h>

inherit MONSTER;

nosave int casting_scry;
nosave int snooped;
nosave object scrying;
nosave int delay;
nosave object following;

create() {
  ::create();
    set_name("smear body");
    set_id( ({ "smear", "smear body", "rottweiler", "smear body the rottweiler" }) );
    set_short("Smear Body the Rottweiler");
    set("race", "blink dog");
    set_languages(({ "common" }));
    delay = 0;
    set_long(
    "Smear Body is a HUGE rottweiler with impressive fangs.  There is a "
    "certain wisdom behind his eyes that tells of a thousand battles.");
    set_body_type("quadruped");
    set_level(29);
    set_max_mp(7000);
    set_mp(7000);
    set_max_hp(6000);
    set_hp(6000);
    set_property("base mp regen", 400);
    set_overall_ac(120);
    set_property("enhance criticals", -2);
    set_property("melee damage", ([ "cutting" : 80, "impaling" : 30 ]));
    set_skill("melee", 145);
    set_skill("dodge", 155);
    set_skill("parry", 120);
    set_skill("perception", 140);
    set_skill("resist stun", 160);
    set_stats("dexterity", 145);
    set_stats("strength", 190);
    set_stats("intelligence", 150);
    set_skill("conjuration", 130);
    set_spell_level("link", 6);
    set_skill("sorcery", 110);
    set_spell_level("transport", 6);
    set_spell_level("slow", 6);
    set_spell_level("scry", 6);
    set_skill("magery", 130);
    set_skill("prayer", 130);
    set_combat_chance(200);
    set_casting_chance(50);
    set_skill("illusionism", 130);
    set_spell_level("blur", 6);
    add_spell("blur", "$(ME)", 1, 245);
    set_spell_level("heal", 6);
    add_spell("heal", "$(ME)");
    set("linked spells", ([ "casting time" : 10, "cost" : 500,
      "scry" : ([ "file" : "/std/spells/scry", "arg" : "$", "target" : 0, "power" : 6 ]),
      "transport" : ([ "file" : "/std/spells/transport", "arg" : 0, "target" : 0, "power" : 6 ]),
      "slow" : ([ "file" : "/std/spells/slow", "arg" : 0, "target" : "$", "power" : 6 ]) ]) );
    set("spell keys", ({ "scry", "transport", "slow" }));
}

void set_snooped() { snooped = !snooped; }

void catch_tell(string str) {
  string who, dir, env;
  object plyr, *inv;

  str = eliminate_colour(str);
  if(snooped) {
    plyr = find_player("diewarzau");
    if(plyr) message("info", "%smear body% "+str, plyr);
  }
  if(!this_object()->query_casting() && sscanf(str, "%s begins casting a spell.", who)) {
    if(who == "Someone") {
      inv = all_inventory(environment());
      inv = filter_array(inv, (: call_other :), "is_player");
      inv = filter_array(inv, (: call_other :), "query_invis");
      inv = filter_array(inv, (: call_other :), "query_casting");
      if(!sizeof(inv)) {
        return;
      }
      plyr = inv[0];
    }
    else
      plyr = present(lower_case(who), environment());
    if(!plyr) {
      return;
    }
    if(plyr->is_player() && (int)plyr->query_level() > 15) {
      force_me("speak common");
      force_me("say DIE, FOUL KNAVE!");
      kill_ob(plyr, 1);
    }
    return;
  }
  if(sscanf(str, "You are magically transported to %s", who)) {
    delayed_call("kill_the_dude", 2, who);
    return;
  }
  return;
}

void init() {
  ::init();
  if(this_player()->is_player() && (int)this_player()->query_level() > 15 &&
    this_player()->query_casting() && !query_casting()) {
    force_me("speak drow");
    force_me("say I am privy to such tricks, foul mage!");
    force_me("cast *6 vacuum bolt at "+(string)this_player()->query_name());
    kill_ob(this_player(), 1);
  }
  return;
}

void kill_the_dude(string who) {
  object plyr;
  
  who = lower_case(who);
  if(scrying && present(scrying, environment()))
    plyr = scrying;
  else plyr = present(who, environment());
  if(plyr && plyr->is_player()) {
    kill_ob(plyr, 1);
    plyr->add_follower(this_object());
    following = plyr;
  }
  return;
}

varargs void heart_beat(int flag) {
  object *att;
  
  ::heart_beat(flag);

 /*
  * Below makes sure the heart beat doesnt get turned off if we are scrying somebody.
  */
  
  if(!heart_beat_on && (scrying || scrying))
    set_heart_beat(heart_beat_on = 1);
    
 /*
  * Below checks if the "scrying" variable has not been set, but someone is
  * attacking, it will set "scrying" to that person.  The !interactive() check
  * prevents link-dead players from being summoned.
  */

  if(query_current_attacker() && (!scrying || !interactive(scrying))) {
    att = filter_array(query_attackers(), (: call_other :), "is_player");
   /*
    * Only do this to players!
    */
    if(sizeof(att))
      scrying = att[0];
    return;
 
  }

 /*
  * Now, if the scrying var is set, and that person is not present, begin casting
  * the linked set.  Do not cast if we are currently beating on the guy we are
  * following.
  */

  if(scrying && scrying->query_ghost()) scrying = 0;
  if(following && following->query_ghost()) {
    following->remove_follower(this_object());
    following = 0;
  }
  if(!scrying) return;
  
  if(!casting_scry  && interactive(scrying) &&
      !present(scrying, environment()) && (!following ||
      !present(following, environment()))) {
    force_me("cast *3 link at "+(string)scrying->query_name());
    casting_scry = 1;
    delayed_call("reset_link", 30);
  }
  
}

void debug_that_shit() {
  write(sprintf("Casting_scry: %d\nscrying: %O\nfollowing: %O\n",
    casting_scry, scrying, following));
}


void reset_link() {
  scrying = 0;
  casting_scry = 0;
  set("linked spells", ([ "casting time" : 10, "cost" : 500,
      "scry" : ([ "file" : "/std/spells/scry", "arg" : "$", "target" : 0, "power" : 6 ]),
      "transport" : ([ "file" : "/std/spells/transport", "arg" : 0, "target" : 0, "power" : 6 ]),
      "slow" : ([ "file" : "/std/spells/slow", "arg" : 0, "target" : "$", "power" : 6 ]) ]) );
  set("spell keys", ({ "scry", "transport", "slow" }));
}
