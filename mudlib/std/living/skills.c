//	/std/living/skills.c
//	from the Nightmare mudlib
//	code for the Nightmare skill system
//	created by Descartes of Borg 1september 1992
//	recoded 14 february 1993

#include <daemons.h>
#include <clock.h>

string char_class;
mapping skills;
mapping spells;
nosave mapping skill_bonus;
int query_skill_bonus(string skill);

void init_skills(string cl);
void add_skill_points(string skill, int amount);
void reduce_skill_points(string skill, int amount);
void set_skill(string str, int x);
void set_last_use(string str);
int skill_exists(string str);
int query_max_skill(string str);
int query_skill(string str);
void set_class(string str);
string query_class();

void init_spells() { spells = ([]); }

void init_skills(string cl) {
    skills = (mapping)SKILLS_D->init_skills(cl);
}

void add_skill_points(string skill, int amount) {
return;
}

void reduce_skill_points(string skill, int amount) {
    return;
}

int query_max_skill(string skill) {
    if(!skills[skill]) return 0;
    else return skills[skill]["max"];
}

int skill_exists(string str) {
    string *vars;

    vars = keys(skills);
    if(str == "picking") return 0;
    if(member_array(str, vars) == -1) return 0;
    else return 1;
}

int spell_exists(string str) {
     if(!spells || undefinedp(spells[str])) return 0;
     return (member_array(str, keys(spells)) < 0)? 0:1;
}

int query_spell_level(string spell) {
    if(!spells || !spells[spell]) return 0;
    return (spells[spell]["level"] > 6)? 6:spells[spell]["level"];
}

int query_skill(string skill) {
    int x;

    if(origin() != "call_other") return (int)this_object()->query_skill(skill);
    if(!skills) {
      skills = ([]);
      return 0;
    }
    if(undefinedp(skills[skill])) return 0;
    x = query_skill_bonus(skill);
    x+= ((skills[skill]["level"] < 0)?0:skills[skill]["level"]);
    return x;
}

int query_base_skill(string skill) {
    if(!skills || !skills[skill]) return 0;
    else if(!skills[skill]["level"] ||
      skills[skill]["level"] < 0)
      return 0;
    else return skills[skill]["level"];
}

int query_base_spell(string spell) {
    if(!spells || !spells[spell]) return 0;
    else if(!spells[spell]["level"] ||
      spells[spell]["level"] < 0) return 0;
    else return spells[spell]["level"];
}

void set_spell_level(string spell, int x) {
    if(!spells) spells = ([]);
    if(!spells[spell]) spells[spell] = ([]);
    if(x < 0 || x > 6) return;
    spells[spell]["level"] = x;
    if(!this_object()->is_player() || wizardp(this_object())) return;
    if(previous_object() && inherits("/std/guilds/spell_room",
                                     previous_object())) return;
    if(previous_object() && base_name(previous_object()) ==
       "/d/standard/setter") return;
    if(strlen(file_name(previous_object())) > 17 &&
	arrange_string(file_name(previous_object()), 17) == "/d/damned/guilds/")
	return;
	  if(file_name(previous_object()) == "/d/damned/akkad/adv_guild_sp")
	return;

    log_file("illegal", (string)this_object()->query_name()+
      ":set_spell_level"+
      ":"+spell+"["+x+"]:"+
      (this_player() ? this_player()->query_name() : file_name(previous_object()))+
      ":"+file_name(previous_object())+
      ":"+getuid(previous_object())+":"+ctime(time())+"\n"
      );
}

void delete_spell(string spell) {
    if(!spells || !spells[spell]) return;
    if(member_array(spell, keys(spells)) < 0) return;
    map_delete(spells,spell);
}

void set_last_use(string str) {
    if(!str) return;
    if(!skills) skills = ([]);
    if(skills[str]) skills;
    skills[str]["last use"] = time();
    return;
}

void set_last_spell_dev(string str, int lev) {
  if(!spells || !spells[str]) return;
  if(!lev) return;
  if(!spells[str]["last dev"] || !pointerp(spells[str]["last dev"]))
    spells[str]["last dev"] = ({ 0, 0 });
  if(spells[str]["last dev"][0] < lev)
    spells[str]["last dev"][1] = 1;
  else spells[str]["last dev"][1] += 1;
  spells[str]["last dev"][0] = lev;
  return;
}

void set_last_dev(string str, int lev) {
  if(!lev) return;
  if(!skills || !skills[str]) return;
  if(!skills[str]["last dev"] || !pointerp(skills[str]["last dev"]))
    skills[str]["last dev"] = ({ 0, 0 });
  if(skills[str]["last dev"][0] < lev)
    skills[str]["last dev"][1] = 1;
  else skills[str]["last dev"][1] += 1;
  skills[str]["last dev"][0] = lev;
  return;
}

int query_spell_dev_times(string str, int lev) {
  if(!spells || !spells[str]) return 0;
  if(!spells[str]["last dev"])
    spells[str]["last dev"] = ({ 0,0 });
  if(lev > spells[str]["last dev"][0]) return 0;
  return spells[str]["last dev"][1];
}

int query_dev_times(string str, int lev) {
  if(!skills || !skills[str]) return 0;
  if(!skills[str]["last dev"])
    skills[str]["last dev"] = ({ 0,0 });
  if(lev > skills[str]["last dev"][0]) return 0;
  return skills[str]["last dev"][1];
}
  
int query_last_use(string str) {
  if(!str) return 0;
  if(!skills || !skills[str]) return 0;
  if(!skills[str]["last use"]) skills[str]["last use"] = 0;
  return skills[str]["last use"];
}

void set_skill(string str, int x) {
    string stat, file;

    if(!skills) skills = ([]);
    if(!skills[str]) skills[str] = ([ "level":0 ]);
    skills[str]["level"] = x;
    if(file_exists((file = "/std/skills/"+replace_string(str," ","_")+".c")))
	stat = (string)file->query_stat();
    if(stat && stringp(stat)) skills[str]["stat"] = stat;
    if(!this_object()->is_player() || wizardp(this_object())) return;
    if(previous_object() && arrange_string(file_name(previous_object()), 17) ==
      "/d/damned/guilds/") return;
    if(previous_object() && base_name(previous_object()) ==
       "/d/standard/setter") return;
    if(file_name(previous_object()) == "/d/damned/akkad/adv_guild_sk")
	return;
    if(file_name(previous_object()) == "/adm/daemon/advance_d")
	return;
    log_file("illegal", (string)this_object()->query_name()+
      ":set_skill"+
      ":"+str+"["+x+"]:"+
      (this_player() ? this_player()->query_name() : file_name(previous_object()))+
      ":"+file_name(previous_object())+
      ":"+getuid(previous_object())+":"+ctime(time())+"\n"
      );
}

void set_class(string str) { char_class = str; }

string query_class() { return char_class; }

void set_skill_stat(string skill, string stat) {
    if(!skills[skill]) return;
    if(!this_player()->query_stats(stat)) return;
    skills[skill]["stat"]=stat;
}

string query_skill_stat(string skill) {
    if(!skills[skill]) return 0;
    return skills[skill]["stat"];
}

protected void reduce_skills() {
    string *tmp_skills;
    int lvl, i;

    tmp_skills = keys(skills);
    lvl = (int)this_object()->query_level();
    if(lvl < 5) return;
    for(i=0;i<sizeof(tmp_skills);i++) {
	if(random(50) < lvl)
	    skills[tmp_skills[i]]["level"] -= random(3)+1;
	    if(skills[tmp_skills[i]]["level"] < 0)
		skills[tmp_skills[i]]["level"] = 0;
    }
return;
}

int add_skill(string type, int level, int max)
{
     string file;

     if(skill_exists(type)) return 0;
     skills[type] = (["level" : level, "points": 0, "max": max]);
     if(file_exists((file = "/std/skills/"+replace_string(type," ","_")+".c")))
	skills[type]["stat"] = (string)file->query_stat();
     return 1;
}

int delete_skill(string type)
{
     if(!skill_exists(type)) return 0;
     map_delete(skills, type);
     return 1;
}

int all_skills(string skill) {
  if(!skills) return 0;
  if(!skills[skill]) return 0;
  if(!skills[skill]["level"] < 0) return 0;
  return 1;
}

string *query_all_skills() {
  return filter_array(keys(skills), "all_skills", this_object());
}

string *query_all_spells() {
  if(mapp(spells)) return keys(spells);
  return ({});
}


void add_skill_bonus(string skill, int amount) {
    if(!skill_bonus) skill_bonus = ([]);
    if(!skill_bonus[skill]) skill_bonus[skill] = amount;
    else skill_bonus[skill] += amount;
    if(!skill_bonus[skill]) map_delete(skill_bonus, skill);
}

int query_skill_bonus(string skill) {
    int x;

    if(!skills[skill]) return 0;
    if(!skill_bonus && !skills[skill]["stat"]) return 0;
    x = 0;
    if(mapp(skills[skill]) && skills[skill]["stat"]) x+=
           ((int)this_object()->query_stats(
        skills[skill]["stat"]) - 40) / 8;
    if(!skill_bonus || !skill_bonus[skill]) x += 0;
    else x += skill_bonus[skill];
    if(intp(this_object()->query_property("luck")))
      x += (int)this_object()->query_property("luck");
    return x;
}








