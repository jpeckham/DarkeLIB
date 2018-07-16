
#include "../ruins.h"
#include <std.h>
inherit "std/room";
 void create() {
::create() ;
set_property("light", 1);
set_property("indoors",0); 
add_exit(ROOMS+"basement2.c", "west");
add_exit(ROOMS+"basement7.c", "north");
add_exit(ROOMS+"basement4.c", "south");
add_exit(ROOMS+"ruins10.c", "stairs");
set("short", "The Basement Of An Ancient House");
set("day long", "The Basement Of An Ancient House. This walls around you
are filled with cracks and chipped paint. The cieling is falling apart and
has alot of holes in it. There seems to be just enough light coming throw
the holes in the ceiling to light up the room. The passage way opens up
into a mid sized room which you are on the mid east side of. There are a
set of stairs leading upwords.");
set("night long", "The Basement Of An Ancient House. This walls around you
are filled with cracks and chipped paint. The cieling is falling apart and
has alot of holes in it. The passage way opens up into a mid sized room
which you are on the mid east side of. There are a set of stairs leading
upwords.");
set_smell("default", "The smell of rat guano fills your nostrils");
set_listen("default", "The sound of rats fills the room");
}
void reset() {
       ::reset();
if(!present("rat"))
{
new(MON+"smallratchild_cheeze.c")->move(this_object());
new(MON+"smallratchild.c")->move(this_object());
new(MON+"smallratchild.c")->move(this_object());
}
}

