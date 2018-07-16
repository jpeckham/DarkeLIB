#include <std.h>
#include "../goblin.h"

inherit ROOM;

void create() {
	::create();
	set_property("indoors", 2);
	set_property("light", 3);
	set("short", "The west cave.");
	set("long", "This is the beginning of the west cave.  This cave is here for the harvesting of mushrooms by snotlings.  There are many different kinds of mushrooms here for you to look at.");
	set_items((["mushrooms" :
	"There are very a lot of mushrooms, each one is different from the others.  You have no need for them as you might pick one up that is poisonous."
]));
set_exits( ({(ROOMS+"gates"), (ROOMS+"cave4")}), ({"east", "west"}));
}

void reset() {
	::reset();
	if(!present("snotling"))
{
new(MON+"snotling_knife.c")->move(this_object());
new(MON+"snotling_knife.c")->move(this_object());
new(MON+"snotling_knife.c")->move(this_object());
new(MON+"snotling_knife.c")->move(this_object());

new(MON+"snotling.c")->move(this_object());
}
}
