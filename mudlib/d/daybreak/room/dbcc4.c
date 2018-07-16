#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbcc5", "north");
    add_exit(ROOMS+"dbcc3","south");
    add_exit("/d/damned/guilds/join_rooms/enchanter_join", "east");
    add_exit(ROOMS+"dbcc9","west");
    set("short","Commerce Court");
    set("long","Commerce Court runs north and west through Daybreak Ridge.");
}
