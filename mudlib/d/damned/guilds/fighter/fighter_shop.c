inherit "/std/vault";

nosave int open;

void reset() {
   ::reset();
   seteuid(getuid());
   if (!present("bob") && open) {
      new("/d/damned/guilds/fighter/big_bob")->move(this_object());
   }
}

void create() {
   ::create();
   set_property("indoors",1);
   set_property("light",3);
   add_exit("/d/damned/guilds/fighter/shop_store", "south");
   add_exit("/d/damned/guilds/join_rooms/fighter_join", "east");
   set_door("door", "/d/damned/guilds/join_rooms/fighter_join", "east",
	    "fighter shop key");
   set_open("door", 1);
   set_locked("door", 0);
   open = 1;
   set_door("iron door", "/d/damned/guilds/fighter/shop_store", "south",
	    "fighter store key");
   set_open("iron door", 0);
   set_locked("iron door", 1);
   set_lock_level("iron door", 22);
   set("short","The Fighters' Shop");
   set("long",
"Several weapons adorn the walls of the fighters' shop.  You spy a sign on "
"the wall.  The main guild room is to the east.");
   set_items( (["sign":"The say reads: 'say bob, help' for assistance."]) );
   set_open_function("open_door", "7:00:00");
   set_close_function("close_door", "16:00:00");
}

void open_door() {
  set_locked("door", 0);
  set_open("door", 1);
  call_other("/d/damned/guilds/join_rooms/fighter_join", "set_locked",
	     "door", 0);
  call_other("/d/damned/guilds/join_rooms/fighter_join", "set_open",
	     "door", 1);
  open = 1;
  this_object()->reset();
  return;
}

void close_door() {
  object ob, *inv;
  int i;
  
  ob = present("bob", this_object());
  if(ob) ob->remove();
  inv = all_inventory(this_object());
  i = sizeof(inv);
  while(i--) {
    if(living(inv[i])) {
      message("info", "The shop is closing now.  You must leave.", inv[i]);
      inv[i]->move(find_object_or_load("/d/damned/guilds/join_rooms/fighter_join"));
    }
  }
  set_open("door", 0);
  set_locked("door", 1);
  call_other("/d/damned/guilds/join_rooms/fighter_join", "set_open", 
		"door", 0);
  call_other("/d/damned/guilds/join_rooms/fighter_join", "set_locked", 
		"door", 1);
   open = 0;
}


