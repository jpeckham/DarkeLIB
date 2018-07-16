//      Wizard:  Khojem
//      s13.c

#include <std.h>
inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("night light",2);
    set_property("indoors", 0);
    set("short", "Wharf Street");
    set("long","Wharf Street extends north and south.  "+
      "The harbor is just to the east.  "+
      "The wooden boards beneath your feet creak with every footstep.  "+
      "A large, rundown warehouse is to the west."
    );
    add_exit("/wizards/khojem/port/room/warehouse","west");
    add_exit("/wizards/khojem/port/room/s12","north");
    add_exit("/wizards/khojem/port/room/s14","south");
    set_items(([
     ({ "warehouse" }) :
        "It looks like an old, weather-ravaged wooden warehouse.",
     ({ "boards","wooden boards" }) :
        "The wooden boards of the dock are supported above the water my timbers.  "+
        "The boards are bleached from exposure to sun and salt air.",
     ({ "street","dirt street","Wharf Street","wharf street","dock" }) :
        "Wharf Street is nothing more than a wooden deck supported above the water."
    ]));
    set_smell("default", "The smell of the sea fills the air.");
    set_listen("default", "Waves in the harbor lap against the dock's pilings.");
}

