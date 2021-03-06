#include <std.h>

#include <security.h>



inherit DAEMON;



int cmd_snoopers(string str) {

    object *who;

    object snooper;

    int i, max;



    if(!archp(previous_object())) return 0;

    who = filter_array(users(), "filter_snoopers", this_object());

    if(!(max = sizeof(who))) {

        write("No one is snooping anyone.\n");

        return 1;

    }

    for (i = 0; i < sizeof(who); i++) {

        seteuid(UID_SNOOP);

	snooper = query_snoop(who[i]);

        seteuid(getuid());

        if(member_group(geteuid(snooper), "superuser") &&

          !member_group(geteuid(previous_object()), "superuser")) continue;

        write(capitalize((string)who[i]->query_name())+" is being snooped by "+capitalize((string)snooper->query_name())+".\n");

    }

    return 1;

}



int filter_snoopers(object ob) {

    object tmp;



    seteuid(UID_SNOOP);

    tmp = query_snoop(ob);

    seteuid(getuid());

    return !(!tmp);

}



void help() {

  write( @EndText

Syntax: snoopers

Effect: Lists users on with who they are snooping.

See also: snoop, snoopable

EndText

  );

}

