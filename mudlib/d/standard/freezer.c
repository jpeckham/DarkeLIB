#include <std.h>
#include <config.h>
#include <rooms.h>

inherit ROOM;

nosave private object *old;

void create() {
    ::create();
    set_property("no teleport", 1);
    set_property("no scry", 1);
    set_property("no attack", 1);
    set("short", "The freezer");
    set("long", "The local freezer.");
    set_exits( 
	      (["square" : "/d/standard/square"]) );
}

/*
protected void clean_room() {
    object *in_here, *to_clean;
    int i;

    if(!sizeof(in_here = livings() & all_inventory(this_object()))) { 
        old = in_here;
        return;
    }
    i = sizeof(to_clean = in_here & (pointerp(old) ? old : ({})));
    old = in_here - to_clean;
    while(i--) to_clean[i]->clean_net_dead();
    i = sizeof(to_clean = all_inventory(this_object()) - in_here);
    while(i--) to_clean[i]->remove();
    call_out("clean_room", MAX_NET_DEAD_TIME);
}
*/

int clean_up() { return 0; }
