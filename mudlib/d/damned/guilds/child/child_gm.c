//	DarkeLIB 0.1
//	Diewarzau 8/8/95

inherit "/std/guilds/npc_guildmaster";

void create() {

  ::create();
  set_join_room("/d/damned/guilds/join_rooms/child_join");
  set("short", "The Newbie Guildmaster");
  set("long", "The Guildmaster will kill anyone period so do "+
      "bother trying.");
   set_level(156);
   set_max_hp(6000000);
   set_hp(6000000);
   set_overall_ac(750);
  set_property("base hp regen", 300);
  set_gender("male");
  set("race", "high-elf");
  set_body_type("human");
  set_id(({ "guildmaster", "newbie", "newbie guildmaster" }));
  set_name("guildmaster");
  set_property("melee damage", ([ "crushing" : 200 ]));
  return;
}

int query_num_rounds() { return 3; }
