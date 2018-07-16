inherit "/std/room";
void create() {
   ::create();
   set_property("indoors",1);
   set_property("light",3);
  set_property("no scry", 1);
set_property("no summon", 1);
	set_property("no teleport",1);
   add_exit("/d/damned/guilds/tinker/ti_treas", "north");
   add_exit("/d/damned/guilds/tinker/train_room", "east");
   set("short","The Forge Room");
   set("long",
"This room is very hot with the fires of several forges. "+
  "You see some tinkers hammering away, making wondrous items. The Tinker Treasury is to the north.");
}
void reset() {
object forge;
::reset();
if(!present("forge")) {
forge = new("/wizards/azura1/greater_forge");
        forge->set_weight(100000000);
        forge->move(this_object());
}
if(!present("guard")) {
    new("/d/damned/guilds/tinker/t_guard")->move(this_object());
}
if(!present("can")) {
    new("/d/damned/guilds/join_rooms/tcan")->move(this_object());
}

}
