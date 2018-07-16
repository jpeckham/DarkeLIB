#include <std.h>
#include "../goblin.h"

inherit ROOM;

void create() {
	::create();
	set_property("indoors", 2);
	set_property("light", 3);
	set("short", "The end of the west cave.");
	set("long", "This is the end of the west cave, there are even more mushrooms than before, some that you believe are even more interesting.  There are many snotlings here, to pick and collect mushrooms, and even more snotlings to sample them.");
set_exits( ({(ROOMS+"cave2")}), ({"east"}));
	add_exit("room2", "west");
	add_invis_exit("west");
}

void reset() {
	::reset();
	if(!present("snotling"))
{
new(MON+"snotling_knife.c")->move(this_object());
new(MON+"snotling_puke.c")->move(this_object());
new(MON+"snotling_puke.c")->move(this_object());
new(MON+"snotling_puke.c")->move(this_object());
new(MON+"snotling.c")->move(this_object());
}
}

