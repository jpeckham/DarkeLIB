#include <std.h>
#include <nevin.h>
inherit MONSTER;

nosave int KILL;

void init() {
	::init();
	if(!query_night()) {
		if(!wizardp(this_player())) {
                             this_object()->remove();
                             return;
		}
	}
	if(!KILL) {
		KILL = 1;
		if( !(TP->chk_kuril_undead() || wizardp(TP)) ) {
			call_out("kill_them", 20, TP);
		}
	}
}

void create() {
	string var;
	::create();
	set_name("wight");
	set_id( "wight" );
	set("race", "undead");
	set_body_type("human");
	set_level(5 + random(3));
	switch(random(5)) {
		case 0: var = "A bloodthirsty wight"; break;
		case 1: var = "A predatious wight"; break;
		case 2:	var = "A bloody wight"; break;
		case 3: var = "A horrid wight"; break;
		case 4: var = "A ferocious wight"; break;
	}
	this_object()->set("short", var);
	set_long(
		"An evil gleam waxes over the opaque eyes of this foul "
		"creature...if moves toward you with purpose in its "
		"stride."
	);
	set_property("physical resistance", ([
		"default": 25,
		"iron": 25,
		"wood": 25
	]) );
	set_overall_ac(7);
	set_max_hp(500);
	set_hp(500);
	set_property("enhance criticals", ([
		"holy": 1,
		"impaling": -1
	]) );
	set_property("melee damage", ([
		"slashing": 15,
		"crushing": 10
	]) );
	set_emotes(5, ({
		"A wight thrashes about raking at the air in from of it.",
		"The wight hisses at you.",
		"The wight glares at you with evil, hate filled eyes."
	}), 0);
	new(ARM+"tattered_clothes")->move(TO);
	force_me("wear tattered clothes");
}

void heart_beat() {
	object who;
	int DRAIN;
	
	::heart_beat();
	who = TO->query_current_attacker();

	if(!who) return;
	if(!present(who)) return;
	if(random(100) < 1) {
		tell_object(who, "%^CYAN%^The wight's hand touches your "
			"flesh...you feel your life force being drained "
			"away....%^RESET%^");
		tell_room(ENV(who), who->query_cap_name()+" shudders from "
			"being touched by the wight.  "+who->query_cap_name() +
			" turns pale as the wight cackles with insane glee!");
		who->add_exp(-random(100));
		DRAIN = random(30);
		who->add_hp(-DRAIN);
		TO->add_hp(DRAIN/4);
	}
}

void kill_them(object player) {
       if(!player) return;
	if(!present(player)) { return; }
	message("info", "The wight shambles toward you and tries to rip your "
		"arms off!", player);
	message("info", "A wight tried to rip "+player->query_cap_name()+
		"'s off!", ENV(player), ({player}) );
	force_me("kill "+player->query_cap_name());
	KILL = 0;
}

void die(object who) {
	tell_room(ENV(TO), "As it finally dies the wight lets out a soft "
		"sigh...");
	::die(who);
	return;
}

int chk_my_mob() {
	return 1;
}

int chk_kuril_undead() {
	return 1;
}
