#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbcc2", "north");
    add_exit("/d/damned/akkad/mine_shop","east");
    add_exit(ROOMS+"dbew2","south");
    set("short","Commerce Court");
    set("long","Commerce Court runs north-south through Daybreak Ridge.");
}
