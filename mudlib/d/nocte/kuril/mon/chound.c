// Coder: %^GREEN%^nEv!N%^RESET%^
// Darkemud

#include <std.h>
#include <nevin.h>
inherit MONSTER;

nosave int GROWL, KILLING;

void init() {
	::init();
	if(!GROWL) { 
		GROWL = 1;
		if( !(TP->chk_kuril_undead() || TP->chk_temple_mob() ||
			wizardp(TP)) ) { 
				call_out("be_mean", 5, TP); 
		}
	}
}

void create() {
	string var;
	::create();
	GROWL = 0;
	KILLING = 0;
	set_name("chaos hound");
	set_id( ({"chaos hound", "hound", "chaos", "mass"}) );
	set("race", "chaos hound");
	set_body_type("quadruped");
	set_level(15);
	switch(random(3)) {
		case 0: var = "A fanged mass of chaos"; break;
		case 1: var = "A fire breathing hound"; break;
		case 2:	var = "A chaos hound"; break;
	}
	this_object()->set("short", var);
	set_long(
		"Formed from primal chaos, this creature's fangs are almost "
		"two feet long.  With spitle that burns holes in the ground "
		"beneath its feet, the constantly changing form of the "
		"chaos hound is difficult to look at for too long.  Summoned "
		"by the dark priests of the Temple of the Blessed Return, "
		"these minions of chaos's only desire is to create "
		"as much destruction as possible.  If it where not for the "
		"magical collars keeping them under control, they would "
		"quickly turn on their masters."
		
	);
	set_property("physical resistance", ([ "default": 30 ]) );
	set_overall_ac(40);
	set_max_hp(1700);
	set_hp(1700);
	set_exp(150000);
	set_property("enhance criticals", ([
		"strike": -2,
		"stress": -2,
		"impact": -2,
		"fire": -2,
		"impaling": -2,
		"cutting": -2,
		"crushing": -2
	]) );
	set_property("melee damage", ([
		"cutting": 30,
		"infernal": 20,
		"fire": 20,
	]) );
	set_moving(0);
	set("aggressive", 0);
	set("gang bang", 0);
	set_emotes(5, ({
		"The chaos hound growls at you!",
		"You notice that your weapon just seems to pass right through "
			"the chaos hound!"
	}), 1);
	new(ARM+"hound_collar.c")->
		move(this_object());
	force_me("wear collar");
}

void be_mean(object player) {
	string str1, str2;
	switch(random(3)) {
		case 0: str1 = "A chaos hound growls menacingly at you."; 
			str2 = "A chaos hound growls menacingly at "+
				player->query_cap_name()+".";
			break;
		case 1: str1 = "A chaos hound's eyes flare bright red as it "
				"glares at you.";
			str2 = "A chaos hound's eyes flare bright red as it "
				"glares at "+player->query_cap_name()+".";
			break;
		case 2: str1 = "Spittle from the chaos hound's fangs "
				" leaves pits in the stones of the road before "
				"you.";
			str2 = player->query_cap_name()+" jumps back as "
				"spittle from a chaos hound's mouth eats "
				"holes into the ground before "+
				player->query_objective()+".";
			break;
	}
	message("info", str1, player);
	message("info", str2, ENV(player), ({player}) );
	GROWL = 0;
	if(!KILLING) {
		KILLING  = 1;
		call_out("kill_them", 30, player);
	}
}

void kill_them(object player) {
	if(!present(player)) { return; }
	message("info", "Without warning the chaos hounds lunges at you!",
		player);
	message("info", "Without any warning, a chaos hound lunges at "+
		player->query_cap_name()+"'s throat!", ENV(player),
		({player}) );
	force_me("kill "+player->query_cap_name());
	KILLING = 0;
}
 
chk_my_mob() {
	return 1;
}

chk_temple_mob() {
	return 1;
}
