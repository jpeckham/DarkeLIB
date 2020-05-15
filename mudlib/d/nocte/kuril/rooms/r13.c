// Coder: %^GREEN%^nEv!N%^RESET%^
// Darkemud

#include <std.h>
#include <nevin.h>
inherit "/wizards/nevin/std/kuril_river";

nosave int THROW;

void init() {
	::init();
	if(!THROW) {
		if(random(3) == 0) { call_out("throw_stuff", 10, TP); }
		THROW = 1;
	}
}

void reset() {
	::reset();
	if(!present("fowl")) {
		new(MON+"fowl")->move(TO);
		new(MON+"fowl")->move(TO);
		new(MON+"fowl")->move(TO);
	}
	if(query_night()) {
		if(!present("ghost")) { new(MON+"ghost")->move(TO); }
	}
}

void create() {
	THROW = 0;
	::create();
	set_property("indoors", 0);
	set_property("light", 3);
	set_property("night light", -2);
	set_property("no castle", 1);
	set("short", "River Near Kuril");
	set("long",
		"Above you can be heard laughter and rowdy conversation.  "
		"Most likely a tavern of sorts.  You can barely make out some "
		"of the features of the buildings above you siloquetted "
		"against the cliff walls."
	);
	set_items( ([
		({"cliffs", "cliff"}): "They rise almost straight up toward "
			"the city above you.",
		"tavern": "You can barely make it out.",
		"river": "It flows swiftly past the city of death as if it "
			"too did not wish to remain too close to that "
			"accursed place."
	]) );
	set_listen("default", "In the distance you can hear crashing water.");
	set_exits( ([
		"northeast": ROOMS+"r14",
		"southwest": ROOMS+"r12"
	]) );
}

void throw_stuff(object who) {
	message("info", "Refuse from above hits "+who->query_cap_name()+
		"right in the head!  Obviously thrown from the tavern above "+
		who->query_subjective()+" now smells of liquor.  Well, look "
		"on the bright side, at least they don't recycle their drink.",
		ENV(TP));
	THROW = 0;
}
