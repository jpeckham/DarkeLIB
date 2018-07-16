#include <std.h>
#include "../goblin.h"

inherit ROOM;

void create() {
	::create();
	set_property("indoors", 2);
	set_property("light", 3);
	set("short", "A room in the hut");
	set("long", "This is a room in the large hut, it is the largest room in the hut, with a long roughly built table in the center.  At the end of the table is a large goblin, maybe one of the champions of the settlement.  There are some goblins running around trying to make themselves look good to the champion.");
set_exits( ({(ROOMS+"goblin2")}), ({"south"}));
	add_exit("goblin3", "west");
	add_exit("room1", "east");
	add_invis_exit("west");
	add_invis_exit("east");
	add_invis_exit("southeast");
}

void reset() {
	::reset();
    if(!present("goblin")){
new(MON+"goblinc1.c")->move(this_object());
new(MON+"goblin.c")->move(this_object());
new(MON+"goblin.c")->move(this_object());
new(MON+"goblin_fanatic.c")->move(this_object());
new(MON+"goblin_fanatic.c")->move(this_object());
	new("/wizards/blial/monsters/goblin.c")->move(this_object());
    }
}

