#include <std.h>
inherit OBJECT;

nosave int ROLLED;

void init() {
	::init();
	add_action("lay_carpet", "lay");
	add_action("roll_carpet", "roll");
}

void create() {
	::create();
	set_name("carpet");
	set_id( ({"carpet"}) );
	set_weight(300);
	set_value(200);
	ROLLED = 1;
}

string query_short() {
	if(ROLLED) { return "A rolled up small carpet"; }
	return "An unrolled small carpet";
}

string query_long() {
	if(ROLLED) { 
		return "A small carpet.  It would be a very nice carpet to "
			"lay on top of and relax.  You should probably "
			"<lay> it out first."; 
	}
	return "A small carpet which has been unrolled for you to relax "
		"on.  To put it away, just <roll> it up.";
}

int lay_carpet(string str) {
	if(!str) {
		notify_fail("What do you want to lay down on the ground?");
		return 0;
	}
	if(str == "carpet") {
		if(!ROLLED) {
			message("info", "The carpet is already unrolled.", TP);
			return 1;
		}
		message("info", "You unroll the carpet and lay it down on the "
			"ground.", TP);
		message("info", TPQCN+" unrolls a carpet and lays down.",
			ENV(TP), ({TP}) );
		ROLLED = 0;
		return 1;
	}
}

int roll_carpet(string str) {
	if(!str) {
		notify_fail("What do wish to roll up?");
		return 0;
	}
	if(str == "carpet") {
		if(ROLLED) {
			message("info", "The carpet is already rolled up.", TP);
			return 1;
		}
		message("info", "You roll up the carpet.", TP);
		message("info", TPQCN+" rolls up a carpet.", ENV(TP),
			({TP}) );
		ROLLED = 1;
		return 1;
	}
}
