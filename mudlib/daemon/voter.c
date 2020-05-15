/*  /std/obj/voter.c
 *  Made for DarkeMUD
 *  Created by Geldron 031796
 *  Copyright (c) 1996
 *  Do not remove this header
 */

inherit DAEMON;


private nosave mapping vote;

int clean_up() {return 1;}
void create() { 
    ::create();
/*
    set_no_clean(1);
*/
}

void set_votes(mapping mp) {
    if(base_name(PO) != "/cmds/adm/_setvote" &&
      base_name(PO) != "/cmds/adm/_tally") return;
    vote = mp;
}

void set_vote(string str, int val) {
    if(!vote) vote = ([]);
    if(base_name(PO) != "/cmds/mortal/_yes" &&
      base_name(PO) != "/cmds/mortal/_no") return;
    vote[str] = val;
}

int query_vote(string str) {
    if(!vote) return 0;
    return vote[str];
}

mapping query_votes() {
    if(!vote) return ([]);
    return vote;
}

int ok_vote() {
    if(!vote) return 0;
    if(sizeof(keys(vote))) return 1;
    return 0;
}

nomask int query_prevent_shadow() { return 1; }
