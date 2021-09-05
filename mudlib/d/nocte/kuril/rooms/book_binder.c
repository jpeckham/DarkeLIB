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
	set("short", "Book Binder's Shop");
	set("long",
		"Half completed books lay atop a small work table in the "
		"corner of this small shop.  Right above the table is a "
		"small sign.  Catering to the mages of Kuril, Rety's "
		"books have found homes in the libraries of some of the "
		"city's most prominent mages.  On display in a case are "
		"tomes of history and lore."
	);
	set("night long",
		"Pieces of unfinished work have been left to sit for the "
		"evening.  The shop is still and quiet."
	);
	set_items( ([
		"sign": "Try 'read'ing it.",
		"wooden door": (: this_object(), "look_wooden_door" :),
		"door": (: call_other, this_object(), "look_door" :),
		"books": "Rety asks if he can help you as you peruse his "
			"work.",
		"table": "Old yet still sturdy.",
	]) );
	set_exits( ([
		"north": ROOMS+"book_binder_storage",
		"south": ROOMS+"fountain_square4"
	]) );
	set_door("door", ROOMS+"fountain_square4", "south", 
		"kuril book binder shop key");
	set_open("door", 1);
	set_locked("door", 0);
	set_door("wooden door", ROOMS+"book_binder_storage", 
		"north", "kuril book binder storage key");
	set_open("wooden door", 0);
	set_locked("wooden door", 1);
	set_lock_level("wooden door", 25);
	set_open_function("open_door", "07:00:00");
	set_close_function("close_door", "17:00:00");
}

void reset() {
	::reset();
	seteuid(getuid());
	if(!present("rety") && open) {
		new(MON+"rety.c")->move(this_object());
	}
}

int read_sign(string str) {
	if(!str) {
		notify_fail("What do you wish to read?\n");
		return 0;
	}
	else if(str == "sign") {
		write("The sign reads: say 'Rety, help' for assistance.");
		return 1;
	}
}

void open_door() {
	set_locked("door", 0);
	set_open("door", 1);
	call_other(ROOMS+"fountain_square4", "set_open",
		"door", 1);
	call_other(ROOMS+"fountain_square4", "set_locked",
		"door", 0);
	open = 1;
	this_object()->reset();
}

void close_door() {
	object ob, *inv;
	int i;

	ob = present("rety", this_object());
	if(ob) ob->remove();
	inv = all_inventory(this_object());
	i = sizeof(inv);
	while(i--) {
		if(living(inv[i])) {
			message("info", "Rety politely asks you to leave as " +
				"he closes his shop.", inv[i]);
			inv[i]->move(find_object_or_load(ROOMS +
				"fountain_square4"));
		}
	}
	set_open("door", 0);
	set_locked("door", 1);
	call_other(ROOMS+"fountain_square4", "set_locked", 
		"door", 1);
	call_other(ROOMS+"fountain_square4", "set_open", 
		"door", 0);
	open = 0;
}

void look_door() {
	if(query_open("door")) {
		write("The door to the Book Binder's Shop is wide open.");
	}
	else {
		write("The door to Book Binder's Shop is closed.");
	}
}

void look_wooden_door() {
	if(query_open("wooden door")) {
		write("The wooden room door is open.");
	}
	else {
		write("The wooden door is closed.");
	}
}
