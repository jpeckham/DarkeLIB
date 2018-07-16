#include <std.h>
#include "../goblin.h"

inherit ROOM;

void create() {
	::create();
	set_property("indoors", 2);
	set_property("light", 3);
	set("short", "A statue filled hallway.");
	set("long", "This is the hallway to the north, there are many statues of goblin champions standing here, each of them is carved in wood for everlasting memory. The floor is made of a dark obsidian and polished to a shine.");
	set_items(([
	"statues" : "These are the statues of long dead goblin heros, some of the statues here are new while others are old and worn out."
]));

set_exits( ({(ROOMS+"hill"), (ROOMS+"idolroom")}), ({"south", "north"}));

}


void reset() {
	::reset();
	if(!present("snotling"))
new(MON+"snotlingc.c")->move(this_object());
}
