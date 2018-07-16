
#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbcc5", "east");
    add_exit(ROOMS+"dbcc7","west");
    set("short","Commerce Court");
    set("long","Commerce Court runs east-west through Daybreak Ridge.");
}
