#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dblw3", "north");
    add_exit(ROOMS+"dblw5","west");
    add_exit(ROOMS+"shop/magic","south");
    set("short","Luminous Way");
    set("long","Luminous Way runs north and west through Daybreak Ridge.");
}
