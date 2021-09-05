//    Focus casting/fast casting

#include <rooms.h>

inherit "/std/Object";

nosave object caster;
nosave string type;

void create() {
  ::create();
  set_name("soul");
  set_id(({ "focus ob" }));
  set_long("A focus soul");
  return;
}

void init() {
  ::init();
  add_action("fix_cast", "cast");
  return;
}

void set_caster(object ob) { caster = ob; }

void set_type(string str) { type = str; }

int fix_cast(string str) {
    string spell_dir, file, spell,arg;
    int pow;
    object spell_ob;

    if(this_player() != caster) return 0;
    if(this_player()->query_ghost()) {
        notify_fail("You cannot do that in an immaterial state.\n");
        return 0;
   }
    seteuid(getuid());
    if(!str || (sscanf(str,"*%d %s at %s",pow,spell,arg) != 3 &&
	sscanf(str,"*%d %s",pow,spell) != 2))
	return notify_fail("Usage: cast *<pow> <spell> at <target>[<args>]\n");
    if(!this_player()->query_spell_level(spell))
	return notify_fail("You do not know that spell.\n");
    spell = replace_string(spell," ","_");
    if(!wizardp(this_player()) || !file_exists(file = "/wizards/" +
	(string)this_player()->query_name() + "/spells/"+spell+".c"))
	file = "/std/spells/"+spell+".c";
    if(!file_exists(file))
	return notify_fail("BUG! You know a spell that doesn't exist!\n");
    if(this_player()->query_paralyzed())
	return notify_fail("You may not cast while stunned!\n");
    write("%^CYAN%^You employ %^RESET%^%^BOLD%^"+(type?type:"focus")+" casting%^RESET%^%^CYAN%^ to cast the spell...");
    spell_ob = clone_object(file);
    if(!spell_ob->query_property("base mp cost")) {
      message("info", "You are not allowed to cast 0 mp cost spells "+
        "with any augment casting skills.", this_player());
      return 1;
    }
    switch(type) {
    default:
    case "focus":
      spell_ob->set_property("base mp cost",
        (int)spell_ob->query_property("base mp cost") * 2);
      this_player()->add_skill_bonus((string)spell_ob->query_property("skill"),
        30);
delayed_call("remove_mod", (int)spell_ob->query_property("casting time")*2 +
        3, this_player(), (string)spell_ob->query_property("skill"));
      break;
    case "fast":
      spell_ob->set_property("casting time",
        (int)spell_ob->query_property("casting time") / 2);
      break;
    }
    spell_ob->begin_casting(this_player(), arg, pow);
    this_player()->set("focus ob", 0);
    this_player()->set("focus casting", 0);
    this_object()->move(ROOM_VOID);
    return 1;
}

void remove_mod(object tp, string skill) {
  if(!tp) {
    remove();
    return;
  }
  tp->add_skill_bonus(skill, -30);
  remove();
  return;
}
