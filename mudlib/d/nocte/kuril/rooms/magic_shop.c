#include <std.h>
#include <nevin.h>

inherit "std/vault";

nosave int open;

void create(){
	::create();
	set_property("indoors", 1);
	set_property("light", 2);
	set_property("night light", 1);
	set_property("no castle", 1);
	set("short", "Magic Shop");
	set("long",
		"Small globes cast a soft light in the Magic Shop.  Animals "
		"from all over the world are being kept in small magic "
		"stasis orbs.  Potions, elixers, and droughts can be "
		"purchased here as well.  Magical reagents and spell "
		"components from the common to the incredibly rare line the "
		"shelves of this crowded shop."
	);
	set_items( ([
		"sign": "The reads: 'say Tendra, help' for assistance",
		({"rune covered door", "rune door", "rune"}):
			 (: this_object(), "look_rune_door" :),
		"door": (: this_object(), "look_door" :),
		({"globes", "orbs"}): "These small enchanted globes hover silently "
			"near the ceiling.",
		"animals": "Frogs, lions, birds...all magically contained "
			"and miniaturized in their own stasis orbs.",
		({"potions", "elixers", "droughts"}): "Perhaps there is a "
			"a love potion here if you ask.",
		({"reagents", "components"}): "You have heard from legend "
			"that many spells require the usage of material "
			"components in order to be cast properly.",
	]) );
	set_exits( ([
		"east": ROOMS+"magus_lane3",
		"west": ROOMS+"magic_shop_storage"
	]) );
	set_door("door", ROOMS+"magus_lane3.c", "east", 
		"kuril magic shop key");
	set_open("door", 1);
	set_locked("door", 0);
	set_door("rune covered door", ROOMS+
			"magic_shop.c", "west", 
			"kuril magic shop storage key");
	set_open("rune covered door", 0);
	set_locked("rune covered door", 1);
	set_lock_level("rune covered door", 50);
	set_open_function("open_door", "07:00:00");
	set_close_function("close_door", "17:00:00");
}

void reset() {
	::reset();
	seteuid(getuid());
	if(!present("tendra") && open) {
		new(MON+"tendra.c")->move(this_object());
	}
}

void open_door() {
	set_locked("door", 0);
	set_open("door", 1);
	call_other(ROOMS+"magus_lane3.c", "set_open",
		"door", 1);
	call_other(ROOMS+"magus_lane3.c", "set_locked",
		"door", 0);
	open = 1;
	this_object()->reset();
}

void close_door() {
	object ob, *inv;
	int i;
	
	ob = present("tendra", this_object());
	if(ob) ob->remove();
	inv = all_inventory(this_object());
	i = sizeof(inv);
	while(i--) {
		if(living(inv[i])) {
			message("info", "Whispering softly, Tendra quietly " +
				"asks you to leave.", inv[i]);
			inv[i]->move(find_object_or_load(ROOMS+
				"magus_lane3.c"));
		}
	}
	set_open("door", 0);
	set_locked("door", 1);
	call_other(ROOMS+"magus_lane3.c", "set_locked", 
		"door", 1);
	call_other(ROOMS+"magus_lane3.c", "set_open", 
		"door", 0);
	open = 0;
}

void look_door() {
	if(query_open("door")) {
		write("The door to Tendra's Magic Shop is wide open.");
	}
	else {
		write("The door to Tendra's Magic Shop is closed.");
	}
}

void look_rune_door() {
	if(query_open("rune covered door")) {
		write("The rune covered door is open.");
	}
	else {
		write("The rune covered door is closed.");
	}
}
