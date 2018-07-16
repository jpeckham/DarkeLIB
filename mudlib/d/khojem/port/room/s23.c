//      Wizard:  Khojem
//      s23.c

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
    ::create();
    set_property("light", 3);
    set_property("night light",2);
    set_property("indoors", 0);
    set("short", "Dock Street");
    set("long","Dock and Noble Streets intersect here.  "+
      "Dock Street extends east and west.  Noble Street runs off to the north.  "+
      "The harbor is just to the south.  "+
      "The wooden boards beneath your feet creak with every footstep."
    );
    add_exit("/wizards/khojem/port/room/s22","north");
    add_exit("/wizards/khojem/port/room/s19","west");
    add_exit("/wizards/khojem/port/room/s24","east");
    set_items(([
     ({ "boards","wooden boards" }) :
        "The wooden boards of the dock are supported above the water my timbers.  "+
        "The boards are bleached from exposure to sun and salt air.",
     ({ "street","dirt street","Dock Street","dock street","Noble Street",
      "noble street","dock","noble" }) :
        "Noble Street is a dusty little road that leads off to the north.  "+
        "Dock Street is nothing more than a wooden deck supported above the water.",
    ]));
    set_smell("default", "The smell of the sea fills the air.");
    set_listen("default", "Waves in the harbor lap against the dock's pilings.");
}

