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
	set_property("light night", -3);
	set_property("no castle", 1);
	set("short", "Dnerr's Adventurers Shop");
	set("long",
		"Goods that any well prepared adventurer should have "
		"line the walls of Dnerr's Adventurers Shop.  "
		"Having spent his earlier years as an adventurer, "
		"Dnerr has retired to a much safer profession.  "
		"Gleaned from years of actual adventuring, his business "
		"insight now provides would be adventurers with the tools "
		"and equipment necessary for their profession.  A wooden "
		"sign greets new visitors by almost smacking them in their "
		"forehead as they enter the shop."
	);
	set_items( ([
		"sign": "Try 'read'ing it.",
		"wooden door": (: call_other, this_object(), "look_wooden_door" :),
		"door": (: call_other, this_object(), "look_door" :),
		"goods": "What magnificent pieces of " +
			"equipment!  You feel like you just have to buy "
			"something."
	]) );
	set_exits( ([
		"north": ROOMS+"general_store_storage",
		"south": ROOMS+"fountain_square2"
	]) );
	set_door("door", ROOMS+"fountain_square2", "south", 
		"kuril leather shop key");
	set_open("door", 1);
	set_locked("door", 0);
	set_door("wooden door", ROOMS+"general_store_storage", 
		"north", "kuril general store storage key");
	set_open("wooden door", 0);
	set_locked("wooden door", 1);
	set_lock_level("wooden door", 25);
	set_open_function("open_door", "07:00:00");
	set_close_function("close_door", "17:00:00");
}

void reset() {
	::reset();
	seteuid(getuid());
	if(!present("dnerr") && open) {
		new(MON+"dnerr")->move(this_object());
	}
}

int read_sign(string str) {
	if(!str) {
		notify_fail("You want to read what?\n");
		return 0;
	}
	else if(str == "sign") {
		write("The sign reads: say 'Dnerr, help' for assistance.");
		return 1;
	}
}

void open_door() {
	set_locked("door", 0);
	set_open("door", 1);
	call_other(ROOMS+"fountain_square2", "set_open",
		"door", 1);
	call_other(ROOMS+"fountain_square2", "set_locked",
		"door", 0);
	open = 1;
	this_object()->reset();
}

void close_door() {
	object ob, *inv;
	int i;

	ob = present("dnerr", this_object());
	if(ob) ob->remove();
	inv = all_inventory(this_object());
	i = sizeof(inv);
	while(i--) {
		if(living(inv[i])) {
			message("info", "Dnerr hurries you to the door as " +
				"he closes up his shop.", inv[i]);
			inv[i]->move(find_object_or_load(ROOMS+
				"fountain_square2"));
		}
	}
	set_open("door", 0);
	set_locked("door", 1);
	call_other(ROOMS+"fountain_square2", "set_locked", 
		"door", 1);
	call_other(ROOMS+"fountain_square2", "set_open", 
		"door", 0);
	open = 0;
}

void look_door() {
	if(query_open("door")) {
		write("The door to Dnerr's Leather Goods Shop is open.");
	}
	else {
		write("The door to Dnerr's Leather Goods Shop is closed.");
	}
}

void look_wooden_door() {
	if(query_open("wooden door")) {
		write("The wooden door is open.");
	}
	else {
		write("The wooden door is closed.");
	}
}
