#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbcc4", "east");
    add_exit(ROOMS+"dbcc8","west");
    set("short","Commerce Court");
    set("long","Commerce Court runs east-west through Daybreak Ridge.");
}
