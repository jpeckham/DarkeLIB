#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbew1", "east");
    add_exit(ROOMS+"dbew3","west");
    add_exit(ROOMS+"dbcc1", "north");
    add_exit(ROOMS+"dblw1","south");

    set("short","Main Street");
    set("long","Main Street runs east-west through Daybreak Ridge.  Commerce court runs in a northerly direction from here, Luminous Way runs in a southerly direction from here.  ");
}
