
#include <security.h>
#include <dirs.h>
#include <rooms.h>

inherit "/adm/daemon/locker_hash.c"

nosave int count;
class uil *unique_id = allocate(10);

int *register_n_items(string key, int x);
int unregister_item(string key, int idx);
class uil find_uid_list(string key);

class uil find_uid_list(string key) {
  if(!unique_id[0]) 

int register_n_items(string key) {
  int spec;
  
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
  init_hash_table();
  seteuid(UID_SECURE_DAEMONSAVE);
  if(file_exists(DIR_SECURE_DAEMONS_SAVE+"/unique_id.v"))
    unique_id = restore_variable(read_file(DIR_SECURE_DAEMONS_SAVE+
      "/unique_id.v"));
  seteuid(getuid());
  clean_up_files();
  return;
}

void clean_up_files() {
  string *dir, file;
  string key;
  int id, i;
  class uil tmp_u_list;

  if(!unique_id[0]) return;
  dir = get_dir("/adm/save/objects/*.o");
  foreach(file in dir) {
    if(sscanf(file, "%s_%d.o", key, id) != 2) continue;
    i = 0;
    while(uniqu

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

