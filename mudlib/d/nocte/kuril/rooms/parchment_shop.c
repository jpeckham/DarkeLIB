#include <std.h>
#include <nevin.h>

inherit "std/vault";

nosave int open;

void init() {
	::init();
	add_action("look_map", "look");
}

void create() {
	::create();
	set_property("indoors", 1);
	set_property("light", 2);
	set_property("night light", 0);
	set_property("no castle", 1);
	set("short", "Llendov's Parchment Shop");
	set("long",
		"Pieces of work lay about the shop like leafs under a tree.  "
		"Being the only master limner of Kuril, Llendov is hired "
		"to illuminate, copy, and transcribe all manners of papers, "
		"forms, texts, manuscripts, maps, and letters.  Born the son "
		"of a warrior, Llendov spent much of his youth swinging a "
		"sword until a battle which removed one of his legs, ended "
		"his career as a warrior.  Fine examples of his work line "
		"the walls of his small shop.  There is a small piece of paper "
		"which catches your attention because of the beautiful "
		"script used to write it.  On the far wall is a wonderful "
		"map of the known world, complete with fantastical monsters "
		"and fine bordering."
	);
	set_items( ([
		"paper": "It reads: 'say Llendov, help' for assistance",
		"wooden door": (: this_object(), "look_wooden_door" :),
		"door": (: this_object(), "look_door" :),
		"work": "He must be a very busy person with so much work "
			"to do.  You hope that you are not bothering him "
			"unduly."
	]) );
	set_exits( ([
		"north": ROOMS+"thieves_court2",
		"south": ROOMS+"parchment_shop_storage"
	]) );
	set_door("door", ROOMS+"thieves_court2", "north", 
		"kuril parchment shop key");
	set_open("door", 1);
	set_locked("door", 0);
	set_door("wooden door", ROOMS+"parchment_shop_storage", 
		"south", "kuril parchment shop storage key");
	set_open("wooden door", 0);
	set_locked("wooden door", 1);
	set_lock_level("wooden door", 25);
	set_open_function("open_door", "07:00:00");
	set_close_function("close_door", "17:00:00");
}

void reset() {
	::reset();
	seteuid(getuid());
	if(!present("llendov") && open) {
		new(MON+"llendov.c")->move(this_object());
	}
}

void open_door() {
	set_locked("door", 0);
	set_open("door", 1);
	call_other(ROOMS+"thieves_court2", "set_open",
		"door", 1);
	call_other(ROOMS+"thieves_court2", "set_locked",
		"door", 0);
	open = 1;
	this_object()->reset();
}

void close_door() {
	object ob, *inv;
	int i;

	ob = present("llendov", this_object());
	if(ob) ob->remove();
	inv = all_inventory(this_object());
	i = sizeof(inv);
	while(i--) {
		if(living(inv[i])) {
			message("info", "Llendov asks you to leave so that he "
				"may close his shop.", inv[i]);
			inv[i]->move(find_object_or_load(ROOMS+
				"thieves_court2"));
		}
	}
	set_open("door", 0);
	set_locked("door", 1);
	call_other(ROOMS+"thieves_court2", "set_locked", 
		"door", 1);
	call_other(ROOMS+"thieves_court2", "set_open", 
		"door", 0);
	open = 0;
}

void look_door() {
	if(query_open("door")) {
		write("The door to Llendov's Parchment Shop is open.");
	}
	else {
		write("The door to Llendov's Parchment Shop is closed.");
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

int look_map(string str) {
	if(!str) { return 0; }
	if(str == "world map") {
		this_player()->more("/wizards/mikus/world_map");
		return 1;
	}
}
