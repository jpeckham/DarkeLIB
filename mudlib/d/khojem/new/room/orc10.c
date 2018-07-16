//      Wizard:  Khojem
//      orc10
//      orc10.c

#include <std.h>

inherit ROOM;

void reset() {
	::reset();
    if (!present("half-orc guard" , this_object()) ) {
        new("/wizards/khojem/new/mon/orc_guard1")->move(this_object());
        new("/wizards/khojem/new/mon/orc_guard1")->move(this_object());	  
    }
}


void create() {
    ::create();
    set_property("light", 0);
    set_property("indoors", 1);
    set("short", "A Half-Orc Barracks");
    set("long", "The room serves as a barracks for the Orcs." +
      "No beds can be found in the room, but small piles of " +
      "straw from the nearby clearing is used as bedding."
    );
    add_exit("orc9","north");
    set_items(([
     ({ "cave","cavern", "barracks" }) :
        "The barracks is small and dark.",
     ({ "straw", "bed", "bedding" }) :
        "The straw bedding is filthy and tick ridden.",
     ({ "walls", "floor" }) :
        "The walls and floor are very hard, dry clay.  Digging out "+
        "would be an impossible task."
    ]));
    set_smell("default", "A dank, musty smell fills the air.  You can "+
      "smell wood burning and food cooking.");
    set_listen("default", "The cave is very quiet.");
}
