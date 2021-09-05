/*
 * Locker Daemon v2.3
 *
 * The locker and saving items daemon.
 * Handles all functions associated with lockers and items
 * which save past reboots.
 *
 * Diewarzau 7/14/96
 * DarkeLIB 0.1
 */

#include <security.h>
#include <dirs.h>
#include <rooms.h>

class uil {
  string key;
  string bitlist;
}

nosave int count;
mapping all_lockers;
mapping *obj_register = allocate(10);
class uil *unique_id = allocate(10);
class uil *unreg_at_reboot = allocate(10);

void save_obj_register(int idx);
void save_all_lockers();
void save_unique_id();
class uil find_uar_list(string key);
class uil find_uid_list(string key);
int register_item(string key);
int *register_n_items(string key, int num);
private void manage_lockers();
int unregister_item(string key, int idx);
int unregister_n_items(string key, int *idx);
void save_unregistered(string key, int *idx);
private void update_objs();

/*
 * The following function is only called once to
 * establish downward compat. with the old locker sys.  
 * It may be deleted later.
 *
 * -Diewarzau
 */
 
void downward_compat() {
  class uil tuil;
  string *dir, file, key, tstr;
  int idx, i;
  mapping tmpor, tmp;

  seteuid(getuid());
  dir = get_dir("/adm/save/objects/*.o");
  foreach(file in dir) {
    if(sscanf(file, "%s_%d.o", key, idx) != 2) continue;
    if(!(tuil = find_uid_list(key))) {
      i = 0;
      if(unique_id[9]) {
	write("Unique id list is full.  Error.");
	return;
      }
      while(unique_id[i]) i++;
      unique_id[i] = new(class uil);
      unique_id[i]->key = key;
      unique_id[i]->bitlist = "";
      tuil = unique_id[i];
    }
    tuil->bitlist = set_bit(tuil->bitlist, idx);
  }
  save_unique_id();
  /*
  reset_eval_cost();
  seteuid(UID_SECURE_DAEMONSAVE);
  if(!file_exists(DIR_SECURE_DAEMONS_SAVE+"/obj_register.v")) {
    write("obj_register not found!");
    return;
  }
  tmpor = restore_variable(read_file(DIR_SECURE_DAEMONS_SAVE+
					    "/obj_register.v"));
  if(!(tuil=find_uid_list("obj"))) {
    i = 0;
    if(unique_id[9]) {
      write("Unique id list full...error!");
      return;
    }
    while(unique_id[i]) i++;
    unique_id[i] = new(class uil);
    unique_id[i]->key = "obj";
    unique_id[i]->bitlist = "";
    tuil = unique_id[i];
  } else
    tuil->bitlist = "";
  foreach(file, tmp in tmpor) {
    if(sscanf(file, "%*s_%d", idx) != 2) continue;
    tuil->bitlist = set_bit(tuil->bitlist, idx);
    idx %= 10;
    if(!obj_register[idx]) obj_register[idx] = ([]);
    obj_register[idx][file] = tmpor[file];
  }
  for(idx=0; idx < 10; idx++)
    save_obj_register(idx);
  save_unique_id();
  */
  seteuid(getuid());
  return;
}

class uil find_uid_list(string key) {
  int i = 0;

  while(i < 10 && unique_id[i]) {
    if(unique_id[i]->key == key)
      return unique_id[i];
    i++;
  }
  return 0;
}

class uil find_uar_list(string key) {
  int i;
  
  while(i < 10 && unreg_at_reboot[i]) {
    if(unreg_at_reboot[i]->key == key)
      return unreg_at_reboot[i];
    i++;
  }
  return 0;
}

void save_obj_register(int idx) {
  seteuid(UID_SECURE_DAEMONSAVE);
  if(file_exists(DIR_SECURE_DAEMONS_SAVE+sprintf("/obj_register_%d.v", idx)))
    rm(DIR_SECURE_DAEMONS_SAVE+sprintf("/obj_register_%d.v", idx));
  write_file(DIR_SECURE_DAEMONS_SAVE+sprintf("/obj_register_%d.v", idx),
    save_variable(obj_register[idx]));
  seteuid(getuid());
  return;
}

void save_all_lockers() {
  seteuid(UID_SECURE_DAEMONSAVE);
  if(file_exists(DIR_SECURE_DAEMONS_SAVE+"/all_lockers.v"))
    rm(DIR_SECURE_DAEMONS_SAVE+"/all_lockers.v");
  write_file(DIR_SECURE_DAEMONS_SAVE+"/all_lockers.v",
    save_variable(all_lockers));
  seteuid(getuid());
  return;
}

void save_unique_id() {
  seteuid(UID_SECURE_DAEMONSAVE);
  if(file_exists(DIR_SECURE_DAEMONS_SAVE+"/unique_id.v"))
    rm(DIR_SECURE_DAEMONS_SAVE+"/unique_id.v");
  write_file(DIR_SECURE_DAEMONS_SAVE+"/unique_id.v",
    save_variable(unique_id));
#if 0
  if(unreg_at_reboot[0]) {
    if(file_exists(DIR_SECURE_DAEMONS_SAVE+"/unreg_at_reboot.v"))
      rm(DIR_SECURE_DAEMONS_SAVE+"/unreg_at_reboot.v");
    write_file(DIR_SECURE_DAEMONS_SAVE+"/unreg_at_reboot.v",
	       save_variable(unreg_at_reboot));
  }
#endif
  seteuid(getuid());
  return;
}

int register_item(string key) {
  int spec;
  class uil tuil;
  int i;

  if(!(tuil = find_uid_list(key))) {
    if(unique_id[9]) {
      error("Cannot register file, uid list full!\n");
      return 0;
    }
    i = 0;
    while(unique_id[i]) i++;
    unique_id[i] = new(class uil);
    unique_id[i]->key = key;
    unique_id[i]->bitlist = "";
    tuil = unique_id[i];
  }
  spec = 1;
  while(test_bit(tuil->bitlist, spec)) spec++;
  tuil->bitlist = set_bit(tuil->bitlist, spec);
  if(tuil=find_uar_list(key)) {
    tuil->bitlist = clear_bit(tuil->bitlist, spec);
  }
  save_unique_id();
  return spec;
}

int *register_n_items(string key, int num) {
  int *spec = allocate(num);
  class uil tuil;
  int i, tmp;

  if(!(tuil = find_uid_list(key))) {
    if(unique_id[9]) {
      error("Cannot register file, uid list full!\n");
      return 0;
    }
    i = 0;
    while(unique_id[i]) i++;
    unique_id[i] = new(class uil);
    unique_id[i]->key = key;
    unique_id[i]->bitlist = "";
    tuil = unique_id[i];
  }
  for(i=0; i<num; i++) {
    while(test_bit(tuil->bitlist, tmp)) tmp++;
    tuil->bitlist = set_bit(tuil->bitlist, tmp);
    spec[i] = tmp++;
  }
  if(tuil = find_uar_list(key)) {
    for(i=0; i<num; i++) {
      tuil->bitlist = clear_bit(tuil->bitlist, spec[i]);
    }
  }
  save_unique_id();
  return spec;
}

int unregister_n_items(string key, int *idx) {
  int x;
  class uil tuil;

  if(!(tuil=find_uid_list(key))) return 0;
  foreach(x in idx) {
    if(!test_bit(tuil->bitlist, x)) continue;
    tuil->bitlist = clear_bit(tuil->bitlist, x);
  }
  save_unique_id();
}

int unregister_item(string key, int idx) {
  int x;
  class uil tuil;
  
  if(!(tuil=find_uid_list(key))) return 0;
  if(!test_bit(tuil->bitlist, idx)) return 0;
  tuil->bitlist = clear_bit(tuil->bitlist, idx);
  save_unique_id();
  return 1;
}

void save_unregistered(string key, int *idx) {
  int x, i;
  class uil uar;

#if 0
  if(!(uar=find_uar_list(key))) {
    i = 0;
    while(unreg_at_reboot[i]) i++;
    unreg_at_reboot[i] = new(class uil);
    unreg_at_reboot[i]->key = key;
    unreg_at_reboot[i]->bitlist = "";
    uar = unreg_at_reboot[i];
  }
  foreach(x in idx) {
    uar->bitlist = set_bit(uar->bitlist, x);
  }
  seteuid(UID_SECURE_DAEMONSAVE);
  if(file_exists(DIR_SECURE_DAEMONS_SAVE+"/unreg_at_reboot.v"))
    rm(DIR_SECURE_DAEMONS_SAVE+"/unreg_at_reboot.v");
  write_file(DIR_SECURE_DAEMONS_SAVE+"/unreg_at_reboot.v",
	     save_variable(unreg_at_reboot));
  seteuid(getuid());
#endif
  return;
}

void create() {
  int i;

  all_lockers = ([]);
  seteuid(UID_SECURE_DAEMONSAVE);
  if(file_exists(DIR_SECURE_DAEMONS_SAVE+"/all_lockers.v"))
    all_lockers = restore_variable(read_file(DIR_SECURE_DAEMONS_SAVE+
      "/all_lockers.v"));
  if(!this_player() && file_exists(DIR_SECURE_DAEMONS_SAVE+"/unique_id.v"))
    unique_id = restore_variable(read_file(DIR_SECURE_DAEMONS_SAVE+
					   "/unique_id.v"));
  if(!this_player() && file_exists(DIR_SECURE_DAEMONS_SAVE+
				   "/unreg_at_reboot.v")) {
      rm(DIR_SECURE_DAEMONS_SAVE+"/unreg_at_reboot.v");
  }
  for(i=0; i<10; i++) {
    if(file_exists(DIR_SECURE_DAEMONS_SAVE+
		   sprintf("/obj_register_%d.v", i)))
      obj_register[i] = restore_variable(read_file(DIR_SECURE_DAEMONS_SAVE+
						   sprintf("/obj_register_%d.v"
							   , i)));
  }
  if(!this_player() && unique_id[0]) {
    manage_lockers();
    unreg_at_reboot = allocate(10);
  }
  seteuid(getuid());
  return;
}

void debug_obj_register(int idx) {
  int oidx = idx%10;

  if(!obj_register[oidx]) {
    write("That obj_register index is zero.");
    return;
  }
  if(!obj_register[oidx][sprintf("locker_%d", idx)]) {
    write("That locker file is not found in obj_register #"+oidx);
    return;
  }
  write(sprintf("%O", obj_register[oidx][sprintf("locker_%d", idx)]));
  return;
}

void update_lockers(object env) {
  object *inv, me;
  string key, room_file;
  int i, num, j;
  string lfile, ofile;
  class uil tuil;

  if(!env) return;
  if(!env->query_is_locker_room()) return;
  room_file = file_name(env);
  inv = filter_array(all_inventory(env), (: call_other :),
		     "query_is_locker");
  if(!inv || !(i=sizeof(inv))) {
    map_delete(all_lockers, room_file);
    save_all_lockers();
    return;
  }
  me = previous_object();
  if(!me || !me->query_is_locker())
    me = 0;
  all_lockers[room_file] = ([]);
  while(i--) {
    key = (string)inv[i]->query_storage_key();
    if(!key) key = "locker";
    tuil = find_uid_list(key);
    num = inv[i]->register_num();
    if(!num) {
      num = register_item(key);
      inv[i]->set_register(num);
    }
    if(this_player() && archp(this_player()))
      write("key: "+key+"   num: "+num);
    if(!me || me == inv[i]) {
      inv[i]->save_me(sprintf("%s_%d", key, num));
      if(this_player() && archp(this_player()))
        write(sprintf("Saved: %s %d", key, num));
    }
    if(key == "pet" && !inv[i]->query_saved_here()) {
      if(this_player() && archp(this_player()))
        write("This pet NOT saved.");
      continue;
    }
    if(this_player() && archp(this_player()))
      write("Base name as saved: "+base_name(inv[i]));
    all_lockers[room_file][sprintf("%s_%d", key, num)] = base_name(inv[i]);
  }
  save_all_lockers();
  return;
}

int register_objects(object *inv, object ob) {
  int i, idx, oidx, lidx, *idxv;
  string key, l_file, fname;

  seteuid(getuid());
  if(!inv || !(i=sizeof(inv))) return 1;
  lidx = (int)ob->register_num();
  key = (string)ob->query_storage_key();
  if(!key) key = "locker";
  if(!lidx) {
    lidx = register_item(key);
    ob->set_register(lidx);
  }
  oidx = lidx%10;
  l_file = sprintf("%s_%d", key, lidx);
  if(!obj_register[oidx]) obj_register[oidx] = ([]);
  else if(obj_register[oidx][l_file]) {
    unregister_n_items("obj",
		       map_array(keys(obj_register[oidx][l_file]),
				 (: atoi(replace_string($1, $2, $3)) :),
				 "obj_", ""));
  }
  obj_register[oidx][l_file] = ([]);
  idxv = register_n_items("obj", i);

  while(i--) {
    idx = idxv[i];
    fname = base_name(inv[i]);
    if(!fname) sscanf(file_name(inv[i]), "%s#%*d", fname);
    obj_register[oidx][l_file][sprintf("obj_%d", idx)] = fname;
    if(file_exists(DIR_OBJECTS+sprintf("/obj_%d.o", idx))) return 0;
    seteuid(geteuid());
    if(!inv[i]->save_me(sprintf("obj_%d", idx))) return 0;
  }
seteuid(geteuid());
  save_obj_register(oidx);
  return 1;
}

int load_objects(object ob) {
  int lidx, i, idx, oidx, *idxv;
  object item;
  string *o_keys, l_file, key;

  seteuid(getuid());
  lidx = (int)ob->register_num();
  if(!lidx) return 1;
  oidx = lidx%10;
  key = (string)ob->query_storage_key();
  if(!key) key = "locker";
  l_file = sprintf("%s_%d", key, lidx);
  if(!obj_register[oidx] || !obj_register[oidx][l_file]) return 1;
  o_keys = keys(obj_register[oidx][l_file]);
  i = sizeof(o_keys);
  if(!i) {
    map_delete(obj_register[oidx], l_file);
    save_obj_register(oidx);
    return 1;
  }
  seteuid(getuid());
  idxv = allocate(i);
  while(i--) {
    item = new(obj_register[oidx][l_file][o_keys[i]]);
    if(!file_exists(DIR_OBJECTS+"/"+o_keys[i]+".o")) {
      write("Could not load one of your objects.");
      item->remove();
      continue;
    }
    if(!item->restore_me(o_keys[i]))
      return 0;
    item->move(ob);
    if(!sscanf(o_keys[i], "obj_%d", idx)) continue;
    rm(DIR_OBJECTS+sprintf("/obj_%d.o", idx));
    idxv[i] = idx;
  }
  unregister_n_items("obj", idxv);
  map_delete(obj_register[oidx], l_file);
  save_obj_register(oidx);
  return 1;
}

void load_lockers(object ob) {
  string room, *l_keys;
  int i;
  object item;
  int num;
  string key;
  class uil tuil;

  room = file_name(ob);
  if(!room || !all_lockers || !all_lockers[room]) {
    return;
  }
  if(!mapp(all_lockers[room])) {
    return;
  }
  l_keys = keys(all_lockers[room]);
  i = sizeof(l_keys);
  if(!i) {
    map_delete(all_lockers, room);
    return;
  }
  while(i--) {
    item = new(all_lockers[room][l_keys[i]]);
    if(file_exists(DIR_OBJECTS+"/"+l_keys[i]+".o"))
      item->restore_locker(l_keys[i]);
    else {
      if(sscanf(l_keys[i], "%s_%d", key, num) == 2) {
	tuil = find_uid_list(key);
	if(tuil) {
	  tuil->bitlist = set_bit(tuil->bitlist, num);
	  save_unique_id();
	}
	item->set_register(num);
	item->save_me(l_keys[i]);
      }
      write("Unable to restore locker, creating anyway.");
    }
    item->move(ob);
  }
  return;
}

private void manage_lockers() {
  string *lox, *dir, file, key, lfile, ofile;
  int idx, rmd, i, dup, dupo, rmdo;
  class uil tuil, otuil;
  mapping obj_reg;
  mapping tmp;

  seteuid(getuid());

 /*
  * Initialize the unique id list for building from scratch.
  */
  
  unique_id = allocate(10);
  unique_id[0] = new(class uil);
  unique_id[0]->key = "obj";
  unique_id[0]->bitlist = "";
  unique_id[1] = new(class uil);
  unique_id[1]->key = "locker";
  unique_id[1]->bitlist = "";

 /*
  * Now loop over all the lockers in all_lockers.  Unregister duplicates, and
  * build unique id list.
  */
  
  foreach(file, tmp in all_lockers) {

    lox = keys(tmp);
    dir = ({});
    foreach(lfile in lox) {
      if(!sscanf(lfile, "%s_%d", key, idx)) return;
      tuil = find_uid_list(key);

     /*
      * Create a new unique id list.
      */
      
      if(!tuil) {
        i = 0;
        while(i < 10 && unique_id[i]) i++;
        if(i < 10) {
          unique_id[i] = new(class uil);
          unique_id[i]->key = key;
          unique_id[i]->bitlist = "";
          tuil = unique_id[0];
        }
        else {
          seteuid(UID_LOG);
          log_file("locker_fail", "Uil failure, key: "+key+" at "+ctime(time())+"\n");
          seteuid(getuid());
          continue;
        }
      }

     /*
      * Test if the locker is a duplicate.  If so, unregister it.
      */
      
      if(test_bit(tuil->bitlist, idx)) {
        dup++;
        dir += ({ lfile });
      } else
        tuil->bitlist = set_bit(tuil->bitlist, idx);
    }

    foreach(lfile in dir) map_delete(tmp, lfile);
  }

 /*
  * Now loop over the obj registers.  If there is a duplicate object,
  * unregister it.  If there is a locker in the register which does not
  * exist in the new unique id list, remove it from the register.
  */

  tuil = find_uid_list("locker");
  otuil = find_uid_list("obj");
  for(i = 0; i < 10; i++) {
    obj_reg = obj_register[i];
    if(!obj_reg) continue;
    lox = ({});

    foreach(lfile, tmp in obj_reg) {
      if(!sscanf(lfile, "locker_%d", idx))
        continue;

     /*
      * Does this locker file exist in the uid list?
      */

      if(!test_bit(tuil->bitlist, idx)) {
        lox += ({ lfile });
        continue;
      }

      dir = ({});
      foreach(ofile, file in tmp) {
        if(!sscanf(ofile, "obj_%d", idx)) continue;

       /*
        * Is it a duplicate object?
        */

        if(test_bit(otuil->bitlist, idx)) {
          dupo++;
          dir += ({ ofile });
        } else
          otuil->bitlist = set_bit(otuil->bitlist, idx);
      }

     /*
      * Unregister those bad boyz!
      */
      
      foreach(file in dir) map_delete(tmp, file);

    }

    foreach(file in lox) map_delete(obj_register[i], file);
  }

 /*
  * Finally, loop over the directory of object files, deleting all those not
  * in the unique id list.
  */

  for(i=0; i<10; i++) {
    tuil = unique_id[i];
    if(!tuil) continue;

    key = tuil->key;
    dir = get_dir("/adm/save/objects/"+key+"_*.o");

    foreach(file in dir) {
      if(!sscanf(file, key+"_%d.o", idx)) continue;

     /*
      * Delete it if not in uid list.
      */

      if(!test_bit(tuil->bitlist, idx)) {
        if(key == "obj") rmdo++;
        else rmd++;
        rm("/adm/save/objects/"+file);
      }
    }
  }

 /*
  * Log it all...
  */

  seteuid(UID_LOG);
  log_file("update_lockers",
  "\nLocker update at: "+ctime(time())+"\n"
  "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n"
  +sprintf("Duplicate lockers unregistered: %d\n", dup)
  +sprintf("Duplicate objects unregistered: %d\n", dupo)
  +sprintf("Locker files deleted: %d\n", rmd)
  +sprintf("Object files deleted: %d\n", rmdo));
  seteuid(getuid());

 /*
  * Now save it all...
  */

  save_unique_id();
  save_all_lockers();
  for(i=0; i<10; i++) if(obj_register[i]) save_obj_register(i);

  return;
}
