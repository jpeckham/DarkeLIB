#include "../newbieville.h"
#include <std.h>

inherit ROOM;
void create() {
    ::create();
    
    set_property("light", 3);
    set_property("indoors", 0);
    set("short", "Common Street");
    set("long", "You are on Common Street, which runs south from town square. To the west is a general store, and to the east is a weapons shop. There are some gates to the south.");
    add_exit(ROOMS+"generalstore", "west");
    add_exit(ROOMS+"weaponshop", "east");
    add_exit(ROOMS+"townsquare", "north");
    add_exit(ROOMS+"sgates", "south");
}
