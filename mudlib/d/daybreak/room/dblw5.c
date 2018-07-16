#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dblw4", "east");
    add_exit("/d/damned/guilds/join_rooms/arch-mage_join","south");
    set("short","Luminous Way");
    set("long","Luminous Way runs east and south through Daybreak Ridge.");
}
