//      Wizard:  Khojem
//      constabulary
//      constable.c

#include <std.h>
inherit ROOM;

int check_mobs();
int my_mob_filter(object mob);

int check_mobs() {
  object *all;

  all = filter_array(livings(), "my_mob_filter", this_object());
  if(!all) return 0;
  return sizeof(all);
}

int my_mob_filter(object mob) {
  if(base_name(mob) == "/wizards/khojem/port/mon/constable")  
    return 1;                                        
  return 0;
}

void reset(){
  ::reset();
  if(!present("constable")) {
    if(check_mobs()<4)
      new("/wizards/khojem/port/mon/constable")->move(this_object());
  }
}

void create() {
  object ob;
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set("short", "The constabulary");
    set("long",
      "The quiet port town of Vo'Sangor can thank the members of the constabulary "+
      "for their contribution in keeping this a peaceful haven in a world of "+
      "havoc and chaos.  For here is the nerve center of an elaborate crime "+
      "prevention and justice system that is unrivaled anywhere in the lands "+
      "of DarkeMud.  Its cells are always empty because criminals far and wide "+
      "know of this constabulary's legend of tracking down and punishing those "+
      "who transgress the laws of Vo'Sangor. "
   );
    add_exit("/wizards/khojem/port/room/ngate","west");
    set_smell("default", "The smell of the sea fills the air.");
    set_listen("default", "The noise of a busy town can be heard outside.");
    
    ob = new("/std/bboard");
    ob->set_name("crime board");
    ob->set_id( ({ "board", "crime board" }) );
    ob->set_board_id("criminals");
    ob->set_max_posts(50);
    ob->set_location("/wizards/khojem/port/room/constable");
    ob->set("short", "Vo'Sangor Constabulatory Board");
    ob->set("long", "Post your criminals here!\n");
    set_property("no steal", 1);
}

