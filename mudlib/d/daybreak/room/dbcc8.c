
#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbcc7", "north");
    add_exit(ROOMS+"shop/money_changer", "west");
    add_exit(ROOMS+"shop/general_store", "south");
    add_exit(ROOMS+"dbcc9","east");
    set("short","Commerce Court");
    set("long","Commerce Court runs north and west through Daybreak Ridge.");
}
