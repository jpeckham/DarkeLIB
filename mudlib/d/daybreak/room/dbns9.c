#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbns10", "north");
    add_exit(ROOMS+"dbns8","south");
 
    set("short","Daybreak Valley Road");
    set("long","Daybreak Valley Road runs north-south through Daybreak Ridge");
}
