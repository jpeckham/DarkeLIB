#include "../ruins.h"
inherit "/std/vault";


void reset() {
   ::reset();
   seteuid(getuid());
   if (!present("smithy")) {
new(MON+"smithy.c")->move(this_object());
   }
}

void create() {
   ::create();
   set_property("indoors",1);
   set_property("light",3);
add_exit(ROOMS+"ruins9.c", "west");
add_exit(ROOMS+"shop3_store.c", "south");
set_door("iron door", (ROOMS+"shop3_store.c"), "south",
"shop3 store");
   set_open("iron door", 0);
   set_locked("iron door", 1);
   set_lock_level("iron door", 22);
   set("short","The Smithy's Armour Store");
set("long", "This is Smithy's Weapon Store. Here, you can purchase the
various types of weaponry needed for protection in the ancient city. There
are many types of weapons that seen to be all about you. An imposing iron
door is seen to the west.");
   set_items( (["sign":"The say reads: 'say Smithy, help' for assistance.",
              "iron door" : "It looks very sturdy.",
              "door" : "It is a large iron door."]) );
}
