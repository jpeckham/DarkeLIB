#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbbw2","east");
    add_exit("/d/damned/guilds/join_rooms/war-priest_join.c","north");
    add_exit("/d/damned/guilds/join_rooms/paladin_join.c","south");
    set("short","Blessing Way");
    set("long","Blessing Way runs east through Daybreak Ridge");
}
