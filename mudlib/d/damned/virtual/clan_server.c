/*    This is the virtual server for the clan stronghold.
 *    it just handles the outer walls, moat, etc.  The rooms are handled
 *    by a modified castle server.
 *
 *    DarkeLIB 1.0
 *    Diewarzau 7/1/96
 */

#include <security.h>
#include <clan.h>
#include <daemons.h>
#define CLAN_DIR "/d/damned/data/clans"

inherit "/std/vault";
inherit "/std/virtual/compile";

nosave string clan_name;
nosave string room_type;
nosave int room_num;
nosave int under_attack;
mapping room_info;

void room_err();
void set_room_info(string key, mixed val);
void remove_room_info(string key);
mixed query_room_info(string key);
int restore_room_info();

int clean_up() {
  if(under_attack) return 0;
  return ::clean_up();
}

int restore_room_info() {
  seteuid(getuid());
  if(!file_exists(CLAN_DIR+sprintf("/%s.%s%d", clan_name,
				   room_type, room_num)))
    return 0;
  room_info = restore_variable(read_file(CLAN_DIR+
					 sprintf("/%s.%s%d", clan_name,
						 room_type, room_num)));
  return 1;
}

void set_room_info(string key, mixed val) {
  if(!room_info) room_info = allocate_mapping(15);
  room_info[key] = val;
  seteuid(getuid());
  if(file_exists(CLAN_DIR+sprintf("/%s.%s%d", clan_name,
				  room_type, room_num)))
    rm(CLAN_DIR+sprintf("/%s.%s%d", clan_name, room_type, room_num));
  write_file(CLAN_DIR+sprintf("/%s.%s%d", clan_name, room_type, room_num),
	     save_variable(room_info));
  return;
}

void remove_room_info(string key) {
  if(!room_info || !room_info[key]) return;
  map_delete(room_info, key);
  if(file_exists(CLAN_DIR+sprintf("/%s.%s%d", clan_name,
                                  room_type, room_num)))
    rm(CLAN_DIR+sprintf("/%s.%s%d", clan_name, room_type, room_num));
  write_file(CLAN_DIR+sprintf("/%s.%s%d", clan_name, room_type, room_num),
             save_variable(room_info));
  return;
}

mixed query_room_info(string key) {
  if(!room_info || !room_info[key]) return 0;
  return room_info[key];
}

void room_err() {
  set_short("Room has a BUG");
  set_long("This room has a bug.\n"
	   "Report to an arch.");
  set_property("light", 3);
  set_property("indoors", 1);
  add_exit("/d/standard/square", "church");
  return;
}

void virtual_setup(string file) {
  string r_type, cname, gfile;
  object ob;
  int rnum;

  seteuid(getuid());
  if(sscanf(file, "%s:%s_%d", r_type, cname, rnum) != 3) {
    room_err();
    return;
  }
  if(getuid() != "Damned") {
    room_err();
    set_long("This room is bugged.\n"
	     "Tell an arch (preferably Diewarzau) this:\n"+
	     sprintf("The Uid of this room is '%s'", getuid()));
    return;
  }
  room_type = r_type;
  room_num = rnum;
  clan_name = cname;
  if(r_type == "courtyard") {
    seteuid(getuid());
    if(!restore_room_info()) {
      set_room_info("extra long", "");
      set_room_info("draw bridge", "/std/diewarzau/obj/misc/dbridge1");
      set_room_info("guards", ({ "/std/diewarzau/obj/mon/clan_guard1",
                    "/std/diewarzau/obj/mon/clan_guard1" }) );
    }
    set_short("The Courtyard of "+capitalize(cname)+" Keep");
    set("day long",
	sprintf("%%^CYAN%%^%|75s%%^RESET%%^\n", 
		"--** Courtyard of "+capitalize(cname)+" Keep **--")+
	"This spacious courtyard is bordered on all four sides by "
	"huge stone walls.  The grass is short here.  In the center "
	"of the courtyard is the main hall of the Stronghold.\n"
	"The teaching room is also here.  Clan members may find all "
	"necessary equipment to teach one another skills and spells "
	"there.\n"
	/*
	"The Clan Leader and other members may have special commands "
	"here.  Type 'help clan' for more info.\n"
	*/
	+room_info["extra long"]);
    set("night long",
	sprintf("%%^CYAN%%^%|75s%%^RESET%%^\n",
                "--** Courtyard of "+capitalize(cname)+" Keep **--")+
	"At night, torches cast an orange glow about the courtyard.  "
	"Long shadows are cast on the tall stone walls which surround you.  "
	"The main hall of the Stronghold is visible in the center.\n"
	"The teaching room is also here.  Clan members may find all "
        "necessary equipment to teach one another skills and spells "
        "there.\n"+
	room_info["extra long"]);
    set_property("indoors", 0);
    set_property("night light", 2);
    add_exit(sprintf("/d/damned/virtual/outer:%s_7.clan", cname),
	     "south");
    add_exit(sprintf("/d/damned/virtual/%s_hall_1.clan_strong", cname),
	     "hall");
    add_exit(sprintf("/d/damned/virtual/teach:%s_1.clan", cname),
	     "teach");
    if(pointerp(room_info["guards"])) {
      foreach(gfile in room_info["guards"]) {
        ob = new(gfile);
        ob->move(this_object());
        ob->setup(cname);
      }
    }
    ob = new(room_info["draw bridge"]);
    ob->move(this_object());
    ob->setup(cname, r_type, rnum);
  }
  else if(r_type == "teach") {
    seteuid(getuid());
    if(!restore_room_info()) {
      set_room_info("teach room", 1);
      set_room_info("actions", ([ "teach" : 
				({ "/std/diewarzau/obj/misc/teach_func",
				     "teach_skill" }),
				"help" : 
				({ "/std/diewarzau/obj/misc/teach_func",
				     "help_teach" }),
				"capacity" : 
				({ "/std/diewarzau/obj/misc/teach_func",
				     "capacity" }) ]));
    }
    switch(room_info["teach room"]) {
    case 1:
    default:
      set_short("Teaching Hall of Clan "+capitalize(cname));
      set_long(
	       "This is the teaching hall of Clan "+capitalize(cname)+".  "
	       "The room is adorned with all manner of tools used in "
	       "the clan's constant exchange of skills and spells.  "
	       "The room is small and rather stuffy, but you suspect that "
	       "it serves its purpose well enough.\n"
	       "\n"
	       "%^YELLOW%^Maximum skill level:%^RESET%^ 60%\n"
	       "%^RED%^Time to teach:%^RESET%^ 2 min.\n"
	       "%^CYAN%^%^BOLD%^Maximum spell level:%^RESET%^ *3\n"
	       "%^RED%^Time to teach:%^RESET%^ 10 min.\n"
	       "Type 'help teach' for assistance.\n");
      break;
    case 2:
      set_short("Teaching Auditorium of Clan "+capitalize(cname));
      set_long(
	       "This hall is quite large, with much open space for "
	       "the teaching of physical skills.  For those more "
	       "cerebral, a library occupies a significant portion "
	       "of the structure.  The equipment is of high quality, "
	       "and you suspect that detailed teaching takes place here.\n"
	       "\n"
	       "%^YELLOW%^Maximum skill level:%^RESET%^ 75%\n"
	       "%^RED%^Time to teach:%^RESET%^ 1 min.\n"
	       "%^CYAN%^%^BOLD%^Maximum spell level:%^RESET%^ *4\n"
               "%^RED%^Time to teach:%^RESET%^ 6 min.\n"
	       "Type 'help teach' for assistance.\n");
      break;
    case 3:
      set_short("Grand Teaching Hall of Clan "+capitalize(cname));
      set_long(
	       "This teaching hall is huge and adorned with the "
	       "finest of equipment.  An extensive library with "
	       "several ancient and valuable tomes is visible across "
	       "a small garden.  Physical skills can easily be "
	       "practiced in the extensive gymnasium here.  The "
	       "teaching that takes place here must be of the highest "
	       "quality.\n"
               "\n"
               "%^YELLOW%^Maximum skill level:%^RESET%^ 90%\n"
               "%^RED%^Time to teach:%^RESET%^ 30 sec.\n"
               "%^CYAN%^%^BOLD%^Maximum spell level:%^RESET%^ *5\n"
               "%^RED%^Time to teach:%^RESET%^ 3 min.\n"
	       "Type 'help teach' for assistance.\n");
      break;
    }
    set_property("indoors", 1);
    set_property("light", 3);
    ob = new("std/bboard");
    ob->set_name("board");
    ob->set_id( ({"board","elementalist_board"}) );
    ob->set_board_id(cname+"_clan_board");
    ob->set_max_posts(25);
    ob->set("short","The "+capitalize(cname)+" Clan Board");
    ob->set("long",
        "A board upon which messages are posted for others to read.\n");
    delayed_call("set_board_location", 2, ob);
    add_exit("/d/damned/virtual/courtyard:"+cname+"_1.clan", "out");
  }
  else if(r_type == "outer") {
    set_property("indoors", 0);
    if(!restore_room_info())
      set_room_info("extra long", "");
    else if(room_info["extra exits"] && room_info["cyard exit"])
      map_delete(room_info["extra exits"], room_info["cyard exit"]);
    switch(rnum) {
    case 1:
      set_room_info("cyard exit", "southeast");
      set_short("Northwest Corner of "+capitalize(cname)+" Keep");
      add_exit("/d/damned/virtual/outer:"+cname+"_2.clan",
	       "east");
      add_exit("/d/damned/virtual/outer:"+cname+"_4.clan",
	       "south");
      break;
    case 2:
      set_room_info("cyard exit", "south");
      set_short("North end of "+capitalize(cname)+" Keep");
      add_exit("/d/damned/virtual/outer:"+cname+"_1.clan",
	       "west");
      add_exit("/d/damned/virtual/outer:"+cname+"_3.clan",
	       "east");
      break;
    case 3:
      set_room_info("cyard exit", "southwest");
      set_short("Northeast corner of "+capitalize(cname)+" Keep");
      add_exit("/d/damned/virtual/outer:"+cname+"_2.clan",
	       "west");
      add_exit("/d/damned/virtual/outer:"+cname+"_5.clan",
	       "south");
      break;
    case 4:
      set_room_info("cyard exit", "east");
      set_short("West end of "+capitalize(cname)+" Keep");
      add_exit("/d/damned/virtual/outer:"+cname+"_1.clan",
	       "north");
      add_exit("/d/damned/virtual/outer:"+cname+"_6.clan",
	       "south");
      break;
    case 5:
      set_room_info("cyard exit", "west");
      set_short("East end of "+capitalize(cname)+" Keep");
      add_exit("/d/damned/virtual/outer:"+cname+"_3.clan",
	       "north");
      add_exit("/d/damned/virtual/outer:"+cname+"_8.clan",
	       "south");
      break;
    case 6:
      set_room_info("cyard exit", "northeast");
      set_short("Southwest corner of "+capitalize(cname)+" Keep");
      add_exit("/d/damned/virtual/outer:"+cname+"_4.clan",
	       "north");
      add_exit("/d/damned/virtual/outer:"+cname+"_7.clan",
	       "east");
      break;
    case 7:
      set_room_info("cyard exit", "north");
      set_room_info("dbridge here", 1);
      if(!room_info["extra long"])
	set_room_info("extra long",
		      "A huge drawbridge bisects the large stone wall which "+
		      "you face.");
      if(!room_info["draw bridge"])
	set_room_info("draw bridge",
		      "/std/diewarzau/obj/misc/dbridge1");
      ob = new(room_info["draw bridge"]);
      ob->move(this_object());
      ob->setup(cname, r_type, rnum);
      set_short("The Entrance to "+capitalize(cname)+" Keep");
      add_exit("/d/damned/virtual/outer:"+cname+"_6.clan",
	       "west");
      add_exit("/d/damned/virtual/outer:"+cname+"_8.clan",
	       "east");
      add_exit("/d/damned/virtual/courtyard:"+cname+"_1.clan",
	       "north");
      break;
    case 8:
      set_room_info("cyard exit", "northwest");
      set_short("The southeast corner of "+capitalize(cname)+" Keep");
      add_exit("/d/damned/virtual/outer:"+cname+"_7.clan",
	       "west");
      add_exit("/d/damned/virtual/outer:"+cname+"_5.clan",
	       "north");
      break;
    default:
      room_err();
      return;
    }
    set_long("To the "+room_info["cyard exit"]+", you see the tall stone "
	     "walls of "+capitalize(cname)+" Keep.  You stand in "
	     "a short grassy field which surrounds the castle.\n"+
	     room_info["extra long"]);
  } else {
    room_err();
  }
  if(room_info && room_info["extra exits"])
    map_mapping(room_info["extra exits"],
		(: add_exit($2, $1) :));
  return;
}

void set_board_location(object ob) {
  ob->set_location(file_name(this_object()));
  return;
}
 
void init() {
  string action;
  mixed func;

  ::init();
  if(room_info && room_info["actions"]) {
    foreach(action, func in room_info["actions"]) {
      if(stringp(func))
	add_action(func, action);
      else if(pointerp(func))
	add_action("clan_cmd", action);
    }
  }
  if(room_type == "outer" && this_player()->is_player() &&
      (string)this_player()->query_guild() != clan_name) {
    map_array(filter_array(all_inventory(load_object(
        sprintf("/d/damned/virtual/courtyard:%s_1.clan", clan_name))),
        (: living :)), (: call_other($1, $2, $1) :), "intruder_alert");
  }
  return;
}

/*

int help_clan(string str) {
  if(str != "clan") return 0;
  if((string)this_player()->query_name() ==
     (string)CLAN_D->clan_leader(clan_name)) {
    write("You are the Clan Leader.  You may do the following:\n"+
	  repeat_string("-=", 35));
    write("admit <player>.............Allows <player> into your guild.\n"
	  "                             He/she must be present.\n"
	  "list members...............List the entire membership of\n"
	  "                             your clan.\n"
	  "setpriv <who> <priv>.......Toggle on/off a particular\n"
	  "                             privelege for <who>.\n"
	  "                             (must be present).\n"
	  "Currently supported privileges:\n"
	  "allow......................A player with this privilege\n"
	  "                             may allow people to join the \n"
	  "                             clan.\n"
	  "noenter....................The drawbridge will not auto-\n"
	  "                             matically open for this member.\n"
	  "nolearn....................A player with this 'privilege' may\n"
	  "                             NOT learn in the teaching room.\n"
	  "drawbridge.................A player with this privilege may\n"
	  "                             raise and lower the draw bridge\n"
	  "                             at will.\n");
    return 1;
  }
  else {
    if(test_member_priv(clan_name, (string)this_player()->query_name(),
			ALLOW_JOIN)) {
      write("admit <player>\n"
	    "Admits <player> into the clan.  He/she must be present.");
    }
    if(test_member_priv(clan_name, (string)this_player()->query_name(),
			DBRIDGE)) {
      write("You may also raise/lower the drawbridge at will.  "
	    "'look at drawbridge' to see how to do that.");
    }
    if(!test_member_priv(clan_name, (string)this_player()->query_name(),
			 DBRIDGE) &&
       !test_member_priv(clan_name, (string)this_player()->query_name(),
			 ALLOW_JOIN)) {
      write("You can perform no special commands.\n");
    }
  }
  return 1;
}
*/

int clan_cmd(string arg) {
  object ob;
  mapping actions;
  string *cmdv, cmd;

  if(!(cmd=query_verb())) return 0;
  if(!room_info || !room_info["actions"])
    return 0;
  actions = copy(room_info["actions"]);
  if(!actions[cmd] || !pointerp(actions[cmd])) return 0;
  return call_other(actions[cmd][0], actions[cmd][1], arg);
}
