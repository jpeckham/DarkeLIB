//    A Generic player castle room.  Reads a text file into the map for
//    the castle itself.
#include <daemons.h>
#include <security.h>
#include <rooms.h>
#define DIR_CASTLE "/d/damned/data/castles"
#define DOORS ([ "wood door" : 2, "iron door" : 5, "steel door" : 8, \
   "vault door" : 16, "reinforced door" : 11 ])
#define COMPASS ([ "north" : "south", "south" : "north", "east" : "west", \
    "west" : "east", "up" : "down", "down" : "up" ])
#define FEATURES ({ "no scry", "no teleport", "no summon", "healing room", \
    "special exits" })
#define EXITS ([ "church" : "/d/standard/square", "shop" : "/d/daybreak/room/shop/general_store" ])

inherit "/std/vault_locker_room";
inherit "/std/virtual/compile";

void bug();
void remove_all_lockers();
int path_to_entry();
void start_heal();
void rewrite_castle();
string query_long_desc();
void manage_all_exits(string dir, object tp);
string *query_all_doors();
string query_long(string str);
string *get_directions();

string desc;
nosave int heal_counter;
nosave int hb_on;

string query_long_desc() { return ::query_long(0); }

int is_castle() { return 1; }

int locker_filter(object item) {
  if(!item->query_is_locker()) return 0;
  if(item->is_pet()) return 0;
  return 1;
}

void remove_all_lockers() {
  int i;
  object *inv;

  if(query("room number") == 1) return;
  inv = filter_array(all_inventory(this_object()), "locker_filter",
      this_object());
  if(!inv || !sizeof(inv)) return;
  i = sizeof(inv);
  while(i--) if(inv[i]) {
    inv[i]->move(sprintf("/d/damned/virtual/%s_room_1.castle", query("owner")));
    inv[i]->toggle_closed();
    if(!inv[i]->query_closed())
      inv[i]->toggle_closed();
  }
  message("info", "A small dwarf appears out of nowhere and takes all of the chests in this room "+
      "to the entry of the castle.", this_object());
  return;
}

string *get_directions() {
  string *exits;
  int i;

  exits = query_exits();
  if(!exits) return ({});
  i = sizeof(exits);
  while(i--) exits[i] = query_exit(exits[i]);
  return exits;
}

int castle_filter(string room) {
  if(catch(call_other(room, "???"))) return 0;
  if(!room->is_castle()) return 0;
  if(!pointerp(room->get_directions())) return 0;
  return 1;
}

int path_to_entry() {
  string *tmp, *e_access, *my_access, e_file;
  int i, j, last_size;

  e_file = sprintf("/d/damned/virtual/%s_room_1.castle", query("owner"));
  if(query("room number") == 1) return 1;
  my_access = filter_array(get_directions(), "castle_filter", this_object());
  last_size = 0;
  while(last_size != sizeof(my_access)) {
    last_size = sizeof(my_access);
    tmp = ({});
    for(i=0; i<sizeof(my_access); i++)
      if(my_access[i]->is_castle())
      tmp += (string *)call_other(my_access[i], "get_directions");
    my_access = distinct_array(my_access + tmp);
    if(member_array(e_file, my_access) >= 0) return 1;
    my_access = filter_array(my_access, "castle_filter", this_object());
  }
  return 0;
}

int receive_objects() {
  object prev;

  prev = previous_object();
  if(!prev || !prev->query_is_locker() || prev->is_pet())
    return ::receive_objects();
  else if(!path_to_entry()) {
    if(environment(prev) && living(environment(prev))) {
      message("info", "You cannot drop chests in a room that doesn't have a direct path "+
            "to the entry to the castle.", environment(prev));
      return 0;
    }
  }
  return ::receive_objects();
}

string query_long(string str) {
  string ret, *exits, *doors;
  int i, j;

  ret = ::query_long(str);
  exits = query_exits();
  if(!(i = sizeof(exits))) return ret;
  doors = ({});
  while(i--)
    if(query_door(exits[i])) doors += ({ exits[i] });
  if(!(i = sizeof(doors))) return ret;
  switch(i) {
  case 1:
    ret += "There is a door over the "+doors[0]+" exit.";
    break;
  case 2:
    ret += "There are doors over the "+doors[0]+" and "+doors[1]+" exits.";
    break;
  default:
    ret += "There are doors over the ";
    for(j=0;j<sizeof(doors);j++){
      if(j == sizeof(doors)-1) ret += "and "+doors[j]+" exits.";
      else ret += doors[j]+", ";
    }
    break;
  }
  return ret;
}

void start_heal() {
  if(hb_on || !query_property("healing room")) return;
  set_heart_beat(1);
  hb_on = 1;
  return;
}

int player_filter(object who) { return who->is_player(); }

void heart_beat() {
  object *inv;
  int i;

  inv = filter_array(all_inventory(this_object()), "player_filter",
      this_object());
  if(!inv || !sizeof(inv)) {
    set_heart_beat(0);
    hb_on = 0;
    return;
  }
  if(!heal_counter || heal_counter <= 0) {
    heal_counter = 6;
    i = sizeof(inv);
    while(i--) inv[i]->do_healing((int)inv[i]->calculate_healing());
  }
  else heal_counter--;
  return;
}
    
void rewrite_castle() {
  string file, door;
  string *exits, plyr;
  int i, num;

  num = query("room number");
  plyr = query("owner");
  seteuid(UID_DAMNED_DATA);
  if(file_exists(file = (DIR_CASTLE + "/" + sprintf("%s.cas%d", plyr, num))))
    rm(file);
  write_file(file, sprintf("light:::%d\n", query_property("light")));
  write_file(file, sprintf("indoors:::%d\n", query_property("indoors")));
  write_file(file, sprintf("short:::%s\n", query_short()));
  write_file(file, sprintf("long:::%s\n", query_long_desc()));
  write_file(file, sprintf("no scry:::%d\n", query_property("no scry")));
  write_file(file, sprintf("no summon:::%d\n", query_property("no summon")));
  write_file(file, sprintf("no teleport:::%d\n", query_property("no teleport")));
  write_file(file, sprintf("special exits:::%d\n", query_property("special exits")));
  write_file(file, sprintf("healing room:::%d\n", query_property("healing room")));
  exits = query_exits();
  if(!exits || !sizeof(exits)) {
    seteuid(getuid());
    return;
  }
  i = sizeof(exits);
  while(i--) {
    if(door = query_door(exits[i])) {
      write_file(file, sprintf("exit:::%s:%s:door:%d:%s\n", exits[i],
            query_exit(exits[i]), query_lock_level(door), door));
    } else
      write_file(file, sprintf("exit:::%s:%s\n", exits[i], query_exit(exits[i])));
  }
  seteuid(getuid());
}
  
void bug() {
  set_short("There is a BUG here...");
  set_long("This room has a bug.  Please report it to an arch immediately.");
  set_property("light", 2);
  set_property("indoors", 1);
  return;
}

void virtual_setup(string file) {
  string plyr, *lines, func, arg, tmp, *doors;
  int i, num;

  if(sscanf(file, "%s_room_%d", plyr, num) != 2) {
    bug();
    return;
  }
  hb_on = 0;
  set_property("storage room", 1);
  set_property("light", 2);
  set_property("indoors", 1);
  set_property("no teleport", 0);
  set_property("no summon", 0);
  set_property("no scry", 0);
  set_property("healing room", 0);
  func = 0;
  arg = "";
  set("owner", plyr);
  set("room number", num);
  set_short(sprintf("%s's Castle Room #%d",capitalize(plyr), num));
  set_long("This is Room #"+num+" of "+capitalize(plyr)+"'s Castle.\n"
          "If you are "+capitalize(plyr)+", type 'help castle' for options.  \n\n");
  if(!file_exists(DIR_CASTLE + "/"+plyr+sprintf(".cas%d", num))) return;
  lines = explode(read_file(DIR_CASTLE + "/"+plyr+sprintf(".cas%d", num)), "\n");
  for(i=0; i<sizeof(lines); i++) {
    if(sscanf(lines[i], "long:::%s", arg) == 1) {
      i++;
      while(sscanf(lines[i], "%*s:::%*s") != 2 &&
        i < sizeof(lines)) {
        if(lines[i] != "")
        arg += "\n"+lines[i];
        i++;
      }
      set("long", arg);
      i--;
      continue;
    }
    else if(sscanf(lines[i], "exit:::%s:%s:door:%d:%s", func, arg, num, tmp) == 4) {
      add_exit(arg, func);
      set_door(tmp, arg, func, plyr+" castle key");
      set_lock_level(tmp, num);
      set_open(tmp, 0);
      set_locked(tmp, 1);
      add_item(tmp, "It is a door on the "+func+" exit.  ");
    }
    else if(sscanf(lines[i], "exit:::%s:%s", func, arg) == 2)
      add_exit(arg, func);
    else if(sscanf(lines[i], "%s:::%s", func, arg) == 2) {
      switch(func) {
      case "light":
      case "indoors":
      case "no summon":
      case "no teleport":
      case "no scry":
      case "healing room":
      case "special exits":
        set_property(func, REFS_D->resolv_str(arg));
        break;
      default:
        set(func, REFS_D->resolv_str(arg));
        break;
      }
      arg = "";
      func = tmp;
    }
  }
  lines = query_exits();
  if(!lines) return;
  i = sizeof(lines);
  if(!i) return;
  doors = ({});
  while(i--)
    if(query_door(lines[i])) doors += ({ article(query_door(lines[i])) });
  if(!(i = sizeof(doors))) return;
  switch(i) {
  case 1:
    add_item("door", "There is "+doors[0]+" here.  ");
    add_item("doors", "There is "+doors[0]+" here.  ");
    break;
  case 2:
    add_item("door", "There are "+doors[0]+" and "+doors[1]+" here.  ");
    add_item("doors", "There are "+doors[0]+" and "+doors[1]+" here.  ");
    break;
  default:
    tmp = "There are ";
    for(num = 0;num < sizeof(doors); num++) {
      if(num == (sizeof(doors)-1)) tmp += "and "+doors[num]+" here.  ";
      else tmp += doors[num] + ", ";
    }
    add_item("door", tmp);
    add_item("doors", tmp);
    break;
  }
  return;
}

void init() {
  object ob;
  
  ::init();
  add_action("castle_help", "help");
  add_action("castle_action", "castle");
  if(this_player()->is_player() && !hb_on)
    start_heal();
  if((string)this_player()->query_name() == query("owner") &&
    !present(query("owner")+" castle key", this_player())) {
    seteuid(getuid());
    ob = new("/std/Object");
    ob->set_weight(2);
    ob->set_name("key");
    ob->set_id(({ "key", "castle key", query("owner")+" castle key" }));
    ob->set_short(capitalize(query("owner"))+"'s castle key");
    ob->set_long("This key is designed for use in castle doors, but not chests.");
    ob->set_property("no save", 1);
    ob->move(this_player());
    call_out("notify_me", 2, this_player());
  }
  return;
}

void notify_me(object who) {
  if(who)
    message("info", "You receive your castle key.", who);
  return;
}

int castle_help(string str) {
  string tmp;
  
  if(str != "castle") return 0;
  if((string)this_player()->query_name() != query("owner")) return 0;
  tmp = 
"Castle commands:\n"
"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n"
"castle list......................Lists all castle 'things' available\n"
"castle exit <direction> <room>...Adds an exit from the current room\n"
"Ex: castle exit north 4          <room> in the direction <direction>\n"
"   ^ adds an exit to room 4      must be north, south, east, west, up,\n"
"                                 or down.\n"
"castle door <door-num> <dir>.....Adds a door to the direction <dir>.\n"
"Ex: castle door 4 north\n"
"\n"
"castle remove door <dir>.........Removes the door on the <dir> exit\n"
"Ex: castle remove door north     and puts it back in your inventory.\n"
"castle remove exit <dir>.........Removes the exit <dir>\n"
"castle feature <feature>.........Adds <feature> to the current room.\n"
"Ex: castle feature no summon     Some feartues include, healing room, \n"
"                                 no summon, no teleport, and no scry.\n"
"castle remove feature <feature>..Removes <feature> from the current\n"
"                                 room and puts it in your inventory.\n"
"castle features..................Lists all featurs in the current\n"
"                                 room.\n"
"castle goto <num>................Teleports you to room number <num>.\n"
"castle where.....................Tells you where anyone in your castle\n"
"                                 in by room number.\n"
"castle short <desc>..............Changes the short dec of the room\n"
"                                 to <desc> (40 char. max)\n"
"castle long......................Opens and editor to put in the\n"
"                                 long description...10 lines max.\n"
"castle light <level>.............Changes the light in the room to\n"
"                                 <level>...may be either 'dark',\n"
"                                 'light', or 'bright'.\n";
  this_player()->more(explode(tmp, "\n"));
  return 1;
}

string *query_all_doors() {
  string *ret, *exits;
  int i;

  exits = query_exits();
  if(!(i = sizeof(exits))) return ({});
  ret = ({});
  while(i--) if(query_door(exits[i])) ret += ({ query_door(exits[i]) });
  return ret;
}

void manage_all_exits(string dir, object tp) {
  int i, max;
  string room;

  if(!(max = tp->query_property("castle rooms"))) return;
  for(i=1;i<=max;i++) {
    if(i == query("room number")) continue;
    room = "/d/damned/virtual/"+
      sprintf("%s_room_%d.castle", query("owner"), i);
    room->manage_exits(this_object(), dir);
  }
  return;
}

void manage_exits(object room, string dir) {
  string door;
  
  if(!COMPASS[dir]) return;
  dir = COMPASS[dir];
  if(query_exit(dir) == base_name(room)) {
    if(door = query_door(dir)) remove_door(door);
    remove_exit(dir);
    rewrite_castle();
  }
  return;
}
  
int castle_action(string arg) {
  string tmp, str2;
  int rooms, i, num1, j;
  string *features;
  string *doors, str1;
  object room, *inv;

  if((string)this_player()->query_name() != query("owner")) return 0;
  if(!arg)
    return notify_fail("Type 'help castle' for assistance.\n");
  if(arg == "list") {
    tmp =
"List of castle items in your inventory:\n"
"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n";
    rooms = (int)this_player()->query_property("castle rooms");
    if(!rooms) rooms = 0;
    tmp += sprintf("Total number of rooms: %d\n\n", rooms);
    tmp += "Doors:\n"
           "-=-=-=-=-=-=-=-=-=-=-=-=-\n";
    doors = (string *)this_player()->query_property("castle doors");
    if(!doors || !sizeof(doors)) tmp += "none.\n\n";
    else {
      for(i=0; i < sizeof(doors); i++)
        tmp += sprintf("%d.  %s\n", i+1, doors[i]);
      tmp += "\n";
    }
    tmp += "Features:\n"
           "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";
    features = (string *)this_player()->query_property("castle features");
    if(!features || !sizeof(features)) tmp += "none.";
    else
      tmp += format_page(features, 2);
    tmp += "\n";
    this_player()->more(explode(tmp, "\n"));
    return 1;
  }
  if(sscanf(arg, "exit %s %d", str1, num1) == 2) {
    if(member_array(str1, keys(COMPASS)) < 0) {
      write("You must choose one of the following directions:\n"+
            "north, south, east, west, up, or down.");
      return 1;
    }
    if(member_array(str1, query_exits()) >= 0) {
      if(str1 == "up" || str1 == "down")
        write("There is already an exit "+str1+".");
      else
        write("There is already an exit to the "+str1+".");
      return 1;
    }
    if(num1 == query("room number")) {
      write("You may not add an exit from a room to the same room.");
      return 1;
    }
    if(num1 > (int)this_player()->query_property("castle rooms")) {
      write("There aren't that many rooms in your castle.");
      return 1;
    }
    manage_all_exits(str1, this_player());
    call_other(sprintf("/d/damned/virtual/%s_room_%d.castle", query("owner"),
          num1), "manage_all_exits", COMPASS[str1], this_player());
    add_exit(sprintf("/d/damned/virtual/%s_room_%d.castle", query("owner"),
          num1), str1);
    call_other(sprintf("/d/damned/virtual/%s_room_%d.castle", query("owner"),
          num1), "add_exit", base_name(this_object()), COMPASS[str1]);
    rewrite_castle();
    call_other(sprintf("/d/damned/virtual/%s_room_%d.castle", query("owner"),
          num1), "rewrite_castle");
    write("Exit added.");
    return 1;
  }
  if(sscanf(arg, "remove exit %s", str1) == 1) {
    if(member_array(str1, query_exits()) < 0) {
      write("There is no such exit out of this room.");
      return 1;
    }
    if(member_array(str1, keys(COMPASS)) < 0) {
      write("You can't remove that exit.");
      return 1;
    }
    if(query_door(str1)) {
      write("There is a door over that exit.  It must be removed first.");
      return 1;
    }
    if(!COMPASS[str1]) {
      write("Somehow, your exits got bugged...notify an arch!");
      return 1;
    }
    call_other(query_exit(str1), "manage_exits", this_object(), str1);
    tmp = query_exit(str1);
    remove_exit(str1);
    rewrite_castle();
    call_other(query_exit(str1), "rewrite_castle");
    if(!path_to_entry()) remove_all_lockers();
    if(!tmp->path_to_entry())
      tmp->remove_all_lockers();
    write("Exit removed.");
    return 1;
  }
  if(sscanf(arg, "door %d %s", num1, str1) ==2) {
    doors = (string *)this_player()->query_property("castle doors");
    if(!doors || num1 < 1 || num1 > sizeof(doors)) {
      write("You do not have a door number "+num1+".");
      return 1;
    }
    if(member_array(str1, query_exits()) < 0) {
      write("There is no "+str1+" exit in this room on which to put a door.");
      return 1;
    }
    if(query_door(str1)) {
      write("There is already "+article(query_door(str1))+" on that exit.");
      return 1;
    }
    if(member_array(str1, keys(COMPASS)) < 0) {
      write("You cannot put a door over that exit.");
      return 1;
    }
    num1--;
    if(!DOORS[doors[num1]]) {
      write("There is a bug with that door...see an arch.");
      return 1;
    }
    tmp = doors[num1];
    i = 2;
    while(member_array(tmp, query_all_doors()) >= 0 ||
          member_array(tmp, (string *)(query_exit(str1))->query_all_doors()) >= 0) {
      tmp = sprintf("%s %d", doors[num1], i);
      i++;
    }
    set_door(tmp, query_exit(str1), str1, query("owner ")+" castle key");
    set_lock_level(tmp, DOORS[doors[num1]]);
    call_other(query_exit(str1), "set_door", tmp, base_name(this_object()),
        COMPASS[str1], query("owner ")+" castle key");
    call_other(query_exit(str1), "set_lock_level", tmp,
        DOORS[doors[num1]]);
    rewrite_castle();
    call_other(query_exit(str1), "rewrite_castle");
    this_player()->set_property("castle doors", exclude_array(doors, num1));
    write("Door '"+tmp+"' added.");
    CASTLE_D->update_room(load_object(query_exit(str1)));
    CASTLE_D->update_room(this_object());
    return 1;
  }
  if(sscanf(arg, "remove door %s", str1) == 1) {
    if(member_array(str1, query_exits()) < 0) {
      write("There is no such exit in this room.");
      return 1;
    }
    if(!(tmp = query_door(str1))) {
      write("There is no door over that exit.");
      return 1;
    }
    remove_door(tmp);
    call_other(query_exit(str1), "remove_door", tmp);
    rewrite_castle();
    call_other(query_exit(str1), "rewrite_castle");
    i = strlen(tmp);
    if(i && atoi(tmp[i-1..i-1]))
      tmp = tmp[0..i-3];
    doors = (string *)this_player()->query_property("castle doors");
    if(!doors) doors = ({});
    doors += ({ tmp });
    this_player()->set_property("castle doors", doors);
    write("Door '"+tmp+"' removed and added to your inventory.");
    CASTLE_D->update_room(this_object());
    CASTLE_D->update_room(load_object(query_exit(str1)));    
    return 1;
  }
  if(sscanf(arg, "short %s", str1) == 1) {
    if(strlen(str1) > 40) {
      write("Truncating description to 40 characters.");
      str1 = arrange_string(str1, 40);
    }
    set_short(str1);
    rewrite_castle();
    write("Short description changed.");
    return 1;
  }
  if(arg == "long") {
    write("Input the long description.  (10 lines max, 74 chars. per line)\nEnter . on a blank line to stop\n"+
          "-=-=-=-=-10-=-=-=-=20-=-=-=-=30-=-=-=-=40-=-=-=-=50-=-=-=-=60-=-=-=-=70-=-\n");
    printf("] ");
    desc = "";
    input_to("next_line", 0, 9);
    return 1;
  }
  if(sscanf(arg, "light %s", str1) == 1) {
    switch(lower_case(str1)) {
    case "dark":
      set_property("light", -1);
      break;
    case "light":
      set_property("light", 2);
      break;
    case "bright":
      set_property("light", 6);
      break;
    default:
      write("Please choose 'light', 'dark', or 'bright' as the light level.");
      return 1;
    }
    rewrite_castle();
    write("Light level altered.");
    return 1;
  }
  if(arg == "where") {
    str1 = "%^CYAN%^Castle Where:\n"+
          "%^BOLD%^-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n";
    num1 = (int)this_player()->query_property("castle rooms");
    for(i=1;i<=num1;i++) {
      str1 += "%^CYAN%^Room #"+i+":\n";
      room = load_object(sprintf("/d/damned/virtual/%s_room_%d.castle", query("owner"), i));
      inv = all_inventory(room);
      if(!inv) continue;
      j = sizeof(inv);
      while(j--) {
        if(!living(inv[j])) continue;
        if(inv[j]->is_player())
          str1 += "  %^BOLD%^"+(string)inv[j]->query_short()+"\n";
        else
          str1 += "  "+(string)inv[j]->query_short()+"\n";
      }
      str1 += "\n";
    }
    this_player()->more(explode(str1, "\n"));
    return 1;
  }
  if(sscanf(arg, "goto %d", num1) == 1) {
    if(num1 < 1 || num1 > (int)this_player()->query_property("castle rooms")) {
      write("There is no such room number in your castle.");
      return 1;
    }
    this_player()->move_player(sprintf("/d/damned/virtual/%s_room_%d.castle", query("owner"), num1), 0);
    return 1;
  }
  if(sscanf(arg, "feature %s", str1) == 1) {
    features = (string *)this_player()->query_property("castle features");
    if(!features || (num1 = member_array(str1, features)) < 0) {
      write("There is no such feature in your inventory.");
      return 1;
    }
    if(query_property(str1)) {
      write("That feature is already installed on this room.");
      return 1;
    }
    if(str1 == "special exits") {
      doors = keys(EXITS);
      i = sizeof(doors);
      while(i--) add_exit(EXITS[doors[i]], doors[i]);
    }
    set_property(str1, 1);
    this_player()->set_property("castle features",
            exclude_array(features, num1));
    rewrite_castle();
    write("Feature '"+str1+"' added to this room.");
    return 1;
  }
  if(sscanf(arg, "remove feature %s", str1) == 1) {
    if(member_array(str1, FEATURES) < 0) {
      write("'"+str1+"' is not a valid feature.");
      return 1;
    }
    if(!query_property(str1)) {
      write("That feature is not installed on this room.");
      return 1;
    }
    if(str1 == "special exits") {
      doors = keys(EXITS);
      i = sizeof(doors);
      while(i--) remove_exit(doors[i]);
    }
    set_property(str1, 0);
    features = (string *)this_player()->query_property("castle features");
    if(!features) features = ({});
    features += ({ str1 });
    this_player()->set_property("castle features", features);
    rewrite_castle();
    write("Feature '"+str1+"' removed and returned to inventory.");
    return 1;
  }
  if(arg == "features") {
    i = sizeof(FEATURES);
    num1 = 0;
    write("Features of this room:\n");
    while(i--)
      if(query_property(FEATURES[i])) {
        write("  "+FEATURES[i]);
        num1 = 1;
      }
    if(!num1) write("none.");
    return 1;
  }
  write("See 'help castle' for syntax.");
  return 1;
}
     
void next_line(string line, int to_go) {
  to_go--;  
  if(line == "." || to_go <= 0) {
    if(to_go <= 0) {
      if(strlen(line) > 74)
        desc += arrange_string(line, 74) + "\n";
      else
        desc += line +"\n";
    }
    write("\nFinished!");
    set_long(desc+"\n");
    rewrite_castle();
    return;
  }
  if(strlen(line) > 74) write("Truncating line");
  desc += arrange_string(line, 74) + "\n";
  printf("] ");
  input_to("next_line", 0, to_go);
  return;
}

