//    Inheritable code for "pets."  Used for monsters which help players
//    out and may be commanded by them.
//
//    DarkeLIB 0.1
//    -Diewarzau 11/9/95

#include <daemons.h>
#include <move.h>
#include <std.h>
#include <rooms.h>
#include <dirs.h>

inherit MONSTER;
inherit "/std/locker_logic";

private string my_file;
string block_exit;
mapping Pet_data;
nosave mapping blocked;
nosave int saved_here = 1;

string *query_non_aggr();
void add_non_aggr(string who);
void delete_non_aggr(string who);
void set_save(int stat);
void set_attack(int stat);
void set_carry(int stat);
void set_aggr_status(int stat);
void set_owner(string who);
int query_aggr_status(object who);
void set_can_change_aggr(int stat);
string query_cap_name();

string query_cap_name() {
  if(!Pet_data["real name"]) return ::query_cap_name();
  else return capitalize(Pet_data["real name"]);
}

int save_me(string file) {
  saved_here = 1;
  return ::save_me(file);
}

    string query_owner() { return Pet_data["owner"]; }

string query_short() {
if(!Pet_data["name"]) return capitalize(article(::query_short()));
return capitalize(Pet_data["name"] + " the " + ::query_short());
}

int is_pet() { return 1; }

void add_exp(int amt) {
  object who;
  int i, lev;
  object *inv;

  if(Pet_data["owner"] && (who = find_player(Pet_data["owner"]))) {
    amt /= 2;
    lev = (int)this_object()->query_level();
    if(amt > 100) {
      inv = all_inventory(environment());
      lev = to_int(pow(to_float(lev), 2.3));
      i = sizeof(inv);
      while(i--) {
        if(inv[i] == this_object()) continue;
        if(inv[i]->is_pet() && (string)inv[i]->query_owner() ==
          (string)this_object()->query_owner())
          lev += to_int(pow(to_float((int)inv[i]->query_level()), 2.3));
      }
      lev = to_int(pow(to_float(lev), 1/2.3));      
    }
    if(lev > ((int)who->query_level() +1))
      amt = to_int(to_float(amt) / pow(2.0,
                   to_float(lev -
                   (1+(int)who->query_level()))));    
    who->add_exp(amt);
  }
  return;
}

void create() {
  ::create();
  Pet_data = ([]);
  blocked = ([]);
  Pet_data["attack"] = 1;
  this_object()->set_lang_prof("common", 10);
  set("aggressive", (: call_other, this_object(), "query_aggr_status" :) );
  set_storage_key("pet");
  set_skill("swimming", 100);
  return;
}

int receive_objects() {
  if(previous_object() && living(previous_object())) return 0;
  return ::receive_objects();
}

void set_can_change_aggr(int stat) {
  Pet_data["change aggr"] = stat;
  return;
}

void add_non_aggr(string who) {
  if(!Pet_data["non aggr"] ||
     !pointerp(Pet_data["non aggr"])) Pet_data["non aggr"] = ({});
  Pet_data["non aggr"] = distinct_array(Pet_data["non aggr"] + ({ who }) );
  return;
}

void delete_non_aggr(string who) {
  if(!Pet_data["non aggr"] ||
     !pointerp(Pet_data["non aggr"])) return;
  if(member_array(who, Pet_data["non aggr"]) < 0) return;
  Pet_data["non aggr"] -= ({ who });
  return;
}

string *query_non_aggr() {
  if(!Pet_data["non aggr"] ||
     !pointerp(Pet_data["non aggr"])) return ({});
  return Pet_data["non aggr"];
}

void set_aggr_status(int stat) {
  Pet_data["aggr status"] = stat;
  return;
}

int query_aggr_status(object who) { 
  if(!living(who)) return 0;
  if(wizardp(who)) return 0;
  if(!Pet_data["aggr status"]) return 0;
  if(who->is_pet() && Pet_data["owner"] == (string)who->query_owner()) 
    return 0;
  if(!who->is_player()) return 1;
  if((string)who->query_name() == Pet_data["owner"]) return 0;
  if(!Pet_data["non aggr"] || !pointerp(Pet_data["non aggr"])) return 1;
  if(member_array((string)who->query_name(), Pet_data["non aggr"]) >= 0)
    return 0;
  return 1;
}


void set_save(int stat) {
  Pet_data["save"] = stat;
  return;
}

void set_owner(string who) {
  Pet_data["owner"] = who;
  return;
}

void set_attack(int stat) {
  Pet_data["attack"] = stat;
  return;
}

void set_carry(int stat) {
  Pet_data["carry"] = stat;
  return;
}

void restore_locker(string file) {
  if(!file || !file_exists(DIR_OBJECTS+"/"+file+".o")) return;
  seteuid(geteuid(this_object()));
  restore_object(DIR_OBJECTS+"/"+file, 1);
  if(Pet_data["name"]) set_name(lower_case(Pet_data["name"]));
  return;
}

string query_my_file() {
  if(!Pet_data["save"]) return 0;
  return my_file;
}

void init() {
  ::init();
  if(block_exit) add_action("exit_func", block_exit);
  return;
}

int exit_func(string null) {
  string *b_keys, p_name;
  int a_flag;

  a_flag = 0;

  if(environment() && member_array(block_exit, (string *)environment()->
     query_exits()) < 0) return 0;
  if(query_attackers() && member_array(this_player(), query_attackers()) >= 0)
    a_flag = 1;
  b_keys = keys(blocked);
  if((string)this_player()->query_name() == Pet_data["owner"]) return 0;
  if(member_array((string)this_player()->query_name(),
	          query_non_aggr()) >= 0) return 0;
	if(this_player()->is_pet() && (string)this_player()->query_owner() ==
	  (string)this_object()->query_owner()) return 0;
	if(!a_flag && this_player()->query_invis() && !this_object()->query("see invis"))
	  return 0;
	else if(this_player()->query_invis() && !this_object()->query("see invis")) {
	  if(random(100) > ((int)this_object()->query_skill("perception")/3))
	    return 0;
	}
	if(this_player()->shadow_form() || this_player()->query_ghost()) return 0;
	p_name = (string)this_player()->query_name();
	if((member_array(p_name, b_keys) < 0 ||
	  blocked[p_name] < time()) && !a_flag) {
	  if(skill_contest((int)this_player()->query_stealth(),
	    (int)this_object()->query_skill("perception"), 1) == 1)
	    return 0;
	}
	blocked[p_name] = time() + 60 * (int)this_object()->query_level();
  write((string)this_object()->query_short() + " bars your way.");
  message("other_action", (string)this_object()->query_short() +
	  " bars "+(string)this_player()->query_cap_name()+
	  "'s way as "+nominative(this_player()) +
	  " tries to go "+query_verb()+".", environment(this_player()),
	  ({ this_player() }) );
  return 1;
}

int move(mixed dest) {
  int flag, res;
  object env;

  if(!environment()) {
    return ::move(dest);
  }
  if(environment()->query_is_locker_room()) {
    flag = 1;
    env = environment();
  }
  else flag = 0;
  if((res = ::move(dest)) == MOVE_OK && flag && saved_here) {
    SAVE_ITEMS_D->update_lockers(env);
  }
  if(res == MOVE_OK) saved_here = 0;
  return res;
}

int query_saved_here() { return saved_here; }

int query_is_locker() {
  if(Pet_data["save"]) return 1;
  else return 0;
}

void directed_message(object from, string mesg) {
  string tmp1, name,nom,who;
  object ob;
  string *tmp_p;
  int i;

  if((string)from->query_name() != Pet_data["owner"]) return;
  if(!Pet_data["name"]) name = (string)this_object()->query_cap_name();
  else name = capitalize(Pet_data["name"]);
  force_me("speak "+(string)from->query_primary_lang());
  if(!this_object()->query_primary_lang())
    force_me("speak common");
  if(parse_command(mesg, environment(), " 'move' / 'go' %s ", tmp1)) {
    if(!environment() || !environment()->query_exits() ||
      member_array(lower_case(tmp1), (string *)environment()->query_exits()) < 0) {
      force_me("say There is no such exit.");
      return;
    }
    this_object()->force_me(lower_case(tmp1));
    return;
  }
  if(parse_command(mesg, environment(), " 'get' / 'take' %s", tmp1)) {
    if(!Pet_data["carry"]) return;
    this_object()->force_me("get "+tmp1);
    return;
  }
  if(parse_command(mesg, environment(), " 'attack' / 'kill' %s", tmp1)) {
    ob = present(tmp1, environment());
    if(!Pet_data["attack"] || !ob || !living(ob)) return;
    if((string)ob->query_name() == Pet_data["owner"]) {
      message("info", (string)this_object()->query_cap_name() +
	      " refuses to attack "+ possessive(this_object()) +
	      " master.", environment());
      return;
    }
    message("other_action", (string)this_object()->query_cap_name() +
	    " attacks " + (string)ob->query_cap_name() + "!",
	    environment(), ({ ob }) );
    message("other_action", (string)this_object()->query_cap_name() +
	    " attacks you!", ob);
    this_object()->kill_ob(ob, 0);
    return;
  }
  if(sscanf(mesg, "drop %s", tmp1)) {
    if(!Pet_data["carry"]) return;
    this_object()->force_me(mesg);
    return;
  }
  if(sscanf(mesg, "give %*s")) {
    if(!Pet_data["carry"]) return;
    this_object()->force_me(mesg);
    return;
  }
  if(strlen(mesg) >= 4 && mesg[0..3] == "save") {
    if(!Pet_data["save"])
      return;
    if(!environment()->query_is_locker_room()) {
      this_object()->force_me("say This room will not save monsters.");
      return;
    }
    SAVE_ITEMS_D->update_lockers(environment());
    this_object()->force_me("say Okay.");
    saved_here = 1;
    return;
  }
  if(strlen(mesg) >= 5 && mesg[0..4] == "speak") {
    this_object()->force_me(mesg);
    return;
  }
  if(strlen(mesg) >= 8 && mesg[0..7] == "list non") {
    if(!Pet_data["change aggr"]) return;
    message("info", name + " whispers something to "+
	    (string)from->query_cap_name()+".", environment(),
	    ({ from, this_object() }) );
    if(!Pet_data["non aggr"] || !pointerp(Pet_data["non aggr"])) {
      message("info", "No players on non-aggressive list.", from);
      return;
    }
    message("info", "Non-aggressive list for "+name+
	    ":\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n"+
            format_page(query_non_aggr(), 2), from);
    return;
  }
  if(sscanf(mesg, "add player %s", tmp1)) {
    if(!Pet_data["change aggr"]) return;
    tmp1 = lower_case(tmp1);
    add_non_aggr(replace_string(tmp1, " ", ""));
    message("info", "Player "+capitalize(replace_string(tmp1, " ", "")) +
	    " added to non-aggressive list for "+
	    name + ".", from);
    if(environment(from) == environment())
      this_object()->force_me("say Done.");
    return;
  }
  if(sscanf(mesg, "remove player %s", tmp1)) {
    if(!Pet_data["change aggr"]) return;
    tmp1 = lower_case(tmp1);
    delete_non_aggr(replace_string(tmp1, " ", ""));
    message("info", "Player "+capitalize(replace_string(tmp1, " ", "")) +
            " removed from non-aggressive list for "+
            name + ".", from);
    if(environment(from) == environment())
      this_object()->force_me("say Done.");
    return;
  }
  if(strlen(mesg) >= 11 && mesg[0..10] == "toggle aggr") {
    if(!Pet_data["change aggr"]) return;
    if(Pet_data["aggr status"]) {
      message("info", name + " set to non-agressive.", from);
      this_object()->set_aggr_status(0);
    }
    else {
      this_object()->set_aggr_status(1);
      message("info", name + " set to agressive.", from);
    }
    if(environment(from) == environment())
      this_object()->force_me("say Done.");
    return;
  }
  if(sscanf(mesg, "name %s", tmp1)) {
    name = replace_string(tmp1, " ", "");
    if(tmp1 != name) {
      message("info", "There cannot be any spaces in a proper name.", from);
      return;
    }
    nom = lower_case(name);
    who = sprintf("%s/%s/%s", DIR_USERS, nom[0..0], nom);
    if( !(file_size(who + ".o") < 0)  ) {
      message("info","You may not name your pet after a player.\n",from);
      return;
    }
    tmp_p = this_object()->query_id();
    if(Pet_data["name"]) {
      while((i = member_array(lower_case(Pet_data["name"]), tmp_p)) >= 0)
      	tmp_p = exclude_array(tmp_p, i);
    }
    if(!Pet_data["real name"])
      Pet_data["real name"] = (string)this_object()->query_name();
    this_object()->set_id(distinct_array(tmp_p + ({ lower_case(name) })));
    Pet_data["name"] = name;
    set_name(lower_case(name));
    message("info", "Name changed to "+name+" for "+
	    query_cap_name() + ".", from);
    if(environment(from) == environment())
      this_object()->force_me("say Ok.");
    return;
  }
  if(strlen(mesg) >= 4 && mesg[0..3] == "help") {
    tmp1 = "Help for "+name+":\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
    tmp1 += "All commands must be tell'ed say'ed or yell'ed to the monster.\n"+
	"See 'help communication' for details on talking to NPC's.\n";
    tmp1 +=
"move <direction>     Move in specified direction\n" +
"name <proper-name>   Give the monster a unique proper name.\n"+
"speak <language>     Command the monster to speak a specific language,\n"+
"                     if known.\n"+
"follow               toggles follow on or off.  The monster must be in\n"+
"                     the same room as you to start following.\n"+
"set owner <who>      Transfers ownership of the monster.\n"
"stop hunting         Clears the pet's hunting list.  Similar\n"
"                     to the player command (see 'help stop').\n";
    if(Pet_data["attack"]) tmp1 +=
"attack <who>         If able, attack the specified person / monster.\n"+
"block exit <exit>    The monster will not allow anyone to exit the\n"+
"                     room by that direction.  Players may still enter\n"+
"                     from that direction, though.  Supplying no\n"+
"                     arguement will turn off the exit blocking.\n"+
"                     NOTE: The room need not have the exit supplied;\n"+
"                     the pet will continue to block AFTER it moves, \n"+
"                     until you tell it to stop.\n";
    if(Pet_data["carry"]) tmp1 +=
"give <what> to <who> These commands allow the monster to carry items\n"+
"get <item>           for the owner.  This can be useful for \n"+
"drop <item>          especially heavy items.\n";
    if(Pet_data["save"]) tmp1 +=
"save                 Saves the monster.  It will stay past reboots, but\n"+
"                     this only works in rooms which also save items, \n"+
"                     like inn rooms, etc.\n";
    if(Pet_data["change aggr"]) tmp1 +=
"toggle aggressive    Toggles the monster between aggressive and not.\n"+
"                     Aggressives will attack anyone not on the non-\n"+
"                     aggressive list (see below) on sight.  It will\n"+
"                     not attack the owner ever.\n"+
"list non-aggressive  Lists all players on the non-aggressive list for\n"+
"                     the monster.  The owner is not shown here.\n"+
"add player <who>     Adds <who> to the non-aggressive list.\n"+
"remove player <who>  Removes <who> from the non-aggressive list.\n";
    from->more(explode(tmp1, "\n"));
    if(environment(from) == environment())
      message("info", name + " whispers something to "+
	      (string)from->query_cap_name() + ".", environment(),
	      ({ from, this_object() }) );
    return;
  }
  if(sscanf(mesg, "set owner %s", tmp1)) {
    ob = find_player(tmp1);
    if(!ob) {
      message("info", "Could not find '"+tmp1+"'.", from);
      return;
    }
    if(environment() != environment(ob)) {
      message("info", "The new owner must be in the same room.", from);
      return;
    }
    i = 0;
    if(pointerp(from->query_followers())) {
    while(member_array(this_object(), (object *)from->query_followers()) >= 0) {
      from->remove_follower(this_object());
      i = 1;
    }
    }
    if(i)
      message("info", (string)this_object()->query_cap_name()+
        " stops following you.", from);
    set_owner((string)ob->query_name());
    message("info", "You have transferred ownership of "+name+" to "+
	    (string)ob->query_cap_name() + ".", from);
    message("info", "Ownership of "+name+" has been transferred to you.", ob);
    this_object()->set_lang_prof((string)ob->query_primary_lang(), 10);
    return;
  }
  if(mesg == "stop hunting") {
    this_object()->stop_hunting();
    force_me("say Okay.");
    return;
  }
  if(mesg == "block exit") {
    block_exit = 0;
    ob = environment();
    if(ob) {
      this_object()->move(ROOM_VOID);
      this_object()->move(ob);
    }
    force_me("say Okay.");
    return;
  }
  if(sscanf(mesg, "block exit %s", tmp1)) {
    if(!Pet_data["attack"]) return;
    if(member_array(tmp1, (string *)environment()->query_exits()) < 0)
      message("info", "Warning, there is no exit with the name '"+tmp1+"'.", from);
    block_exit = tmp1;
    ob = environment();
    if(ob) {
      this_object()->move(ROOM_VOID);
      this_object()->move(ob);
    }
    force_me("say Now blocking exit: "+tmp1);
    return;
  }
  if(strlen(mesg) >= 6 && mesg[0..5] == "follow") {
    if((object *)from->query_followers())
      i = member_array(this_object(), (object *)from->query_followers());
    else i = -1;
    if(i < 0) {
      if(environment() != environment(from)) {
	message("info", "You must be in the same room if you want "+name+
		" to follow you.", from);
      	return;
      }
      from->add_follower(this_object());
      message("info", name + " starts following you.", from);
      return;
    }
    else {
      message("info", name + " stops following you.", from);
      from->remove_follower(this_object());
      return;
    }
  }
  return;
}
