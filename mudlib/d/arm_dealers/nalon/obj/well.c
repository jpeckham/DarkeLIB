#include <std.h>
#include <move.h>
#include <well.h>
inherit "/std/Object";

nosave int TELL;

int check_living(object *inv);


void create() {
	::create();
        set_name("well");
        set_id(({ "well" }) );
        set_short("%^YELLOW%^An old well%^MAGENTA%^%^BOLD%^");
	set_long(

"The monks well, it has served as there water source "
"since the founding of there village.  While old and "
"decreped, it also serves as the centerstone of the village.");
	set_property("duration", 600);
        TELL = 1;
        call_out("story_telling", DELAY);
}

void init() {
  	::init();
  	add_action("fill_vessel", "fill");
        add_action("drink_water", "drink");
        add_action("drink_well", "drink");
	add_action("splash_water", "splash");
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
               case 0: story = "\nA tired monk comes in, "
                               "weary from the exertion of "
                               "training, he takes a long drink "
                               "and sits down.\n";
			break;
               case 1: story = "\nA pretty young woman enters, "
                               "most likely the wife of a monk. "
                               "She proceeds to fill a bucket, "
                               "then go back the way she came "
                               "without so much as a glance in "
                               "your direction.\n";
			break;
               case 2: story = "\nA group of young monks arrive "
                               "bantering and practicing the "
                               "latest moves they learned on "
                               "each other.  After a quick "
                               "stop for a drink, they go along "
                               "thier way happily.\n";
			break;
               case 3: story = "\nA wisened old monk walks in "
                               "slowly, his bones seem to be "
                               "creaking with age.  He stiffly "
                               "bends over to get a drink, "
                               "disgruntled that no one did "
                               "it for him, he moves on.\n";
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

int drink_well(string str) {
	string drink;
	int count;
	if(!str) {
		notify_fail("What do you want to drink?\n\n");
		return 0;
	}
        if( !(str == "well") ) {
		notify_fail("What do you want to drink?\n\n");
		return 0; 
	}
	if(TP->query_quenched() > 420) {
		message("info", "You'll blow up if you drink another drop!", 
			TP);
		return 1;
	}
        message("info", "You take a long drink of water from the well. "
               "You feel very refreshed.", TP);
	TP->add_quenched(-1000);
	count = 0;
	while( (TP->query_quenched() < 450) && (count < 90 ) ) {
		TP->add_quenched(5);
		count += 1;
	}
        switch(random(3)) {
                case 0: drink = " shoves "+(string)TP->query_possessive()+
			" face into the "
                        "well and takes a long drink of water.";
			break;
		case 1: drink = " takes a drink of water from the "
                        "well.";
			break;
		case 2: drink = " drinks so fast that water flies out "
			"of "+(string)TP->query_possessive()+" nose!";
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
                write(CAP(str)+ " is not here at the well.");
		return 1;
	}
	write("You splash "+CAP(str)+" in the face!");
	say(TPQCN+" splashes water at "+CAP(str)+"!");
	return 1;
}

int get(){
  return 0;
}
