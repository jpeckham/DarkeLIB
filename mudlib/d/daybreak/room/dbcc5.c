#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbcc4", "south");
    add_exit(ROOMS+"dbcc6","west");
    add_exit(ROOMS+"shop/armourer", "north");
//      add_exit("/wizards/thrace/shop/ingot_shop", "east"); // yea whatever thrace laf
    set("short","Commerce Court");
    set("long","Commerce Court runs north and west through Daybreak Ridge.");
}
