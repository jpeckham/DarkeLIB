inherit "/std/guilds/vault_join_room";

void create() {
  //  IMPORTANT:  The following MUST appear before ::create();
  set_class_name("ranger");

  ::create();
  set_property("light", 2);
  set_property("indoors", 0);
new("d/damned/guilds/join_rooms/tcan")->move(this_object());
  set_related_guilds(({ "cleric", "fighter" }));
  set("short", "The Ranger's Guild Joining Room");
  set("long", "A forest-like setting surrounds you. Plants, birds, small animals etc., can be seen all around in every direction. The walls are covered in a damp moss and the floors are carpeted with low-growing plants in true tree-hugger fasion. The smell of evergreens prevades your nostrils and the sounds of chittering squirrels can be heard from above. To the east a dimly lit room can be seen and below you hangs a rope ladder. Off to the west you can see branches of a nearby tree.\n");
  //  The "\n" is important at the end of the long desc.

  set_exits( ([ 
              "west" : "wizards/khojem/new/room/htree2",
              "east" : "/d/damned/guilds/ranger/shop",
              "down" : "/d/damned/guilds/ranger/rg_skills" ]) );

    set("guildmaster", "/d/damned/guilds/ranger/rg_gm");
  //  The guildmaster is automatically cloned if no pc guildmaster is
  //  found.

  set_guild_obj("/d/damned/guilds/ranger/rg_obj");
  //  This is the filename for the guild's guildmaster object, carried
  //  by the PC guildmaster if any.

  //  The following lines set the properties for the guild.

  set_property("base hp regen", 4);
  set_property("base mp regen", 10);
  //  Base heling per tick.  Note that the default is 3 for both.
  //  Keep in mind that this increases with level.

  set_property("hp advance", 27);
  set_property("mp advance", 26);
  //  Base advance per level im max mp/hp.  Default is 10.

  set_property("guild mods",
       ([ "intelligence" :  10, "constitution" : 5, "dexterity" : 5 ]) );
  //  Stat mods received upon joining.

  //  The following line is necessary:
  this_object()->reset();
  return;
}
