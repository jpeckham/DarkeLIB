inherit "std/room";


void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit("/d/damned/akkad/ak_haven6", "east");
    add_exit("/d/damned/akkad/ak_center", "west");
    add_exit("/d/damned/akkad/ak_markne", "north"); 
    add_exit("/d/damned/akkad/ak_markse", "south");
    set("short","East side of Market Square");
    set("long","You are on the East side of Market Square. "+
	"It is a place of great commerce and central to the city's "+
	"economy.  At the center of it all stands a granite statue " +
	"of Dranaxar, the first archmage.  It is an island of calm " +
	"amidst the activity of the marketplace.");

}
