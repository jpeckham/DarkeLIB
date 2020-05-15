#include <std.h>
#include <nevin.h>

inherit "std/vault";

nosave int open;

void init() {
	::init();
	add_action("read sign", "read");
}

void create(){
	::create();
	set_property("indoors", 1);
	set_property("light", 2);
	set_property("night light", -3);
	set_property("no castle", 1);
	set("short", "Kerchesh's Fish Shop");
	set("long",
		"Lying upon large blocks of ice, fish of various types are "
		"sitting on display for purchase.  BIG ones, little ones, "
		"blue ones, red ones.  On the floor a pool of chilly water "
		"has begun to collect from the melting ice.  A sign has been "
		"placed on the wall behind the ice blocks.  Greeting each "
		"visitor to his shop is Kerchesh, the local fish seller."
	);
	set("night long",
		"All of the store's fish and other seafood items have been "
		"stored for the evening in the shop's storage room."
	);
	set_items( ([
		"sign": "The reads: 'say Kerchesh, help' for assistance.",
		"metal door": (: call_other, this_object(), "look_metal_door" :),
		"door": (: call_other, this_object(), "look_door" :),
		"ice": "Wow, ice must be really expensive in these parts or " +
			"Kerchesh knows something you don't.",
		"fish": "You suddenly feel the urge to swallow an entire " +
			"fish whole!  Oh boy, that smell must be getting to " +
			"you."
	]) );
	set_exits( ([
		"west": ROOMS+"fish_market_storage",
		"north": ROOMS+"merchant_road5"
	]) );
	set_door("door", ROOMS+"merchant_road5", "north", 
		"kuril fish shop key");
	set_open("door", 1);
	set_locked("door", 0);
	set_door("metal door", ROOMS+"fish_market_storage", 
		"west", "kuril fish shop storage key");
	set_open("metal door", 0);
	set_locked("metal door", 1);
	set_lock_level("metal door", 25);
	set_open_function("open_door", "06:00:00");
	set_close_function("close_door", "17:00:00");
}

void reset() {
	::reset();
	seteuid(getuid());
	if(!present("kerchesh") && open) {
		new(MON+"kerchesh")->move(this_object());
	}
}

int read_sign(string str) {
	if(!str) {
		notify_fail("What do you wish to read?\n");
		return 0;
	}
	else if(str == "sign") {
		write("The sign reads: say 'Kerchesh, help' for assistance.");
		return 1;
	}
}

void open_door() {
	set_locked("door", 0);
	set_open("door", 1);
	call_other(ROOMS+"merchant_road5", "set_open",
		"door", 1);
	call_other(ROOMS+"merchant_road5", "set_locked",
		"door", 0);
	open = 1;
	this_object()->reset();
}

void close_door() {
	object ob, *inv;
	int i;

	ob = present("kerchesh", this_object());
	if(ob) ob->remove();
	inv = all_inventory(this_object());
	i = sizeof(inv);
	while(i--) {
		if(living(inv[i])) {
			message("info", "Kerchesh waves a slimy old fish " +
				"in your face--the scent is enough to " +
				"push you out the door as he closes up " +
				"shop for the night", inv[i]);
			inv[i]->move(find_object_or_load(ROOMS+
				"merchant_road5"));
		}
	}
	set_open("door", 0);
	set_locked("door", 1);
	call_other(ROOMS+"merchant_road5", "set_locked", 
		"door", 1);
	call_other(ROOMS+"merchant_road5", "set_open", 
		"door", 0);
	open = 0;
}

void look_door() {
	if(query_open("door")) {
		write("The door to Kerchesh's Fish Shop is wide open.");
	}
	else {
		write("The door to Kerchesh's Fish Shop is closed.");
	}
}

void look_metal_door() {
	if(query_open("metal door")) {
		write("The metal door is open.");
	}
	else {
		write("The metal door is closed.");
	}
}
