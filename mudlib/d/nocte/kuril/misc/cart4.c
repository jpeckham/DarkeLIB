// Coder: %^GREEN%^nEv!N%^RESET%^
// Darkemud
// storage object for kuril merchants

#include <std.h>
#include <move.h>
#include <nevin.h>

#define OBJ this_player()->query_objective()
#define AMOUNT 10
#define DELAY random(120) + 120

inherit "/std/bag_logic";

nosave int TELL;

//status query_storage_locker() { return 1; }

void init() {
	::init();
	if(query_night()) {
		if(!wizardp(this_player())) {
			TO->move(ROOMS+"incinerator");
		}
	}
	add_action("push_cart", "push");
	add_action("open_cart", "open");
	if(!TELL) {
		TELL = 1;
		call_out("story_telling", DELAY);
	}
}

void create() {
	int i;
    ::create();
    set_name("cart");
    set_id( ({ "cart", "merchant cart" }) );
    set_long("It is a cart used by the merchants of Kuril.");
    set_value(0);
    set_weight(1000000);
    set_short("%^YELLOW%^%^BOLD%^A merchant's cart%^MAGENTA%^%^BOLD%^");
    set_max_internal_encumbrance(30000);
    set_possible_to_close(1);
	for(i=0;i<AMOUNT;i++) { 
		new(WEAP+"dagger_belt")->move(TO); 
	}
	toggle_closed();
	TELL = 1;
	call_out("story_telling", DELAY);
    return;
}

void story_telling() {
	object *inv;
	int i;
	string story;
	switch(random(2)) {
		case 0: story = "\nA citizen buys something from one of the "
				"many merchants in the marketplace.\n";	
			break;
		case 1: story = "\nA travelling priest begins to preach "
				"the to the crowd at the marketplace.  Most "
				"of the onlookers simply laugh at the man "
				"who is promptly dragged off by a group of "
				"city watchman.\n";
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
int push_cart(string str) {
	if(!str) { return 0; }
	if(str == "cart") {
		if(present("merchant")) {
			message("info", "As you reach for the cart handles "
				"the merchant slaps your hands away.", TP);
			message("info", "A merchant slaps "+TPQCN+"'s hands "
				"away as he notices "+TPQCN+" trying to make "
				"off with his cart.", ENV(TP), ({TP}) );
		}
		if(!present("merchant")) {
			message("info", "You try to push the cart but it "
				"is just too heavy for you!", TP);
			message("info", TPQCN+" tries to walk off with a "
				"merchant's cart, but it is just too heavy "
				"for "+OBJ+".", ENV(TP), ({TP}) );
		}
	}
	return 1;
}

int open_cart(string str) {
	if(!str) { return 0; }
	if(str == "cart") {
		if(!wizardp(TP)) {
			message("info","The cart is closed shut with an "
				"incredibly strong force, it is unlikely "
				"that you will be able to get it open.", TP);
			return 1; 
		}
	}
}

int move(mixed dest) {
  if(!objectp(dest)) return ::move(dest);
  if(living(dest) && !wizardp(dest)) return MOVE_NOT_ALLOWED;
  return ::move(dest);
}

int clean_up() { return 0; }
