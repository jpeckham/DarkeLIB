#include "../daybreak.h"
inherit "std/room";


void init(){
::init();
//  add_action("no_east", "east");
}
void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbcc4", "north");
    add_exit(ROOMS+"dbcc2","south");
    add_exit("/d/damned/guilds/join_rooms/mercenary_join","east");
    set("short","Commerce Court");
    set("long","Commerce Court runs north-south through Daybreak Ridge.");
}

int no_east(string str) {
        if(wizardp(TP)) { return 0; }
        if(TP->query_class() == "mercenary") { return 0; }
        if(TP->is_pet()) { return 0; }
//        if(TP->query_class() == "child") { return 0; }
        message("info", "The Mercenary Guild will be opening soon, stay tuned to the news for further info.", this_player());
        return 1;
}
