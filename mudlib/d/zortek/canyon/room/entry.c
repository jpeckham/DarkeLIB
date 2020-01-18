// Entry to Muerta Canyon
// Created by Zortek
// 031596

#include <std.h>
#include "/d/zortek/canyon/canyon.h"

inherit ROOM;

int no_exit() { return 1;}

void init() {
    ::init();
    if(present("criegan",this_object()))
    if(this_player()->query_property("zor_mon") ) {
      add_action("no_exit","up");
    }
}

void reset() {
    object ob;
    ::reset();
    if(!present("criegan", this_object())) {
       ob=new(MON+"quest1");
       ob->move(this_object());
    }
}

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("night light",1);
    set("short", "Entrance to Muerta Canyon");
    set("day long", @DAY
The path decends sharply at what appears to be the entrance to a
deep canyon.  The light dims as the sun is eclipsed by the rising
steeply rising stone walls.  It appears as if it would be very
easy for this path to be blocked somehow.
DAY
   );
    set("night long", @NIGHT
The path decends sharply and suddenly at what appears to be the
entrance of a deep canyon.  What little lunar light you had is
quickly eclipsed by the rising stone walls.  It appears as if it
would be very easy for this path to become blocked somehow.
NIGHT
    );
    add_exit(ROOMS+"path1", "down");
    add_exit("/d/damned/virtual/room_39_24.world", "up");
    set_items(([
    "path": "This path is an unremarkable, dirt path.",
    ({ "canyon", "walls", "wall" }): @CANYON
The canyon appears to be a river-cut land deformation.  The walls
seem to be comprised mainly of sandstone with occational layers
of limestone visible.
CANYON
,
    ]));
    set_smell("default", "There is a hint of decay in the air.");
    set_listen("default", "You faintly hear normal insect sounds.");
}
