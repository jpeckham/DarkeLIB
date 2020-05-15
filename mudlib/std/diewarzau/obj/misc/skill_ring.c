//    A skill/spell ring.

#include <move.h>

inherit "/std/armour";

mapping skills;
mapping spells;
nosave object shadow;

void create() {
  ::create();
  skills = ([]);
  spells = ([]);
  set_short("An enchanter's ring");
  set_long(
  "This ring is used to receive imbued skills an spells.  When worn, it can "
  "give the wearer extra skills and spells.  To see what skills and spells are "
  "stored on this ring, type 'help <ring>', ex: help ring 3.");
  set_weight(2);
  set_value(96000);
  set_name("ring");
  set_type("ring");
  set_ac(1);
  set_limbs(({ "right hand", "left hand", "first branch",
    "second branch", "third branch", "fourth branch" }));
  set_id(({ "ring", "enchanter ring", "enchanters ring", "enchanter's ring" }));
  set_wear((: call_other, this_object(), "do_skills" :));
  set_remove((: call_other, this_object(), "remove_skills" :));
  return;
}

int query_active() {
  if((!mapp(spells) || !sizeof(keys(spells))) &&
    (!mapp(skills) || !sizeof(keys(skills)))) return 0;
  return 1;
}

void init() {
  ::init();
  add_action("list_skills", "help");
  return;
}

void set_skill(string skill, int lev) {
  if(!skills) skills = ([]);
  skills[skill] = lev;
  return;
}

void set_spell_level(string spell, int lev) {
  if(!spells) spells = ([]);
  spells[spell] = lev;
  return;
}

int list_skills(string str) {
  string *sk_keys;
  string tmp;
  int i;
  
  if(present(str, this_player()) != this_object()) return 0;
  tmp = "Skills:\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
  if(!skills || !sizeof(keys(skills))) tmp += "none.\n\n";
  else {
    sk_keys = keys(skills);
    i = sizeof(sk_keys);
    while(i--) sk_keys[i] = sprintf("%-33s %3d%%", sk_keys[i], skills[sk_keys[i]]);
    tmp += format_page(sk_keys, 1) + "\n";
  }
  tmp += "Spells:\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
  if(!spells || !sizeof(keys(spells))) tmp += "none.\n\n";
  else {
    sk_keys = keys(spells);
    i = sizeof(sk_keys);
    while(i--) sk_keys[i] = sprintf("%-35s *%1d", sk_keys[i], spells[sk_keys[i]]);
    tmp += format_page(sk_keys, 1) + "\n";
  }
  this_player()->more(explode(tmp, "\n"));
  return 1;
}

int do_skills() {
  object ob;
  int i;
  string *s_keys;

  if((!skills || !sizeof(keys(skills))) &&
    (!spells || !sizeof(keys(spells)))) return 1;
  seteuid(getuid());
  ob = new("/std/spells/shadows/skill_shadow");
  if(skills && sizeof(s_keys=keys(skills))) {
    i = sizeof(s_keys);
    while(i--)
      ob->add_skill(s_keys[i], skills[s_keys[i]]);
  }
  if(spells && sizeof(s_keys=keys(spells))) {
    i = sizeof(s_keys);
    while(i--)
      ob->add_spell(s_keys[i], spells[s_keys[i]]);
  }
  ob->start_shadow(this_player());
  shadow = ob;
  return 1;
}

int remove_skills() {
  if(!shadow) return 1;
  shadow->external_destruct(shadow);
  return 1;
}

int move(mixed dest) {
  int res;

  if(!environment() || !environment()->is_player()) return ::move(dest);
  res = ::move(dest);
  if(res == MOVE_OK) remove_skills();
  return res;
}

