inherit "std/room";


void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit("/d/damned/akkad/ak_haven3", "west");
    add_exit("/d/damned/akkad/ak_center", "east");
    add_exit("/d/damned/akkad/ak_marknw", "north"); 
    add_exit("/d/damned/akkad/ak_marksw", "south");
    set("short","West side of Market Square");
    set("long","You are on the West side of Market Square. "+
	"It is a place of great commerce and central to the city's "+
	"economy.  At the center of it all stands a granite statue " +
	"of Dranaxar, the first archmage.  It is an island of calm " +
	"amidst the activity of the marketplace.");

}
