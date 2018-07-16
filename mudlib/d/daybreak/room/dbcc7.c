
#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbcc6", "east");
    add_exit(ROOMS+"dbcc8","south");
    add_exit("/d/damned/guilds/join_rooms/tinker_join", "west");
    set("short","Commerce Court");
    set("long","Commerce Court runs east and south through Daybreak Ridge.");
}
