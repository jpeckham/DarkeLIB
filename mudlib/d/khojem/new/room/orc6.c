//      Wizard:  Khojem
//      orc6
//      orc6.c

#include <std.h>

inherit ROOM;

void reset() {
	::reset();
    if (!present("half-orc shaman" , this_object()) ) {
      new("/wizards/khojem/new/mon/orc_shaman")->move(this_object());
    }
}


void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set("short", "Shaman's Chamber");
    set("long", "This room is the home of the Shaman.  The priest's home "+
      "is sparely furnished.  The dirt floor is tampered hard due long "+
      "use and the walls are bare."
    );
    add_exit("orc3","north");
    set_items(([
     ({ "cave","cavern", "chamber", "room" }) :
        "The chamber is small and cramped, but since there is no "+
        "furniture it is large enough for someone to live here.",
     ({ "walls", "floor" }) :
        "The walls and floor are very hard, dry clay.  Digging out "+
        "would be an impossible task."
    ]));
    set_smell("default", "A dank, musty smell fills the air.  You can "+
      "smell wood burning and food cooking.");
    set_listen("default", "You can hear the pit fire burning with "+
      "intensity.");
}
