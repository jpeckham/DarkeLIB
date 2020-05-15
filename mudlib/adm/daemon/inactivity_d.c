/*	Player inactivity daemon.  Checks for what players haven't logged
 *	on for a cretain amount of time, and DELETES THEIR ASS.
 *	Admins can change things such as time till delete, what players
 *	are immune, and what positions are immune.
 *	DarkeLIB 0.1
 *      Diewarzau 4/1/94
 */

#include <security.h>
#include <clock.h>
#include <daemons.h>
#include <dirs.h>
#include <objects.h>

private void save_me();
void check_inactivity();
mapping remove_info;

void create() {
  seteuid(getuid());
  seteuid(UID_SECURE_DAEMONSAVE);
  if(file_exists(DIR_SECURE_DAEMONS_SAVE+"/inactivity.o"))
    restore_object(DIR_SECURE_DAEMONS_SAVE+"/inactivity");
  else
    remove_info = ([ "max player time" : MONTH * 3,
		   "max wiz time" : MONTH * 6,
		   "who protected" : ({}),
		   "positions protected" : ({ "superuser", "assist" }) ]);
  if(!event_pending(this_object())) {
    check_inactivity();
    event("check_inactivity",DAY*4,0,1);
  }
  
  save_me();
  seteuid(getuid());
}

void set_max_player_time(int time) { remove_info["max player time"] =
				       time * MONTH;
save_me(); }

void set_max_wiz_time(int time) { remove_info["max wiz time"] =
				    time * MONTH;
save_me(); }

void add_protected_player(string who) {
  who = lower_case(who);
  remove_info["who protected"] = distinct_array(
						remove_info["who protected"] + ({ who }));
  save_me();
}

int remove_protected_player(string who) {
  who = lower_case(who);
  if(member_array(who, remove_info["who protected"]) < 0)
    return 0;
  remove_info["who protected"] -= ({ who });
  save_me();
  return 1;
}

void add_position_protected(string postn) {
  postn = lower_case(postn);
  remove_info["positions protected"] = distinct_array(
						      remove_info["positions protected"] + ({ postn }));
  save_me();
}

int remove_position_protected(string postn) {
  postn = lower_case(postn);
  if(member_array(postn, remove_info["positions protected"]) < 0)
    return 0;
  remove_info["positions protected"] -= ({ postn });
  save_me();
  return 1;
}

private void save_me() {
  seteuid(UID_SECURE_DAEMONSAVE);
  save_object(DIR_SECURE_DAEMONS_SAVE+"/inactivity");
  seteuid(getuid());
}


void check_inactivity() {
  mixed *user_dir;
  string *letters, postn, tmp;
  string em;
  int i, j, k;
  
  seteuid(UID_LOG);
  log_file("auto_remove","User inactivity check at "+ctime(time()) +":\n");
  seteuid(UID_USERSAVE);
  letters = get_dir(DIR_USERS+"/");
  i = sizeof(letters);
  while(i--) {
    if(file_size(DIR_USERS+"/"+letters[i]) != -2 ||
       strlen(letters[i]) > 1) continue;
    user_dir = get_dir(DIR_USERS+"/"+letters[i]+"/",-1);
    j = sizeof(user_dir);
    while(j--) {
      if(user_dir[j][1] == -2 || sizeof(user_dir[j]) < 3) continue;
      tmp = read_file(DIR_USERS+"/"+letters[i]+"/"+user_dir[j][0]);
      if(!tmp) continue;
      sscanf(tmp,"%*sposition \"%s\"%*s",postn);
      sscanf(tmp, "%*semail \"%s\"%*s", em);
      if(sscanf(tmp, "%*slevel %d%*s", k) == 3 && k <= 2)
        k = remove_info["max player time"] / 8;
      else if(postn == "player" || postn == "ambassador" || postn ==
	 "high mortal") k = remove_info["max player time"];
      else k = remove_info["max wiz time"];
      if(member_array(postn,remove_info["positions protected"]) > -1)
	continue;
      else if(member_array(replace_string(user_dir[j][0],".o",""),
			   remove_info["who protected"]) > -1) continue;
      else if((time() - user_dir[j][2]) < k) continue;
      else {
	EMAIL_D->unregister_email(em);
	rm(DIR_USERS+"/"+letters[i]+"/"+user_dir[j][0]);
	seteuid(UID_LOG);
	log_file("auto_remove",user_dir[j][0] + " ("+postn+
		 ") was auto-removed.\n");
	INFORM_D->do_inform("auto_removed_users","Info: "+
			    user_dir[j][0]+" ("+postn+") was just auto-removed.\n");
	seteuid(getuid()); }
    }
  }
  seteuid(getuid());
  return;
}
