// Wishing Well
// Created by Zortek
// 100596

#include <std.h>
#include "/d/zortek/canyon/canyon.h"
#define ZTEST "/wizards/zortek/virtual/"
inherit ROOM;

int no_exit() { return 1;}
int no_consume(string str) {
  write("You are too in awe of the well to consume anything.");
  return 1;
}
int climb_down(string str) {
  switch(str) {
    // climb <what>
    case "down":
    case "into well":
    case "into the well":
    case "into hole":
    case "into the hole":
      write(@TEXT
You climb over the short wall of stones and find safe footing on
some iron rungs set into the wall of the well.
TEXT
);
      this_player()->move("/wizards/zortek/canyon/room/well1.c");
      return 1;
    // enter <what>
    case "well":
    case "hole":
      write(@TEXT
After a moment of struggling, you enter the well and find safe
footing on some iron rungs into the wall.
TEXT
);
      this_player()->move("/wizards/zortek/canyon/room/well1.c");
      return 1;
    default:
      notify_fail("What?\n");
      return 0;
  }
}
void init() {
  int x;
    ::init();
  add_action("no_consume", "drink");
  add_action("no_consume", "eat");
  add_action("climb_down", "climb");
  add_action("climb_down", "enter");
  if(this_player()->is_player()) {
    write("%^BLUE%^An overwhelming sense of awe strikes you!%^RESET%^");
    x = this_player()->query_stoned();
    if(x < 700) {
      this_player()->add_stoned(250);
      this_player()->add_quenched(-1*(int)this_player()->query_quenched());
      this_player()->add_stuffed(-1*(int)this_player()->query_stuffed());
      this_player()->add_intox(-1*(int)this_player()->query_intox());
    }
  }
}

void reset() {
   ::reset();
}

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("night light",3);
    set_property("no scry", 1);
    set("short","Well of Souls");
    set("long", @LONG
You have entered a room that contains a ring of stones that
encircle a perfectly circular hole in the floor.
LONG
);
   add_exit(ZTEST+"5,7.ztest", "north");
   set_smell("default", "The air smells of old mildew.");
   set_listen("default", "The room is strangely quiet.");
   reset();
}
