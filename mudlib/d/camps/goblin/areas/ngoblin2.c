#include <std.h>
#include "../goblin.h"

inherit ROOM;

void create() {
	::create();
	set_property("indoors", 2);
	set_property("light", 3);
	set("short", "A little room for practice.");
	set_long("This is a room where goblins practice squig hunting.  The room itself is covered with wood chips from all the bashing that the goblins have done.");
set_exits( ({(ROOMS+"settlement2")}), ({"east"}));
}
void reset() {
	::reset();
	if(!present("goblin"))
{
new(MON+"goblin_club.c")->move(this_object());
new(MON+"goblin_club.c")->move(this_object());
new(MON+"goblin_club.c")->move(this_object());
new(MON+"goblin_club.c")->move(this_object());
new(MON+"goblin_club.c")->move(this_object());

}
}
