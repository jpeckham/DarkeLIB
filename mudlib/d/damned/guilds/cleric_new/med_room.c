//    Meditation Room for Clerics, War Priests, Paladins
//
//    DarkeLIB 0.1
//    -Diewarzau 8/8/95
//    -Zortek

#include <std.h>

inherit ROOM;

nosave mapping skill_list;
nosave string join_room;

int calc_skill_inc(object who, string skill);
int dev_cost(object who, string skill);

void create() {
  ::create();
    skill_list = ([]);
}

void set_join_room(string str) { join_room = str; }

void set(string what, mixed arg) {
  if((what == "long" || what == "day long" || what == "night long") &&
     stringp(arg))
    arg += "\nType 'meditate <skill>' to find enlightenment in that skill.\n"+
      "Type 'list meditations' to list all meditations available to you.\n";
  ::set(what, arg);
  return;
}

void init() {
  ::init();
    add_action("train_skill", "meditate");
    add_action("list_skills", "list");
    return;
}

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
  if(environment(who) &&
     member_array("std/guilds/spell_room.c", deep_inherit_list(environment(who)))
     >= 0) return ::receive_objects();
  if(join_room->is_member((string)who->query_name())) return 1;
  if((string)join_room->query_member_status((string)who->query_name()) ==
     "allow train") {
    join_room->kick_member((string)who->query_name());
    return 1;
  }
  write(@TEXT
You have not been confirmed in the faith, and as such, must seek
a blessing from the Pope or an Arch Bishop in order for you to find
any enlightenment here.
TEXT
);
  return 0;
}

train_skill(string str) {
  int cost, my_dev, my_lev;

  if(!str) return 0;
  my_dev = (int)this_player()->query_property("dev points");
  my_lev = (int)this_player()->query_level();
  str = lower_case(str);
  if(!str) return 0;
  if(!skill_list[str]) {
    write("You meditate on "+str+" but find no enlightenment.");
    return 1;
  }
  if(skill_list[str]["level"] > my_lev) {
    write("You are unprepared for the enlightenment of "+str+".");
    return 1;
  }
  cost = dev_cost(this_player(), str);
  if(cost > my_dev) {
    write("You are insufficiently developed to gain enlightenment in "+str+".\n"+
	  "You only have "+my_dev+" divinity points.");
    return 1;
  }
  this_player()->add_dev(-1 * cost);
  this_player()->set_skill(str, (int)this_player()->query_base_skill(str) +
			   calc_skill_inc(this_player(), str));
  this_player()->set_last_dev(str, my_lev);
  write("You are enlightened to "+str+" in "+(int)this_player()->query_skill(str)+"%");
  write("It will cost you "+dev_cost(this_player(), str)+
        " divinity points for further enlightenment in "+str+".");
  write("You have "+sprintf("%d", (my_dev - cost)) + 
	" divinity points remaining.");
  return 1;
}
/* Zortek: 031996 Rewrote list_skills to eliminate the
 * need for file creation and removal via call_out.
 */  
int list_skills(string str) {
  int i, j, lev, skil;
  string *foo, *list, col;

  if(!str || str != "meditations") return 0;
  lev = this_player()->query_level();
  foo = ({"%^BOLD%^-=-=-=-=-=-=-=-=-=  %^BLUE%^Meditation Listing%^RESET%^BOLD%^  =-=-=-=-=-=-=-=-=-%^RESET%^"});
  if(member_array("bad standing", (string *)join_room->query_member_privs((string)this_player()->query_name())) >= 0)
    foo += ({ "%^RED%^%^BOLD%^-=-=-=-=-=-=-=-=-=-=  %^RESET%^%^BLUE%^Bad Standing%^RESET%^RED%^%^BOLD%^  =-=-=-=-=-=-=-=-=-=-%^RESET%^"});
  for(i=1;i <= lev; i++) {
    list = filter_array(keys(skill_list), "skill_filter", this_object(), i);
    if(!list || !sizeof(list)) continue;
    j = sizeof(list);
    foo += ({"%^BLUE%^Level "+sprintf("%2d",i)+" ------------------- Cost to Increase ---- Rating%^RESET%^"});
    while(j--) {
      skil = dev_cost(this_player(), list[j]);
      col = "%^BOLD%^GREEN%^";
      if(skil > this_player()->query_property("dev points") ) col = "%^RED%^";
      foo += ({ sprintf("    %-25s %s%-6d%%^RESET%%^ %6d%% %9d%%",
                list[j],
                col,
                skil,
			    calc_skill_inc(this_player(), list[j]),
			    (int)this_player()->query_skill(list[j])) });
    }
  }
  foo += ({"%^BOLD%^-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-%^RESET%^"});
  this_player()->more(foo);
  return 1;
}

int skill_filter(string skill, int lev) {
  if(!skill_list || !skill_list[skill]) return 0;
  return (skill_list[skill]["level"] == lev);
}

int dev_cost(object who, string skill) {
  string file;
  int times, mult;

  if(!who || !who->is_player()) return 0;
  if(!skill) return 0;
  if(!skill_list[skill] || !skill_list[skill]["secondary"])
    mult = 1;
  else mult = 2;
  if((string)who->query_class() != (string)join_room->query_class_name()) {
    if(member_array((string)who->query_class(), (string *)join_room->
		    query_related_guilds()) >= 0)
      mult *= 3;
    else mult *= 5;
  }
  if((int)who->query_base_skill(skill) >= 75) mult *= 2;
  if((int)who->query_base_skill(skill) >= 100) mult *= 2;
  if((int)who->query_base_skill(skill) >= 150) mult *= 2;
  if(member_array("bad standing", (string *)join_room->query_member_privs((string)this_player()->query_name())) >= 0)
    mult *= 3;
  file = "/std/skills/"+replace_string(skill, " ", "_")+".c";
  if(!file_exists(file)) return 0;
  file = replace_string(file, ".c", "");
  times = (int)who->query_dev_times(skill, (int)who->query_level());
  if(!times) return (mult * (int)file->query_dev());
  else return (mult * times * (int)file->query_fast_dev());
}

varargs void add_skill(string skill, int lvl, int flag) {
  skill_list[skill] = ([]);
  skill_list[skill]["level"] = lvl;
  if(flag) skill_list[skill]["secondary"] = flag;
  return;
}

int calc_skill_inc(object who, string skill) {
  int base;

  if(!who || !skill) return 0;
  base = (int)who->query_base_skill(skill);
  switch(base) {
  case 0..25:
    return 5;
  case 26..40:
    return 3;
  case 41..59:
    return 2;
  case 60..10000:
    return 1;
  }
  return 1;
}

