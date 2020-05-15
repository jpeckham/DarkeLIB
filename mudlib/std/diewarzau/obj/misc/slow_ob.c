//    Shadow form object.c
#include <rooms.h>

inherit "/std/Object";

nosave string moving;

int drop() { return 1; }

void create() {
  ::create();
  set_short(0);
  moving = 0;
  set_long(0);
  return;
}

void init() {
  string *exits;
  int i;
  
  ::init();
  if(this_player() != environment() || !environment(this_player())) return;
  exits = (string *)this_player()->query("exits");
  if(!exits) return;
  i = sizeof(exits);
  while(i--) {
    add_action("slow_move", exits[i]);
  }
  return;
}

void reset_exits() {
  object prev;

  prev = environment();
  this_object()->move(ROOM_VOID);
  this_object()->move(prev);
  return;
}

int slow_move(string null) {
  string verb;
  object prev;
  
  verb = query_verb();
  if(!verb) return 0;
  if(moving) {
    write("You are already trying desperately to move "+moving+"!");
    return 1;
  }
  if(!environment(this_player())->query_exit(moving)) {
    write("That exit does not exist.");
    return 1;
  }
  moving = verb;
  write("You begin to s-l-o-w-l-y move "+moving+".");
  delayed_call("actually_move", 5, this_player());
  return 1;
}

void actually_move(object tp) {
  if(!tp) return;
  message("info", "You move "+moving+"...finally!", tp);
  tp->move_player((string)environment(tp)->query_exit(moving),
    "min");
  moving = 0;
  return;
}

