#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit("/d/damned/virtual/room_8_9.world", "mountains");
    add_exit(ROOMS+"dbns9","south");
    add_exit("/d/nocte/kuril/rooms/entrance","east");
 
    set("short","Daybreak Valley Road");
    set("long","Daybreak Valley Road runs north-south through Daybreak Ridge");
}
