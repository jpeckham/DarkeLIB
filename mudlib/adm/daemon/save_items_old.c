//    Object daemon.
//    -Diewarzau 1/28/96
//    DarkeLIB 1.0

#include <security.h>
#include <dirs.h>
#include <rooms.h>

nosave mapping dir_register;
nosave object *lroom_register;
nosave int count;
mapping all_lockers;
mapping obj_register;

void save_all_lockers();
void save_obj_register();
int register_item(string key);
void manage_lockers();
int unregister_item(string key, int idx);
void build_dir_register();
void add_to_register(string file);
object *ok_to_update(object *all_rooms);

void build_dir_register() {
  dir_register = ([]);
  seteuid(getuid());
  map_array(get_dir("/adm/save/objects/*.o"),
    (: add_to_register :));
  return;
}

void add_to_register(string file) {
  int num;
  string key;

  if(sscanf(file, "%s_%d.o", key, num) != 2)
    return;
  if(!dir_register[key])
    dir_register[key] = ({ num });
  else dir_register[key] += ({ num });
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

/* Used to load up old save_items.o */

void downward_compat() {
  seteuid(UID_SECURE_DAEMONSAVE);
  restore_object(DIR_SECURE_DAEMONS_SAVE+"/save_items");
  seteuid(getuid());
}
  
void save_obj_register() {
  seteuid(UID_SECURE_DAEMONSAVE);
  if(file_exists(DIR_SECURE_DAEMONS_SAVE+"/obj_register.v"))
    rm(DIR_SECURE_DAEMONS_SAVE+"/obj_register.v");
  write_file(DIR_SECURE_DAEMONS_SAVE+"/obj_register.v",
    save_variable(obj_register));
  seteuid(getuid());
  return;
}

int register_item(string key) {
  int spec;
  
  if(!dir_register[key]) dir_register[key] = ({});
  spec = 1;
  while(member_array(spec, dir_register[key]) >= 0) spec++;
  dir_register[key] += ({ spec });
  return spec;
}

int unregister_item(string key, int idx) {
  int x;
  
  if(!dir_register[key]) return 0;
  x = member_array(idx, dir_register[key]);
  if(x<0) return 0;
  dir_register[key] = exclude_array(dir_register[key], x);
  if(member_array(idx, dir_register[key]) >= 0) {
    seteuid(UID_LOG);
    log_file("locker_errors", "Duplicate item in register: "+
      sprintf("%s_%d", key, idx)+"\n");
    if(previous_object() && previous_object() != this_object())
      log_file("locker_errors", "Object: "+file_name(previous_object())+"\n");
    seteuid(getuid());
  }
  return 1;
}
  
void create() {
  all_lockers = ([]);
  dir_register = ([]);
  obj_register = ([]);
  lroom_register = ({});
  seteuid(UID_SECURE_DAEMONSAVE);
  if(file_exists(DIR_SECURE_DAEMONS_SAVE+"/all_lockers.v"))
    all_lockers = restore_variable(read_file(DIR_SECURE_DAEMONS_SAVE+
      "/all_lockers.v"));
  if(file_exists(DIR_SECURE_DAEMONS_SAVE+"/obj_register.v"))
    obj_register = restore_variable(read_file(DIR_SECURE_DAEMONS_SAVE+
      "/obj_register.v"));
  build_dir_register();
  seteuid(getuid());
  return;
}

void update_lockers(object env) {
  object *inv, me;
  string key, room_file;
  int i, num;

  if(!env) return;
  if(!env->query_is_locker_room()) return;
  room_file = file_name(env);
  inv = all_inventory(env);
  all_lockers[room_file] = ([]);
  if(!inv || !(i=sizeof(inv))) {
    map_delete(all_lockers, room_file);
    save_all_lockers();
    return;
  }
  me = previous_object();
  if(!me->query_is_locker()) me = 0;
  while(i--) {
    if(!inv[i]->query_is_locker()) continue;
    key = (string)inv[i]->query_storage_key();
    if(!key) key = "locker";
    if(!(num = inv[i]->register_num())) {
      num = register_item(key);
      inv[i]->set_register(num);
    }
    if(this_player() && archp(this_player()))
      write("key: "+key+"   num: "+num);
    if(!me || (inv[i] == me && environment(me) == env)) {
      inv[i]->save_me(sprintf("%s_%d", key, num));
      if(this_player() && archp(this_player()))
        write(sprintf("Saved: %s %d", key, num));
    }
    all_lockers[room_file][sprintf("%s_%d", key, num)] = base_name(inv[i]);
  }
  save_all_lockers();
  return;
}

int register_objects(object *inv, object ob) {
  int i, idx, lidx;
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
  l_file = sprintf("%s_%d", key, lidx);
  if(!i) {
    map_delete(obj_register, l_file);
    save_obj_register();
    return 1;
  }
  obj_register[l_file] = ([]);
  while(i--) {
    idx = register_item("obj");
    fname = base_name(inv[i]);
    if(!fname) sscanf(file_name(inv[i]), "%s#%*d", fname);
    obj_register[l_file][sprintf("obj_%d", idx)] = fname;
    if(file_exists(DIR_OBJECTS+sprintf("/obj_%d.o", idx))) return 0;
    if(!inv[i]->save_me(sprintf("obj_%d", idx))) return 0;
  }
  save_obj_register();
  return 1;
}

int load_objects(object ob) {
  int lidx, i, idx;
  object item;
  string *o_keys, l_file, key;

  seteuid(getuid());
  lidx = (int)ob->register_num();
  if(!lidx) return 1;
  key = (string)ob->query_storage_key();
  if(!key) key = "locker";
  l_file = sprintf("%s_%d", key, lidx);
  if(!obj_register[l_file]) return 1;
  o_keys = keys(obj_register[l_file]);
  i = sizeof(o_keys);
  if(!i) {
    map_delete(obj_register, l_file);
    save_obj_register();
    return 1;
  }
  seteuid(getuid());
  while(i--) {
    item = new(obj_register[l_file][o_keys[i]]);
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
    unregister_item("obj", idx);
  }
  map_delete(obj_register, l_file);
  save_obj_register();
  return 1;
}

void print_obj_register(object what) {
  string l_file, key;

  seteuid(getuid());
  key = (string)what->query_storage_key();
  if(!key) key = "locker";
  l_file = sprintf("%s_%d", key, (int)what->register_num());
  if(!obj_register[l_file]) {
    write("File not found in obj register.");
    return;
  }
  write(sprintf("Obj register:\n%O", obj_register[l_file]));
  return;
}

void debug_dir_register() {
  string *dir, key;
  int i, num;
  
  seteuid(getuid());
  dir = get_dir(DIR_OBJECTS+"/*.o");
  i = sizeof(dir);
  while(i--) {
    if(sscanf(dir[i], "%s_%d.o", key, num) != 2) {
      write("File: "+dir[i]+" does not conform to format.");
      continue;
    }
    if(!dir_register[key]) {
      write(sprintf("File: %s is registered as %s, not in dir_register.",
        dir[i], key));
      continue;
    }
    if(member_array(num, dir_register[key]) < 0) {
      write(sprintf("File: %s not found in dir_register.", dir[i]));
    }
  }
  return;
}

void load_lockers(object ob) {
  string room, *l_keys;
  int i;
  object item;

  room = file_name(ob);
  if(!room || !all_lockers || !all_lockers[room]) {
    lroom_register += ({ ob });
    return;
  }
  if(!mapp(all_lockers[room])) {
    lroom_register += ({ ob });
    return;
  }
  l_keys = keys(all_lockers[room]);
  i = sizeof(l_keys);
  if(!i) {
    map_delete(all_lockers, room);
    lroom_register += ({ ob });
    return;
  }
  while(i--) {
    item = new(all_lockers[room][l_keys[i]]);
    if(file_exists(DIR_OBJECTS+"/"+l_keys[i]+".o"))
      item->restore_locker(l_keys[i]);
    else
      write("Unable to restore locker, creating anyway.");
    item->move(ob);
  }
  lroom_register += ({ ob });
  return;
}

void manage_lockers() {
  int i;
  object tmp, *inv;
  string *l_keys;

  count = 1;
  if(!all_lockers) return;
  l_keys = keys(all_lockers);
  i = sizeof(l_keys);
  if(!i) return;
  seteuid(UID_LOG);
  log_file("update_lockers", "Locker update: "+ctime(time())+"\n");
  inv = ({});
  while(i--) {
    if(replace_string(l_keys[i], "#", "") != l_keys[i]) {
      map_delete(all_lockers, l_keys[i]);
      continue;
    }
    if(!call_other(l_keys[i], "query_is_locker_room")) {
      map_delete(all_lockers, l_keys[i]);
      continue;
    }
    inv += ({ load_object(l_keys[i]) });
  }
  delayed_call("manage_lockers2", 8, inv);
  return;
}

void manage_lockers2(object *rooms) {
  int i, j, k, lidx, idx, rmd, flag;
  string *o_keys, l_file, key, *r_keys, *dir;
  object *inv;
  mapping my_register;

  if(count > 5) {
    seteuid(UID_LOG);
    log_file("update_lockers", "After 5 tries, all locker rooms not responding.\nAborting.\n");
    seteuid(getuid());
    return;
  }
  if(inv = ok_to_update(rooms)) {
    seteuid(UID_LOG);
    log_file("update_lockers", "All lockers not responded.\n"
    "Locker rooms not registered:\n"+implode(map_array(inv, (: file_name :)), "\n")+"\n");
    seteuid(getuid());
    count++;
    delayed_call("manage_lockers2", 8, rooms);
    return;
  }
  inv = ({});
  rmd = 0;
  i = sizeof(rooms);
  seteuid(UID_LOG);
  log_file("update_lockers", "Phase 2: "+ctime(time())+"\n");
  my_register = ([]);
  while(i--) {
    inv = all_inventory(rooms[i]);
    j = sizeof(inv);
    if(!j) {
      map_delete(all_lockers, file_name(rooms[i]));
      continue;
    }
    flag = 0;
    while(j--) {
      if(!inv[j]->query_is_locker()) continue;
      flag = 1;
      key = (string)inv[j]->query_storage_key();
      if(!my_register[key]) my_register[key] = ({});
      lidx = (int)inv[j]->register_num();
      my_register[key] += ({ lidx });
    }
    if(!flag)
      map_delete(all_lockers, file_name(rooms[i]));
  }
  r_keys = keys(dir_register);
  i = sizeof(r_keys);
  if(!i) return;
  while(i--) {
    if(r_keys[i] == "obj") continue;
    dir = get_dir(DIR_OBJECTS+"/"+r_keys[i]+"_*");
    j = sizeof(dir);
    while(j--) {
      if(!sscanf(dir[j], r_keys[i]+"_%d.o", lidx)) continue;
      if(my_register[r_keys[i]] &&
        member_array(lidx, my_register[r_keys[i]]) >= 0)
        continue;
      if(file_exists(DIR_OBJECTS+"/"+dir[j])) {
        rm(DIR_OBJECTS+"/"+dir[j]);
        rmd++;
      }
      l_file = sprintf("%s_%d", r_keys[i], lidx);
      if(!obj_register[l_file]) continue;
      o_keys = keys(obj_register[l_file]);
      k = sizeof(o_keys);
      if(!k) continue;
      while(k--) {
        if(!sscanf(o_keys[k], "obj_%d", idx)) continue;
        if(file_exists(DIR_OBJECTS+sprintf("/obj_%d.o", idx))) {
          rm(DIR_OBJECTS+sprintf("/obj_%d.o", idx));
          rmd++;
        }
      }
      map_delete(obj_register, l_file);
    }
  }
  seteuid(getuid());
  save_all_lockers();
  save_obj_register();
  build_dir_register();
  log_file("update_lockers", "DONE!\n"+sprintf("Files deleted: %d\n", rmd));
  return;
}

object *ok_to_update(object *all_rooms) {
  int i;
  
  i = sizeof(all_rooms);
  while(i--)
    if(member_array(all_rooms[i], lroom_register) < 0)
      return all_rooms - lroom_register;
  return 0;
}


void debug_all_lockers(string key) {
  write(sprintf("%O", all_lockers[key])+"\n");
  return;
}
