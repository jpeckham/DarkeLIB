//      Wizard:  Khojem
//      Orc Chief's Chambers
//      chief.c

#include <std.h>

inherit ROOM;

void reset() {
	::reset();
    if (!present("half-orc bodyguard" , this_object()) )
      new("/wizards/khojem/new/mon/orc_bodyguard")->move(this_object());
    if (!present("half-orc chief" , this_object()) )
      new("/wizards/khojem/new/mon/orc_chief")->move(this_object());
}

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set("short", "Half-Orc Chief's Chamber");
    set("long", "You are in the Half-Orc Chief's Chamber.  "+
      "From here the chief makes his plans against the Wood-Elves.  "+
      "A small fire burns here for light and warmth in the dank cave."
    );
    add_exit("orc9","west");
    set_items(([
     ({ "cave","cavern", "chamber" "chief's chamber" }) :
        "The walls are bare dirt.",
     ({ "walls", "floor" }) :
        "The walls and floor are very hard, dry clay.  Digging out "+
        "would be an impossible task.",
     ({ "fire", "small fire" }) :
        "It is a very, small wood fire and close to burning out."
    ]));
    set_smell("default", "A dank, musty smell fills the air.  You can "+
      "smell wood burning and food cooking.");
    set_listen("default", "The cave is very quiet.");
}
