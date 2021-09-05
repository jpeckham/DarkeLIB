//	/std/container.c

//	from the Nightmare mudlib

//	code inherited by all items which can hold things

//	created by Sulam@TMI for the TMI mudlib distribution

//	13 january 1992

// Bug with invisible objects fixed by Pallando 93-07-14



#include <std.h>



inherit OBJECT;



private nosave int internal_encumbrance;

int possible_to_close;

int is_closed;

int max_internal_encumbrance;



void set_max_internal_encumbrance(int max);

int query_max_internal_encumbrance();

int query_internal_encumbrance();

int receive_objects();

int add_encumbrance(int enc);



// Implement an object that can contain things.

// The 'remove()' function is implemented by the move.c object

// and will take care of any objects inside this object.



void set_max_internal_encumbrance(int max) {

    max_internal_encumbrance = max;

}



void set_possible_to_close(int pos) {

    possible_to_close = pos;

}



int query_internal_encumbrance() {

    return internal_encumbrance;

}



int query_max_internal_encumbrance() {

    return max_internal_encumbrance;

}



int toggle_closed() {

    if (possible_to_close) {

      if (is_closed) is_closed = 0;

      else is_closed = 1;

      return 1;

    }

    else return 0;

}



// This function is called from move()



int receive_objects() {

    if (is_closed) return 0;

    return 1;

}



int add_encumbrance(int enc) {

    if( !max_internal_encumbrance ) return 1;

    if( enc + internal_encumbrance > max_internal_encumbrance ) return 0;

    internal_encumbrance += enc;

    return 1;

}



string describe_living_contents(object *exclude) {

    object *inv;

    mapping list;

    string *shorts;

    string tmp, ret;

    int i, x;



  if(!exclude) exclude = ({});

    i = sizeof(inv = filter_array(all_inventory(this_object())-exclude,

      (: living :)));

    if(!i) return "";

    list = ([]);

    while(i--) {

	x = (int)previous_object()->query_skill("perception") + 

	  ((effective_light(previous_object()) - 2) * 8);

   	if(inv[i]->query_hiding() && skill_contest((int)inv[i]->query_hiding(),

						   x, 1) != 2 &&

	   !wizardp(previous_object()))

	  continue;

        if(!(tmp = (string)inv[i]->query_short())) {

            if(wizardp(inv[i]) || random(101)> (int)previous_object()->query_level()) continue;

            tmp = "a shadow";

        }

	if(inv[i]->query_invis() && !previous_object()->query("see invis")) 

	  continue;

        if(!tmp); else

        if(!list[tmp]) list[tmp] = ({ inv[i] });

        else list[tmp] += ({ inv[i] });

    }

    i = sizeof(shorts = keys(list));

    ret = "";

    while(i--) {

        if((x=sizeof(list[shorts[i]])) < 2) ret += shorts[i]+"\n";

        else ret += capitalize(consolidate(x, shorts[i]))+"\n";

    }

    return ret;

}



string describe_item_contents(object *exclude) {

    object *inv;

    mapping list;

    string ret, tmp;

    string *shorts;

    int i, x;



    i = sizeof(inv = filter_array((all_inventory(this_object())-exclude),

      "filter_non_living", this_object()));

    if(!i) return "";

    list = ([]);

    while(i--) {

        if(!(tmp = (string)inv[i]->query_short())) continue;

        if(tmp == "") continue;

        if(!list[tmp]) list[tmp] = ({ inv[i] });

        else list[tmp] += ({inv[i] });

    }

    i = sizeof(shorts = keys(list));

    if(!i) return "";

    if((x=sizeof(list[shorts[--i]])) == 1)

      ret = capitalize(shorts[i]);

    else ret = capitalize(consolidate(x, shorts[i]));

    if(!i) return (x <2 ? ret+" is here.\n" : ret +" are here.\n");

    else if(i==1)

      return ret+" and "+consolidate(sizeof(list[shorts[0]]), shorts[0])+

        " are here.\n";

    else {

        while(i--) {

            if(!i) ret += ", and ";

            else ret += ", ";

            ret += consolidate(sizeof(list[shorts[i]]), shorts[i]);

        }

    }

    return ret+" are here.";

}





int filter_living(object ob) { return living(ob); }

int filter_non_living(object ob) { return !living(ob); }



int query_closed() { return is_closed; }

