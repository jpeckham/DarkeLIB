#include "../daybreak.h"
inherit "std/room";

void create() {
   ::create();
     set_property("light",3);
     set_property("night light", 2);
     add_exit(ROOMS+"dbns2", "north");
     add_exit("/d/damned/virtual/room_8_9.world","mountains");
     set("short","Outside the southern gate of Daybreak Ridge");
     set("long","You are standing just outside the southern gate of Daybreak Ridge.\n"+
         "The town looks very inviting to the weary traveler.  You hear " +
         "faint laughter coming from the cozy taverns inside the town." +
         "As you look away from the town, you see the mountains stretch " +
         "endlessly before you.");
}
