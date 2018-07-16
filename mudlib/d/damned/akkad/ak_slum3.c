inherit "std/room";


void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit("/d/damned/akkad/ak_slum1", "south");
    add_exit("/d/damned/guilds/join_rooms/nightblade_join", "west");
    set("short","Dead end in the Slums");
    set("long","This is a dead end in the slums.  There does not appear " +
	"to be much of interest here...but wait, you notice a small door "+
	"to the west.  It seems designed not to attract too much attention.  "+
	"Could this possibly be the entrance to the notorious Nightblade "+
	"Guild?");
}
