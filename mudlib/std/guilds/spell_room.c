//    Spell training room for guilds.
//
//    DarkeLIB 0.1
//    -Diewarzau 8/8/95

#include <std.h>

inherit ROOM;

nosave mapping skill_list;
nosave string join_room;

int dev_cost(object who, string skill);
int prereq_check(object who, string str);

void create() {
  ::create();
    if(!query_property("train verb"))
      set_property("train verb", "train");
    if(!query_property("train noun"))
      set_property("train noun", "spells");
    skill_list = ([]);
}

void set_join_room(string str) { join_room = str; }

int is_spell_room() { return 1; }

void set(string what, mixed arg) {
  if((what == "long" || what == "day long" || what == "night long") &&
     stringp(arg))
    arg += "\nType '"+query_property("train verb")+" <spell>' to train your level in that skill.\n"+
      "Type 'list "+query_property("train noun")+"' to list all spells available to you.\n";
  ::set(what, arg);
  return;
}

void init() {
  ::init();
    if(!query_property("train verb")) set_property("train verb", "train");
    add_action("train_skill", query_property("train verb"));
    add_action("list_skills", "list");
    return;
}

/*
int receive_objects() {
  object who;

  who = previous_object();
  if(!who) return 0;
  if(!who->is_player()) return ::receive_objects();
  if(!join_room) {
    write("BUG!  Join_room not found");
    return 1;
  }
  if(wizardp(who) || !who->is_player()) return ::receive_objects();
  if(join_room->is_member((string)who->query_name())) return 1;
  if(environment(who) && member_array("std/guilds/skill_room.c", deep_inherit_list(environment(who)))
    >= 0) return ::receive_objects();
  if((string)join_room->query_member_status((string)who->query_name()) ==
     "allow train") {
    join_room->kick_member((string)who->query_name());
    return 1;
  }
  write("You must be either a member of this guild or have special permission from "+
	"the guildmaster to train here.");
  return 0;
}
*/

int train_skill(string str) {
  int cost, my_spell, my_dev, my_lev;
  object who;

  who=this_player();
  my_dev = (int)this_player()->query_property("dev points");
  my_spell = (int)this_player()->query_base_spell(str);
  my_lev = (int)this_player()->query_level();
  if(!str) return 0;
  str = lower_case(str);
  if(!str) return 0;
      if(((string)join_room->query_member_status((string)who->query_name())!= "member")&&!wizardp(who)){
    write("You are not a member of this guild, you can't train here.");
    return 1;
  }
  if(!skill_list[str]) {
    write("There is no such spell in this guild.");
    return 1;
  }
  if(skill_list[str]["level"] > my_lev) {
    write("That spell is not available at your level.");
    return 1;
  }
  if(my_spell >= 6) {
    write("You may only develop a spell to level *6.");
    return 1;
  }
  if(!prereq_check(this_player(), str))
  {
    write("You prerequisite spell is not high enough.");
    return 1;
  }
  cost = dev_cost(this_player(), str);
  if( !this_player()->query_property("UNPURE") )
    cost = cost * 9 / 10;
  if(cost > my_dev) {
    write("You need "+cost+" development points to train that spell.\n"+
	  "You only have "+my_dev+".");
    return 1;
  }
  this_player()->add_dev(-1 * cost);
  this_player()->set_spell_level(str, my_spell + 1);
  this_player()->set_last_spell_dev(str, my_lev);
  write("You now have "+str+" at *"+(int)this_player()->
	query_spell_level(str)+".");
  if((my_spell + 1) < 6)
    write("It will cost you "+dev_cost(this_player(), str)+
          " to advance this spell further.");
  write("You have "+sprintf("%d", (my_dev - cost)) +
        " development points left.");
  return 1;
}
/* Zortek: 031996 Rewrote list_skills to eliminate the
 * need for file creation and removal via call_out.
 */  
int list_skills(string str) {
  int i, j, lev, skil;
  string *list;
  string *foo, col;

     if(((string)join_room->query_member_status((string)this_player()->query_name()) != "member")&&!wizardp(this_player())){
    write("You are not a member of this guild, you can't train here.");
    return 1;
  }
  if(!str || str != (string)query_property("train noun")) return 0;
  lev = this_player()->query_level();
  foo = ({"%^BOLD%^-=-=-=-=-=-=-=-=-= %^BLUE%^SPELL LISTING%^RESET%^BOLD%^ =-=-=-=-=-=-=-=-=-%^RESET%^"});
  if(member_array("bad standing", (string *)join_room->query_member_privs((string)this_player()->query_name())) >= 0)
    foo += ({ "%^RED%^%^BOLD%^-=-=-=-=-=-=-=-=-=-=  %^RESET%^%^BLUE%^Bad Standing%^RESET%^RED%^%^BOLD%^  =-=-=-=-=-=-=-=-=-=-%^RESET%^"});
  for(i=1;i <= lev; i++) {
    list = filter_array(keys(skill_list), "skill_filter", this_object(), i);
    if(!list || !sizeof(list)) continue;
    j = sizeof(list);
    foo += ({"%^BLUE%^Level "+sprintf("%2d",i)+" ---------------------------- COST --- RANK%^RESET%^"});
    while(j--) {
      skil = dev_cost(this_player(), list[j]);
      col = "%^BOLD%^GREEN%^";
      if(skil > this_player()->query_property("dev points") ) col = "%^RED%^";
      foo += ({ sprintf("    %-33s %s%-6d%%^RESET%%^    *%-3d",
                   list[j],
                   col,
			       skil,
			       (int)this_player()->query_spell_level(list[j])) });
    }
  }
  foo += ({"%^BOLD%^-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-%^RESET%^"});
  this_player()->more(foo);
  return 1;
}

void delete_file(string file) {
  seteuid(getuid());
  rm(file);
  return;
}

int skill_filter(string skill, int lev) {
  if(!skill_list || !skill_list[skill]) return 0;
  return (skill_list[skill]["level"] == lev);
}

int dev_cost(object who, string skill) {
  string file;
  int spell_lev, times, mult, cost;
  int subt = (int)who->query_property("dev cost");

  if(!who || !who->is_player()) return 0;
  spell_lev = (int)who->query_base_spell(skill) + 1;
  if(!skill) return 0;
  if(spell_lev > 6) return 0;
  if(!skill_list[skill] || !skill_list[skill]["secondary"])
    mult = 1;
  else mult = 2;
  if((string)who->query_class() != (string)join_room->query_class_name()) {
    if(member_array((string)who->query_class(), (string *)join_room->
		    query_related_guilds()) >= 0)
      mult *= 3;
    else mult *= 5;
  }
  if(member_array("bad standing", (string *)join_room->query_member_privs((string)this_player()->query_name())) >= 0)
    mult *= 3;
  file = "/std/spells/"+replace_string(skill, " ", "_")+".c";
  if(!file_exists(file)) return 0;
  file = replace_string(file, ".c", "");
  times = who->query_spell_dev_times(skill, (int)who->query_level());
  if(!times) cost = (mult * spell_lev * (int)file->query_property("dev cost"));
  else cost = (spell_lev * mult * times * (int)file->query_property(
	       "fast dev cost"));
  if(subt < 0) cost += subt;
  if(cost < 1) cost = 1;
  return cost;
}

int prereq_check(object who, string str)
{
object spell;
string prereq;
spell = find_object("/std/spells/"+replace_string(str, " ", "_"));

if(!prereq = spell->query_property("prereq")) return 1;
if(who->query_spell_level(str) >= who->query_spell_level(prereq)) return 0;
else return 1;
}

string *query_all_spells() {
  if(!skill_list) return ({});
  return keys(skill_list);
}

varargs void add_spell(string skill, int lvl, int flag) {
  string file;

  file = "/std/spells/"+replace_string(skill, " ", "_") + ".c";
  if(!file_exists(file)) return;
  file = replace_string(file, ".c", "");
  skill_list[skill] = ([]);
  if(lvl) skill_list[skill]["level"] = lvl;
  else skill_list[skill]["level"] = (int)file->query_property("spell level");
  if(flag) skill_list[skill]["secondary"] = flag;
  return;
}
