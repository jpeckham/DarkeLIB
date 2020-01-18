// Bridge room between real rooms and the virtual
// rooms of Muerta Canyon
// Created by Zortek
// 031596

#include <std.h>
#include "/d/zortek/canyon/canyon.h"

inherit ROOM;

int no_exit() { return 1;}

void init() {
    ::init();
    if(this_player()->query_property("zor_mon")) {
      add_action("no_exit","up");
    }
}

void reset() {
   object ob;
    ::reset();
   if(!present("lemure", this_object()) ) {
      ob = new(MON+"lemure.c");
      ob->set("short", "Exiled Lemure");
      ob->set_level(20);
      ob->set_max_hp(2750);
      ob->set_hp(2750);
      ob->move(this_object());
   }
}

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 0);
    set_property("night light",0);
    set("short", "Base of Muerta Canyon");
    set("day long", @DAY
You are at the base of the opening to Muerta Canyon. The rocks
looming above you seem to open up slightly allowing the faintest
amount of light to filter in.  The ground is barren and devoid
of any plant life and the air is stagnant.  A foot trail appears
to lead south.
DAY
   );
    set("night long", @NIGHT
You are at the base of the opening to Muerta Canyon. The rock
walls loom above to both side.  The ground is barren and devoid
of any plant life and the air is stagnant.  A foot trail appears
to lead south.
NIGHT
    );
   add_exit(ROOMS+"path4", "up");
   add_exit("/wizards/zortek/virtual/ztest/rooms/-1,3.ztest", "south");

   set_smell("default", "There is a certain scent of decay in the air.");
   set_listen("default", "It is strangely quiet.");
   reset();
}
