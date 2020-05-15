/*  /d/damned/arena/booths_room.c

 *  Made for DarkeMUD

 *  Created by Geldron 031096

 *  Copyright (c) 1996, DarkeMUD

 *  Do not remove this header

 */



#include <daemons.h>

#include "arena.h"



inherit ROOM;



private nosave object challenger, challengee;

private nosave string challenger_oldenv, challengee_oldenv;

private nosave int count;

private nosave int fighting;



nomask int in_use();

nomask int ok_bet();

void accept(object ob);

void decline(object ob, int tmp);



void create() {

    ::create();

    set_property("indoors", 1);

    set_property("light", 2);

    set_property("night light", 1);

    set_property("no attack", 1);

    set_short("Arena");

    set_long("The complexity of this room is so immense, it is almost "

      "impossible for your incompetent mind to comprehend it.  A couple "

      "booths are setup for people to do several things, including "

      "answering the bets that people set, and asking to challenge "

      "someone.  The closest place to the arena, where you can see "

      "everything that happens resides south.  Another arena, bigger "

      "than the ones registered here is available, north.\n\nA small poster reads:\n\n"

      "		Welcome to the " + ARENA_NAME + "!");

    set_exits( ([

	"south" : "/d/damned/arena/listen_room",

	/*

		"north" : "/d/damned/arena/1,0",

	*/

	"east" : "/d/damned/virtual/room_12_12.world",

	"up" : "/d/damned/arena/master_room",

      ]) );

    add_pre_exit_function("up", "go_up");

    set_no_clean(1);

}



int go_up() {

    if(TP) {

	if(arena_ownerp(TP)) return 1;

	if(member_array(TPQN, "/d/damned/arena/master_room"->query_coowners())

	  != -1) return 1;

	message("arena", "A magical force prevents you from going up.", TP);

    }

    return 0;

}



void init() {

    ::init();

    add_action("challenge_user", "challenge");

    add_action("list_coowners", "co-owners");

}



int challenge_user(string arg) {

    object ob;

    string name;



    if(!arg) {

	message("challenge", syntax("challenge USER"), TP);

	return 1;

    }

    ob = find_player(arg);

    if(!ob) {

	message("challenge", arg + ": not logged in.", TP);

	return 1;

    }

    name = ob->query_cap_name();

    if(ob == TP) {

	message("challenge", "You cannot challenge yourself.", TP);

	return 1;

    }

    if(ob->query_ghost()) {

	message("challenge", "You can't challenge ghosts.", TP);

	return 1;

    }

    if(arenap(ob)) {

	message("challenge", name + " is busy fighting in the arena.", TP);

	return 1;

    }

    if((int)ob->query_challenged()) {

	message("challenge", name + " has already been challenged by "

	  "someone else.", TP);

	return 1;

    }

    if(in_use()) {

	message("challenge", "The arena is already in use.", TP);

	return 1;

    }

    if(ob->query_attackers() && sizeof(ob->query_attackers()) > 0) {

	message("challenge", name + " is in combat.", TP);

	return 1;

    }

    message("challenge", "You challenge " + name + " to a "

      "battle in the Arena!", TP);

    message("challenge", TPQCN + " challenges you to a battle "

      "in the Arena!", ob);

    message("challenge", "Type 'accept' to accept the challenge, or "

      "'decline to decline the challenge.  If within one minute you do not "

      "choose either, you'll automatically decline the challenge.", ob);

    ob->set_challenged(1);

    challenger = TP;

    CHAT_D->send_arena_message(TPQCN + " challenges " + name +

      " to a battle in the Arena!");

    count = 1;

    call_out("time_out", 60, lower_case(name));

    return 1;

}



int list_coowners(string arg) {

    write("The current co-owners are: " + implode(

	(string *)find_object("/d/damned/arena/master_room")->query_coowners(),

	", ") + ".");

    return 1;

}



void reset() {

    ::reset();

    if(!present("neon sign")) new("/d/damned/arena/neon_sign")

	->move(TO);

}



void time_out(string str) {

    if(count && find_player(str)) 

	decline(find_player(str), 1);

    else

	count = 0;

}



nomask int in_use() {

    if(challenger || challengee) return 1;

    return 0;

}



nomask int ok_bet() {

    if(in_use() && fighting && "/d/damned/arena/battle_room"

      ->query_waiting()) return 1;

    return 0;

}



object query_challenger() { return challenger; }



object query_challengee() { return challengee; }



void bet(int val, object ob, string str) {

    if(!ok_bet()) return;

    CHAT_D->send_arena_message(ob->query_cap_name() +

      " bets " + val + " platinum on " + str + ".");

}



void tally_bets(string msg) {

    if(BN(PO) != "/d/damned/arena/listen_room") return;

    CHAT_D->send_arena_message(msg);

}



void accept(object ob) {

    string *tmp;

    int i;



    CHAT_D->send_arena_message(ob->query_cap_name() +

      " accepts the challenge!");

    CHAT_D->send_arena_message("Bets can now be placed if you "

      "have permission.");

    CHAT_D->send_arena_message("The current betting modifier is at " +

      find_object("/d/damned/arena/master_room")->query_betting_modifier() +

      " * amount you bet = gained.");

    ob->set_challenged(0);

    message("arena", "Saving body...", ob);

    if((int)ARENA_SAVE_D->save(ob))

	message("arena", "Done.", ob);

    else message("arena", "Error!  Please notify Geldron.", ob);

    message("arena", "Saving body...", challenger);

    if((int)ARENA_SAVE_D->save(challenger))

	message("arena", "Done.", challenger);

    else message("arena", "Error!  Please notify Geldron.", challenger);

    challengee = ob;

    count = 0;

    challengee_oldenv = BN(ENV(challengee));

    challenger_oldenv = BN(ENV(challenger));

    challenger->new_body();

    challengee->new_body();

    challengee->set_hp(challengee->query_max_hp());

    challenger->set_hp(challenger->query_max_hp());

    challengee->set_mp(challengee->query_max_mp());

    challenger->set_mp(challenger->query_max_mp());

    challengee->move("/d/damned/arena/battle_room");

    challenger->move("/d/damned/arena/battle_room");

    challengee->force_me("unequip");

    challenger->force_me("unequip");

    fighting = 1;

    find_object("/d/damned/arena/battle_room")->begin_count(challenger,

      challengee);

}



void decline(object ob, int tmp) {

    if(tmp) CHAT_D->send_arena_message(ob->query_cap_name() +

	  " declines the challenge! (out of time)");

    else

	CHAT_D->send_arena_message(ob->query_cap_name() +

	  " declines the challenge!");

    ob->set_challenged(0);

    challenger = 0;

    count = 0;

}



void finish_battle(object ob) {

    string *tmp, *tmp2;

    int i;



    if(BN(PO) != "/d/damned/arena/battle_room") return;

    if(!ob || !challenger || !challengee || !challenger_oldenv || 

      !challengee_oldenv) return;

    CHAT_D->send_arena_message(ob->query_cap_name() + " lost the battle!\n" +

      challenger->query_cap_name() + " (" + challenger->query_hp() +

      "/" + challenger->query_max_hp() + "), " + challengee->query_cap_name() +

      " (" + challengee->query_hp() + "/" + challengee->query_max_hp() + ")");

    message("arena", "Restoring old body...", challenger);

    if((int)ARENA_SAVE_D->restore(challenger))

	message("arena", "Done.", challenger);

    else message("arena", "Error!  Please notify Geldron.", challenger);

    message("arena", "Restoring old body...", challengee);

    if((int)ARENA_SAVE_D->restore(challengee))

	message("arena", "Done.", challengee);

    else message("arena", "Error!  Please notify Geldron.", challengee);

    call_out("move_all", 0, challenger, challenger_oldenv);

    call_out("move_all", 0, challengee, challengee_oldenv);

    message("arena", "Thanks for using Geldron's arena!\nLet him know "

      "if you have any suggestions/comments/etc.", challenger);

    message("arena", "Thanks for using Geldron's arena!\nLet him know "

      "if you have any suggestions/comments/etc.", challengee);

    challenger_oldenv = 0;

    challengee_oldenv = 0;

    challenger = 0;

    challengee = 0;

    fighting = 0;

}



protected void move_all(object ob, string str) {

    ob->move_player(str);

}



nomask int query_prevent_shadow() { return 1; }

