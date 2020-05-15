/*  /d/damned/arena/listen_room.c

 *  Made for DarkeMUD

 *  Created by Geldron 031096

 *  Copyright (c) 1996, DarkeMUD

 *  Do not remove this header

 */





inherit ROOM;



nosave mapping bet;



void create() {

    seteuid(getuid());

    bet = ([]);



    ::create();

    set_property("indoors", 1);

    set_property("light", 2);

    set_property("night light", 1);

    set_property("no attack", 1);

    set_short("Watching Room");

    set_long("The arena is clearly visible here.  Everything that takes "

      "place inside the arena can be seen from here.  There isn't much "

      "to this room, with its boring atmosphere.\n\nA green blob "

      "on the wall reads:\n\n"

      "You can bet in this room, but you must seek permission from Sagan and "

      "the co-owners of the arena.  For a list of them, go north and type "

      "\"co-owners\".  To bet, its as simple as typing \"bet <XXX> platinum "

      "on <challenger or challengee's name>\".\n");

    set_listen("default", "The very noisy room north has an effect on this room.");

    set_exits( ([

	"north" : "/d/damned/arena/booths_room",

      ]) );

    set_no_clean(1);

}



void init() {

    ::init();

    add_action("bet", "bet");

}



int bet(string arg) {

    object tmp1, tmp2;

    string betee;

    int val;



    sscanf(arg, "%d platinum on %s", val, betee);

    tmp1 = find_object("/d/damned/arena/booths_room")->query_challenger();

    tmp2 = find_object("/d/damned/arena/booths_room")->query_challengee();

    if(!"/d/damned/arena/booths_room"->ok_bet() || !tmp1 || !tmp2) {

	message("betting", "You cannot bet right now.", TP);

	return 1;

    }

    if(!(int)TP->query_bet_allowed()) {

	message("betting", "You must seek permission from Sagan or any "

	  "other co-owners.", TP);

	return 1;

    }

    if(!intp(val)) {

	message("betting", "Invalid bet.", TP);

	return 1;

    }

    if(val < "/d/damned/arena/master_room"->query_minimum_bet()) {

	message("betting", "The minimum bet is " + find_object(

	    "/d/damned/arena/master_room")->query_minimum_bet() + 

	  " platinum.", TP);

	return 1;

    }

    if(bet[TPQN] && bet[TPQN] != "") {

	message("betting", "Your bet is already set to " +

	  bet[TPQN] + " platinum.", TP);

	return 1;

    }

    if((int)TP->query_money("platinum") < val) {

	message("betting", "You do not have that much platinum.", TP);

	return 1;

    }

    TP->add_money("platinum", -(val));

    if(betee == (string)tmp1->query_name())

	TP->set_bet(tmp1);

    else if(betee == (string)tmp2->query_name())

	TP->set_bet(tmp2);

    else {

	message("betting", syntax("bet AMT platinum on FIGHTER"), TP);

	return 1;

    }

    if(!bet) bet = ([]);

    bet[TPQN] = val;

    TP->set_bet_allowed(0);

    message("betting", betee == (string)tmp1->query_name() ?

      "You bet " + val + " platinum on " + tmp1->query_cap_name() + "." :

      "You bet " + val + " platinum on " + tmp2->query_cap_name() + ".", TP);

    "/d/damned/arena/booths_room"->bet(val, TP, CAP(betee));

    write_file("/d/damned/arena/logs/bets",

      CAP(TPQN) + " betted " + val + " platinum on " +

      CAP(betee) + " at " + ctime(time()) + ".\n"

      "Level was at " + (int)TP->query_level() + " and " + CAP(betee) +

      "'s level was at " + (int)find_player(betee)->query_level() + ".\n");

    return 1;

}



void finish_battle(object ob) {

    object arg;

    string *kbets;

    int *vbets, i, tmp;



    kbets = keys(bet);

    vbets = values(bet);

    for(i = 0; i < sizeof(kbets); i++) {

	if((object)find_player(kbets[i])->query_bet() == ob)

	    "/d/damned/arena/booths_room"->tally_bets(

	      CAP(kbets[i]) + " lost the bet, losing " + vbets[i] +

	      " platinum!");

	else {

	    tmp = vbets[i] * (int)find_object("/d/damned/arena/master_room")->query_betting_modifier();

	    "/d/damned/arena/booths_room"->tally_bets(

	      CAP(kbets[i]) + " won " + tmp + " platinum!");

	    arg = new("/std/obj/coins");

	    arg->add_money("platinum", tmp);

	    arg->move(find_player(kbets[i]));

	}

    }

    bet = ([]);

}



nomask int query_prevent_shadow() { return 1; }

