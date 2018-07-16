#include <std.h>
#include "../goblin.h"

inherit ROOM;

void create() {
	::create();
	set_property("indoors", 2);
	set_property("light", 3);
	set("short", "A room in a cave, with three exits.");
	set("long", "You are standing in front of what seems to be a large marble parthenon.  The parthenon has three passageways, to the north, northwest and northeast.  Intricate carvings of goblins fighting many other races, and running away from elves are depicted here.  In the north there seems to be an excessive amount of shouting and screaming.");
set_exits( ({(ROOMS+"settlement2"), (ROOMS+"shamanroom"), (ROOMS+"hallway"), (ROOMS+"grom")}), ({"south", "northwest", "north", "northeast"}));
}

void reset() {
	::reset();
	if(!present("goblin"))
{
new(MON+"goblin_silly.c")->move(this_object());
new(MON+"goblin_silly.c")->move(this_object());
}
}
