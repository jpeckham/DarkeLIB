#include <std.h>
#include "../goblin.h"

inherit ROOM;

void create() {
	::create();
	set_property("indoors", 2);
	set_property("light", 3);
	set("short", "The room with a statue of a god");
	set("long", "This is a large and spacious room which seems to be overtly clean.  There is nothing in this room except for a large statue.  The statue pretty much fills the whole room up with its massive size.");
set_exits( ({(ROOMS+"hallway")}), ({"south"}));
	add_exit("room1", "down");
	add_exit("cave3", "west");
	add_invis_exit("west");
	add_invis_exit("down");

}
void reset() {
	::reset();
if(!present("statue"))
new(MON+"idol.c")->move(this_object());
if(!present("goblin")) 
new(MON+"goblin_silly.c")->move(this_object());
}

