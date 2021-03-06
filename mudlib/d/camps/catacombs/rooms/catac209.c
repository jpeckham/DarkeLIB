/* Room coded by Duridian for DarkeMud. */
 
#include <std.h>
inherit ROOM;
 
void create() {
   ::create();
   set_property("light", 1);
   set_property("indoors", 1);
   set("short", "Dark Dungeon");
   set("long",
   "The ceiling of the passageway drops steeply here as the gradient "
   "decreases, leading you still further downward.  The corridor takes "
   "a sharp turn to the southwest, and in that direction you spy a "
   "tall skeleton wielding a large sword and wearing a white shield.",
   );
   set_smell("default", "The air is very dry.");
   set_listen("default", "You hear evil laughter to the west.");
   set_items( ([
                ({"walls", "wall", "floor", "dust"}):
        "The walls and floor are covered in a thin layer of dust.",
                ({"torches", "small torches"}):
        "The small torches that are affixed to the wall provide very little light.",
                ]) );
   set_exits( ([
                "southwest": PATH+"catac212",
                "south": PATH+"catac208",
                ]) );
}
