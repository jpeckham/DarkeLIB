#include <std.h>
#include <nevin.h>
inherit "std/vault";

nosave int open;

void init() {
	::init();
	add_action("read_sign", "read");
}

void create(){
	::create();
	set_property("indoors", 1);
	set_property("light", 2);
	set_property("night light", -3);
	set_property("no castle", 1);
	set("short", "Rebecca's Bakery");
	set("long",
		"Fine white powder floats around in the air and covers "
		"everything in the room...AAACHHOO!!!!!  Bless you!  "
		"Rolling pins, pastry cutters, and bags of flour and sugar "
		"lay about.  You notice that the storekeeper is laughing "
		"to herself...or perhaps at you.  Oh boy, don't you just love "
		"it when you start drooling all over yourself?  After "
		"wiping all the drool from your face you notice that a sign "
		"has been placed on the back wall."
	);
	set_smell("default", "The wonderful scent of cookies, breads, and "
		"pastries fill the air.");
	set_items( ([
		"sign": "Try 'read'ing it.",
		({"pantry door", "pantry"}): (: call_other, this_object(), "look_pantry" :),
		"door": (: call_other, this_object(), "look_door" :),
		"powder": "You just can't get enough of that stuff can you?",
		({"rolling pins", "pins", "rolling"}): "Made from beautiful " +
			"marble, they could be make for a formidable "
			"weapon if need be.",
		({"cutters", "pastry cutters"}): "The pastry cutters are in "
			"the shapes of animals and toys.",
		({"bags", "flour", "sugar"}):  "My, my, my...you better get "
			"some help for that problem of yours."
	]) );
	set_exits( ([
		"north": ROOMS+"bakery_storage",
		"south": ROOMS+"noblemans_lane1"
	]) );
	set_door("door", ROOMS+"noblemans_lane1", "south", 
		"kuril bakery key");
	set_open("door", 1);
	set_locked("door", 0);
	set_door("pantry door", ROOMS+"bakery_storage", 
		"north", "kuril bakery storage key");
	set_open("pantry door", 0);
	set_locked("pantry door", 1);
	set_lock_level("pantry door", 25);
	set_open_function("open_door", "05:00:00");
	set_close_function("close_door", "15:00:00");
}

void reset() {
	::reset();
	seteuid(getuid());
	if(!present("rebecca") && open) {
		new(MON+"rebecca")->
			move(this_object());
	}
}

int read_sign(string str) {
	if(!str) {
		notify_fail("What do you wish to read?\n");
		return 0;
	}
	else if(str == "sign") {
		write("The sign reads: say 'Rebecca, help' for assistance.");
		return 1;
	}
}

void open_door() {
set_locked("door", 0);
	set_open("door", 1);
	call_other(ROOMS+"noblemans_lane1", "set_locked", 
		"door", 0);
	call_other(ROOMS+"noblemans_lane1", "set_open",
		"door", 1);
	open = 1;
	this_object()->reset();
}

void close_door() {
	object ob, *inv;
	int i;

	ob = present("rebecca", this_object());
	if(ob) ob->remove();
	inv = all_inventory(this_object());
	i = sizeof(inv);
	while(i--) {
		if(living(inv[i])) {
			message("info", "Rebecca asks you to leave" +
				" her shop as it is time to close.", inv[i]);
			inv[i]->move(find_object_or_load(ROOMS+
				"kuril/noblemans_lane1"));
		}
	}
	set_open("door", 0);
	set_locked("door", 1);
	call_other(ROOMS+"noblemans_lane1", "set_locked", 
		"door", 1);
	call_other(ROOMS+"noblemans_lane1", "set_open", 
		"door", 0);
	open = 0;
}

void look_door() {
	if(query_open("door")) {
		write("The door to Rebecca's Bakery is wide open.");
	}
	else {
		write("The door to Rebecca's Bakery is closed.");
	}
}

void look_pantry() {
	if(query_open("pantry door")) {
		write("The door to the pantry is open; wondrous scents "
			"emanate from within.");
	}
	else {
		write("The door to the pantry is closed.");
	}
}
