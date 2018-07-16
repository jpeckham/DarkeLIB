#include <std.h>
#include "../goblin.h"

inherit ROOM;

void create() {
	::create();
	set_property("indoors", 2);
	set_property("light", 3);
	set("short", "A hut built in a cave");
	set("long", "A large hut in the east.  This hut is made of wood and rock , small rats scurry around along the floor where bits of mushrooms have been sitting.  There are no windows in the room, which is only illuminated by a torch.  Goblins are practicing here with their weapons and spears.");
set_exits( ({(ROOMS+"goblin3"), (ROOMS+"settlement")}), ({"north", "west"}));
}

void reset() {
	::reset();
	if(!present("goblin"))
{
new(MON+"goblin_spear.c")->move(this_object());
new(MON+"goblin_spear.c")->move(this_object());
new(MON+"goblin_spear.c")->move(this_object());
new(MON+"goblin.c")->move(this_object());
new(MON+"goblin.c")->move(this_object());
new(MON+"goblin.c")->move(this_object());
}
}

