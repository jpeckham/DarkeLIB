// Coder: %^GREEN%^nEv!N%^RESET%^
// Darkemud
// blur ring-->adds a new blur shadow, but will not check for a max number
// of them.

#include <std.h>
#include <nevin.h>
#define OBJ 	this_player()->query_objective()

inherit "/std/armour";

nosave int CHARGES;

void init() {
	::init();
	add_action("look_symbol", "look");
	add_action("activate_ring", "say");
}

void create() {
	::create();
	set_name("quivering ring");
	set("id", ({ "ring", "quivering ring"}) );
	set("short", "A quivering ring");
	set("long", "The ring quivers in your grasp.  What a strange little "
		"ring.  It appears to have some type of symbol on it.");
	set_weight(5);
	set_value(200);
	set_type("ring");
	set_limbs( ({"right hand", "left hand", "first hand", "second hand",
		"third hand", "fourth hand", "first branch", "second branch",
		"third branch", "fourth branch"}) );
	set_ac(1);
	set_material("chyrite");
	set_property("duration blur", 30);
	CHARGES = 3;
}

string query_long() {
	string str;
	switch(CHARGES) {
		case 1: str = "The ring is quiet in your hand.  Almost faded "
			"away complete is a symbol of some sort on the ring.";
			break;
		case 2: str = "The ring quivers slightly.  There is a faded "
			"symbol upon it.";
			break;
		case 3: str = "The ring quivers in your grasp.  What a strange "
			"little ring.  It appears to have some type of symbol "
			"on it.";
			break;
	}
	return str;
}

int look_symbol(string str) {
	string *langs;
	int i;
	if(!str) {
		notify_fail("What do you wish to look at?");
		return 0;
	}
	if(str == "symbol") {
		langs = TP->query_all_languages();
		i = sizeof(langs);
		if(member_array("yin", langs)) {
			message("info", "The symbol is translated as "
				"'rik'tah', or 'obscurement' in the Yin "
				"language.", TP);
			return 1;
		}
		message("info", "You are unable to understand what the "
			"symbol means.", TP);
		return 1;
	}
}

int activate_ring(string str) {
	object ob;
	if(!str) { return 0; }
	if(str == "rik'tah") {
		if(!(TP->query_primary_lang() == "yin")) { return 0; }
		if((random(10) + 1) > TP->query_lang_prof("yin")) { return 0; }
		if(TP->query_blurred()) {
			message("info", "%^RED%^%^BOLD%^Nothing seems to happen.", TP);
			call_other(TO, "use_ring");
			return 0;
		}
		message("info", "%^CYAN%^%^BOLD%^You activate the ring!  A haze fills your "
			"%^CYAN%^%^BOLD%^vision for a brief moment.  Your form seems to be "
			"%^CYAN%^%^BOLD%^faded slightly.", TP);
		message("info", "%^CYAN%^%^BOLD%^"+TPQCN+" seems to fade from view as a ring "
			"%^CYAN%^%^BOLD%^"+OBJ+" is holding flares to life!", ENV(TP), ({TP}) );
		ob = new("/std/spells/shadows/blur_shadow");
		ob->set_penalty(2 * (3 + random(10)));
		ob->start_shadow(TP, props["duration blur"], "%^CYAN%^%^BOLD%^Your material form "
			"%^CYAN%^%^BOLD%^solidifies once again.");
		call_other(TO, "use_ring");
		return 0;
	}
}

void use_ring() {
	CHARGES -= 1;
	if(CHARGES < 1) {
		message("info", "%^RED%^%^BOLD%^A quivering ring fades away.", TP);
		TO->remove();
		return;
	}
	message("info", "%^RED%^%^BOLD%^The ring seems to fade a little.", TP);
	write_file("/d/nocte/kuril/logs/qui_ring.log", TPQCN+" activated a "
		"quivering ring at "+ctime(time())+".\n");
}
