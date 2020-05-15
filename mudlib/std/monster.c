//  /std/monster.c
//  from the Nightmare mudlib
//  inheritable code for npc's
//
//  modified for use in DarkeLIB 0.1
//  Diewarzau 7/18/95
//  Modified by Misery 031096 to fix heart beat problem

#pragma optimize
#include <std.h>
#include <move.h>
#include <rooms.h>
#include <objects.h>
#include <daemons.h>
#include <party.h>
#define HEAL_SPELLS ({ "heal", "cure critical wounds", "cure serious wounds", "cure light wounds", "mass heal" })

inherit LIVING;

int fllw;
int combat_chance;
object tmp;
nosave int heart_beat_on;
int level;
nosave int speed;
nosave int moving;
mapping emotes, speech, spell_chance;
string body_type;
private nosave mixed __Die;
int casting_chance;
nosave mapping spell_delays;
string *combat_spells;

int test_heart();
string return_chat();
string return_achat();
void set_exp(int x);
void set_level(int x);
int query_level();
void chase(object plyr);
void move_around();
void set_emotes(int x, mixed *arr, int att);
void am_i_alive();
void extra_heart_beat(int tmp);
void do_chats(string *langs);
void do_emote(string pre);
void do_spell();
object put_money(int tmp_size, string *currs, object money);

void create() {
  ::create();
  emotes = ([]);
  speech = ([]);
  spell_chance = ([]);
  seteuid(getuid());
  this_object()->__INIT();
  set_property("ambidextry", 1);
  speed = 0;
}

void init() {
  mixed tmp;
  int fllw;
  int aggr;

  if(environment(this_object()))
    if(interactive(environment(this_object()))) {
      tell_object(environment(this_object()), "You mistakenly believed "
        "you picked it up.  It is actually on the ground.");
      move(environment(this_object()));
    }
  if(!heart_beat_on) set_heart_beat(heart_beat_on = 1);
  if((this_player()->query_invis() && !this_object()->query("see invis")) ||
    skill_contest((int)this_object()->
    query_skill("perception"),(int)this_player()->query_stealth(), 1) != 1)
    return;

  if(this_player()->query_ghost()) return;
  if(wizardp(this_player())) return;
  fllw = query("chase");
  tmp = query("aggressive");
  if(functionp(tmp))
    tmp = (*tmp)(this_player());
  else
    if(tmp) tmp = 1;
  if(living(this_player())) {
    if(tmp) {
      aggr = 1;
    }
    if(fllw) chase(this_player());
    if(aggr && (query("race") != this_player()->query_race()) ) 
      kill_ob(this_player(), 0);
  }
}

void chase(object plyr) {
   if( plyr==this_object()->query_following() ) return;
   plyr->add_follower(this_object());
   return;
}
   
void set_name(string str) {
  set_living_name(str);
  ::set_name(str);
  if(query_level() < 1) set_level(1);
}

varargs void heart_beat(int recurs_flag) {
  string *langs;
  string pre;
  int tmp;

  if(!recurs_flag) {
    remove_delayed_call("move_around");
    player_age += 2;
    ok_to_heal++;
    tmp = (int)this_object()->query_property("extra heart beat");
    if(tmp) extra_heart_beat(tmp);
  }
  else ok_to_heal--;
  if(player_age > ok_to_heal) {
    do_healing(calculate_healing());
  }
  else calculate_healing();
  adjust_exp();
  if(sizeof(langs = keys(speech))) do_chats(langs);
  if(!query_current_attacker()) pre = "";
  else pre = "attack ";
  if(emotes[pre+"chance"] > random(100)) do_emote(pre);
  if(mapp(spell_chance) && sizeof(keys(spell_chance))) do_spell();
  continue_attack();
  if(speed && moving >= speed) move_around();
  else if(speed) moving++;
  magic_round = 0;
  am_i_alive();
  if(!test_heart())
  {
    if(speed)
      delayed_call("move_around", speed*2);
    set_heart_beat(heart_beat_on=0);
  }
  return;
}

void restart_heart_beat() {
  set_heart_beat(1);
}

void am_i_alive() {
  if(this_object())
    if((int)this_object()->query_hp() < 0)
      if((object *)this_object()->query_attackers())
        if(!sizeof((object *)this_object()->query_attackers()))
          this_object()->die();
}

void extra_heart_beat(int tmp) {
  int i;

  tmp += 100;
  if(tmp <= 0) return;
  tmp = 20000/tmp;
  if((100*player_age)%tmp > 190) return;
  else if(tmp > 0) {
    i = 1;
    while(tmp > 100*i) {
      i++;
      this_object()->heart_beat(1);
    }
    if(tmp%100 != 0) {
      tmp = 20000/(tmp%100);
      if((100*player_age)%tmp <= 190) this_object()->heart_beat(1);
    }
  }
}

void do_emote(string pre) {
  mixed emotef;
  string emote;

  
  if(this_object()) {
    emotef = emotes[pre+"msg"][random(sizeof(emotes[pre+"msg"]))];
      if(functionp(emotef)) { emote = (*emotef)(); }
      else { emote = emotef; }
      if(emote) {
         message("environment", emote, environment(this_object()),
           ({ this_object() }) );
        }  
    }
}

void do_chats(string *langs) {
  string lang, pre;
  string *foo;

  if(!query_current_attacker()) pre = "";
  else pre = "attack ";
  lang = langs[random(sizeof(langs))];
  if(this_object()->query_property("random langs")){
    foo = this_object()->query_all_languages();
    this_object()->force_me("speak "+foo[random(sizeof(foo))]);
  }
  if(this_object() && random(100) < speech[lang][pre+"chance"])
    this_object()->force_me("say " +
      speech[lang][pre+"msg"][random(sizeof(speech[lang][pre+"msg"]))]);
}

int spell_filter(string spell) {
  if(!this_object()->query_spell_level(spell)) return 0;
  if(!spell_chance || !spell_chance[spell]) return 0;
  if(spell_chance[spell] != replace_string(spell_chance[spell], "$A", "") &&
    !query_current_attacker()) return 0;
  if(combat_spells && member_array(spell, combat_spells) >=0 &&
  !query_current_attacker()) return 0;
  if(spell_chance[spell+" delay"]) {
    if(!spell_delays) spell_delays = ([]);
    if(spell_delays[spell] && (time() - spell_delays[spell]) <
      spell_chance[spell + " delay"]) return 0;
    }
  if(((int)this_object()->query_max_hp() -
    (int)this_object()->query_hp()) > 20) return 1;
  if(member_array(spell, HEAL_SPELLS) >= 0 &&
    spell_chance[spell] != replace_string(spell_chance[spell], "$(ME)",""))
    return 0;
  return 1;
}

void do_spell() {
  string *spell_keys;
  object *random_dudes, spell_ob;
  string spell_choice, spell_file, cast_string;
  int i, z, max_pow, cast_chance;

  if(this_object())
    spell_keys = filter_array(keys(spell_chance), "spell_filter",this_object());
  z = sizeof(spell_keys);
  if(query_current_attacker()) cast_chance = combat_chance;
  else cast_chance = casting_chance;
  if(z && random(300) < cast_chance && !this_object()->query_casting()) {
    spell_choice = spell_keys[random(z)];
    spell_file = "/std/spells/"+
      replace_string(lower_case(spell_choice)," ","_");
    if(stringp(spell_chance[spell_choice]))
      cast_string = spell_chance[spell_choice];
    else cast_string = "";
    cast_string = replace_string(cast_string, "$(ME)",
                  lower_case((string)this_object()->query_name()));
    if(query_current_attacker())
    cast_string = replace_string(cast_string, "$A",
                  (string)(query_current_attacker()->query_name()));
    seteuid(getuid());
    if(file_exists(spell_file+".c")) spell_ob = new(spell_file);
    if(spell_ob) {
      random_dudes = filter_array(all_inventory(environment(
        this_object())), "not_party", spell_ob, this_object());
      if(random_dudes && (i = sizeof(random_dudes))) {
        i = random(i);
        cast_string = replace_string(cast_string, "$R",
                      (string)random_dudes[i]->query_name());
      }
      max_pow = (int)this_object()->query_spell_level(spell_choice);
      while(max_pow > 0 &&
        ((max_pow * (int)spell_ob->query_property("base mp cost")) > query_mp()))
        max_pow--;
      if(max_pow > 0) {
        spell_ob->begin_casting(this_object(), cast_string, max_pow);
        if(!spell_delays) spell_delays = ([]);
        spell_delays[spell_choice] = time();
      }
    }
  }
}

void die(object ob) {
  object money_ob;
  object *contents;
  int i, tmp_size;
  string *currs;

  if(functionp(__Die)) {
    if(!((int)((*__Die)(ob)))) return;
  }
  else
    if(stringp(__Die))
      message("other_action",__Die,environment(this_object()),({this_object()}));
    else message("other_action", query_cap_name()+
                 " drops %^RED%^dead%^RESET%^ before you.",
                 environment(this_object()),
                 ({ this_object() }));
  if(this_object()->query_property("no corpse"))
    tmp = new("/std/obj/no_corpse");
  else
    tmp = new("/std/obj/mon_corpse");
  tmp->set_name(query_cap_name());
  tmp->copy_body(this_object());
  tmp->move(environment(this_object()));
  tmp_size=sizeof(currs=query_currencies());
 
  if(tmp_size){ money_ob = new("/std/obj/coins");
	money_ob = put_money(tmp_size, currs, money_ob); 
   /* moved to function --parnell mar 05
    {
    
    for(i=0; i<tmp_size; i++) {
      money_ob->add_money(currs[i], query_money(currs[i]));
      add_money(currs[i], -query_money(currs[i]));
    }
   */
    money_ob->move(tmp);
  }
  contents = all_inventory(this_object());
  for(i=0;i<sizeof(contents);i++) {
    if(contents[i]->move(tmp)) 
      if(!contents[i]->remove())
        destruct(contents[i]);
  }
  remove();
}

int heal_filter(string spell) {
  return (member_array(spell, HEAL_SPELLS) >= 0);
}


  object put_money(int tmp_size, string *currs, object money) 
   {object money_ob;
    money_ob = new("/std/obj/coins");
    for(i=0; i<tmp_size; i++) {
      money->add_money(currs[i], query_money(currs[i]));
      add_money(currs[i], -query_money(currs[i]));
         }
  return money;
 }

int query_npc() { return 1; }

varargs void move_player(mixed dest, string message) {
  object prev;
  string here,going,temp1,temp2,temp3;
  prev = environment(this_object());
  here = file_name(prev);
  if (stringp(dest)) {
    if (sscanf(dest,"/%s",temp1)!=1) {
      sscanf(here,"/%s",temp1);
      going = "";
      while(sscanf(temp1,"%s/%s",temp2,temp3)==2) {
        going = going + "/"+temp2;
        temp1 = temp3;
      }
      temp1 = dest;
      if (file_size(going+"/"+temp1+".c") != -1) {
        dest = going+"/"+temp1;
      }
    }
  }
  if(move(dest) == MOVE_OK) {
    say(query_cap_name() + " arrives.\n", this_object());
    tell_room(prev, query_cap_name() + " leaves " + message + ".\n");
  }
}

varargs void add_spell(string spell, string cast_string, int combat, int delay) {
  if(!stringp(spell) || !stringp(cast_string)) return;
  if(!mapp(spell_chance)) spell_chance = ([]);
  spell_chance[spell] = cast_string;
  if(delay) spell_chance[spell+" delay"] = delay;
  if(combat) {
    if(!combat_spells) combat_spells = ({});
    combat_spells = distinct_array(combat_spells + ({ spell }));
  }
}

void set_casting_chance(int chance) {
  casting_chance = chance;
}

void set_combat_chance(int chance) {
  combat_chance = chance;
}

void move_around() {
  string *exits;

  moving = 0;
  if(!speed) return;
  if(!this_object()) return;
  if(environment(this_object()))
    exits = (string*)environment(this_object())->query_exits();
  else exits = 0;
  if(exits) command(exits[random(sizeof(exits))]);

}

void set_moving(int i) { moving = i; }

void set_speed(int i) { speed = i; }

void
__INIT()
{
  enable_commands();
  seteuid(getuid(this_object()));
  init_limb_data();
  init_stats();
  init_skills(0);
  init_living();
  set_heart_beat(heart_beat_on = 1);
  speed = 0;
  set_gender("neuter");
}

int kill_ob(object who, int which) {
  int i;
  object *inv;

  if(!heart_beat_on)
    set_heart_beat(heart_beat_on = 1);
  inv = all_inventory(environment());
  i = sizeof(inv);
  if(query("gang bang") && which !=2 &&(who->is_player() || who->is_pet())) {
    while(i--) {
      if(inv[i]->is_player() || inv[i]->is_pet()) continue;
      if(query_attackers() && member_array(inv[i], query_attackers()) >= 0) continue;
      inv[i]->kill_ob(who, 2);
    }
  }
  return ::kill_ob(who, which);
}

int query_num_rounds() {
  if(level) return (level > 30)?4:((level/10)+1);
  else return 1;
}

void set_level(int x) {
  int y;
  level = x;
  if(x >= 10 && x <= 20) set_skill("resist stun", (x-9)*5+10);
  else if(x > 20) set_skill("resist stun", 60 + 3*(x-20));
  set_overall_ac(5*x / 2);
if(x >= 20) set_overall_ac(6*x);
  if(x>10)
     set_skill("reverse stroke", (x-10)*2+5);
  if(x >= 10)
    set_property("enhance criticals", -1 * (x/10) );
  if(x >= 5) {
    set_skill("concentrate", 25 + x*5);
    set("concentrate", 1);
  }
  set_property("base hp regen", 5 * x);
  set_property("base mp regen", 5 * x);
set_skill("melee", 25 + (x*4) + random(5) );
  set_skill("perception", 15+(x*4) + random(10) );
  if(x >= 12) set("gang bang", 1);
  set_skill("attack", 20 + (x*4) + random(10) );
  set_skill("defense", (x*4) + random(10) );
  set_skill("blade", 30 + (x*4) + random(10) );
 if(x > 10) set_property("magic resistance", (x-10)*5);
  set_skill("knife", (40 + x*4) + random(10) );
  set_skill("blunt", 30 + (x*4) + random(10) );
  set_skill("two handed staff", 30 + (x*4) + random(10) );
  set_skill("projectile", 30 + (x*4) + random(10) );
  set_skill("two handed blade", 30 + (x*4) + random(10) );
  set_stats("constitution", 50 + x*2 + random(20) );
  set_stats("strength", 50 + x*3 + random(20) );
  set_stats("intelligence", 50 + x*2 + random(20) );
  set_stats("wisdom", 50 + x*2 + random(20) );
  set_stats("dexterity", 50 + x*3 + random(20) );
  set_stats("charisma", 50 + x*2 + random(20) );
  set_skill("dodge", 20 + x*4 + random(10) );
  set_skill("parry", 40 + x*4 + random(10) );
  set_property("melee damage", ([ "crushing" : 9 + 2*x, "impact" : 9 + 2*x ]));
  if(this_object()->is_pet())
    set_exp(x*1000);
  else
    if(x < 5) set_exp( (int)ADVANCE_D->get_exp(x) );
  else if(x < 10) set_exp( (int)ADVANCE_D->get_exp(x) - 
                           (int)ADVANCE_D->get_exp(x-1) );
  else set_exp(( (int)ADVANCE_D->get_exp(x) - 
                 (int)ADVANCE_D->get_exp(x-1) ) / 4);
  if(x <= 1) set_exp(2000+random(1000));
 y = 50 + (level *50) + random(50);
if(level>10) y+=40*level;
if(level>15) y+=40*level;
  if(level>20) y+=50*level;
  if(level>25) y+=50*level;
  set_max_hp(y);
  set_hp(y);
  set_max_mp(38 + (level * 150));
  set_mp(query_max_mp());
}

int query_level() { return level; }

// Added by Valodin, June 28, 1993
// Sets the body type to a certain race using the race daemon
void set_body_type(string str) {
  mapping monster_bod;
  int mag, max_mag;
  string *mon_limbs;
  int i, max;

  if(!str || !RACE_D->is_monster_race(str)) str = "human";
  mag = query_mp();
  max_mag = query_max_mp();
  init_limb_data();
  monster_bod = (mapping)RACE_D->monster_body(str, query_max_hp());
  for(i = 0, max = sizeof(mon_limbs = keys(monster_bod)); i < max; i++)
    add_limb(mon_limbs[i], monster_bod[mon_limbs[i]]["limb_ref"], 
      monster_bod[mon_limbs[i]]["max_dam"], 0, 0);
  set_wielding_limbs((string *)RACE_D->query_monster_wielding_limbs(str));
  set_fingers((int)RACE_D->query_monster_fingers(str));
  body_type = str;
  set_mp(mag);
  set_max_mp(max_mag);
}

string query_body_type()
{
  return body_type;
}

void set_exp(int x) {
  if( x > (int)ADVANCE_D->get_exp(level) )
    player_data["general"]["experience"] = (int)ADVANCE_D->get_exp(level);
  else player_data["general"]["experience"] = x;
}

// These two functions remain for backwards Nightmare 1.* and 2.* compat

void set_chats(int x, mixed *arr) { set_emotes(x, arr, 0); }

void set_achats(int x, mixed *arr) { set_emotes(x, arr, 1); }

void set_melee_damage(mixed what) {
  string *tmp;
  int i;

  if(!mapp(what)) {
    if(member_array(what,DAMAGE_TYPES) < 0) return;
    props["melee damage"] = ([ what : 10 ]);
    return;
  }
  tmp = keys(what);
  i = sizeof(tmp);
  while(i--) {
    if(member_array(tmp[i],DAMAGE_TYPES) < 0) map_delete(what, tmp[i]);
  }
  if(!mapp(props["melee damage"])) props["melee damage"] = ([]);
  props["melee damage"] += what;
}

mixed query_melee_damage() { return props["melee damage"]; }

void set_emotes(int x, mixed *arr, int att) {
  emotes[(att ? "attack chance" : "chance")] = x;
  emotes[(att ? "attack msg" : "msg")] = arr;
}

void set_speech(int x, string lang, string *arr, int att) {
  speech[lang] = ([ (att ? "attack chance" : "chance"): x,
    (att ? "attack msg" : "msg"): arr ]);
}

void set_alignment(int x) { player_data["general"]["alignment"] = x; }

string query_race() { return query("race"); }

int test_heart() {
  object env;
  object *inv;
  int i;

  if(query_current_attacker()) return 1;
  if((query_hp() < (query_max_hp()-5)) ||
    (query_mp() < (query_max_mp()-5))) return 1;
  if(!(env = environment())) return 0;
  i = sizeof(inv = all_inventory(env));
  while(i--) 
    if(interactive(inv[i]) || inv[i]->query("aggressive")) return 1;
  return 0;
}

int query_heart_status() { return heart_beat_on; }

void receive_message(string cl, string msg) {
  object from;
  string who, line, me;

  msg = eliminate_colour(msg);
  if(sscanf(msg, "%s tells you: %s", who, line) == 2) {
    if(from = find_player(lower_case(who))) {
      while(strlen(line) > 1 && line[0] == ' ')
        line = replace_string(line, " ", "", 1);
      this_object()->directed_message(from, line);
    }
  }
  else if(sscanf(msg, "%s says in %*s: %s,%s", who, me, line) == 4) {
    if((from = present(lower_case(who), environment())) &&
       (this_object() == present(lower_case(me), environment()) || plural_id(me))) {
      while(strlen(line) > 1 && line[0] == ' ')
        line = replace_string(line, " ", "", 1);
      this_object()->directed_message(from, line);
    }
  }
  else if(sscanf(msg, "%s yells in %*s: %s,%s", who, me, line) == 4
     && id(lower_case(me))) {
    from = find_player(lower_case(who));
    if(!from) from = present(lower_case(who), environment());
    if(!from) from = find_living(lower_case(who));
    if(from && (this_object() == present(lower_case(me), environment()) ||
       plural_id(me))) {
      while(strlen(line) > 1 && line[0] == ' ')
        line = replace_string(line, " ", "", 1);
      this_object()->directed_message(from, line);
    }
  }
  if(objectp(this_object()->query("tells to")))
    message("info", "%% "+msg, (object)this_object()->query("tells to"));
  this_object()->catch_tell(msg);
}

void set_languages(string *langs) {
  int i;

  i = sizeof(langs);
  while(i--) set_lang_prof(langs[i], 10);
}

void set(string str, mixed val) {
  mapping tmp_map;

  ::set(str, val);
  if(str == "race") {
    LANG_D->init_languages(this_object());
    if(stringp(val) && val == "undead") {
      tmp_map = (mapping)this_object()->query_property("enhance criticals");
      if(mapp(tmp_map)) tmp_map += ([ "holy" : 2 ]);
      else tmp_map = ([ "holy" : 2 ]);
      set_property("enhance criticals", tmp_map);
    }
  }
  return;
}

void set_die(mixed val) {
  __Die = val;
}

mixed query_die() { return __Die; }
