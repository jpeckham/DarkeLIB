#include <std.h>
#include "../goblin.h"

inherit ROOM;

void create() {
	::create();
	set_property("indoors", 2);
	set_property("light", 2);
	set("short", "A room a cave, with 3 exits.");
set_exits( ({(ROOMS+"settlement2"), (ROOMS+"shamanroom"), (ROOMS+"hallway"), (ROOMS+"grom")}), ({"south", "northwest", "north", "northeast"}));
}
