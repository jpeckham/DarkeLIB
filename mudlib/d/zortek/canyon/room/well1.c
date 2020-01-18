// Entry to Muerta Canyon
// Created by Zortek
// 031596

#include <std.h>
#include "/d/zortek/canyon/canyon.h"

inherit ROOM;

void init() {
    ::init();
}

void reset() {
  ::reset();
}

void create() {
    ::create();
    set_property("light", -3);
    set_property("indoors", 0);
    set_property("night light",-3);
    set("short", "Top of the Well of Souls");
    set("long",@TEXT
You are at the top of a well.  Above you is a pitch black barrier
that appears to absorb all light.  There seems to be a light wind
blowing downward, but fortunately the rungs seem steady and secure.
TEXT
);
    add_exit(ROOMS+"well2", "down");
    set_items(([
    ({ "barrier", "black barrier", "pitch black barrier" }): @BARRIER
The barrier feels and smells like cold, dead flesh.  It absorbs all
light and reflects none.  Despite its texture, the barrier appears
to be solid and inpenitrable.
BARRIER
,
    ({ "rung", "rungs", "iron rung", "iron rungs" }): @RUNG
These loops of metal have been set into the wall of the well at
comfortable intervals that makes descent relatively easy in the
highly close quarters.  Despite the moist air, the rungs show no
evidence of corrosion.
RUNG
,
    ]));
    set_smell("default", "Fresh air seems to come from above.");
    set_listen("default", "Only faint echos of your movements can be heard.");
   reset();
}
