// Lets a wizard restart his player object, or change himself into another
// player, if he knows the passwords.

// Wizards with root access don't get prompted for passwords.

// Originally written by Sulam, 1-21-92. The conversion to mudlib 0.9
// didn't treat it kindly. Huma put a lot of work into it and got it
// working again, 11-15-92.
// Zortek fixed security hole left by Huma and hacked from TMI->NM
// 02.15.93
// June 17, 1993. Updated by Vargon the Insane and adapted for Rhovania.
// Zortek ported from Rhovan to TrekVerse 03.94
// Zortek ported from TrekVerse to DarkeMud 04.96

#include <objects.h>
#include <dirs.h>
#include <std.h>
#include <security.h>
#include <daemons.h>
#include <flags.h>
#define LOCKED 3

create() {
  seteuid(UID_ROOT);
  write(geteuid());
}

object *oplayer = ({ });
object *nplayer = ({ });
int *count = ({ });
string *name = ({ });
int *samep = ({ });

protected void get_password(string pass);
protected void enter_world();
protected int check_password(string pass);
protected void try_again();
protected void switch_player();
protected void remove_player();
private nosave string pname;

int temp;

int cmd_su(string nme) {
  if(!wizardp(this_player())) return 0;
  if(geteuid(previous_object()) != UID_SU) {
    write("You won't get anywhere that way.\n");
    log_file("su","/adm/daemon/su_d called by "+
      file_name(previous_object())+", player "+
      this_player()->query_name()+" "+ctime(time())+"\n");
      return 945;
  }
  if(!nme) {
    write(@NME
Allows you to switch users and load a new player object.

USAGE: su <name>

You must know the password of the player you are switching
to, unless you are re-entering yourself.
NME
);
    return 1;
  }
  if((temp=file_size(DIR_USERS+"/"+extract(nme,0,0)+"/"+nme+".o"))<0){
    notify_fail(temp+"Sorry, that player does not exist.\n");
    return 0;
  }
  if(member_array(this_player(),oplayer)>=0){
    notify_fail("Sorry, already in SU\n");
    return 0;
  }
  pname = nme;
  oplayer+=({this_player()});
  nplayer+=({0});
  count+=({0});
  name+=({ nme });
#if 0
  if(MULTI_D->query_prevent_login(nme)){
    destruct(this_player());
    return 1;
// Prevent players with multiple character having both on at once
  }
#endif
  if((string)this_player()->query_name()==nme){
    samep+=({1});
    switch_player();
    return 1;
  }
  if(!master()->query_member_group(this_player()->query_name(),"superuser" )){
    samep+=({0});
    write("Enter password: ");
    input_to("get_password", 3);
  }
  else {
    samep+=({1});
    switch_player();
  }
  return 1;
}

protected void get_password(string pass){
  int i;
  
  i = member_array(this_player(),oplayer);
  write("\n");
  if(pass==""){
    try_again();
    return;
  }
  if(!check_password(pass)){
    count[i]++;
    write("Sorry, wrong password.\n");
    if(count[i] > 3){
      remove_player();
      return;
    }
    write("Please reenter your password: ");
    input_to("get_password", 3);
    return;
  }
  switch_player();
  enter_world();
}

protected void enter_world() {
  int i;
  i = member_array(this_player(), oplayer);
  /*
   * log_file("ENTER", name+"\tenter\t\t" + ctime(time()) + 
   *   " from " + query_ip_name(player) + "\n");
   */
  if (i == -1) return;
  nplayer[i]->secure();
}

protected int check_password(string pass){
  int i;
  string password;
  i=member_array(this_player(),oplayer);
  oplayer[i]->save_player();
  nplayer[i]=new(OB_USER);
  master()->load_player_from_file(name[i],nplayer[i]);
  password=(string)nplayer[i]->query_password();
  if(password!=crypt(pass,password)){
    destruct(nplayer[i]);
    return 0;
  }
  return 1;
}

protected void try_again(){
  int i;
  i=member_array(this_player(),oplayer);
  if(nplayer[i]) destruct(nplayer[i]);
  remove_player();
  write("Sorry, that isn't a valid entry.\n");
}

protected void switch_player() {
  int pos, volume, capacity, err;
  pos=member_array(this_player(),oplayer);
  if(!nplayer[pos]){
    nplayer[pos] = new(OB_USER);
    oplayer[pos]->save_player();
    err=catch(master()->load_player_from_file(name[pos], nplayer[pos]));
  }
  if(err) {
    throw("switch_player(): "+err);
    return;
  }
  nplayer[pos]->set_name(pname);
  if(exec(nplayer[pos], oplayer[pos])){ 
    object *inv;
    int i;
    nplayer[pos]->setup();
    write("You are transferred.\n");
    nplayer[pos]->move_player(environment(oplayer[pos]),"somewhere");
    if (samep[pos]){
      volume=(int)oplayer[pos]->query("volume");
      capacity=(int)oplayer[pos]->query("capacity");
    }
    for(inv=all_inventory(oplayer[pos]);i<sizeof(inv);i++){
      if(!function_exists("query_auto_load",inv[i]))
        inv[i]->move(nplayer[pos]);
    }
    if(samep[pos]){
      nplayer[pos]->set("volume",volume);
      nplayer[pos]->set("capacity",capacity);
    }
    oplayer[pos]->save_player();
    oplayer[pos]->remove();
  }
  else{
    write("Error: exec()");
    nplayer[pos]->remove();
  }
}

protected void remove_player(){
  int i;
  
  i=member_array(this_player(),oplayer);
  oplayer=oplayer[0..i-1]+oplayer[i+1..sizeof(oplayer)-1];
  nplayer=nplayer[0..i-1]+nplayer[i+1..sizeof(nplayer)-1];
  name=name[0..i-1]+name[i+1..sizeof(name)-1];
  count=count[0..i-1]+count[i+1..sizeof(count)-1];
  samep=samep[0..i-1]+samep[i+1..sizeof(count)-1];
}

int help() {
  write (@HELP
Usage: <su [player]>

The su command allows you to change into another character, or a fresh
version of your own character. It is equivalent to logging out and logging
back in, but quicker. The most important thing that happens when you su
is that a new copy of the player object is cloned. This is useful if you
have just modified the player object and need to test the changes.
With an argument, su prompts you for the password of the player named
<player> and polymorphs you into that player. If you pass your
own name as argument, su does not ask for the password and you appear as
yourself in a new clone of the player object.
HELP
);
  return 1 ;
}
