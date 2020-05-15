#include <std.h>

#include <security.h>

#include <save.h>



inherit DAEMON;



nosave mapping personal;

nosave mapping party;

mapping guild_pool;

int guild_total;



mapping query_all_guild_pool() { return guild_pool; }

mapping query_all_personal() { return personal; }

int query_guild_pool(string name) { return guild_pool[name]; }

int query_personal(string name) { return personal[name]; }

int query_guild_total() { return guild_total; }

int add_guild_pool(string name, int amt) {

  if(!guild_pool[name]) guild_pool[name] = amt;

  else guild_pool[name] += amt;

  guild_total += amt;

  return guild_pool[name];

}

int add_personal(string name, int amt) {

  if(!personal[name]) personal[name] = amt;

  else personal[name] += amt;

  return personal[name];

}



void create() {

  // Setup Variables

  personal = ([]);

  party =([]);

  guild_pool = ([]);

  guild_total = 0;

  // Restore from backup

  seteuid(UID_DAEMONSAVE);

  if(catch(restore_object(SAVE_SEER))) {

    rm("/daemon/save/seer_pool_d.o");

    cp("/daemon/save/seer_pool_d.bak", "/daemon/save/seer_pool_d.o");

    restore_object(SAVE_SEER);

  }

  call_out("age_pools", 60);

  seteuid(UID_DAEMONSAVE);

  save_object(SAVE_SEER);

  seteuid(getuid());

}



void age_pools() {

  string *names;

  int x,y;

  object who;

  

  if(!guild_pool) guild_pool = ([]);

  if(!sizeof(guild_pool)) return;

  guild_total = 0;

  names = keys(guild_pool);

  x = sizeof(names);

  if(!x) return;

  while(x--) {

    y = guild_pool[names[x]];

    if(y < 1) {

      map_delete(guild_pool, names[x]);

      who = find_living(names[x]);

      if(!who) continue;

      who->add_mp(y);

      who->add_hp(y);

      message("info", "You feel drained.", who);

    }

    else {

      guild_pool[names[x]] -= to_int(y/50);

      guild_total += guild_pool[names[x]];

    }

  }

  call_out("age_pools", random(60)+60);

}    

