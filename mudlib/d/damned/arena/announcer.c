/*  /d/damned/arena/announcer.c

 *  Made for DarkeMUD

 *  Created by Geldron 031096

 *  Copyright (c) 1996, DarkeMUD

 *  Do not remove this header

 */





inherit MONSTER;



nosave int count;



void create() {

    ::create();

    set_name("announcer");

    set_id( ({ "announcer" }) );

    set_short("a short, stout announcer");

    set_long("The young man has a talent for being an announcer, with his "

      "manly and loud voice.  He certainly has an air of perfection, as if "

      "nothing could hurt him.");

    set_gender("male");

    set_body_type("humanoid");

    set("race", "human");

}



void catch_tell(string str) {

    if(find_object("/d/damned/arena/listen_room"))

	tell_room(find_object("/d/damned/arena/listen_room"), "Arena: " +

	  str);

    return;

}



int query_hp() {

    return 10000; 

}



void do_damage(string limb, int damage) {

    string tmp;



    count++;

    switch(count) {

    case 1: tmp = "I said, please stop."; break;

    case 2: tmp = "Will you please respect my request?"; break;

    case 3: tmp = "How dare you!"; break;

    case 4: tmp = "Shall I call the guards to come?"; break;

    case 5: tmp = "Sorry for overreacting, just don't do it again."; break;

    default: tmp = "Please stop."; break;

    }

    if(count > 4) count = 0;

    message("say", "Announcer says: %^CYAN%^" + tmp + "%^RESET%^",

      ENV(TO));

    return;

}



void set_count(int arg) {

    count = arg;

}



int query_count() { return count; }



int kill_ob(object who, int which) { error(""); }



object *query_attackers() { return ({ }); }



object *query_hunted() { return ({ }); }



nomask int query_prevent_shadow() { return 1; }

