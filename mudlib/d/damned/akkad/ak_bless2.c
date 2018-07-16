inherit "std/room";


void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit("/d/damned/akkad/ak_bless3", "west");
    add_exit("/d/damned/akkad/ak_bless1","east");
    add_exit("/d/standard/square", "south");
    set("short","On Blessing Way");
    set("long","You are on the Blessing Way, the street of the clerics." +
         "  The entrance to the church is here to the south. "+
	"The clerics guild is to the east. ");
    
}
