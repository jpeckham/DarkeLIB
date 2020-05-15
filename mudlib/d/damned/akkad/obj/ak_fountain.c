// Coder: %^GREEN%^nEv!N%^RESET%^
// Darkemud
// Fountain for Tranquility Square, Akkad
// Modified from Khojem's vat.c code

#include <move.h>
inherit "/std/Object";

#define TPQCN	this_player()->query_cap_name()
#define TP	this_player()
#define TO	this_object()
#define OBJ	this_player()->query_objective()
#define POS	this_player()->query_possessive()
#define CAP	capitalize
#define TPQM	this_player()->query_money
#define ENV 	environment
#define DELAY   random(60) + 120

nosave int TELL;

int check_living(object *inv);

int move(object dest) {
	if(living(dest)) return MOVE_NOT_ALLOWED;
	else return ::move(dest);
}

void create() {
  	::create();
  	set_name("fountain");
  	set_id(({ "fountain" }) );
  	set_short("%^CYAN%^%^BOLD%^a crystal clear fountain%^MAGENTA%^%^BOLD%^");
  	set_long(
		"A crystal clear fountain sends sparkling drops of water "
		"into the air.  The great statue of Dranaxar rests in the "
		"center of the fountain surrounded by sculptures of "
		"fantasical creatures.  It is here that the citizens of "
		"Akkad obtain their water.  Small birds also make use of the "
		"fountain, bathing in small pools of water created by children "
		"splashing about.  The occasional person tosses a coin into "
		"the water to make a wish."
 	);
	set_property("duration", 600);
	TELL = 1;
	call_out("story_telling", DELAY);
}

void init() {
  	::init();
  	add_action("fill_vessel", "fill");
	add_action("drink_fountain", "drink");
	add_action("wash_me", "bath");
	add_action("splash_water", "splash");
	add_action("toss_coin", "toss");
	add_action("make_wish", "wish");
	add_action("be_cheap", "search");
	add_action("be_gross", "spit");
	add_action("no_throw", "throw");
	if(!TELL) {
		TELL = 1;
		call_out("story_telling", DELAY );
	}
}

int no_throw(string str) {
	string *command;
	command = explode(str, " ");
	if( (member_array("throw", command)) &&
		(member_array("at", command)) ) {
		write("A sudden jet of water from one of the fountain statues "
			"sends you sprawling.");
		say(TPQCN+" tries to throw a weapon, but is sent sprawling by a "
			"jet of water from a fountain statue.");
		return 1;
	}
	return 0;
}

int override_functions(string str) {
	string *command;
	command = explode(str, " ");
	if(member_array("reverse", command)) { 
		return 1; 
	}
	if(member_array("vibrating", command)) { 
		return 1; 
	}
	if(member_array("steal", command)) { 
		return 1; 
	}
}

void story_telling() {
	object *inv;
	string story;
	switch(random(8)) {
		case 0: story= "A wandering minstrel begins playing "
				"a lovely song.  You are drawn into "
				"the story of a once beatiful city "
				"lost to the evils of the world.  "
				"It's shattered remains still exist "
				"by the name of Kuril.";
			break;
		case 1: story = "A musical trio begins playing a "
				"rustic elven tune from Fir'nvillee.";
			break;
		case 2: story = "A weary traveller sits down to "
				"drink water from the fountain.  His "
				"sun parched skin and the large "
				"scimitar at his side tell you that he "
				"is most likely from Bir'Barrut.";
			break;
		case 3: story = "A road weary sell sword splashes "
				"cool water onto her face.  Burns "
				"cover her hands and face.  She tells "
				"you of her ill-fated expedition "
				"to the eastern side of the Island of "
				"Kuril.";
			break;
		case 4: story = "A lost child cries out loud.  After "
				"some coaxing with a small piece of "
				"candy she tells you that her parents "
				"fell to a trap laid by a group of "
				"bandits northwest of Akkad.";
			break;
		case 5: story = "A merchant shouts 'Mushrooms!  "
				"Mushrooms for sale!  Only the best "
				"from the goblin village itself!'";
			break;
		case 6: story = "A shivering mageling sits down next "
				"to you by the fountain.  When asked "
				"why he shivers so, he tells you "
				"about his journey and a fight "
				"with ice wielding snowmen.  The rest "
				"of his story is unintelligible "
				"under the chattering of his teeth.";
			break;
		case 7: story = "A bard sings a song about a "
				"great hero who lost his soul.";
			break;
		case 8: story = "A group of pilgrims fill their waterskins "
				"and move into the crowd of Tranquility "
				"Square.";
			break;
		case 9: story = "A beautiful maiden fills a pitcher of water "
				"and walks off.";
				break;
		case 10: story = "A wounded soldier stops for a drink and a "
				"brief rest before continuing on her way.";
				break;
		case 11: story = "A happy dog jumps into the fountain and "
				"splashes about.  It jumps out and soaks "
				"everyone nearby as it shakes itself dry.";
				break;
		case 12: story = "A elderly thranx sits down for a rest all "
				"the while shaking his head quietly.  From his "
				"worn clothes and bleeding feet, he must be "
				"one of the many exiles from New Faerie Dust.";
				break;
		case 13: story = "A stout dwarf stomps up to the fountain and "
				"thrusts his entire face into into the clear "
				"water.  After whiping his face with the "
				"back of his sleeve he mutters something about "
				"needing to get back to the mines.";
				break;
	}
	message("info", story, ENV(TO));
	TELL = 0;
	inv = all_inventory(TO);
	if(check_living(inv)) {
		TELL = 1;
		call_out("story_telling", DELAY);
	}
}

int check_living(object *inv) {
	int i;
	while(i--) if(living(inv[i])) return 1;
	return 0;
}


int fill_vessel(string str) {
	object ob;
	ob = present(str, this_player());
	if(!ob) {
		write("You don't have "+article(str)+".");
		return 1;
	}
	if(!ob->id("water skin")) {
 		write("You can't fill "+article(str)+" with water!");
		return 1;
	}
	if(ob->full()) {
		write("That waterskin is full already.");
		return 1;
	}
	ob->fill("water", 5, 0);
	write("You fill the waterskin.");
	message("other_action", (string)this_player()->query_cap_name() +
		" fills a waterskin with water from the vat.",
		environment(this_player()), ({ this_player() }) );
	return 1;
}

int drink_fountain(string str) {
	string drink;
	if(!str) {
		notify_fail("What do you want to drink?");
		return 0;
	}
	if( !(str == "fountain") ) { 
		notify_fail("What do you want to drink?");
		return 0; 
	}
	if(TP->query_quenched() > 420) {
		message("info", "You'll blow up if you drink another drop!", 
			TP);
		return 1;
	}
	message("info", "You take a long drink of water from the clear "
		"fountain.  You feel very refreshed.", TP);
	TP->add_quenched(-1000);
	while(TP->query_quenched() < 450) {
		TP->add_quenched(5);
	}
	switch(random(5)) {
		case 0: drink = " shoves "+POS+" face into the "
			"fountain and takes a long drink of water.";
			break;
		case 1: drink = " takes a drink of water from the "
			"fountain.";
			break;
		case 2: drink = " drinks so fast that water flys out "
			"of "+POS+" nose!";
			break;
		case 3: drink = " takes up a bucket and finishes it "
			"off with a wipe of the chin.";
			break;
		case 4: drink = " laps up some water at the fountain.";
			break;
	}
	say(TPQCN+drink);
	return 1;
}

int wash_me(string str) {
	write("You jump into the fountain and begin to wash yourself all "
		"over.  Ah, much better.  You feel better having taken "
		"your annual bath.");
	say(TPQCN+" jumps into the fountain and begins washing "+OBJ+
		"self all over!  The water takes on a much darker colour as "
		+TPQCN+" steps forth from the now dirty fountain.  But, "
		"in the blink of an eye the fountain is again clean as "
		"powerful magick goes to work to purify the water once again.");
	return 1;
}

int splash_water(string str) {
	if(!str) {
		notify_fail("Who do you want to splash with water?");
		return 0;
	}
	if(!present(str)) {
		write(CAP(str)+ " is not here at the fountain.");
		return 1;
	}
	write("You splash "+CAP(str)+" in the face!");
	say(TPQCN+" splashes water at "+CAP(str)+"!");
	return 1;
}

int toss_coin(string str) {
	object ob;
	int TOSSED, COIN;
	if(!str) {
		notify_fail("I believe you wish to: toss <coin type> coin into "
			"fountain.");
		return 0;
	}
	if(str == "mithril coin into fountain") {
		if(TPQM("mithril") == 0) { 
			write("You do not have any mithril!");
			return 1;
		}
		write("Your shiny mithril coins hits the surface of "
			"the water and disappears in a flash of "
			"light.  You sure do feel lucky!");
		say(TPQCN+" tosses a shiny mithril coin into the fountain.  "
			"Upon striking the surface of the water, "+TPQCN+
			"'s coin disappears in a flash of light!");
		if( !(TP->query_property("luck") >=1) ) {
			ob = new("/std/spells/shadows/luck_shadow");
			ob->set_luck(1);
			ob->start_shadow(TP, props["duration"],
				 "You don't feel so lucky anymore.");
		}
		return 1;
	}
	TOSSED = 0;
	COIN = 0;
	switch(str) {
		case "gold coin into fountain": COIN = 0; break;
		case "silver coin into fountain": COIN = 1; break;
		case "electrum coin into fountain": COIN = 2; break;
		case "copper coin into fountain": COIN = 3; break;
		default: COIN = -1; break;
	}
	if(COIN == -1) { 
		write("I believe you with to: toss <coin type> coin into "
			"fountain.");
		return 0; 
	}
	switch(COIN) {
		case 0: str = "gold";
			if(TPQM("gold") > 0) { TOSSED = 1; };
			TP->add_money("gold", -1);
			break;
		case 1: str = "silver"; 
			if(TPQM("silver") > 0) { TOSSED = 1; };
			TP->add_money("silver", -1);
			break;
		case 2: str = "electrum"; 
			if(TPQM("electrum") > 0) { TOSSED = 1; };
			TP->add_money("electrum", -1);
			break;
		case 3: str = "copper"; 
			if(TPQM("copper") > 0) { TOSSED = 1; };
			TP->add_money("copper", -1);
			break;
	}
	if(!TOSSED) { 
		write("You do have not have any "+str+"!");
		return 1; 
	}
	write("You toss a "+str+ " coin into the water.  It disappears is a "
		"small puff of smoke.");
	say(TPQCN+" tosses a "+str+" coin into the "
		"fountain.  The coin disappears in a puff of smoke.");
	return 1;
}

int make_wish() {
	string str;
	write("You concentrate really hard and make a wish.");
	switch(random(2)) {
		case 0: str = " squinches up "+POS+" forehead and concentrates "
			"really hard while making a wish at the fountain."; break;
		case 1: str = " almost bursts a vein in "+POS+" forehead "
			"from thinking to hard about what to wish for."; break;
		case 2: str = " scratches "+POS+" head with a dirt covered "
			"hand while trying to think of a wish to make."; break;
		case 3: str = " rubs the belly of the statue of Dranaxar "
			"thinking that will bring "+OBJ+" a wish."; break;
	}
	say(TPQCN+str);
	return 1;
}

int be_cheap(string str) {
	if(!str) { return 0; }
	if(str == "fountain") {
		write("You search around for all of those nice wonderful "
			"coins, but they all seem to have disappeared.");
		say(TPQCN+" gets on all fours searching around for the coins "
			"others have tossed into the fountain...what a "
			"cheap-o!");
	}
	return 1;
}

int be_gross(string str) {
	write("You spit into the fountain...gross!");
	say(TPQCN+" spits into the fountain...no manners!");
	return 1;
}

int clean_up() { return 0; }
