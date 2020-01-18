// Spawning Pool
// Created by Zortek
// 031596

#include <std.h>
#include "/d/zortek/canyon/canyon.h"
#define ZTEST "/wizards/zortek/virtual/"
inherit ROOM;

int no_exit() { return 1;}
int no_consume(string str) {
  write("Despite your attempts, nausea prevents you from consuming anything.");
  return 1;
}

void init() {
  int x;
    ::init();
  add_action("bathe_fun", "bathe");
  add_action("no_swim", "swim");
  add_action("bathe_fun", "soak");
  add_action("enter_pool", "enter");
  add_action("no_consume", "drink");
  add_action("no_consume", "eat");
  if(this_player()->is_player()) {
    write("%^ORANGE%^You feel ill!%^RESET%^");
    x = this_player()->query_stoned();
    if(x < 700) {
      this_player()->add_stoned(250);
      this_player()->add_quenched(-1*(int)this_player()->query_quenched());
      this_player()->add_stuffed(-1*(int)this_player()->query_stuffed());
      this_player()->add_intox(-1*(int)this_player()->query_intox());
    }
  }
}

int bathe_fun(string str) {
  int x;
  write("You bathe in the pool of goo, soaking in its benefits.");
  x = (int)this_player()->query_property("spawn");
  this_player()->add_poisoning(30*(6-(x/10)));
  if(x <= 50 )
    x += random(10);
    this_player()->set_property("spawn", x);
  return 1;
}

int no_swim(string str) {
  write(@SWIM
It does not appear that swimming is a viable option in this gaping
wound of the earth.  If you desired intimate contact with this
festering eruption of puss and infernal fluid, perhaps you might
want to soak or bathe in it.  Although, even the thought of such
causes the bile to rise in your throat and your head to spin.
SWIM
);
  this_player()->do_damage("torso", 10);
  return 1;
}

void reset() {
  object ob;
  int x = 1;
   ::reset();
#if 0
  if(!present("lemure", this_object()) ) {
    while(x--) {
      ob = new(MON+"lemure.c");
      ob->set("short", "Lost Lemure");
      ob->set_level(15);
      ob->move(this_object());
    }
  }
#endif
}

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("night light",3);
    set("short", "Festering Wound of the Earth");
    set("long", @LONG
You have entered what is perhaps the most vile and retched place you
have ever visited.  You entire body is revolted at the sights and smells
that accost you.  The physical revulsion is only a part of your desire
to flee--The very air bears the taint of infernal corruption and decay.
At the far end of this room is a circular opening in the floor.
LONG
);
   add_exit(ZTEST+"1,4.ztest", "south");
   set_smell("default", "The pungent odor of cancerous puss assults your nose.");
   set_listen("default", "A pool of bubbling goo churns steadily, occationally belching noxious gasses and erupting chunks of matter that splatter the walls and ceiling.");
   reset();
}
