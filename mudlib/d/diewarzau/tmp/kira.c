#include <std.h>

inherit MONSTER;

nosave int count;

create() {
  object ob;
  
  ::create();
  set_name("kira");
  set_id( ({ "kira", "mage", "arch-mage" }) );
  set_short("Kira the Arch-Mage");
  set("race", "high-elf");
  set_long("Kira is a tall, slender high-elf clad in ornate robes.  "
	   "Her beauty is equalled only by the hate that burns in her eyes.");
  set("aggressive");
  set_body_type("human");
  set_level(15);
  set_exp(280000);
  set_stats("intelligence", 131);
  set_stats("wisdom", 110);
  set_skill("conjuration", 78);
  set_skill("magery", 78);
  set_skill("illusionism", 70);
  set_spell_level("fireball", 6);
  set_spell_level("lightning bolt", 5);
  set_spell_level("blur", 6);
  add_spell("fireball", "$A");
  add_spell("lightning bolt", "$A");
  add_spell("blur", "$(ME)", 0, 720);
  set_spell_level("power word, stun", 6);
  add_spell("power word, stun", "$A", 1, 36);
  set_skill("two handed staff", 75);
  set_casting_chance(10);
  set_combat_chance(70);
  ob = new("/d/damned/virtual/quarter-staff_6.weapon");
  ob->set_property("enchantment", 4);
  ob->set_property("hit bonus", 6);
  ob->set_property("brittle", 7);
  ob->add_enchantment("flame", "elemental blade", 6, 1000, "fire");
  force_me("wield staff in right hand and left hand");
  set_wimpy(0);
  add_money("gold", 11);
  add_money("mithril", 1);
  add_money("silver", 200+random(100));
  set_max_hp(800);
  set_hp(800);
  set_mp(1200);
  set_speed(60);
  set_moving(1);
}

void catch_tell(string str) {
  string who;
  object ob;
  
  if(sscanf(str, "%s begins casting%*s", who) == 2) {
    ob = present(lower_case(who), environment());
    if(ob && ob->is_player())
      force_me("cast *6 fireball at "+(string)who->query_name());
  }
  return;
}

void remove_count() {
  count = 0;
  return;
}

void heart_beat() {
  object ob;
  string ex;
  
  ::heart_beat();
  if(!environment()) return;
  if(query_current_attacker() &&
     (int)this_object()->query_hp() < 100 &&
     (int)query_current_attacker()->query_hp() >
     (int)this_object()->query_hp()) {
    ex = (string *)environment()->query_exits();
    ex = filter_array(ex, (: !(call_other($2, $3, $1)) :),
		      environment(), "query_door");
    if(sizeof(ex)) force_me(ex[0]);
    else force_me("cast *6 fireball at "+(string)
		  query_current_attacker()->query_name());
  } else if((int)this_object()->query_hp() < 100 &&
            query_current_attacker())
    force_me("cast *6 fireball at "+(string)
	     query_current_attacker()->query_name());
  if(!count && query_current_attacker()) {
    force_me("activate flame on staff");
    count = 1;
    delayed_call("remove_count", 1002);
  }
}
