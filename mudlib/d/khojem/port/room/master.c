//      Wizard:  Khojem
//      Harbor Master's Office
//      master.c

#include <std.h>
inherit ROOM;

void reset(){
  ::reset();
  if(!present("master"))
    new("/wizards/khojem/port/mon/dagget")->move(this_object());
}  

void create() {
    ::create();
    set_property("light", 2);
    set_property("night light",1);
    set_property("indoors", 1);
    set("short", "Harbor Masters");
    set("long","This is the small shaby office of the harbor master.  "+
      "Here the Harbor Master coordinates the berthing of merchant "+
      "vessels at the wharfs of Vo'Sangor.  Given the worn rundown "+
      "look of the office you figure it must not pay well."
    );
    add_exit("/wizards/khojem/port/room/s19","south");
    set_items(([
     ({ "office", "shack" }) :
        "The office is littered with papers and leftover food articles.  "+
        "It is basically just a wooden shack.",
     ({ "papers" }) :
        "The papers appear to be some official looking documents with big "+
        "round ink marks from round rubber stamps."
    ]));
    set_smell("default", "The smell of the sea fills the air.");
    set_listen("default", "You hear the sounds of a busy seaport to the south.");
}
