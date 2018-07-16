inherit "std/room";


void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
     add_exit("/d/damned/guilds/join_rooms/paladin_join", "south");
    add_exit("/d/damned/akkad/ak_sd2", "north");
    set("short","South end of Iron Trail");
    set("long","You are at the south end of Iron Trail.  The entrance to " +
	"Paladins Guild is south of you, while the Cavaliers Guild is to the" +
	" west.");
    
}
