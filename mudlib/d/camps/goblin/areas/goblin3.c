#include <std.h>
#include "../goblin.h"

inherit ROOM;

void create() {
	::create();
	set_property("indoors", 2);
	set_property("light", 3);
	set("short", "A room in the hut");
	set("long", "An airy room in a large hut.  This is where on of the goblin champions makes his territory.  The room itself is decorated with many skulls of different creatures, from humans to halflings and lizards to rabbits.  Plants also sit in the corners of the room.");
set_exits( ({(ROOMS+"goblin1")}), ({"south"}));
	add_exit("goblin4", "east");
	add_exit("room2", "north");
	add_invis_exit("north");
	add_invis_exit("east");
}

void reset() {
	::reset();
	if(!present("goblin"))
{
new(MON+"goblin_spear.c")->move(this_object());
new(MON+"goblin_spear.c")->move(this_object());
new(MON+"goblin_spear.c")->move(this_object());
new(MON+"goblin_spear.c")->move(this_object());
new(MON+"goblin.c")->move(this_object());
new(MON+"goblinc1.c")->move(this_object());
}
}

