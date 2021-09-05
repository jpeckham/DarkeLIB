// Coder: %^GREEN%^nEv!N%^RESET%^
// Darkemud
// Fountain for Fountain Square, Kuril
// Modified from Khojem's vat.c code

#include <move.h>
#include <fountain.h>
inherit "/std/Object";

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
	set_short("%^CYAN%^%^BOLD%^a sparkling fountain%^MAGENTA%^%^BOLD%^");
	set_long(
		"A sparkling fountain sits here in the middle of the square.  "
		"A jet of water shoots out from the center of the fountain.  "
		"The water falls back gently into the pool casting colours "
		"all around you."
 	);
	set_property("duration", 600);
	TELL = 1;
	call_out("story_telling", DELAY);
}

void init() {
  	::init();
  	add_action("fill_vessel", "fill");
	add_action("drink_fountain", "drink");
	add_action("splash_water", "splash");
	add_action("toss_coin", "toss");
	add_action("make_wish", "wish");
	add_action("be_cheap", "search");
	add_action("be_gross", "spit");
	if(!TELL) {
		TELL = 1;
		call_out("story_telling", DELAY );
	}
}

void story_telling() {
	object *inv;
	int i;
	string story;
	switch(random(4)) {
		case 0: story = "\nA frail old woman leans over to fill her "
				"leaking bucket.  The sentries around the "
				"fountain laugh as she spills her first "
				"bucket of water and falls to the ground.  "
				"With determined steps she silently picks "
				"herself off and fills another bucket of "
				"water.\n";
			break;
		case 1: story = "\nA cloaked figure passes through by the "
				"fountain.  It peers into the fountain "
				"and then quickly leaves...you swear you can "
				"hear it laughing in the distance...\n";
			break;
		case 2: story = "\nA shadow passes over your eyes.  You look "
				"up only to see a large winged creature "
				"flying about over the city.  A shrill "
				"cry echoes around you.  All around you "
				"citizens duck for cover and hide their "
				"eyes in terror.\n";
			break;
		case 3: story = "\nA group of new slaves are dragged off toward  "
				"the school of necromancy.  Some of them "
				"obviously know what is in store for them "
				"for they are trying will all their might "
				"to strangle themselves with the chains that "
				"bind them.  The guards escorting them alway "
				"only laugh at their struggling.\n";
			break;
	}
	message("info", story, ENV(TO));
	TELL = 0;
	inv = all_inventory(TO);
	while(i--) {
		if(inv[i]->is_player()) {
			TELL = 1;
			call_out("story_telling", DELAY);
			return;
		}
	}
	return;
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
	return 1;
}

int drink_fountain(string str) {
	string drink;
	int count;
	if(!str) {
		notify_fail("What do you want to drink?\n\n");
		return 0;
	}
	if( !(str == "fountain") ) { 
		notify_fail("What do you want to drink?\n\n");
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
	count = 0;
	while( (TP->query_quenched() < 450) && (count < 90 ) ) {
		TP->add_quenched(5);
		count += 1;
	}
	switch(random(5)) {
		case 0: drink = " shoves "+(string)TP->query_possessive()+
			" face into the "
			"fountain and takes a long drink of water.";
			break;
		case 1: drink = " takes a drink of water from the "
			"fountain.";
			break;
		case 2: drink = " drinks so fast that water flies out "
			"of "+(string)TP->query_possessive()+" nose!";
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

int splash_water(string str) {
	if(!str) {
		notify_fail("Who do you want to splash with water?\n\n");
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
			"fountain.\n\n");
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
		TP->add_money("mithril", -1);
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
		case 0: str = " squinches up "+TPQP+" forehead and concentrates "
			"really hard while making a wish at the fountain."; break;
		case 1: str = " almost bursts a vein in "+TPQP+" forehead "
			"from thinking to hard about what to wish for."; break;
		case 2: str = " scratches "+TPQP+" head with a dirt covered "
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
