#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbns5", "north");
    add_exit(ROOMS+"db_square","south");
 
    set("short","Daybreak Valley Road");
    set("long", NSBASE+NSRES);
}
