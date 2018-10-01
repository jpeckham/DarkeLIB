/* FILE:    /adm/obj/master.c
 * CREDIT:  mudlib.n and lil (inspirational influence)
 * AUTHOR:  zortek@trekverse
 *
 * The master object is the central control-authority object for
 * mudlib security and access.  This object will be loaded by the
 * MudOS driver following a successful load of the simulated efun
 * file.  Critical information on the internal workings of this
 * object can be found in the man master_object file.
 */
#include <config.h>
#include "master.h"

/*
flag(4)                   Driver Applies                  flag(4)

NAME
       flag  -  handle  mudlib specific flags specified at driver
       startup

SYNOPSIS
       void flag( string );

DESCRIPTION
       This master apply is called for each command  line  option
       passed to the driver with the -f flag.

MudOS                       5 Sep 1994     
*/
void flag(string str)
{
  write( sprintf("SKIPPED: driver -f %s ignored\n", str));
}

/*
epilog(4)                 Driver Applies                epilog(4)

NAME
       epilog  - returns an array of the filenames of the objects
       to be preloaded.

SYNOPSIS
       string *epilog( int load_empty );
       void preload( string filename );

DESCRIPTION
       The driver calls  epilog()  in  master  after  the  master
       object  has  been loaded.  Mudlibs typically use epilog to
       initialize data structures in  master  (such  as  security
       tables  etc).   epilog()  should returns an array of file-
       names which correspond to objects that the mudlib wants to
       have  preloaded;  that  is, loaded before the first player
       logs in.  For each filename returned  in  the  array,  the
       driver will called preload(filename) in master.

       The variable 'load_empty' is non-zero if the -e option was
       specified when starting up the driver.  It can be used  as
       a signal to the mudlib to not load castles, etc.

SEE ALSO
       preload(4)

MudOS                       5 Sep 1994                          1
*/
string *epilog(int load_empty)
{
  if(load_empty)
    write("driver -e ignored. No load_empty option enabled in master.c");
  call_out("socket_preload", 5);
  return read_database(PRELOAD_DB);
}

void load_groups() {
  string *lines,
         *members,
         *names;
  string grp,
         str,
         inner_group;
  int i,
      k,
      x,
      max;

  groups = ([]);
  if(!(max=sizeof(lines=explode(read_file(GROUPS_DB), "\n")))) {
    write("ERROR: master.c->load_groups(): groups.db corruption.\n");
    message("info","Security error (10B3) detected...Aborting Process",users());
    shutdown();
    return;
  }
  for(i=0; i<max; i++) {
    if(!lines[i] || lines[i] == "" || lines[i][0] == '#') continue;
    if(sscanf(lines[i], "(%s): %s", grp, str) != 2) {
      write("ERROR: master.c->load_groups(): groups.db read failure on line"+(i+1)+".\n");
      message("info","Security error (10B4) detected...Aborting Process",users());
      shutdown();
      return;
    }
    members = ({});
    if(names = explode(str, " ")) {
      for(k=0; k<sizeof(names); k++) {
        if(sscanf(names[k], "(%s)", inner_group)) {
          if((x=member_array(inner_group, keys(groups))) != -1) {
            members += groups[inner_group];
          }
        }
        else {
          members += ({ names[k] });
//        message("info","Groups Integrity Check: "+names[k], users() );
        }
      }
      if(!sizeof(groups[grp] = members)) map_delete(groups, grp);
    } 
  }
}

//ZNOTE: Pospone rewrite of this function to Phase 5
string error_handler(mapping errs, int caught)
{
  int i,
      sz,
      flag;
  string log_str;
  object wiz;
  object hb_ob = 0;
  mapping *traceback;

  traceback = errs["trace"];
  if(!errs["object"]) return "";
  log_str = sprintf("\nError in object: %s (%s)\n",
    file_name(errs["object"]), ctime(time()));
  log_str += sprintf("> %s\n", errs["error"]);
  log_str += sprintf("File: %-30s line %d\n\nTraceback\n%70'-='s\n",
    errs["program"], errs["line"], "");
  sz = sizeof(traceback);
  flag = 0;
  for(i=0;i<sz;i++) {
    if(!traceback[i]["object"]) continue;
    if(!query_shadowing(traceback[i]["object"]) && traceback[i]["object"]->is_player() && wizardp(traceback[i]["object"]) &&
      traceback[i]["function"] == "cmd_hook") {
      flag++;
      wiz = traceback[i]["object"];
    }
    if(living(traceback[i]["object"]) && 
              traceback[i]["function"] == "heart_beat") {
      hb_ob = traceback[i]["object"];
    }
    else if((traceback[i]["file"] == "cmds/system/_call.c" ||
      traceback[i]["file"] == "cmds/system/_clone.c" ||
      traceback[i]["file"] == "cmds/system/_update.c" ||
      traceback[i]["file"] == "cmds/system/_cc.c") &&
      arrange_string(errs["error"], 24) != "*Error in loading object")
      flag++;
    else if(!query_shadowing(traceback[i]["object"]) &&
      traceback[i]["object"]->is_player() && wizardp(traceback[i]["object"]) &&
      traceback[i]["function"] == "move_player") {
      flag += 2;
      wiz = traceback[i]["object"];
    }
    else if(file_name(traceback[i]["object"]) == "/cmds/system/_call" &&
      traceback[i]["function"] == "resolv_str")
      flag -= 20;
    else if(i == (sz-2) && traceback[i]["file"] == "adm/simul_efun/get_object.c" &&
      arrange_string(errs["error"], 18) == "*Can't load object")
      flag -= 20;
    log_str += sprintf("%-4s %s() in %s\n", sprintf("%d:", i+1), traceback[i]["function"],
      file_name(traceback[i]["object"]));
    log_str += sprintf("     Program: %s\n     File: %28s line %d\n\n", traceback[i]["program"],
      traceback[i]["file"], traceback[i]["line"]);
  } 
  if(flag >= 2 && wiz) {
    if(!caught) wiz->more(explode(log_str, "\n"));
    else if(!wiz->query("error report"))
      wiz->set("error report", log_str);
    else wiz->set("error report", (string)wiz->query("error report") +
      log_str);
  }
  if(hb_ob && !caught)
    log_file("hb_error", repeat_string("-=", 36)+"\n"+
             sprintf("Heart beat error in: %s (%s)",
                     hb_ob->query_name(),
                     file_name(hb_ob))+ log_str+"\n");
  if(!caught)
    return log_str;
  else {
    log_file("caught_errors", log_str);
    return "";
  }
} 

int view_errors(object user) 
{
  if(wizardp(user)) return 1;
  return 0;
}

int valid_save_binary(string file)
{
  string *tmp;
  int sz;

//ZNOTE: Add error reporting on all return 0's
  if(!file || !stringp(file)) return 0;
  tmp = explode(file, "/");
  if(sizeof(tmp) && tmp[0] == "adm") return 0;
  if(sizeof(tmp) && tmp[(sizeof(tmp)-1)] == "tmp") return 0;
  if(sizeof(tmp) && tmp[0] == "wizards") return 1;
  if((sizeof(tmp) == 2 || sizeof(tmp) == 3) && tmp[0] == "std") return 1;
  if(sizeof(tmp) && tmp[0] == "daemon") return 1;
  if(sizeof(tmp) >= 4 && tmp[2] == "akkad") return 1;
  if(strlen(file) >= 17 && file[0..16] = "/d/damned/virtual")
  {
    sz = sizeof(tmp);
    if(replace_string(tmp[sz-1], "server", "") != tmp[sz-1]) return 1;
  }
    return 0;
}

void load_access() 
{
  string *lines,
         *borg;
  string str,
         dir,
         tmp;
  int i,
      j,
      maxi,
      maxj;

  access = ([]);
//ZNOTE: call ACCESS_DAEMON here Phase 4.
  lines = explode(read_file(ACCESS_DB), "\n");
  maxi = sizeof(lines);
  if(!maxi || !intp(maxi))
  {
  	error("Error in reading access database.\n");
    write("Error in reading access database.\n");
    shutdown();
    return;
  }
  for(i=0; i < sizeof(lines); i++) 
  {
    if(!lines[i] || lines[i] == "" || lines[i][0] == '#') 
      continue;
    if(sscanf(lines[i], "(%s): %s", dir,tmp) !=2) 
    {
      error("Error in reading access database at line "+(i+1)+".\n");
      write("Error in reading access database at line "+(i+1)+".\n");
      shutdown();
      return;
    }
    borg = explode(tmp, " ");
    maxj = sizeof(borg);
    if(!maxj || !intp(maxj)) continue;
    access[dir] = ([]);
    for(j=0; j < sizeof(borg); j++)
    {
      if(sscanf(borg[j], "(%s)[%s]", tmp, str) != 2) 
      {
        write("Error in reading access database at line "+(i+1)+".\n");
        shutdown();
        return;
      }
      access[dir][tmp] = ({ 0, 0 });
      if(str == "r") 
        access[dir][tmp][READ] = 1;
      if(str == "w") 
        access[dir][tmp][WRITE] = 1;
      if(str == "rw" || str == "wr") 
        access[dir][tmp] = ({ 1, 1 });
    }
  }
}

void load_privs() {
    string *lines;
    string obj, euid;
    int i, max;

    privs = ([]);
    if(!(max = sizeof(lines=explode(read_file(PRIVS_DB), "\n")))) {
  write("Error in reading privs database.\n");
  shutdown();
  return;
    }
    for(i=0; i<max; i++) {
  if(!lines[i] || lines[i] == "" || lines[i][0] == '#') continue;
  if(sscanf(lines[i], "(%s): %s", obj, euid) != 2) {
      write("Error in reading privs database at line "+(i+1)+".\n");
      shutdown();
      return;
  }
  if(!sizeof(privs[obj] = explode(euid, " ")))
      map_delete(privs, obj);
    }
}

void preload(string str) {
    string err;
    if(!file_exists(str+".c")) 
    {
      write(sprintf("ERROR: Preload failed no file found: %s", str));
    }
    write(sprintf("Preloading: %s...\n", str));
    if(err=catch(call_other(str, "???")))
    write("\nGot error "+err+" when loading "+str+".\n");
}

void socket_preload() {
    string *items;
    int i, max;

    max = sizeof(items=explode(read_file(PRELOAD_SOCKET_DB), "\n"));
    for(i=0; i<max; i++) {
      if(!items[i] || items[i] == "" || items[i][0] == '#') 
      {
        //continue;
        //catch(call_other(items[i], "???"));
        write(sprintf("socket_preload begin - %s\n",items[i]));
        catch(call_other(items[i], "create"));
        write(sprintf("socket_preload end - %s\n",items[i]));
      }
    }
}

int valid_write(string file, object ob, string fun) {
    string tmp, name, whoisit;
    string *file_str;
    int i;

    while(strlen(file) > 2 && file[0..1] == "//")
  file = replace_string(file, "/", "", 1);
    if(file_str = explode(file,"/"))
  while(sizeof(file_str) && (i=member_array("",file_str)) > -1)
      file_str = exclude_array(file_str, i);
    if(check_access(file, ob, WRITE)) {
  if(!ob->is_player()) return 1;
  whoisit = ob->query_name(1);
  if(member_group(whoisit, "superuser")) return 1;
  if(file_str[sizeof(file_str)-1] == whoisit) return 1;
  if(file_str[sizeof(file_str)-1] == (lower_case(whoisit)+".o") &&
    geteuid(ob) != UID_USERSAVE)
      return 0;
  if(sscanf(file_str[sizeof(file_str)-1],"%s.%s",name,tmp) == 2 &&
    name == whoisit && tmp != "c" && tmp != "o")
      return 1;
  if(member_group(name,"superuser") && tmp == "o" && geteuid(ob) !=
    UID_USERSAVE) return 0;
  if((sscanf(file,"/wizards/%s/%s",name,tmp) != 2 ||
      name != whoisit) && ob && wizardp(ob))
      INFORM_D->do_inform("external_edits","Info: "+capitalize(
    whoisit)+" just wrote to " + file,({ ob }));
  return 1;
    } else return 0;
}

int valid_read(string file, object ob, string fun) {
    return check_access(file, ob, READ);
}

nomask int check_access(string file, object ob, int ind) {
    string *path, *grps;
    string euid, tmp, fn, tmp2;
    int i, j;

    while(strlen(file) > 2 && file[0..1] == "//")
  file = replace_string(file, "/", "", 1);
    if((euid=geteuid(ob)) == UID_ROOT) return 1;
    if(sscanf(file, user_path(euid)+"%s", tmp) ==1) return 1;
    if(sscanf(euid, "%sobj", tmp) == 1 && sscanf(file,user_path(tmp)+"%s",tmp)
      ==1) return 1;
    if(!access) load_access();
    if(!groups) load_groups();
    if(!privs) load_privs();
    fn = base_name(ob);
    if(sscanf(file, REALMS_DIRS+"/%s", tmp) ||
      sscanf(file, DOMAINS_DIRS+"/%s", tmp))
  if(groups["ambassador"] && member_array(euid, groups["ambassador"])
    != -1) return 0;
    if(!(path = explode(file, "/"))) path = ({});
    else while(sizeof(path) && (i=member_array("",path)) > -1)
      path = exclude_array(path, i);
    i = sizeof(path);
    while((i--) != -1) {
  if(i== -1) file = "/";
  else file = "/"+implode(path[0..(i)], "/");
  if(access[file]) {
      if(access[file]["all"] && access[file]["all"][ind] == 1) return 1;
      else if(access[file][euid]) return access[file][euid][ind];
      else {
    j = sizeof(grps = keys(access[file]));
    while(j--) {
        if(groups[grps[j]] && member_array(euid, groups[grps[j]]) != -1
          && access[file][grps[j]][ind] == 1) return 1;
    }
    return 0;
      }
  }
    }
    return 0;
}

object connect() {
    object ob;
    string err;

    if(err=catch(ob = clone_object(OB_LOGIN))) {
  write("It looks like someone is working on the user object.\n");
  write(err);
  destruct(this_object());
    }
    return ob;
}

mixed compile_object(string file) {
    return (mixed)VIRTUAL_D->compile_object(file);
}

static void crash(string err) {
    log_file("crashes", mud_name()+" crashed "+ctime(time())+" with error "+
      err+".\n");
    shout("Atmos tells you think "+mud_name()+" is crashing!\n");
    shout("Atmos forced you to: quit\n");
}

int valid_shutdown(string euid) {
    log_file("shutdowns", mud_name()+" was shut down by "+
      (previous_object() ? (euid=geteuid(previous_object())) : euid)+" "+
      ctime(time())+"\n");
    if(euid != UID_ROOT && euid != UID_SHUTDOWN) return 0;
    return 1;
}

int valid_seteuid(object ob, string id) {
    string fn, uid;
    string *dirs;
    int i;

    if((uid = getuid(ob)) == id) return 1;
    if(uid == UID_ROOT) return 1;
    if(file_name(ob) == OB_SIMUL_EFUN) return 1;
    if(uid == UID_SYSTEM && id != UID_ROOT && id != UID_BACKBONE)
  return 1;
    dirs = explode(base_name(ob), "/");
    if(sizeof(dirs) >= 3 && implode(dirs[0..2],"/") == "std/spells/shadows"
      && id == UID_SHADOW) return 1;
    fn = base_name(ob);
    if((i=strlen(fn)) > 7 && fn[i-7..i-1] == ".castle" && id == UID_DAMNED_DATA)
  return 1;
    if((i=strlen(fn)) > 7 && fn[i-5..i-1] == ".mine" && id == UID_DAMNED_DATA)
  return 1;
    if(!privs) load_privs();
    return (privs[fn=base_name(ob)] && (member_array(id, privs[fn]) != -1));
}

int valid_bind(object binder, object old_obj, object new_obj) {
    if(new_obj == binder) return 1;
    return 0;
}

int valid_shadow(object ob) {
    string tmp;
    object shadower;

    shadower = previous_object();
    if(!find_object(OB_SIMUL_EFUN)) return 0;
    if(ob == (object)master() || ob == (object)find_object(OB_SIMUL_EFUN)) return 0;
    if(geteuid(shadower) == UID_SHADOW) return 1;
    if(sscanf(base_name(shadower),"/std/spells/shadows/%s",tmp) == 1)
  return 1;
    if(ob->is_player()) {
  tmp = "User shadow: ";
  tmp += file_name(shadower)+"->";
  tmp += (string)ob->query_name(1);
  INFORM_D->do_inform("user_shadows",tmp);
  tmp+= "\nTime: " + ctime(time()) + "  Object euid: "+geteuid(shadower);
  log_file("shadows",tmp+"\n");
    } return 1;
}


int valid_snoop(object snooper, object snoopee) {
    if(!snoopee && snooper) return 1;
    if(archp(snooper) && !archp(snoopee)) return 1;
    if(archp(snooper) && member_group(getuid(snoopee), "assist")) return 1;
    if(snoopee->query_snoop()) {
#if 0
  tell_object(snoopee, "You are now being snooped by "+
    (string)snooper->query_cap_name()+".\n");
#endif
  return 1;
    }
    return 0;
}

int valid_exec(string nom) {
    if(nom[0] != '/') nom = "/"+nom;
    if(nom == OB_LOGIN+".c") return 1;
    return 0;
}

int valid_hide(object who) {
    if(!objectp(who)) return 0;
    return member_group(geteuid(who), "superuser") ||
    member_group(geteuid(who), "assist");
}

int valid_override(string file, string nom) {
    if(file == "/adm/simul_efun/overrides" || file == OB_SIMUL_EFUN) return 1;
    if(function_exists(nom, find_object(OB_SIMUL_EFUN))) return 0;
    return 1;
}

int valid_socket(object caller, string func, mixed *info) {
  if(func != "external") return 1;
  if(geteuid(caller) != UID_EXTERNAL) return 0;
  return 1;
}

int load_player_from_file(string nom, object ob) {
    int x;

    if(geteuid(previous_object()) != UID_ROOT) return 0;
    export_uid(ob);
    x = (int)ob->restore_player(nom);
    seteuid(nom);
    export_uid(ob);
    seteuid(UID_ROOT);
    return x;
}

void save_player_to_file(object ob) {
    string nom;
    int x;

    if(base_name(previous_object()) != OB_USER) return;
    if(!(nom = (string)ob->query_name())) return;
    seteuid(UID_ROOT);
    export_uid(ob);
    ob->actually_save_player(nom);
    seteuid(nom);
    export_uid(ob);
    seteuid(UID_ROOT);
}

string get_wiz_name(string file) {
    string nom, dir, tmp;

    if(file[0] != '/') file = "/"+file;
    if(sscanf(file, REALMS_DIRS+"/%s/%s", nom, tmp) == 2) return nom;
    if(sscanf(file, DOMAINS_DIRS+"/%s/%s", nom, tmp) == 2) return nom;
    sscanf(file, "/%s/%s", nom, tmp);
    return nom;
}

/* Tweaked this function to show the wizard the errors
 * just so it wouldn't be so annoying
 * - Geldron 030696
 */
void log_error(string file, string msg) {
    string nom, home;

    if(!(nom = get_wiz_name(file))) nom = "log";
    if(!catch(write_file(DIR_ERROR_LOGS + "/" + nom, msg)))
  if(this_player() && wizardp(this_player()))
      message("Nloading_error", msg, this_player());

}

void destruct_environment_of(object ob) {
    if(!interactive(ob)) return;
    tell_object(ob, "The world about you swirls into nothingness, as "+
      "you are quickly teleported somewhere else.\n");
    ob->move(ROOM_VOID);
}

string make_path_absolute(string file) {
    return resolv_path((string)this_player()->get_path(), file);
}

int player_exists(string str) {
    return file_exists(DIR_USERS+"/"+str[0..0]+"/"+str+".o");
}

string get_root_uid() { return UID_ROOT; }

string get_bb_uid() { return UID_BACKBONE; }

string creator_file(string str) {
       string str2;
       str2 = (string)call_other(OB_SIMUL_EFUN, "creator_file", str);
       if(str2 == 0) return UID_MUDLIB;//parnell says i donno wtf to return if it's 0 but mudlib sounds good
       else return str2;
     //  return (string)call_other(OB_SIMUL_EFUN, "creator_file", str); //old way returned 0 which caused NOT STRING errors
}

string domain_file(string str) {
    string nom, tmp;

    if(str[0] != '/') str = "/"+str;
    if(sscanf(str, DOMAINS_DIRS+"/%s/%s", nom, tmp) == 2) return nom;
    return (string)0;
}

string author_file(string str) {
    string nom, tmp;

    if(str[0] != '/') str = "/"+str;
    if(sscanf(str, REALMS_DIRS+"/%s/%s", nom, tmp) == 2) return nom;
    return 0;
}

static int slow_shutdown() {
    log_file("game_log", "Armageddon loaded by master: "+ctime(time())+".\n");
    new(OB_SHUT)->move(ROOM_START);
    SHUT_D->do_armageddon();
    return 1;
}

int save_ed_setup(object who, int code) {
    string file;

    if(!intp(code)) return 0;
    rm(file = user_path(getuid(who))+".edrc");
    return write_file(file, code+"");
}

int retrieve_ed_setup(object who) {
    string file;
    int x;

    if(!file_exists(file = user_path(getuid(who))+".edrc")) return 0;
    return atoi(read_file(file));
}

string get_ed_buffer_save_file_name(string file) {
    string str;

    return (file_size(str=user_path(geteuid(this_player()))) == -2 ?
      str+"dead.edit" : DIR_TMP+"/"+geteuid(this_player())+".edit");
}

int is_locked() { return MUD_IS_LOCKED;
}


int query_member_group(string who, string grp) {
    if(!groups) load_groups();
    if(groups[grp]) return (member_array(who, groups[grp]) != -1);
    else return 0;
}

mapping query_groups() {
    if(!groups) load_groups();
    return copy(groups);
}

