#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbvl2", "north");
    add_exit(ROOMS+"dbvl4", "east");
    add_exit("/d/damned/guilds/join_rooms/elementalist_join", "west");
    set("short","Victory Lane");
    set("long","Victory Lane runs north and east through Daybreak Ridge");
}
