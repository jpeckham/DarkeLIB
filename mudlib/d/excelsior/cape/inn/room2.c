// /wizards/excelsior/cape/inn/room1
//	Inn rooms created by excelsior
//	Players can save their stuff there.

#include <std.h>
#include <security.h>
#include <daemons.h>
#define INN_ROOM "/wizards/excelsior/cape/inn/entrance"
#define ROOM_NUM 2

inherit "/std/vault_locker_room";

nosave int has_error;

void re_create(string arg);

int query_is_locker_room() {
  if(has_error) return 0;
  return 1;
}

void init() {
  ::init();
  if((string)this_player()->query_name() == (string)
     INN_ROOM->query_owner(file_name(this_object()))) {
     add_action("change_desc", "describe");
     add_action("revert","revert");
  }
  add_action("pre_open", "open");
}

int pre_open(string str) {
  if(str) set_locked(str, 0);
  return ::OpenDoor(str);
}

int revert() {
	string owner, file;
	write("Reverting room description.");
	owner = (string)INN_ROOM->query_owner("/wizards/excelsior/cape/inn/room"+ROOM_NUM);
    if(owner && file_exists(file = ("/open/inn_descs/cape_"+owner)))
    rm(file);
	re_create("room"+ROOM_NUM);
}

void virtual_setup(string arg) {
  string room, file;
  int room_num;
  string hall_exit, owner;

  if(!sscanf(arg, "room%s", room)) {
    write("BUG in the inn server! not room");
    has_error = 1;
    return;
  }
  has_error = 0;
  set("short", "Inn room #"+ROOM_NUM);
  set("long", "This is really a nice quaint little place you have "+
  	"come upon.  While you stay here, you may customize "+
	"the room description, and that description will save for all "+
	"further stays in the inn.  You may also store chests, safes, "+
	"etc. in here for your items.  Also, if you're worried about "+
	"thieves, guards and certain monsters will save here over "+
	"reboots.\n"+
        "To customize the description, type 'describe room'.\n"+
        "To revert to this room description, type 'revert'.");
  if(!sscanf(room, "%d", room_num)) {
    write("BUG in the inn server! Room not an int");
    return;
  }
  owner = (string)INN_ROOM->query_owner("/wizards/excelsior/cape/inn/room"+ROOM_NUM);
  seteuid(getuid());
  if(owner && file_exists(file = ("/open/inn_descs/cape_"+owner)))
    set("long", read_file(file));
  set_door("door "+ROOM_NUM, "/wizards/excelsior/cape/inn/hall", "out", "cape key "+ROOM_NUM);
  set_lock_level("door "+ROOM_NUM, 15);
  add_exit("/wizards/excelsior/cape/inn/hall","out");
  set_items(([ "door" : "It appears to be a nice, sturdy door." ]));
  return;
}

void re_create(string arg) {
  this_object()->create();
  this_object()->virtual_setup(arg);
  return;
}

int change_desc(string str) {
  string file;
  
  if(str != "room") return 0;
  write("Enter your desired description (10 lines max).\n"+
	"When you are finished, type a period on a blank line.");
  write("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
  seteuid(getuid());
  file = "/open/inn_descs/cape_"+(string)this_player()->query_name();
  if(file_exists(file)) rm(file);
  printf("] ");
  input_to("add_line", 0, 0, file);
  return 1;
}

void add_line(string str, int num, string file) {
  if(num >= 10 || str == ".") {
    write("Finished!");
    set("long", read_file(file));
    return;
  }
  write_file(file, str + "\n");
  printf("] ");
  input_to("add_line", 0, num+1, file);
  return;
}

void create() {
  ::create();
    set_property("indoors", 1);
    set_property("light", 2);
    virtual_setup("room"+ROOM_NUM);
    return;
}











