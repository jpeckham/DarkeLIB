#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbns1", "south");
    add_exit(ROOMS+"dbns3","north");
    add_exit("/d/damned/inn/main_room", "east");
    set("short","Daybreak Valley Road");
    set("long","Daybreak Valley Road is the main north-south artery in Daybreak Ridge.  The road is lined with hedge rows about four and a half feet tall which are cut to allow entry to the various homes and shops along the road.  Tall lamp posts are placed along the road to provide light during the night.  To the east the Blue Dragon inn sits, inviting you to enter and relax, further north you can see the fountain in the town square.");
}
