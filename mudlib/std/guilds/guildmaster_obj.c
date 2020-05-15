//    A generic guildmaster object to be held by PC guildmasters.  It may be inheritd by other
//    objects, 
//
//    DarkeLIB 0.1
//    -Diewarzau 8/8/95
inherit "/std/Object";

nosave string join_room;
string owner;
int summonuse;

void set_join_room(string str);
void make_me_master(object who);
int transport(string str);
void moving(mixed *inf);
int summon(string str);

void set_join_room(string str) { join_room = str; }

string query_join_room() { return join_room; }

void init_arg(mixed *arg) {
  set_join_room(arg[0]);
  owner = arg[1];
  return;
}

void create() {
  ::create();
  set_property("no steal", 1);
  return;
}

mixed *query_auto_load() { 
  string file_name;

  file_name = base_name(this_object());
  return ({ file_name, ({ join_room, owner }) });
}

int drop() { return 1; }

void init() {
  if(this_player()->is_player() && environment() &&
     environment() == this_player() &&
     (string)this_player()->query_name() !=
     (string)join_room->query_master()) {
    remove();
    return;
  }
  else if(wizardp(this_player()) && (string)join_room->query_master() ==
    (string)this_player()->query_name()) {
    write("%^RED%^You are a wizard, and have been removed from the guildmaster position.");
    join_room->reset_master();
    remove();
    return;
  }
  if(owner && owner == (string)this_player()->query_name()) {
    add_action("allow_player", "allow");
    add_action("kick_player", "banish");
    add_action("list_membership", "list");
    add_action("help", "help");
    add_action("set_privs", "setpriv");
    add_action("clean_guild", "clean");
    add_action("transport", "transport");
    add_action("summon", "summon");
      add_action("pretitle_change", "pretitle");
      add_action("pretitle_list", "listpretitles");
}
  else call_out("rid_gm_ob", 20);
}

int summon(string str) {
   object who;
   str = lower_case(str);
   if( this_player()->query_property("UNPURE") )
      return notify_fail("Your knowledge of life has been contaminated. "+
        "You may not use this power.\n");
   if(!str) return notify_fail("You must specify a guildmember to summon.\n");
   if(!join_room->is_member(str)) return notify_fail("You may only summon "+
      "members of your guild.\n");
   if(time() < (summonuse + 600) ) return notify_fail("You may only use this "+
      "ability once every 10 minutes.\n");
   who=find_living(str);
   if(!who || !interactive(who) ) return notify_fail("Either that person does "+
      "not exist, or is link-dead.  Try again later.\n");
   if(environment(who)->query_property("no summon") ||
      environment(this_player())->query_property("no teleport") ) {
      summonuse = time();
      return notify_fail("Your attempt to summon "+capitalize(str)+" fails.\n");
   }
   summonuse = time();
   message("info", "%^GREEN%^%^BOLD%^You are magically transported to "+
          "the presence of your Guildmaster.", who);
   who->move_player(environment(this_player()), 0);
   return 1;
}   

int transport(string str) {
   mixed *inf;
   if( this_player()->query_property("UNPURE") )
      return notify_fail("Your knowledge of life has been contaminated. "+
        "You may not use this power.\n");
   if(sizeof(this_player()->query_attackers()))
      return notify_fail("You may not transport during combat.");
   if(!str) return notify_fail("You must specify home or chamber.\n");
   if(str == "home" )
      str = "/d/damned/guilds/join_rooms/"+(string)this_player()->query_class()+
         "_join.c";
   else if(str == "chamber")
      str = "/d/damned/akkad/ak_chamber.c";
   else return notify_fail("You must specify a destination of home or chamber.\n");
   message("info","You concentrate on your symbol of power and prepare to transport.",
      this_player());
   inf = ({ (str), (this_player()) });
   call_out("moving",15,inf);
   return 1;
}

void moving(mixed *inf) {
   object who;
   who = inf[1];
   if(!who->query_invis()) {
      message("info",who->query_cap_name()+" begins to glow brightly "+
         "and is gone when the glow fades.",environment(who));
      message("info",who->query_cap_name()+" shimmers into existance.",
         find_object((string)inf[0]));
   }
   message("info","You are transported elsewhere.",who);
   who->move((string)inf[0]);
   return;
}


      
int set_privs(string str) {
  string player, priv;

  if(sscanf(str, "%s for %s", priv, player) != 2)
    return notify_fail("Usage: setpriv <privilege> for <player>\n");
  if(!join_room) {
    write("BUG!  Couldn't find the join room");
    return 1;
  }
  player = lower_case(player);
  if(!join_room->is_member(player)) {
    write(capitalize(player) + " is not a member of your guild.");
    return 1;
  }
  if(member_array(priv, (string *)join_room->query_member_privs(player))
     >= 0) {
    write("Removing privilege '"+priv+"'.");
    join_room->remove_member_priv(player, priv);
    return 1;
  }
  write("Adding privilege '"+priv+"'.");
  join_room->add_member_priv(player, priv);
  return 1;
}

int clean_guild(string str) {

  if(!join_room) {
    write("BUG!  Couldn't find the join room");
    return 1;
  }
  join_room->clean_members();
  return 1;
}

void rid_gm_ob() {
  if(!environment()) remove();
  if((string)environment()->query_name() != owner) remove();
  return;
}

int list_membership(string str) {
  string *mem, list;
  int i;

  if(!str || str != "members") return 0;
  if(!join_room) {
    write("BUG!  Can't find the join_room.");
    return 1;
  }
  mem = (string *)join_room->query_all_members();
  for(i=0;i<sizeof(mem);i++) mem[i] = capitalize(mem[i]);
  list = "Membership list:\n---------------------------------------------------------------\n"+
	format_page(mem, 3);
	this_player()->more(explode(list, "\n"));
  return 1;
}

int kick_player(string str) {

  if(!str) return 0;
  str = lower_case(str);
  if(!join_room) {
    write("BUG!  Couldn't find the join room");
    return 1;
  }
  join_room->gm_kick(str);
  return 1;
}


int help(string str) {
  string help_str;
  
  if(str != "guildmaster") return 0;
  help_str =
 	  "allow <player>\n"+
	  "\tAllows <player> to join your guild.\n"+
	  // "\ndismiss <player>\n"+
	  //"\tKicks <player> out of your guild.  The player does NOT have to be on to do this.\n"+
	  "\nlist members\n"+
	  "\tGet a complete list of guild membership.\n"+
	  "\nhelp guildmaster\n"+
	  "\tThis help.\n" +
	  "\nsetpriv <privilege> for <player>\n"+
	  "\tGives adds or removes <privilege> from <player>.\n"+
	  "Current privileges:\n"+
	  "'treasury restrict'  - Bars the player from entering the treasury\n"
	  "'bad standing' - puts the player in bad standing with the guild.  "
	  "All skills and spells are 3x cost.\n"
	  "'allow join' - other players in the guild may allow players to join "
          "'council' - appoints that player to the guild council "
	  "\nPURITY ABILITIES - These only work if you are a pure guildmember.\n"+
	  "summon <name>\n"+
	  "\tAllows you to summon a guildmember to your location.  May only be\n"+
	  "\tused once every 10 minutes.\n"
    "\ntransport <location>\n"+
     "\tAllows you to transport to your <home> (joinroom) or <chamber>.\n"+
     "\tThis will not work if you are in combat and there is a 15 second delay.\n\n";
	  this_player()->more(explode(help_str, "\n"));
  return 1;
}

void override_set_owner(string str) {
  if(!archp(this_player())) return;
  owner = str;
  return;
}

void make_me_master(object who) {
  if(!join_room) return;
if(who->query_guild())
{
	write("You are in a clan, and cannot become guildmaster");
	remove();
	return;
}
  if(join_room->set_master((string)who->query_name())) {
    owner = (string)who->query_name();
    who->save_player(owner);
    this_object()->move(this_player());
  }
  else remove();
  return;
}

int allow_player(string str) {
  object who;

  if(!str) return 0;
  who = present(lower_case(str), environment(this_player()));
  if(!who) {
    write(capitalize(str)+" isn't here.");
    return 1;
  }
  if(who->query_class() && (string)who->query_class() != "child") {
    write(capitalize(str)+" is already the member of a guild.");
    return 1;
  }
  if(!join_room) {
    write("BUG... join_room not found.");
    return 1;
  }
  join_room->set_member_status((string)who->query_name(), "not joined");
  write(capitalize(str) + " may now join your guild.");
  message("info", "You may now join the guild.", who);
  return 1;
}


int pretitle_change(string str) {
  string type, title, guild;
  guild = (string)this_player()->query_class();

  if(sscanf(str, "%s %s", type, title) ==2){
     write_file( "/data/pretitles/"+guild+"/"+type+".pt", title, 1 );
     write("New Pretitle Set.");
     return 1;
  }
  write("Syntax: pretitle <type> <pretitle>");
  return 1;
}

int pretitle_list() {
   string guild;
   string novice, apprentice, journeyman, craftsman, artisan, expert, master, adept, grand;
   
   guild=this_player()->query_class();
   if(file_size("/data/pretitles/"+guild+"/novice.pt") > -1){
        novice = read_file( "/data/pretitles/"+guild+"/novice.pt", 1, 1 );
        novice = replace_string(novice, "\n", "");

   }
   else {
        novice = "Novice";
   }
   if(file_size("/data/pretitles/"+guild+"/apprentice.pt") > -1){
        apprentice = read_file( "/data/pretitles/"+guild+"/apprentice.pt", 1, 1 );
        apprentice = replace_string(apprentice, "\n", "");

   }
   else {
        apprentice = "Apprentice";
   }
   if(file_size("/data/pretitles/"+guild+"/journeyman.pt") > -1){
        journeyman = read_file( "/data/pretitles/"+guild+"/journeyman.pt", 1, 1 );
        journeyman = replace_string(journeyman, "\n", "");

   }
   else {
        journeyman = "Journeyman";
   }
   if(file_size("/data/pretitles/"+guild+"/craftsman.pt") > -1){
        craftsman = read_file( "/data/pretitles/"+guild+"/craftsman.pt", 1, 1 );
        craftsman = replace_string(craftsman, "\n", "");

   }
   else {
        craftsman = "Craftsman";
   }
   if(file_size("/data/pretitles/"+guild+"/artisan.pt") > -1){
        artisan = read_file( "/data/pretitles/"+guild+"/artisan.pt", 1, 1 );
        artisan = replace_string(artisan, "\n", "");

   }
   else {
        artisan = "Artisan";
   }
   if(file_size("/data/pretitles/"+guild+"/expert.pt") > -1){
        expert = read_file( "/data/pretitles/"+guild+"/expert.pt", 1, 1 );
        expert = replace_string(expert, "\n", "");

   }
   else {
        expert = "Expert";
   }
   if(file_size("/data/pretitles/"+guild+"/master.pt") > -1){
        master = read_file( "/data/pretitles/"+guild+"/master.pt", 1, 1 );
        master = replace_string(master, "\n", "");

   }
   else {
        master = "Master";
   }

write("%^GREEN%^------------------------------------------------------------");
write("%^BLUE%^            Pretitles for: "+capitalize(guild));
write("%^GREEN%^------------------------------------------------------------");
write("Novice: "+novice);  
write("Apprentice: "+apprentice); 
write("Journeyman: "+journeyman); 
write("Craftsman: "+craftsman); 
write("Artisan: "+artisan); 
write("Expert: "+expert); 
write("Master: "+master);
write("%^GREEN%^------------------------------------------------------------");
 
return 1;
}
