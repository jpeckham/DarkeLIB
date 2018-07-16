#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"db_square", "east");
    add_exit(ROOMS+"dbew7","west");
 
    set("short","Main Street");
    set("long","Main Street runs east-west through Daybreak Ridge");
}
