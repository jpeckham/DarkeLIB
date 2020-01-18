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
    set_property("light", -6);
    set_property("indoors", 0);
    set_property("night light",-6);
    set("short", "Bottom of the Well of Souls");
    set("long",@TEXT
You are at the bottom of a well and standing knee deep in water.
There are some iron rungs set into the wall that lead up.  The
wind is strong here and seems to blow through an open archway
to the south.
TEXT
);
    add_exit(ROOMS+"well3", "up");
    add_exit("/wizards/zortek/virtual/ztest/rooms/8,5.ztest", "south");
    set_items(([
    ({ "rung", "rungs", "iron rung", "iron rungs" }): @RUNG
These loops of metal have been set into the wall of the well at
comfortable intervals that makes descent relatively easy in the
highly close quarters.  Despite the moist air, the rungs show no
evidence of corrosion.
RUNG
,
    ({"archway","arch"}):@ARCH
The archway to the south is only about four feet high at its peak
and is covered with hundreds of runes and symbols.
ARCH
,
    ]));
    set_smell("default", "Fresh air seems to rush from above.");
    set_listen("default", "Wind is surging in your ears.");
   reset();
}
