//      Wizard:  Khojem
//      annex 6
//      annex6.c

#include <std.h>

inherit ROOM;

void reset(){
  ::reset();
  if(!present("scout") && random(4)) 
    new("/wizards/khojem/new/mon/elf_scout")->move(this_object());
}  

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 0);
    set("short", "A wooded path");
    set("long", "You are on a path.  The trees grow "+
      "close around and cut off some of the light from above. "
    );
    add_exit("/wizards/khojem/new/room/annex6","south");
    add_exit("/wizards/khojem/new/room/annex8","north");
    set_items(([
     ({ "tree","trees", "woods" }) :
        "The trees are large, healthy trees filled with foliage.",
     ({ "leaf","leaves" }) :
        "The leaves sway in the breeze, but still allow much light by them.",
     ({ "trail","path" }) :
        "The trail is worn from frequent use."
     ]));
    set_smell("default", "The smell of growth and decaying wood is everywhere.");
    set_listen("default", "You hear the occasional sound of birds singing.");
}
