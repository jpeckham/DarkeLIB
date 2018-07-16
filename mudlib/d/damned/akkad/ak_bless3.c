inherit "std/room";


void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit("/d/damned/guilds/join_rooms/war-priest_join", "north");
    add_exit("/d/standard/hospital", "south");
    add_exit("/d/damned/akkad/ak_victory8", "west");
    add_exit("/d/damned/akkad/ak_bless2","east");
    set("short","On Blessing Way east of Victory");
    set("long",
	"To the north is the Hall of the Honoured, home of the Priests of "
	"K'thach. " +
	"There is a hospital to the south.  " +
	"The entrance to the church is to the east.  Victory Street is to "+
	"west.  There is a hospital to the south. ");
}
