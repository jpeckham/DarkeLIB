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
	set_property("night light", 0);
	set_property("no castle", 1);
	set("short", "Lupo's Butcher Shop");
	set("long",
		"A large chopping board sits in the middle of the room "
		"while an aproned man raises his cleaver...CHOOM!  With "
		"insane delight Lupo chops off a large piece of meat, "
		"which immediately falls on the filthy ground.  He then proceeds "
		"to kick the piece of meat around the room all the while "
		"yelling expletives too foul to repeat.  Hmm, I guess being "
		"the only butcher shop in Kuril has allowed Lupo some "
		"luxuries in his salesmanship technique."
	);
	set("night long",
		"While the shop is closed, the rats and mice come in to "
		"take their share of the pickings from off of the floor."
	);
	set_items( ([
		({"board", "chopping board"}): "Old but sturdy, this board " +
			"has seen a lot of work in this shop.",
		"sign": "Try 'read'ing it.",
		"metal door": (: call_other, this_object(), "look_metal_door" :),
		"metal": (: call_other, this_object(), "look_metal_door" :),
		"door": (: call_other, this_object(), "look_door" :),
		"meat": "Lupo screams at you: YOU LOOKING AT MY MEAT!?!?!?!"
	]) );
	set_exits( ([
		"east": ROOMS+"thieves_court3",
		"south": ROOMS+"butcher_shop_storage"
	]) );
	set_door("door", ROOMS+"thieves_court3", "east", 
		"kuril butcher shop key");
	set_open("door", 1);
	set_locked("door", 0);
	set_door("metal door", ROOMS+"butcher_shop_storage", 
		"south", "kuril butcher shop storage key");
	set_open("metal door", 0);
	set_locked("metal door", 1);
	set_lock_level("metal door", 25);
	set_open_function("open_door", "07:00:00");
	set_close_function("close_door", "17:00:00");
}

void reset() {
	::reset();
	seteuid(getuid());
	if(!present("lupo") && open) {
		new(MON+"lupo")->move(this_object());
	}
}

int read_sign(string str) {
	if(!str) {
		notify_fail("What do you want to read now!?!?\n");
		return 0;
	}
	else if(str == "sign") {
		write("The sign reads (in barely legible scrawl): "
			"say 'Lupo, help' for assistance.");
		return 1;
	}
}
	
void open_door() {
	set_locked("door", 0);
	set_open("door", 1);
	call_other(ROOMS+"thieves_court3", "set_open",
		"door", 1);
	call_other(ROOMS+"thieves_court3", "set_locked",
		"door", 0);
	open = 1;
	this_object()->reset();
}

void close_door() {
	object ob, *inv;
	int i;

	ob = present("lupo", this_object());
	if(ob) ob->remove();
	inv = all_inventory(this_object());
	i = sizeof(inv);
	while(i--) {
		if(living(inv[i])) {
			message("info", "Lupo gives you the boot as he closes" +
				" his shop.", inv[i]);
			inv[i]->move(find_object_or_load(ROOMS+
				"thieves_court3"));
		}
	}
	set_open("door", 0);
	set_locked("door", 1);
	call_other(ROOMS+"thieves_court3", "set_locked", 
		"door", 1);
	call_other(ROOMS+"thieves_court3", "set_open", 
		"door", 0);
	open = 0;
}

void look_door() {
	if(query_open("door")) {
		write("The door to Lupo's Butcher Shop is wide open.");
	}
	else {
		write("The door to Lupo's Butcher Shop is closed.");
	}
}

void look_metal_door() {
	if(query_open("metal door")) {
		write("Uh oh, someone has been in Lupo's back room.");
	}
	else {
		write("The metal door is closed.");
	}
}
