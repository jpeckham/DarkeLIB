//      /adm/daemon/player_d.c
//      from the Nightmare Mudlib
//      a daemon to keep track of player progress
//      created by Descartes of Borg 10 july 1993

#include <objects.h>
#include <save.h>
#include <security.h>

string *monsters;
mapping *player_list;

void create() {
    seteuid(getuid());
    monsters = ({});
    player_list = ({});
    seteuid(UID_DAEMONSAVE);
    restore_object(SAVE_PLAYER_LIST);
    seteuid(getuid());
}

mapping *query_player_list() { return player_list; }

void add_player_info() {
    int x;
    object ob;
    int i;

    if(!(ob = previous_object()) || !userp(ob)) return;
    if(wizardp(ob)) {
      i = sizeof(player_list);
      while(i--)
        if(player_list[i]["name"] == (string)ob->query_name() && wizardp(ob))
          player_list -= ({ player_list[i] });
      return;
    }
    x = -1;
    i = sizeof(player_list);
    while(i--) if(player_list[i]["name"] == (string)ob->query_name()) x = i;
    if((sizeof(player_list) == 20) && (x == -1) &&
      (((int)ob->query_exp()) < player_list[19]["experience"]))
        return;
    if(x > -1) player_list -= ({ player_list[x] });
    player_list += ({ ([ "name":(string)ob->query_name(), "experience":(int)ob->query_exp(),
      "kills":sizeof((string *)ob->query_kills()),
      "deaths": sizeof((mixed *)ob->query_deaths()),
      "quests": sizeof((string *)ob->query_quests()),
      "major accomplishments": sizeof((mixed *)ob->query_mini_quests()),
      "level": (int)ob->query_level(),
      "class": (string)ob->query_class(),
      "guild": (string)ob->query_guild()
    ]) });
    player_list = sort_array(player_list, "sort_list", this_object());
    if(sizeof(player_list) > 50) player_list = player_list[0..49];
    seteuid(UID_DAEMONSAVE);
    save_object(SAVE_PLAYER_LIST);
    seteuid(getuid());
}

int sort_list(mapping alpha, mapping beta) {
    if(alpha["experience"] > beta["experience"]) return -1;
    else if(alpha["experience"] < beta["experience"]) return 1;
    else return strcmp(alpha["name"], beta["name"]);
}

int add_kill(string str) {
    int x;

    if(!stringp(str) || (base_name(previous_object()) != OB_USER &&
      geteuid(previous_object()) != UID_ROOT)) return -1;
    if(!monsters) monsters = ({});
    if((x=member_array(str, monsters)) == -1) {
        monsters += ({ str });
        x = member_array(str, monsters);
        seteuid(UID_DAEMONSAVE);
        save_object(SAVE_PLAYER_LIST);
        seteuid(getuid());
    }
    return x;
}

string query_monster(int x) { return monsters[x]; }
