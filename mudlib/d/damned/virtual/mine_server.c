//    Mine server for player mines.
//    -Diewarzau 1/28/96
//    DarkeLIB 1.0
#include <security.h>
#include <move.h>

inherit "/std/vault";
inherit "/std/virtual/compile";

nosave int ch_open;
mapping minerals;
nosave int lower_room;

void save_this();

void save_this() {
  seteuid(UID_DAMNED_DATA);
  save_object("/d/damned/data/mines/"+query("owner")+"_mine");
  seteuid(getuid());
  return;
}

void virtual_setup(string file) {
  int num, east, south;
  string plyr;

  set_property("light", 2);
  set_property("indoors", 1);
  if(sscanf(file, "%s_mine%d_%d_%d", plyr, num, east, south) != 4) {
    set_short("BUG IN MINE");
    set_long("BUG IN MINE");
    add_exit("/d/standard/square", "church");
    return;
  }
  set("owner", plyr);
  if(num == 1) {
    lower_room = 0;
    set_short("Entrance to "+capitalize(plyr)+"'s Mine");
    set_long("You stand in the entrance to "+capitalize(plyr)+"'s mine.  "
    "There is a wooden hatch with a lock on it which presumably leads "
    "to the mine below.  "
    "A small lantern provides light here.");
    set_items(([ "lantern" : "It is permanently attached.",
        "hatch" : "It looks surprisingly strong." ]));
    add_exit(sprintf("/d/damned/virtual/room_%d_%d.world", east, south), "out");
    add_exit(sprintf("/d/damned/virtual/%s_mine2_%d_%d.mine", plyr, east, south),
            "down");
    set_door("hatch", sprintf("/d/damned/virtual/%s_mine2_%d_%d.mine", plyr, east, south),
            "down", plyr+" mine key");
    set_open("hatch", 0);
    set_locked("hatch", 1);
    set_lock_level("hatch", 35);
    return;
  } else if(num == 2) {
    lower_room = 1;
    ch_open = 0;
    set_property("light", -1);
    set_short(capitalize(plyr)+"'s Mine");
    set_long("This is "+capitalize(plyr)+"'s mine.  All of the mine workers must "
    "be elsewhere, but you see an interesting-looking chest against a wall.");
    add_exit(sprintf("/d/damned/virtual/%s_mine1_%d_%d.mine", plyr, east, south),
            "up");
    set_door("hatch", sprintf("/d/damned/virtual/%s_mine1_%d_%d.mine", plyr, east, south),
            "up", plyr+" mine key");
    set_open("hatch", 0);
    set_locked("hatch", 1);
    set_lock_level("hatch", 35);
    seteuid(UID_DAMNED_DATA);
    if(file_exists("/d/damned/data/mines/"+plyr+"_mine.o"))
      restore_object("/d/damned/data/mines/"+plyr+"_mine");
    seteuid(getuid());
    set_items(([ "chest" : (: call_other, this_object(), "describe_chest" :),
          "hatch" : "It looks surprisingly strong." ]));
    return;
  }
  set_property("light", 3);
  set_short("This mine has a BUG...");
  set_long("This mine has a BUG...");
  return;
}

void init() {
  ::init();
  if(lower_room) {
    add_action("open_chest", "open");
    add_action("close_chest", "close");
    add_action("get_from", "get");
    add_action("get_from", "take");
    add_action("put_into", "put");
  }
  return;
}

int put_into(string str) {
  string what;
  object ob;

  if(!sscanf(str, "%s in chest", what))
    return notify_fail("Put what in what?\n");
  ob = present(what, this_player());
  if(!ob) {
    write("You do not have '"+what+"'");
    return 1;
  }
  if(!ob->id("ingot")) {
    write("You may only put ingots in the chest.");
    return 1;
  }
  if(!ob->query_property("material_name")) {
    write("Cannot read ingot material type....BUG!");
    return 1;
  }
  this_object()->add_mineral((string)ob->query_property("material_name"),
      1);
  ob->remove();
  write("You put the ingot in the chest.");
  return 1;
}

int get_from(string str) {
  string type, *min_keys;
  int amt, i, j, flag;
  object ob;

  seteuid(getuid());
  if(str == "all from chest") {
    if(!ch_open)
      return notify_fail("The chest is closed.\n");
    if(!minerals || !mapp(minerals) || !(i=sizeof(min_keys=keys(minerals)))) {
      write("There is nothing in the chest.");
      return 1;
    }
    message("info", (string)this_player()->query_cap_name()+
          " takes everything from the mineral chest.", environment(this_player()),
          ({ this_player() }));
    while(i--) {
      flag = 0;
      for(j=1;j<=minerals[min_keys[i]];j++) {
        ob = new("/wizards/diewarzau/obj/misc/ingot");
        ob->set_material("metal/"+min_keys[i]);
        if((int)ob->move(this_player()) != MOVE_OK) {
          flag = 1;
          ob->move(environment(this_player()));
        }
      }
      write("You take "+consolidate(minerals[min_keys[i]], min_keys[i]+ " ingot")+ " from the chest.");
      map_delete(minerals, min_keys[i]);
      if(flag) {
        write("You cannot carry all of the "+min_keys[i]+" ingots, and you drop some.");
        message("info", (string)this_player()->query_cap_name()+ " drops some "+
            min_keys[i] + " ingots.", environment(this_player()), ({ this_player() }));
      }
    }
    write("You have emptied the chest.");
    save_this();
    return 1;
  }
  if(sscanf(str, "%d %s ingots from chest", amt, type) == 2 ||
    sscanf(str, "%d %s from chest", amt, type) == 2) {
    if(!ch_open)
      return notify_fail("The chest is closed.\n");
    type = lower_case(type);
    if(amt < 0) {
      write("Uhhh, nice try...");
      return 1;
    }
    if(!minerals || !mapp(minerals) || !minerals[type]) {
      write("There is no "+type+" in the mineral chest.");
      return 1;
    }
    if(minerals[type] < amt) {
      write("There are not that many "+type+" ingots in the chest.");
      return 1;
    }
    minerals[type] -= amt;
    flag = 0;
    write("You take "+consolidate(amt, type+" ingot")+" from the chest.");
    message("info", (string)this_player()->query_cap_name()+
      " takes "+consolidate(amt, type+" ingot")+" from the mineral chest.",
	environment(this_player()), ({ this_player() }));
    for(i=1;i<=amt;i++) {
      ob = new("/wizards/diewarzau/obj/misc/ingot");
      ob->set_material("metal/"+type);
      if((int)ob->move(this_player()) != MOVE_OK) {
        flag = 1;
        ob->move(environment(this_player()));
      }
    }
    if(flag) write("You cannot carry all the ingots and you drop some.");
    message("info", (string)this_player()->query_cap_name()+
        " accidentally drops some "+type+" ingots.",
        environment(this_player()), ({ this_player() }));
    save_this();
    return 1;
  }
  if(sscanf(str, "%s ingots from chest", type) == 1 ||
     sscanf(str, "%s from chest", type) == 1 ) {
    if(!ch_open)
      return notify_fail("The chest is closed.\n");
    type = lower_case(type);
    if(!minerals || !mapp(minerals) || !minerals[type]) {
      write("There is no "+type+" in the mineral chest.");
      return 1;
    }
    amt = minerals[type];
    map_delete(minerals, type);
    flag = 0;
    write("You take "+consolidate(amt, type+" ingot")+" from the chest.");
    message("info", (string)this_player()->query_cap_name()+
      " takes "+consolidate(amt, type+" ingot")+" from the mineral chest.",
	environment(this_player()), ({ this_player() }));
    for(i=1;i<=amt;i++) {
      ob = new("/wizards/diewarzau/obj/misc/ingot");
      ob->set_material("metal/"+type);
      if((int)ob->move(this_player()) != MOVE_OK) {
        flag = 1;
        ob->move(environment(this_player()));
      }
    }
    if(flag) write("You cannot carry all the ingots and you drop some.");
    message("info", (string)this_player()->query_cap_name()+
        " accidentally drops some "+type+" ingots.",
        environment(this_player()), ({ this_player() }));
    save_this();
    return 1;
  }
  notify_fail("get what from what?\n");
  return 0;
}

int open_chest(string str) {
  if(str != "chest") return notify_fail("Open what?\n");
  if(ch_open)
    return notify_fail("The chest is already open.\n");
  ch_open = 1;
  write("With a creaking sound, the mineral chest opens.");
  message("info", (string)this_player()->query_cap_name() +
        " opens the mineral chest.", environment(this_player()),
        ({ this_player() }));
  return 1;
}

int close_chest(string str) {
  if(str != "chest") return notify_fail("Close what?\n");
  if(!ch_open)
    return notify_fail("The chest is already closed.\n");
  ch_open = 0;
  write("You close the mineral chest.");
  message("info", (string)this_player()->query_cap_name() +
        " closes the mineral chest.", environment(this_player()),
        ({ this_player() }));
  return 1;
}

int describe_chest() {
  string *min_keys, *items;
  int i;
  
  if(!ch_open) {
    write("It is closed.");
    return 1;
  }
  write("This chest is used for the storage of minerals.\nIt contains:\n");
  if(!minerals || !mapp(minerals) || !(i = sizeof(min_keys=keys(minerals)))) {
    write("nothing.");
    return 1;
  }
  items = allocate(i);
  while(i--)
    items[i] = sprintf("%-15s: %4d ingots", min_keys[i], minerals[min_keys[i]]);
  write(format_page(items, 2));
  return 1;
}

void add_mineral(string type, int amt) {
  if(amt <= 0) return;
  if(!minerals) minerals = ([]);
  if(!minerals[type]) minerals[type] = amt;
  else minerals[type] += amt;
  save_this();
  return;
}

