#include <std.h>
#include <nevin.h>

inherit "std/vault";

nosave int open;

void create(){
	::create();
	set_property("indoors", 1);
	set_property("light", 1);
	set_property("night light", 1);
	set_property("no castle", 1);
	set("short", "Mordin's Corpse Market");
	set("long",
		"Corpses litter the cold ground is this macabre shop. "
		"Pieces of wood have been hammered into the walls to "
		"close up the windows " 
		"in order to keep the room at a cool temperature. "
		"Some bodies, having been recently relieved of their "
		"souls have silent screams on their still warm faces. "
		"A few necromancers wade through the bodies to pick "
		"the choicest bodies for reanimation...or as some "
		"would put it, eternal torture.  Only a sign decorates "
		"the walls.  The front door is to the south."
	);
	set_items( ([
		"sign": "The reads: 'say Mordin, help' for assistance",
		"wooden door": (: call_other, this_object(), "look_wooden_door" :),
		"door": (: call_other, this_object(), "look_door" :),
		({"corpse", "corpses", "bodies"}): "From the smell, some "
			 "have been in the shop for quite a long time.",
		"necromancers": "From the School of Necromancy, they "
			"come here every night to add soldiers and slaves "
			"to their unholy army of the dead."
	]) );
	set_exits( ([
		"north": ROOMS+"corpse_shop_storage",
		"south": ROOMS+"marketplace1"
	]) );
	set_door("door", ROOMS+"marketplace1", "south", 
		"kuril corpse shop key");
	set_open("door", 1);
	set_locked("door", 0);
	set_door("wooden door", ROOMS+"corpse_shop_storage", 
		"north", "kuril corpse shop storage key");
	set_open("wooden door", 0);
	set_locked("wooden door", 1);
	set_lock_level("wooden door", 25);
	set_open_function("open_door", "18:00:00");
	set_close_function("close_door", "06:00:00");
}

void reset() {
	::reset();
	seteuid(getuid());
	if(!present("mordin") && open) {
		new(MON+"mordin")->move(this_object());
	}
}

void open_door() {
	set_locked("door", 0);
	set_open("door", 1);
	call_other(ROOMS+"marketplace1", "set_open",
		"door", 1);
	call_other(ROOMS+"marketplace1", "set_locked",
		"door", 0);
	open = 1;
	this_object()->reset();
}

void close_door() {
	object ob, *inv;
	int i;

	ob = present("mordin", this_object());
	if(ob) ob->remove();
	inv = all_inventory(this_object());
	i = sizeof(inv);
	while(i--) {
		if(living(inv[i])) {
			message("info", "A shadow washes over you...before " +
				"you realize what has happened you find " +
				"yourself outside of Mordin's shop.", inv[i]);
			inv[i]->move(find_object_or_load(ROOMS+
				"marketplace1"));
		}
	}
	set_open("door", 0);
	set_locked("door", 1);
	call_other(ROOMS+"marketplace1", "set_locked", 
		"door", 1);
	call_other(ROOMS+"marketplace1", "set_open", 
		"door", 0);
	open = 0;
}

void look_door() {
	if(query_open("door")) {
		write("The door to the Corpse Shop is wide open.");
	}
	else {
		write("The door to the Corpse Shop is closed.");
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
