#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit("/d/damned/virtual/room_8_9.world", "mountains");
    add_exit(ROOMS+"dbew2","west");

    set("short","Main Street");
    set("long","Main Street runs east-west through Daybreak Ridge.");
}
