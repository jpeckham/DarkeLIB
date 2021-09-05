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
	set("short", "Candlemaker's Shop");
	set("long",
		"Long rows of candles line the walls of Lydia's Candleshop.  "
		"A small but quaint shop, Lydia specializes in candles and "
		"a few other light sources.  Although poor light sources for "
		"the hardened adventurer, candles are well suited for lighting "
		"homes and castles.  "
		"A soft glow fills the room from behind the counter.  "
		"On top of a side table is a stack of torches and lanterns.  "
		"Placed upon the counter is a small sign."
	);
	set("night long",
		"The store has been left unattended for the evening."
	);
	set_items( ([
		"sign": "Try 'read'ing it.",
		"wooden door": (: call_other, this_object(), "look_wooden_door" :),
		"door": (: call_other, this_object(), "look_door" :),
		({"candle", "candles"}): "Lydia shows you a few of hers "
			"as you catch her attention.",
		"counter": "As you peer behind the counter you see...why it's "
			"a collection of rare elven lanterns!  Now those would "
			"fetch a nice price indeed!",
	]) );
	set_exits( ([
		"north": ROOMS+"candle_shop_storage",
		"south": ROOMS+"marketplace2"
	]) );
	set_door("door", ROOMS+"marketplace2", "south", 
		"kuril candle shop key");
	set_open("door", 1);
	set_locked("door", 0);
	set_door("wooden door", ROOMS+"candle_shop_storage", 
		"north", "kuril candle shop storage key");
	set_open("wooden door", 0);
	set_locked("wooden door", 1);
	set_lock_level("wooden door", 25);
	set_open_function("open_door", "07:00:00");
	set_close_function("close_door", "17:00:00");
}

void reset() {
	::reset();
	seteuid(getuid());
	if(!present("lydia") && open) {
		new(MON+"lydia")->move(this_object());
	}
}

int read_sign(string str) {
	if(!str) {
		notify_fail("Now what do you wish to read o literate one?");
		return 0;
	}
	else if(str == "sign") {
		write("The sign reads: say 'Lydia, help' for assistance.");
		return 1;
	}
}

void open_door() {
	set_locked("door", 0);
	set_open("door", 1);
	call_other(ROOMS+"marketplace2", "set_open",
		"door", 1);
	call_other(ROOMS+"marketplace2", "set_locked",
		"door", 0);
	open = 1;
	this_object()->reset();
}

void close_door() {
	object ob, *inv;
	int i;

	ob = present("lydia", this_object());
	if(ob) ob->remove();
	inv = all_inventory(this_object());
	i = sizeof(inv);
	while(i--) {
		if(living(inv[i])) {
			message("info", "Lydia escorts you to the door as she" +
				" closes her shop.", inv[i]);
			inv[i]->move(find_object_or_load(ROOMS+
				"marketplace2"));
		}
	}
	set_open("door", 0);
	set_locked("door", 1);
	call_other(ROOMS+"marketplace2", "set_locked", 
		"door", 1);
	call_other(ROOMS+"marketplace2", "set_open", 
		"door", 0);
	open = 0;
}

void look_door() {
	if(query_open("door")) {
		write("The door to the Candle Shop is wide open.");
	}
	else {
		write("The door to the Candle Shop is closed.");
	}
}

void look_wooden_door() {
	if(query_open("metal door")) {
		write("The wooden door is open.");
	}
	else {
		write("The wooden door is closed.");
	}
}
