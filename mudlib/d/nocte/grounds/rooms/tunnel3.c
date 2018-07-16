#include <std.h>
#include <nevin.h>

inherit ROOM;

void create() {
	::create();
	set_property("indoors", 1);
	set_property("light", -3);
	set_property("no castle", 1);
	set("short", "Inside the Tunnel");
	set("long",
		"It sure is cold in here!  Rubbing your hands together doesn't "
		"seem to do anything though....  The cave continues to the "
		"south."
	);
	set_listen("default", "You hear your teeth begin to chatter.");
	set_exits( ([
		"west": ROOMS+"tunnel2.c",
		"south": ROOMS+"tunnel4.c"
	]) );
}

void reset() {
	::reset();
	if(!present("_skeleton1"))
		new(MON+"skeleton1.c")->
		move(this_object());
	if(!present("_skeleton2"))
		new(MON+"skeleton2.c")->
		move(this_object());
	if(!present("_skeleton3"))
		new(MON+"skeleton3.c")->
		move(this_object());
}
