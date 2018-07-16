#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbew10", "north");
    add_exit(ROOMS+"dbvl2", "south");
    set("short","Victory Lane");
    set("long","Victory Lane runs north-south through Daybreak Ridge");
}
