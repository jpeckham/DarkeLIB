// Coder: %^GREEN%^nEv!N%^RESET%^
// Darkemud

#include <std.h>
#include <nevin.h>
inherit "/std/vault";

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
	set("short", "Alchemist Shop");
	set("long",
		"Vials, bottles, jars, flasks, beakers, powders, herbs, and "
		"other assorted glassware line the walls and floor of the "
		"alchemist shop.  Only the finest alchemical goods "
		"and products are sold here.  Yrtrid, the owner of this "
		"fine store, is known for boasting that even if he doesn't "
		"have an item...he can find it for you...at the right price, "
		"of course.  Along the east wall is a rune inscripted door "
		"leading most likely to the storeroom.  A sign sits upon "
		"the counter top for all customers to read."
	);
	set_items( ([
		({"vials", "bottles", "jars", "flasks", "beakers", "glassware"})
			: "Of various shapes, sizes, and colors, they are "
			"filled with all types of ingredients.",
		({"powders", "herbs"}): "Hmm, now what could those be used "
			"for?",
		"walls": "Covered from floor to ceiling with thousands of "
			"items, if it's not here, maybe it doesn't exist?",
		"sign": "Try 'read'ing it.",
		({"rune door", "rune inscripted door", "inscripted door"}):
			(: call_other, this_object(), "look_rune_door" :),
		"door": (: call_other, this_object(), "look_door" :),
		({"goods", "products"}): "You should probably ask the "
			"shopkeeper if you would like to see what is "
			"currently available."
	]) );
	set_exits( ([
		"east": ROOMS+"alchemist_stoarge",
		"west": ROOMS+"guardmans_road4"
	]) );
	set_smell("default", "The sharp scent of chemicals and herbs greets "
		"you as you enter the shop.");
	set_door("door", ROOMS+"guardmans_road4", "west",
		"kuril alchemist shop key");
	set_open("door", 1);
	set_locked("door", 0);
	set_door("rune inscripted door", ROOMS+"alchemist_storage",
		"east", "kuril alchemist storage key");
	set_open("rune inscripted door", 0);
	set_locked("rune inscripted door", 1);
	set_open_function("open_door", "07:00:00");
	set_close_function("close_door", "17:00:00");
}

void reset() {
	::reset();
	seteuid(getuid());
	if(!present("yrtrid") && open){
		new(MON+"yrtrid")->
			move(this_object());
	}
	return;
}

int read_sign(string str) {
	if(!str) {
		notify_fail("What do you wish to read?\n");
		return 0;
	}
	if(str == "sign") {
		write("The sign reads: say 'Yrtrid, help' for assistance.");
		return 1;
	}
}

void open_door() {
	set_locked("door", 0);
	set_open("door", 1);
	call_other(ROOMS+"guardmans_road4", "set_locked",
		"door", 0);
	call_other(ROOMS+"guardmans_road4", "set_open",
		"door", 1);
	open = 1;
	this_object()->reset();
}

void close_door() {
	object ob, *inv;
	int i;
	
	ob = present("yrtrid", this_object());
	if(ob) ob->remove();
	inv = all_inventory(this_object());
	i = sizeof(inv);
	while(i--) {
		if(living(inv[i])) {
			message("info", "With a wave of his hand, Yrtrid "
				"transports you outside of his shop in prepar"
				"ation to close up.", inv[i]);
			inv[i]->move(find_object_or_load(ROOMS+
				"guardmans_road4"));
		}
	}
	set_open("door", 0);
	set_locked("door", 1);
	call_other(ROOMS+"guardmans_road4", "set_locked",
		"door", 1);
	call_other(ROOMS+"guardmans_road4", "set_open",
		"door", 0);
	open = 0;
}

void look_door() {
	if(query_open("door")) {
		write("The door to the Alchemist shop is open.");
	}
	else {
		write("The door to the Alchemist shop is closed.");
	}
}

void look_rune_door() {
	if(query_open("rune inscripted door")) {
		write("The rune inscripted door is open.");
	}
	else {
		write("The rune inscripted door is closed.");
	}
}
