/*  /wizards/misery/arena/booths_room.c
 *  Made for DarkeMUD
 *  Created by Misery 031096
 *  Copyright (c) 1996, DarkeMUD
 *  Do not remove this header
 */

#include <std.h>
#include <daemons.h>
#include "arena.h"
#include "/wizards/misery/def.h"
#include "/wizards/misery/sefun.c"

inherit ROOM;

private nosave object challenger, challengee;
private nosave mapping challenger_limbs, challengee_limbs;
private nosave string *challenger_limbs2, *challengee_limbs2;
private nosave string challenger_oldenv, challengee_oldenv;
private nosave int count, challenger_oldhp, challenger_oldmp;
private nosave int challengee_oldhp, challengee_oldmp;
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
      "south" : "/wizards/misery/arena/listen_room",
      "north" : "/wizards/misery/arena/1,0",
      "east" : "/d/damned/virtual/room_12_12.world",
      "up" : "/wizards/misery/arena/master_room",
    ]) );
    add_pre_exit_function("up", "go_up");
    set_no_clean(1);
}

int go_up() {
    if(TP) {
	if(arena_ownerp(TP)) return 1;
	if(member_array(TPQN, "/wizards/misery/arena/master_room"->query_coowners())
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
    if(ob->query_challenged()) {
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
	(string *)find_object("/wizards/misery/arena/master_room")->query_coowners(),
	", ") + ".");
    return 1;
}

void reset() {
    ::reset();
    if(!present("neon sign")) new("/wizards/misery/arena/neon_sign")
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
    if(in_use() && fighting && "/wizards/misery/arena/battle_room"
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
    if(FN(PO) != "/wizards/misery/arena/listen_room") return;
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
      find_object("/wizards/misery/arena/master_room")->query_betting_modifier() +
      " * amount you bet = gained.");
    ob->set_challenged(0);
    challengee = ob;
    count = 0;
    challengee_oldenv = FN(ENV(challengee));
    challenger_oldenv = FN(ENV(challenger));
    challengee_oldhp = challengee->query_hp();
    challenger_oldhp = challenger->query_hp();
    challengee_oldmp = challengee->query_mp();
    challenger_oldmp = challenger->query_mp();
    tmp = (string *)challenger->query_limbs();
    challenger_limbs = ([]);
    challenger_limbs2 = allocate(sizeof(tmp));
    for(i = 0; i < sizeof(tmp); i++) {
	challenger_limbs[tmp[i]] = challenger->query_dam(tmp[i]);
	challenger_limbs2[i] = challenger->query_crippled(tmp[i]);
    }
    tmp = (string *)challengee->query_limbs();
    challengee_limbs = ([]);
    challengee_limbs2 = allocate(sizeof(tmp));
    for(i = 0; i < sizeof(tmp); i++) {
	challengee_limbs[tmp[i]] = challengee->query_dam(tmp[i]);
	challengee_limbs2[i] = challengee->query_crippled(tmp[i]);
    }
    challenger->new_body();
    challengee->new_body();
    challengee->set_hp(challengee->query_max_hp());
    challenger->set_hp(challenger->query_max_hp());
    challengee->set_mp(challengee->query_max_mp());
    challenger->set_mp(challenger->query_max_mp());
    challengee->move("/wizards/misery/arena/battle_room");
    challenger->move("/wizards/misery/arena/battle_room");
    challengee->force_me("unequip");
    challenger->force_me("unequip");
    fighting = 1;
    find_object("/wizards/misery/arena/battle_room")->begin_count(challenger,
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

    if(FN(PO) != "/wizards/misery/arena/battle_room") return;
    if(!ob || !challenger || !challengee || !challenger_oldenv || 
      !challengee_oldenv) return;
    CHAT_D->send_arena_message(ob->query_cap_name() + " lost the battle!\n" +
      challenger->query_cap_name() + " (" + challenger->query_hp() +
      "/" + challenger->query_max_hp() + "), " + challengee->query_cap_name() +
      " (" + challengee->query_hp() + "/" + challengee->query_max_hp() + ")");
    challenger->set_attackers( ({ }) );
    challengee->set_attackers( ({ }) );
    challenger->stop_hunting();
    challengee->stop_hunting();
    tmp = keys(challenger_limbs);
    tmp2 = values(challenger_limbs);
    for(i = 0; i < sizeof(tmp); i++) {
	challenger->set_dam(tmp[i], tmp2[i]);
	challenger->set_crippled(tmp[i], challenger_limbs2[i]);
    }
    tmp = keys(challengee_limbs);
    tmp2 = values(challengee_limbs);
    for(i = 0; i < sizeof(tmp); i++) {
	challengee->set_dam(tmp[i], tmp2[i]);
	challengee->set_crippled(tmp[i], challengee_limbs2[i]);
    }
    challenger->set_hp(challenger_oldhp);
    challengee->set_hp(challengee_oldhp);
    challenger->set_mp(challenger_oldmp);
    challengee->set_mp(challengee_oldmp);
    challenger->move(challenger_oldenv);
    challengee->move(challengee_oldenv);
    challenger_oldenv = 0;
    challengee_oldenv = 0;
    challenger_limbs = 0;
    challengee_limbs = 0;
    challenger_limbs2 = 0;
    challengee_limbs2 = 0;
    challenger_oldhp = 0;
    challengee_oldhp = 0;
    challenger_oldmp = 0;
    challengee_oldmp = 0;
    challenger = 0;
    challengee = 0;
    fighting = 0;
}

nomask int query_prevent_shadow() { return 1; }
