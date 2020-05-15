/*  /d/damned/arena/battle_room.c

 *  Made for DarkeMUD

 *  Created by Geldron 031096

 *  Copyright (c) 1996, DarkeMUD

 *  Do not remove this header

 */



#pragma no_warnings



inherit ROOM;



private nosave object challenger, challengee;

private nosave int waiting;



void leaving();

void finish_battle(object ob);



void create() {

    ::create();

    set_property("indoors", 1);

    set_property("light", 2);

    set_property("night light", 1);

    set_property("no teleport", 1);

    set_property("no quit and save", 1);

    set_property("no summon", 1);

    set_property("no attack", 1);

    set_property("no magic", 1);

    set_short("Battle Room");

    set_long("Carcasses and bones are scattered around the battleground.  "

      "Blood and gore stills remains, a very apauling resource for the young "

      "fighters.");

    add_exit("/d/damned/arena/booths_room", "leave");

    add_pre_exit_function("leave", "leaving");

    set_no_clean(1);

}



void init() {

    ::init();

    add_action("no_can_do", "give");

    add_action("no_can_do", "drop");

    add_action("no_can_do_before", "cast");

}



int no_can_do(string arg) {

    message("arena", "%^RED%^You cannot do that in the arena.%^RESET%^", TP);

    return 1;

}



int no_can_do_before(string arg) {

    if(waiting) {

	message("arena", "%^RED%^You cannot do that in the arena.%^RESET%^", TP);

	return 1;

    } else return 0;

}



void reset() {

    ::reset();

    if(!present("announcer"))

	new("/d/damned/arena/announcer")->move(TO);

}



void clean_up() {

    return;

}



void begin_count(object ob1, object ob2) {

    challenger = ob1;

    challengee = ob2;

    message("arena", "%^BOLD%^%^RED%^You will be able to fight in 30 seconds!"

      "%^RESET%^", TO);

    message("arena", "%^RED%^It might be a good idea to get equipped in this "

      "short period of time.%^RESET%^\n%^RED%^Recommended commands: equip, wear, "

      "wield.%^RESET%^", TO);

    waiting = 1;

    call_out("allow_fighting", 30);

    return;

}



void allow_fighting() {

    message("arena", "%^MAGENTA%^You can now fight!%^RESET%^", TO);

    set_property("no attack", 0);

    set_property("no magic", 0);

    set_property("no summon", 0);

    waiting = 0;

    call_out("check_status", random(3) + 3);

}



void check_status() {

    if(!challenger || !challengee) return;

    message("arena", (string)challenger->query_cap_name() + "'s Hp: %^BOLD%^" +

      (int)challenger->query_hp() + "%^RESET%^", TO);

    message("arena", (string)challengee->query_cap_name() + "'s Hp: " +

      "%^BOLD%^" + (int)challengee->query_hp() + "%^RESET%^", TO);

    call_out("check_status", random(3) + 3);

}



void finish_battle(object ob) {

    "/d/damned/arena/booths_room"->finish_battle(ob);

    "/d/damned/arena/listen_room"->finish_battle(ob);

    if(present("announcer", TO))

	present("announcer", TO)->set_count(0);

    set_property("no attack", 1);

    set_property("no magic", 1);

    set_property("no summon", 1);

    challenger = 0;

    challengee = 0;

}



int query_waiting() { return waiting; }



nomask int query_prevent_shadow() { return 1; }



int leaving() {

    if(waiting) {

	message("arena", "You can't leave yet.", TP);

	return 0;

    }

    finish_battle(TP);

    return 0;

}

