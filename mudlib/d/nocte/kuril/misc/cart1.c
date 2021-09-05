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
		if(!wizardp(TP)) {
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
		new(MISC+"fruit1")->move(TO); 
		new(MISC+"fruit2")->move(TO);
		new(MISC+"fruit3")->move(TO);
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
		case 0: story = "\nA citizen purchases some good at the "
				"marketplace and quickly fades into the large "
				"crowd.\n";
			break;
		case 1: story = "\nSomeone brushes up against you...in this "
				"crowd it is impossible to tell who it is.  "
				"For some reason your money pouch feels abit "
				"lighter.\n";
				inv = all_inventory(TO);
                         for(i=0;i<sizeof(inv);i++){
				if(inv[i]->is_player()) {
					inv[i]->add_money("silver", -100);
				}
                                      }
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
