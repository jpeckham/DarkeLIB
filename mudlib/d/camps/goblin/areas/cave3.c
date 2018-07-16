#include <std.h>
#include "../goblin.h"

inherit ROOM;

void create() {
	::create();
	set_property("indoors", 2);
	set_property("light", 3);
	set("short", "The end of the east cave.");
	set("long", "This is the end of the east cave, there are even more mushrooms here, bright ones, dark ones, shiny ones, dull ones, there are so many that it dazzles you with their multitude of colors.  Snotlings are here to pick and collect mushrooms, there are also snotlings taking a few samples.");
set_exits( ({(ROOMS+"cave1")}), ({"west"}));
	add_exit("room1", "north");
	add_exit("idolroom", "east");
	add_invis_exit("north");
	add_invis_exit("east");

}
void reset() {
	::reset();
	if(!present("snotling"))
{
new(MON+"snotling.c")->move(this_object());
new(MON+"snotling.c")->move(this_object());
new(MON+"snotling_knife.c")->move(this_object());
new(MON+"snotling_puke.c")->move(this_object());
new(MON+"snotling_puke.c")->move(this_object());
}
}

