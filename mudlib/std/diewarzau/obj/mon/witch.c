#include <std.h>



inherit MONSTER;



nosave object scrying;

nosave int casting_scry;

nosave int snooped;



create() {

  ::create();

    set_name("witch");

    set_id( ({ "witch", "zerrah" }) );

    set_short("Zerra the Witch");

    set("race", "drow-elf");

    set_languages(({ "common", "drow" }));

    set_long(

    "Zerra is an elderly drow who possesses many magical abilities.  "

    "She appears preoccupied despite your presence.");

    set_body_type("human");

    set_level(29);

    set_max_mp(7000);

    set_mp(7000);

    set_max_hp(6000);

    set_hp(6000);

    set_property("base mp regen", 400);

    set_overall_ac(120);

    set_property("enhance criticals", -2);

    set_property("melee damage", ([ "fire" : 55, "electricity" : 55 ]));

    set_skill("melee", 85);

    set_skill("dodge", 125);

    set_skill("parry", 120);

    set_stats("dexterity", 145);

    set_stats("intelligence", 150);

    set_skill("conjuration", 130);

    set_spell_level("vacuum bolt", 6);

    add_spell("vacuum bolt", "$A");

    set_spell_level("link", 6);

    set_skill("sorcery", 110);

    set_spell_level("scry", 6);

    set_skill("magery", 130);

    set_spell_level("sanctuary", 6);

    set_skill("prayer", 130);

    set_combat_chance(200);

    set_casting_chance(50);

    set_spell_level("blizzard", 6);

    add_spell("blizzard", "", 1);

    set_skill("illusionism", 130);

    set_spell_level("blur", 6);

    add_spell("blur", "$(ME)", 1, 245);

    set_spell_level("armour of faith", 6);

    set_spell_level("decrease metabolism", 6);

    add_spell("decrease metabolism", "$A", 1, 200);

    add_spell("armour of faith", "$(ME)", 0, 1082);

    set_spell_level("heal", 6);

    add_spell("heal", "$(ME)");

    set("linked spells", ([ "casting time" : 10, "cost" : 500,

      "scry" : ([ "file" : "/std/spells/scry", "arg" : "$", "target" : 0, "power" : 6 ]),

      "summon" : ([ "file" : "/std/spells/summon", "arg" : 0, "target" : 0, "power" : 6 ]),

      "sanctuary" : ([ "file" : "/std/spells/sanctuary", "arg" : 0, "target" : 0, "power" : 1 ]),

      "fireball" : ([ "file" : "/std/spells/fireball", "arg" : 0, " target" : "$", "power" : 6 ]) ]) );

    set("spell keys", ({ "scry", "summon", "sanctuary", "fireball" }));

}



void set_snooped() { snooped = !snooped; }



void catch_tell(string str) {

  string who;

  object plyr, *inv;



  str = eliminate_colour(str);

  if(snooped) {

    plyr = find_player("diewarzau");

    if(plyr) message("info", "%witch% "+str, plyr);

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

      force_me("cast *6 vacuum bolt at "+(string)plyr->query_name());

    }

    return;

  }

  if(str == "YOU CAN MOVE AGAIN!!!" && !query_casting() && (inv=query_attackers())) {

    if(sizeof(inv) >= 3)

      force_me("cast *6 blizzard");

    else if(sizeof(inv))

      force_me("cast *6 vacuum bolt at "+

        (string)inv[0]->query_name());

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



varargs void heart_beat(int flag) {

  object *att;

  

  ::heart_beat(flag);



 /*

  * Below makes sure the heart beat doesnt get turned off if we are scrying somebody.

  */

  

  if(!heart_beat_on && scrying)

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

 

  }



 /*

  * Now, if the scrying var is set, and that person is not present, begin casting

  * the linked set.

  */

  

  else if(!casting_scry && scrying && interactive(scrying) &&

      !present(scrying, environment())) {

    force_me("remove sanctuary");

    force_me("cast *3 link at "+(string)scrying->query_name());

    casting_scry = 1;

    delayed_call("reset_link", 24);

  }

}



void reset_link() {

  scrying = 0;

  casting_scry = 0;

  set("linked spells", ([ "casting time" : 10, "cost" : 500,

      "scry" : ([ "file" : "/std/spells/scry", "arg" : "$", "target" : 0, "power" : 6 ]),

      "summon" : ([ "file" : "/std/spells/summon", "arg" : 0, "target" : 0, "power" : 6 ]),

      "sanctuary" : ([ "file" : "/std/spells/sanctuary", "arg" : 0, "target" : 0, "power" : 1 ]),

      "fireball" : ([ "file" : "/std/spells/fireball", "arg" : 0, "target" : "$", "power" : 6 ]) ]) );

  set("spell keys", ({ "scry", "summon", "sanctuary", "fireball" }));

}

