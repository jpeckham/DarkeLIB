inherit "/std/guilds/join_room";

void create() {
  set_class_name("seer");

  ::create();
  set_property("light", 2);
  set_property("indoors", 1);
  set_related_guilds(({ }));
  set("short", "Audience Hall");
  set("long", @TEXT
You have entered the the Audience Hall of the Great Oracle.
The room is bare save for a single rattan peacock chair
opposite the landing from the circular stair that leads
further into the tower and back down to the Hall of Trials.

TEXT
);
  //  The "\n" is important at the end of the long desc.

  set_exits( ([ 
    "down" : "/d/damned/guilds/seer/rooms/trial_room",
    "up"   : "/d/damned/guilds/seer/rooms/main_room",
    ]) );
  set("guildmaster", "/d/damned/guilds/seer/mobs/seer_gm");
  set_guild_obj("/d/damned/guilds/seer/items/seer_obj");

  set_property("base hp regen", 1);
  set_property("base mp regen", 10);
  set_property("hp advance", 10);
  set_property("mp advance", 55);
  set_property("guild mods",([ "intelligence" : 15, "wisdom" :  25 ]) );

  new("d/damned/guilds/join_rooms/tcan")->move(this_object());
  this_object()->reset();
  return;
}




