#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbvl5", "east");
    add_exit(ROOMS+"dbvl3", "west");
    set("short","Victory Lane");
    set("long","Victory Lane runs north and east through Daybreak Ridge");
}
