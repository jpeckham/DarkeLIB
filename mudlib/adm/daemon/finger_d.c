/*  /bin/daemon/fingerd.c
 *  DarkeMud finger daemon
 *  rewritten from the TMI mudlib coded by
 *  Sulam, Jubal, Brian, and Buddha (12.8.91 - 3.8.92)
 *  Zortek
 */
#include <objects.h>
#include <dirs.h>
#include <daemons.h>
#include <config.h>
 
inherit OB_USER;//why does the daemon inherit from a user?
string true_name;
object ob;
 
string alig_str(string what, int x);
string query_title();
string query_last_on();
string query_true_name();
string get_idle_string(object ob, int verbose);
 
void create() { seteuid(getuid()); }
 
protected void complete_variables() {
  if(!true_name) true_name = "";
  if(!real_name) real_name = "???";
  if(!last_on) last_on = "no record";
  if(!ip) ip = "nowhere";
  if(!level) level = -1;
  if(!race) race = "Unborn";
  if(!player_age) player_age = 0;
}
 
string general_finger_display() {
  string finger, name, rname, level, idle, email;
  object *who;
  int i, amt;
  
  finger =  "%^BOLD%^        Active Finger List for "+mud_name()+"\n";
  finger += "World has been up since "+ctime(time()-uptime())+" CST\n";
  finger += "%^BLUE%^---------------------------------------------------------------------\n";
  finger += "%^CYAN%^Character       Player               Email                      Idle\n";
  finger += "%^BLUE%^---------------------------------------------------------------------%^RESET%^\n";
  who = users();
  for(i=0; i<sizeof(who); i++) {
    if(who[i]->query_invis()) continue;
    if(!who[i]->query_name()) continue;
    name = (string)who[i]->query_cap_name();
    rname= (string)who[i]->query_rname();
     if(wizardp(this_player())) 
      email = (string)who[i]->query_email();
     else 
      email = "censored";  
    idle = query_idle_string(who[i], 0);
    finger += sprintf("%-15s %-17s %-23s %-5s\n", name, rname, email, idle);
  }
  finger += "%^BLUE%^----------------------------------------------------%^RESET%^\n";
  return finger;
}
 
string user_finger_display(string who) {
  mapping mail_stat;
  string tmp, pos, stuff, finger, level, tmp9;
  
  if(!who)
    return "You must specify whom you wish to finger.\n";
  true_name = lower_case(who);
  tmp = true_name;
  who = sprintf("%s/%s/%s", DIR_USERS, tmp[0..0], tmp);
  if(file_size(who + ".o") < 0) 
    return "That character does not exist in Darke.\n";
  restore_object(who);
  mail_stat = (mapping)LOCALPOST_D->mail_status(tmp);
  if(ob) {
    finger =  "%^BLUE%^-------------------------------------------------%^RESET%^\n";
    finger += "%^BOLD%^"+sprintf("%|47s", query_title())+"%^RESET%^\n";
    finger += "%^BLUE%^-------------------------------------------------%^RESET%^\n";
    
    if(ob->query_description()) 
      finger += wrap(capitalize(query_true_name())+" "+(string)ob->query_description(),48) +"\n";
    else
      finger += "%^BOLD%^"+capitalize(query_true_name())+" is too lazy to set a description!%^RESET%^\n";
    finger += "%^BLUE%^-------------------------------------------------%^RESET%^\n";
    stuff = ob->query_position();

    switch(stuff) {
      case "superuser": level = "%^BOLD%^RED%^God Wizard%^RESET%^"; break;
      case "admin" :    level = "%^BOLD%^CYAN%^Admin Wizard%^RESET%^"; break;   
      case "assist":    level = "%^BOLD%^YELLOW%^Arch Wizard%^RESET%^"; break;
      case "mentor":    level = "%^BOLD%^Mentor Wizard%^RESET%^"; break;
      case "elder":     level = "%^BOLD%^GREEN%^Elder Wizard%^RESET%^"; break;
      case "wizard":    level = "%^BOLD%^BLUE%^Wizard%^RESET%^"; break;
      case "player":    level = "Mortal"; break;
      default:          level = "Unknown";
    }
    if(archp(this_player()) && !wizardp(ob))
      level = sprintf("%d", ob->query_level());
    finger += "%^BOLD%^CYAN%^Darke Position:\t\t%^RESET%^"+level+"\n";
    finger += "%^BOLD%^CYAN%^Player (Real Life):\t%^RESET%^"+(string)ob->query_rname()+"%^RESET%^\n";
    if(this_player() && archp(this_player()))
      if((stuff = (string)ob->query_email()) != "")
        finger += "%^BOLD%^CYAN%^Email:\t\t\t%^RESET%^"+stuff+"\n";   
    finger += "%^BOLD%^CYAN%^Date of Birth:\t\t%^RESET%^"+ctime(ob->query_birthday())+"\n";
    if(ob->query_invis()) {
      finger += "%^BOLD%^CYAN%^Last on:\t\t%^RESET%^";
      finger += query_last_on()+"\n";
    }
    else {
      finger += "%^BOLD%^CYAN%^Logged in since:\t%^RESET%^";
      finger += query_last_on()+"\n";
      stuff = get_idle_string(ob, 1);
      if(stuff != "") 
        finger += "\t%^BOLD%^RED%^"+stuff+"%^RESET%^\n";
    }
    if(this_player() && wizardp(this_player())) {
      finger += "%^BOLD%^CYAN%^Birth IP :\t\t%^RESET%^"+(string)ob->query_first_site()+"\n";
      finger += "%^BOLD%^CYAN%^Recent IP:\t\t%^RESET%^"+(string)ob->query_ip()+"\n";
    }
    finger += "%^BLUE%^-------------------------------------------------%^RESET%^\n";
    finger += query_cap_name() + " has ";
    if(!mail_stat["unread"]) 
      finger += "completely 'read' all of "+ob->query_possessive()+" mail.\n";
    else if(mail_stat["unread"] == 1) 
      finger += "merely a single, lonely letter unread.\n";
    else 
      finger += mail_stat["unread"] + " unread pieces of mail.\n";
    tmp = user_path(tmp);
    if(file_size(tmp + ".project") >= 0) 
      finger += "Project: "+read_file(tmp+".project");
    if(wizardp(ob)) {
      finger += "%^BLUE%^-------------------------------------------------%^RESET%^\n";
      if(file_size(tmp+".plan") >= 0) 
        finger += "Plan:\n"+read_file(tmp+".plan");
      else 
        finger += "%^BOLD%^"+capitalize(ob->query_name())+" is to lazy to create a plan.%^RESET%^\n";
    }
    finger += "%^BLUE%^-------------------------------------------------%^RESET%^\n";
  }
  else {
    complete_variables();
    true_name = tmp;
    finger =  "%^BLUE%^-------------------------------------------------%^RESET%^\n";
    finger += "%^BOLD%^"+sprintf("%|47s", query_title())+"%^RESET%^\n";
    finger += "%^BLUE%^-------------------------------------------------%^RESET%^\n";
    if(query_description()) 
      finger += wrap(capitalize(query_true_name())+" "+query_description(),48) +"\n";
    else
      finger += "%^BOLD%^"+capitalize(query_true_name())+" is too lazy to set a description!%^RESET%^\n";
    finger += "%^BLUE%^-------------------------------------------------%^RESET%^\n";

    stuff = query_position();
    switch(stuff) {
      case "superuser": level = "%^BOLD%^RED%^God Wizard%^RESET%^"; break;
      case "assist":    level = "%^BOLD%^YELLOW%^Arch Wizard%^RESET%^"; break;
      case "mentor":    level = "%^BOLD%^Mentor Wizard%^RESET%^"; break;
      case "elder":     level = "%^BOLD%^GREEN%^Elder Wizard%^RESET%^"; break;
      case "wizard":    level = "%^BOLD%^BLUE%^Wizard%^RESET%^"; break;
      case "player":    level = "Mortal"; break;
      default:          level = "Unknown";
    }
    finger += "%^BOLD%^CYAN%^Darke Position:\t\t%^RESET%^"+level+"\n";
    finger += "%^BOLD%^CYAN%^Player (Real Life):\t%^RESET%^"+query_rname()+"%^RESET%^\n";
    if(this_player() && archp(this_player()))
      if((stuff = query_email()) != "")
        finger += "%^BOLD%^CYAN%^Email:\t\t\t%^RESET%^"+stuff+"\n";   
    finger += "%^BOLD%^CYAN%^Date of Birth:\t\t%^RESET%^"+ctime(query_birthday())+"\n";
    finger += "%^BOLD%^CYAN%^Last on:\t\t%^RESET%^";
    finger += query_last_on()+"\n";
    if(this_player() && wizardp(this_player())) {
      finger += "%^BOLD%^CYAN%^Birth IP :\t\t%^RESET%^"+query_first_site()+"\n";
      finger += "%^BOLD%^CYAN%^Recent IP:\t\t%^RESET%^"+query_ip()+"\n";
    }
    finger += "%^BLUE%^-------------------------------------------------%^RESET%^\n";
    finger += query_cap_name() + " has ";
    if(!mail_stat["unread"]) 
      finger += "completely 'read' all of "+query_possessive()+" mail.\n";
    else if(mail_stat["unread"] == 1) 
      finger += "merely a single, lonely letter unread.\n";
    else 
      finger += mail_stat["unread"] + " unread pieces of mail.\n";
    tmp = user_path(tmp);
    finger += "%^BLUE%^-------------------------------------------------%^RESET%^\n";
    if(file_size(tmp + ".project") >= 0) { 
      finger += "Project: "+read_file(tmp+".project");
      finger += "%^BLUE%^-------------------------------------------------%^RESET%^\n";
    }
    if(file_size(tmp+".plan") >= 0) {
      finger += "Plan:\n"+read_file(tmp+".plan");
      finger += "%^BLUE%^-------------------------------------------------%^RESET%^\n";
    }

  }
  return finger;
}
 
string get_idle_string(object ob, int verbose) {
  int time_sec, tmp;
  string line;
 
  if(!ob) return 0;
  if(!interactive(ob)) {
  if(verbose) return "Disconnected.";
  else return "disconnected";
  }
  line = "";
   time_sec = query_idle(ob);
   if(time_sec < 60) return line;
  tmp = time_sec / 86400;
  if(tmp) {   
  if(!verbose) return tmp + " d";
  else line += tmp + " day" + (tmp>1 ? "s " : " ");
  }
  tmp = (time_sec%86400)/3600;
  if(tmp) {   
    if(!verbose) {
  if(tmp>9) return tmp+" h";
  else line += tmp+":";
    }
  else line += tmp + " hour"+(tmp>1 ? "s " : " ");
   }
   tmp = (time_sec%3600)/60;
   if(tmp) {   
   if(!verbose) {  
    if(strlen(line)>0 && tmp < 10) line += "0";
    line += sprintf("%d", tmp);
  }
  else line += sprintf("%d", tmp) + " minute"+(tmp>1 ? "s " : " ");
  }
  if(verbose) line += "idle time";
  else line += "idle";
  return line;
}
 
string query_last_on() { return last_on; }
 
string query_title() {
  string pre, post;
  if(!getenv("TITLE"))
    return query_cap_name();
  if(sscanf(getenv("TITLE"), "%s $N %s", pre, post) != 2)
  {
      if(sscanf(getenv("TITLE"), "$N %s", post) != 1)
        return capitalize(query_true_name());
      else
        return query_cap_name()+" "+post;
  }
  return pre+" "+capitalize(query_true_name())+" "+post;
}
 
string alig_str(string what, int x) {
  int y;
   string out;
  
  out = "";
  if(!what) return 0;
  y = strlen(what);
  out += what;
  if(y>x) {
  out = what[0..x-1] + " ";
  return out;
  }
  x = x-y;
  if(x<1) return out;
  for(y=0; y<=x; y++) out += " ";
  return out;
}
 
string query_cap_name() { return capitalize(true_name); }
string query_true_name()
{
  return true_name;
}
