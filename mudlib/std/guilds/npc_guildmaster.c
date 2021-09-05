//    A generic npc guildmaster which is used if there is no pc guildmaster.  Hey will give permission
//    to anyone who asks to join as long as they are not a member of another guild.
//
//    DarkeLIB 0.1
//    -Diewarzau 8/8/95

#include <std.h>

inherit MONSTER;

private nosave string join_room;

void set_join_room(string str);

void set_join_room(string str) { join_room = str; }

void create() {
  ::create();
    if(!join_room && previous_object())
      join_room = base_name(previous_object());
    set_languages(({ "common" }));
    set_speech(5, "common", ({ "Hi.  Welcome to our guild.",
		       "Type 'say guildmaster, join' to get permission to join." }), 0);
    return;
}

void catch_tell(string str) {
  string tmp;

  if(sscanf(str, "%*s says something in %s", tmp)) {
    if(!random(2)) return;
    force_me("speak common");
    force_me("say I can't understand "+tmp);
    return;
  }
}

void directed_message(object who, string str) {
  if(str == "join" && who) {
    if(who->query_class() && (string)who->query_class() != "child") {
      tell_object(who, "Guildmaster tells you: Sorry, you already belong to a guild.");
      return;
    }
    if(!join_room) {
      tell_object(who, "Guildmaster tells you: Uh-oh, there is a bug in the joining room.");
      return;
    }
    join_room->set_member_status(lower_case((string)who->query_name()), "not joined");
    tell_object(who, "Guildmaster tells you: You may now join the guild.");
    return;
  }
  return;
}

