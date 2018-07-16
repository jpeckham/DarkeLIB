#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbvl5", "west");
    add_exit(ROOMS+"shop/weaponsmith", "east");
    set("short","Victory Lane");
    set("long","Victory Lane runs west through Daybreak Ridge");
}
