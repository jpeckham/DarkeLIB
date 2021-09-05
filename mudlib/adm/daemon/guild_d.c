//    A generic inheritable object fro guild joining rooms.  Note that 
//    the player must have permission from the guildmaster to join.
//    NPC guildmasters will give permission to any guildless person.
//    Handles all necessary changes to the character, including
//    setting of class, mods of stats, etc.  The join room also 
//    saves itself so the guildmaster can have access to the total
//    guild membership.
//
//    DarkeLIB 0.1
//    -Diewarzau 8/8/95
//
//    Initiated a rewrite to daemonize many of the quild queries,
//    storage, and maintenance for the purpose of reducing cpu,
//    memory overhead and centralizing the function calls.
//    Zortek 4-15-96
    
#include <std.h>
#include <security.h>
// #include <guilds.h>

#define LEGAL_GUILDS ({"arch-mage","cavalier","chaotic-lord","cleric",\
"elementalist","enchanter","fighter","mindmage","necromancer","nightblade",\
"paladin","sword-mage","thief","tinker","war-priest"})

string *all_masters;
/* contains a list of all guild masters */

mapping guilds;
/* ([ guild0: ([
 * Guildmaster   "gm"    : "gm_name",
 * Join_room     "join"  : "/file_name",
 * Treasury      "treas" : "/file_name",
 * Stat Mods     "stats" : (["stat0":mod, "stat1":mod, "stat2":mod,...]),
 * Advance       "adv"   : (["hp":num,"mp":num,]),
 * Regen         "regen" : (["hp":num,"mp":num,]),
 *              ]),...]);
 */
mapping guests;
/* ([ guild0: ([ "name0":({ "room0", "room1", "room2",...}),...]),...]); */
mapping membership;
/* ([ guild0: ([
 * Allow Member  "allow"   :({"name0","name1","name2",...]),
 * All Members   "all"     :({"name0","name1","name2",...]),
 * New Members   "new"     :({"name0","name1","name2",...]),
 * Full Members  "full"    :({"name0","name1","name2",...]),
 * Elite Members "elite"   :({"name0","name1","name2",...]),
 * Punished      "punished":({"name0","name1","name2",...]),
 * Banish        "banish"  :({"name0","name1","name2",...]),
 * Ally          "ally"    :({"name0","name1","name2",...]),
 * Enemy         "enemy"   :({"name0","name1","name2",...]),
 * Money         "money"   :(["total":amt,"name0":amt,"name1":amt,...]),
 *             ]),...]);
 */

mapping privs;
/* ([ guild0: ([
 * Add Join      "a_join"   :({"name0","name1","name2",...}),
 * Del Join      "d_join"   :({"name0","name1","name2",...}),
 * Add punish    "a_punish" :({"name0","name1","name2",...}),
 * Del punish    "d_punish" :({"name0","name1","name2",...}),
 * Add banish    "a_banish" :({"name0","name1","name2",...}),
 * Del banish    "d_banish" :({"name0","name1","name2",...}),
 * Add ally      "a_ally"   :({"name0","name1","name2",...}),
 * Del ally      "d_ally"   :({"name0","name1","name2",...}),
 * Add enemy     "a_enemy"  :({"name0","name1","name2",...}),
 * Del enemy     "d_enemy"  :({"name0","name1","name2",...}),
 * Guest         "guest"    :({"name0","name1","name2",...}),
 * Money         "money"    :({"name0","name1","name2",...}),
 * Asst GM       "assist"   :({"name0","name1","name2",...}),
 *            ]),...]);
 */

mapping related_guilds;
/* ([ guild0:({ "relation0", "relation1",...}),...]); */

mapping guild_obj;
/* ([ guild0:file_name, guild1:file_name,...]); */

mapping last_on;
/* ([ guild0: time(), guild1: time(),...]); */
void save_guilds();

//----------BEGIN FUNCTIONS-------------
mapping query_all_privs() {
  if(!mapp(privs)) privs = ([]);
  return privs;
}
mapping query_guild_privs(string guild) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(privs)) privs = ([]);
  if(!mapp(privs[guild])) privs[guild] = ([]);
  return privs[guild];
}
string *query_privs(string guild, string what, string which) {
  string foo, bar;

  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(privs)) privs = ([]);
  if(!mapp(privs[guild])) privs[guild] = ([]);
  switch(which) {
    case "add": foo = "a_"; break;
    case "del": foo = "d_"; break;
    default:    foo = "";
  }
  switch(what) {
    case "join":
    case "punish":
    case "banish":
    case "ally":
    case "enemy":  bar = what; break;
    case "guest":
    case "money":  
    case "assist": bar = what; foo = ""; break;
    default: return ({});
  }
  if(undefinedp(privs[guild][foo+bar]))
    return ({});
  else return privs[guild][foo+bar];
}
string add_privs(string guild, string who, string what, string which) {
  string foo, bar;
  if(!which) which = "";
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(privs)) privs = ([]);
  if(!mapp(privs[guild])) privs[guild] = ([]);
  switch(which) {
    case "add": foo = "a_"; break;
    case "del": foo = "d_"; break;
    default:    foo = "";
  }
  switch(what) {
    case "join":
    case "punish":
    case "banish":
    case "ally":
    case "enemy":  bar = what; break;
    case "guest":
    case "money":  
    case "assist": bar = what; foo = ""; break;
    default: return 0;
  }
  if(undefinedp(privs[guild][foo+bar]))
    privs[guild][foo+bar] = ({ who });
  else if(member_array(who, privs[guild][foo+bar]) == -1)
    privs[guild][foo+bar] = distinct_array(privs[guild][foo+bar]+({ who }));
  log_file("guilds/"+guild+"/privs", ctime(time())+"--"+
           capitalize(who)+" was privilidged for "+which+
           " "+what+" by "+capitalize(this_player()->query_name())+".\n");
  return who;
}
string rm_privs(string guild, string who, string what, string which) {
  string foo, bar;

  if(!which) which = "";
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(privs)) privs = ([]);
  if(!mapp(privs[guild])) privs[guild] = ([]);
  switch(which) {
    case "add": foo = "a_"; break;
    case "del": foo = "d_"; break;
    default:    foo = "";
  }
  switch(what) {
    case "join":
    case "punish":
    case "banish":
    case "ally":
    case "enemy":  bar = what; break;
    case "guest":
    case "money":  
    case "assist": bar = what; foo = ""; break;
    default: return 0;
  }
  if(undefinedp(privs[guild][foo+bar]))
    return 0;
  if(member_array(who, privs[guild][foo+bar]) != -1) {
    privs[guild][foo+bar] -= ({ who });
    if(privs[guild][foo+bar] == ({}))
      map_delete(privs[guild], foo+bar);
    log_file("guilds/"+guild+"/privs", ctime(time())+"--"+
           capitalize(who)+" was DEprivilidged for "+which+
           " "+what+" by "+capitalize(this_player()->query_name())+".\n");
    return who;
  }
  return 0;
}
string *query_all_masters() {
  if(!all_masters) all_masters = ({});
  return all_masters;
}
mapping query_all_guilds() {
  if(!mapp(guilds)) guilds = ([]);
  return guilds;
}
mapping query_guild(string guild) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guilds)) guilds = ([]);
  if(undefinedp(guilds[guild])) guilds[guild] = ([]);
  return guilds[guild];
}
string query_gm(string guild) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guilds)) guilds = ([]);
  if(undefinedp(guilds[guild])) guilds[guild] = ([]);
  if(undefinedp(guilds[guild]["gm"]) ) return "none";
  return guilds[guild]["gm"];
}
string set_gm(string guild, string name) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guilds)) guilds = ([]);
  if(!mapp(guilds[guild])) guilds[guild] = ([]);
  if(member_array(guilds[guild]["gm"], all_masters) != -1)
      all_masters -= ({ guilds[guild]["gm"] });
  if(member_array(name, all_masters) == -1) all_masters = distinct_array(all_masters+({ name }));
  guilds[guild]["gm"] = name;
  save_guilds();
  return guilds[guild]["gm"];
}
void reset_gm(string guild) {

  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guilds)) guilds = ([]);
  if(!mapp(guilds[guild])) guilds[guild] = ([]);
  if(member_array(guilds[guild]["gm"], all_masters) != -1)
    all_masters -= ({ guilds[guild]["gm"] });
  map_delete(guilds[guild], "gm");
  save_guilds();
  return;
}
string query_join(string guild) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guilds)) guilds = ([]);
  if(!mapp(guilds[guild])) guilds[guild] = ([]);
  return guilds[guild]["join"];
}
string set_join(string guild, string join_room) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guilds)) guilds = ([]);
  if(!mapp(guilds[guild])) guilds[guild] = ([]);
  guilds[guild]["join"] = join_room;
  save_guilds();
  return guilds[guild]["join"];
}
string query_treas(string guild) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guilds)) guilds = ([]);
  if(!mapp(guilds[guild])) guilds[guild] = ([]);
  return guilds[guild]["treas"];
}
string set_treas(string guild, string treas_room) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guilds)) guilds = ([]);
  if(!mapp(guilds[guild])) guilds[guild] = ([]);
  guilds[guild]["treas"] = treas_room;
  save_guilds();
  return guilds[guild]["treas"];
}
mapping query_stats(string guild) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guilds)) guilds = ([]);
  if(!mapp(guilds[guild])) guilds[guild] = ([]);
  return guilds[guild]["stats"];
}
mapping set_stats(string guild, mapping stats) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guilds)) guilds = ([]);
  if(!mapp(guilds[guild])) guilds[guild] = ([]);
  guilds[guild]["stats"] = stats;
  save_guilds();
  return guilds[guild]["stats"];
}
mapping query_advance(string guild) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guilds)) guilds = ([]);
  if(!mapp(guilds[guild])) guilds[guild] = ([]);
  return guilds[guild]["adv"];
}
mapping set_advance(string guild, mapping stuff) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guilds)) guilds = ([]);
  if(!mapp(guilds[guild])) guilds[guild] = ([]);
  guilds[guild]["adv"] = stuff;
  save_guilds();
  return guilds[guild]["adv"];
}
mapping query_regen(string guild) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guilds)) guilds = ([]);
  if(!mapp(guilds[guild])) guilds[guild] = ([]);
  if(!guilds[guild]["regen"]) return 0;
  return guilds[guild]["regen"];
}
mapping set_regen(string guild, mapping stuff) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guilds)) guilds = ([]);
  if(!mapp(guilds[guild])) guilds[guild] = ([]);
  guilds[guild]["regen"] = stuff;
  save_guilds();
  return guilds[guild]["regen"];
}
mapping query_all_membership() {
  return membership;
}
int is_member(string guild, string name) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return -1;
  if(!mapp(membership)) membership = ([]);
  if(undefinedp(membership[guild])) membership[guild] = ([]);
  return member_array(name, membership[guild]["new"]);
}
int is_guest(string guild, string name) {
  string *foo;
  if(member_array(guild, LEGAL_GUILDS) == -1) return -1;
  if(!mapp(guests)) guests = ([]);
  if(undefinedp(guests[guild])) guests[guild] = ([]);
  foo = keys(guests[guild]);
  return member_array(name, foo);
}
mapping query_membership(string guild) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(membership)) membership = ([]);
  if(undefinedp(membership[guild])) membership[guild] = ([]);
  return membership[guild];
}
mixed query_membership_group(string guild, string what) {
  if(!guild || !what) return 0;
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(membership)) return 0;
  if(undefinedp(membership[guild])) return 0;
  if(undefinedp(membership[guild][what])) return 0;
  return membership[guild][what];
}
int query_specific_membership(string guild, string what, string who) {
  if(!guild || !what || !who) return -1;
  if(member_array(guild, LEGAL_GUILDS) == -1) return -1;
  if(!mapp(membership)) return -1;
  if(undefinedp(membership[guild])) return -1;
  if(!pointerp(membership[guild][what])) return -1;
  return member_array(who, membership[guild][what]);
}
string *query_guest_rooms(string guild, string who) {
  if(!guild || !who) return ({});
  if(member_array(guild, LEGAL_GUILDS) == -1) return ({});
  if(!mapp(guests)) return ({});
  if(undefinedp(guests[guild])) return ({});
  if(!pointerp(guests[guild][who])) return ({});
  return guests[guild][who];
}
string allow_member(string guild, string target) {
  if(!target || !guild) return 0;
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(membership)) membership = ([]);
  if(undefinedp(membership[guild])) membership[guild] = ([]);
  if(!undefinedp(membership[guild]["banish"]))
    if(!undefinedp(membership[guild]["banish"][target])) return 0;
  if(!pointerp(membership[guild]["allow"])) membership[guild]["allow"] = ({target});
  else membership[guild]["allow"] = distinct_array(membership[guild]["allow"]+({target}));
  log_file("guilds/"+guild+"/allow", ctime(time())+"--"+capitalize(target)+
          " was allowed by "+capitalize(this_player()->query_name())+".\n");
  save_guilds();
  return target;
}
void unallow_member(string guild, string target) {
  if(!target || !guild) return 0;
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(membership)) membership = ([]);
  if(!mapp(membership[guild])) membership[guild] = ([]);
  if(undefinedp(membership[guild]["allow"])) return;
  else membership[guild]["allow"] -= ({ target });
  log_file("guilds/"+guild+"/allow", ctime(time())+"--"+capitalize(target)+
          " was DISallowed by "+capitalize(this_player()->query_name())+".\n");
  save_guilds();
  return;
}
string punish_member(string guild, string target, string who) {
  if(!target || !guild || !who) return 0;
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(membership)) membership = ([]);
  if(!mapp(membership[guild])) membership[guild] = ([]);
  if(!pointerp(membership[guild]["punshed"]))
    membership[guild]["punished"] = ({ target });
  else membership[guild]["punished"] =
       distinct_array(membership[guild]["punished"]+({ target }));
  log_file("guilds/"+guild+"/punish", ctime(time())+"--"+capitalize(target)+
          " was punished by "+capitalize(who)+".\n");
  save_guilds();
  return target;
}
void unpunish_member(string guild, string target, string who) {
  if(!target || !guild || !who) return;
  if(member_array(guild, LEGAL_GUILDS) == -1) return;
  if(!mapp(membership[guild])) membership[guild] = ([]);
  if(!pointerp(membership[guild]["punshed"])) return;
  else membership[guild]["punished"] -= ({ target });
  log_file("guilds/"+guild+"/punish", ctime(time())+"--"+capitalize(target)+
          " was UNpunished by "+capitalize(who)+".\n");
  save_guilds();
  return;
}
string add_member(string guild, string what, string who) {
  if(!what || !guild || !who) return 0;
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(membership)) membership = ([]);
  if(!mapp(membership[guild])) membership[guild] = ([]);
  switch(what) {
    case "new":
    case "full":
    case "elite": break;
    default: return 0;
  }
  if(member_array(who, membership[guild][what] == -1)) {
    if(!pointerp(membership[guild][what])) membership[guild][what] = ({ who });
    else membership[guild][what] =
         distinct_array(membership[guild][what]+({ who }));
    log_file("guilds/"+guild+"/membership", ctime(time())+"--"+capitalize(who)+
          " became a "+capitalize(what)+" member by order of "+
          capitalize(this_player()->query_name())+".\n");
    save_guilds();
    return who;
  }
  return "error";
}
int purge_member(string guild, string who, string purger) {
  string *foo;
  int x,y;
  if(!guild || !who || !purger) return 0;
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  foo = ({"allow","all","new","full","elite","punished","banish","ally","enemy"});
  for(x=0,y=0;x<sizeof(foo);x++) {
    if(!pointerp(membership[guild][foo[x]])) continue;
      membership[guild][foo[x]] -= ({ who });
      log_file("guilds/"+guild+"/purge", ctime(time())+"--"+capitalize(who)+
             " was purged from "+capitalize(foo[x])+" by order of "+
             capitalize(purger)+".\n");
      y++;
  }
  if(!undefinedp(membership[guild]["money"])) {
    map_delete(membership[guild]["money"], who);
    log_file("guilds/"+guild+"/purge", ctime(time())+"--"+capitalize(who)+
             " was purged from the MONEY registry by order of "+
             capitalize(purger)+".\n");
  }
  if(!undefinedp(guests[guild][who])) {
    map_delete(guests[guild], who);
    log_file("guilds/"+guild+"/purge", ctime(time())+"--"+capitalize(who)+
             " was purged from the GUEST registry by order of "+
             capitalize(purger)+".\n");
  }
  save_guilds();
  return y;
}
string add_ally(string guild, string who) {
  if(!guild || !who) return 0;
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(membership)) membership = ([]);
  if(!mapp(membership[guild])) membership[guild] = ([]);
  if(!pointerp(membership[guild]["ally"]))
    membership[guild]["ally"] = ({ who });
  else membership[guild]["ally"] =
       distinct_array(membership[guild]["ally"]+({ who }));
  log_file("guilds/"+guild+"/ally", ctime(time())+"--"+capitalize(who)+
          " was made an ally by "+capitalize(this_player()->query_name())+".\n");
  save_guilds();
  return who;
}
void rm_ally(string guild, string who) {
  if(!guild || !who) return;
  if(member_array(guild, LEGAL_GUILDS) == -1) return;
  if(!mapp(membership[guild])) membership[guild] = ([]);
  if(!pointerp(membership[guild]["ally"])) return;
  else membership[guild]["ally"] -= ({ who });
  log_file("guilds/"+guild+"/ally", ctime(time())+"--"+capitalize(who)+
          " was REMOVED as an ally by "+capitalize(this_player()->query_name())+".\n");
  save_guilds();
  return;
}
string add_enemy(string guild, string who) {
  if(!guild || !who) return 0;
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(membership)) membership = ([]);
  if(!mapp(membership[guild])) membership[guild] = ([]);
  if(!pointerp(membership[guild]["enemy"]))
    membership[guild]["enemy"] = ({ who });
  else membership[guild]["enemy"] =
       distinct_array(membership[guild]["enemy"]+({ who }));
  log_file("guilds/"+guild+"/enemy", ctime(time())+"--"+capitalize(who)+
          " was declared an enemy by "+capitalize(this_player()->query_name())+".\n");
  save_guilds();
  return who;
}
void rm_enemy(string guild, string who) {
  if(!guild || !who) return;
  if(member_array(guild, LEGAL_GUILDS) == -1) return;
  if(!pointerp(membership[guild]["enemy"])) return;
  else membership[guild]["enemy"] -= ({ who });
  log_file("guilds/"+guild+"/enemy", ctime(time())+"--"+capitalize(who)+
          " was REMOVED as an enemy by "+capitalize(this_player()->query_name())+".\n");
  save_guilds();
  return;
}
int transact_money(string guild, string who, int amt) {
  if(!guild || !who || !amt) return 0;
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(membership)) membership = ([]);
  if(undefinedp(membership[guild])) membership[guild] = ([]);
  if(undefinedp(membership[guild]["money"])) membership[guild]["money"] = (["total":0,]);
  if(undefinedp(membership[guild]["money"][who])) {
    membership[guild]["money"][who] = amt;
  }
  else
    membership[guild]["money"][who] += amt;
  membership[guild]["money"]["total"] += amt;
  if(amt > 0)
    log_file("guilds/"+guild+"/money", ctime(time())+"--"+capitalize(who)+
             " deposited "+amt+".\n");
  else
    log_file("guilds/"+guild+"/money", ctime(time())+"--"+capitalize(who)+
             " WITHDREW "+amt+".\n");
  if(membership[guild]["money"][who] == 0)
    map_delete(membership[guild]["money"], who);
  save_guilds();
  return 1;
}
int add_money(string guild, string who, int amt, string what) {
  if(!guild || !who || !amt) return 0;
  if(amt < 0) return 0;
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(membership)) membership = ([]);
  if(undefinedp(membership[guild])) membership[guild] = ([]);
  if(undefinedp(membership[guild]["money"])) membership[guild]["money"] = (["total":0,]);
  membership[guild]["money"]["total"] += amt;
  log_file("guilds/"+guild+"/money", ctime(time())+"--"+capitalize(who)+
           " paid "+amt+" for "+what+".\n");
  save_guilds();
  return amt;
}
string *add_guest(string guild, string who, string room) {
  if(!guild || !who || !room) return 0;
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guests)) guests = ([]);
  if(undefinedp(guests[guild])) guests[guild] = ([]);
  if(undefinedp(guests[guild][who])) guests[guild][who] = ({ room });
  else
    guests[guild][who] =
      distinct_array(guests[guild][who]+({ room }));
  log_file("guilds/"+guild+"/guest", ctime(time())+"--"+capitalize(who)+
          " was granted guest access to "+room+" by "+capitalize(this_player()->query_name())+".\n");
  save_guilds();
  return guests[guild][who];
}
string *rm_guest(string guild, string who, string room) {
  if(!guild || !who || !room) return 0;
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guests)) guests = ([]);
  if(undefinedp(guests[guild])) guests[guild] = ([]);
  if(undefinedp(guests[guild][who])) 
    return ({});
  else
    guests[guild][who] -= ({ room });
  if(guests[guild][who] == ({}) ) {
    map_delete(guests[guild], who);
    return ({});
  }
  log_file("guilds/"+guild+"/guest", ctime(time())+"--"+capitalize(who)+
          " was FORBIDDEN guest access to "+room+" by "+capitalize(this_player()->query_name())+".\n");
  save_guilds();
  return guests[guild][who];
}
int delete_guest(string guild, string who) {
  if(!guild || !who) return 0;
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guests)) return 0;
  if(undefinedp(guests[guild])) return 0;
  if(undefinedp(guests[guild][who])) return 0;
  map_delete(guests[guild], who);
  log_file("guilds/"+guild+"/guest", ctime(time())+"--"+capitalize(who)+
          " was REMOVED as a guest by "+capitalize(this_player()->query_name())+".\n");
  save_guilds();
  return 1;
}
void set_related_guilds(string guild, string *relations) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(related_guilds)) related_guilds = ([]);
  related_guilds[guild] = relations;
  save_guilds();
  return;
}
void add_related_guild(string guild, string relation) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(member_array(relation, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(related_guilds)) related_guilds = ([]);
  if(!pointerp(related_guilds[guild]))
    related_guilds[guild] = ({ relation });
  else
    related_guilds[guild] =
      distinct_array(related_guilds[guild]+({ relation }));
  save_guilds();
  return;
}
void rm_related_guild(string guild, string relation) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(related_guilds)) related_guilds = ([]);
  if(!pointerp(related_guilds[guild]))
    return;
  else
    related_guilds[guild] -= ({ relation });
  save_guilds();
  return;
}
string *query_related_guilds(string guild) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(related_guilds) || !pointerp(related_guilds[guild])) return ({ });
  return related_guilds[guild];
}
void set_guild_obj(string guild, string file) {
  if(member_array(guild, LEGAL_GUILDS) == -1) return 0;
  if(!mapp(guild_obj)) guild_obj = ([]);
  guild_obj[guild] = file;
  save_guilds();
  return;
}
void create() {


  guilds = ([]);
  membership = ([]);
  related_guilds = ([]);
  guild_obj = ([]);
  all_masters = ({});
  privs = ([]);
  last_on = ([]);
  seteuid(UID_SECURE_DAEMONSAVE);
  if(file_exists("/adm/save/daemons/guild_d.o"))
    restore_object("/adm/save/daemons/guild_d",1);
  seteuid(getuid());
  return;
}
void reset() {
  seteuid(UID_SECURE_DAEMONSAVE);
//  save_object("/adm/save/daemons/guild_archive/"+time()+"backup");
  seteuid(getuid());
  return;
}
void save_guilds() {
  seteuid(UID_SECURE_DAEMONSAVE);
  save_object("/adm/save/daemons/guild_d");
  seteuid(getuid());
  return;
}
int query_last_on(string guild) {
  if(last_on[guild])
    return last_on[guild];
  else return 0;
}
int set_last_on(string guild, int num) {
  if(!mapp(last_on)) last_on = ([]);
  return last_on[guild] = num;
}
