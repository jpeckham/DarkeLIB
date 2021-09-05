#include <post.h> 
#include <objects.h>
#include <security.h> 
 
private string __Letter; 
private string *__Undeleted; 
nosave private int __LetterPtr, __PostalPtr; 
nosave private string *__LettersDir, *__PostalDir; 
 
protected void manage_letters();
private void save_letter(string str);
private int restore_letter(string str);

int clean_up_names(){
   string *dirs;
   int x, y, removed;
   string *names, name;
   
   removed = 0;
   seteuid(UID_POSTALSAVE);
   dirs = get_dir("/adm/save/postal/");
   for(x=0;x<sizeof(dirs);x++){
      names = get_dir("/adm/save/postal/"+dirs[x]+"/");
      for(y=0;y<sizeof(names);y++){
         if(sscanf(names[y], "%s.o", name) != 1)
            continue;
         if(!user_exists(name)){
            removed++;
            rm("/adm/save/postal/"+dirs[x]+"/"+names[y]);
         }
      }
   }
   return removed;
}    

int clean_up_mudmails(){
    string *dirs;
    int x, y, i, removed;
    string *names, *ids, name;
    object ob;
    mixed *info;
    mapping data;
   
    removed = 0;
    ids = ({});
    seteuid(UID_POSTALSAVE);
    dirs = get_dir("/adm/save/postal/");
    for(x=0;x<sizeof(dirs);x++){
        names = get_dir("/adm/save/postal/"+dirs[x]+"/");
        for(y=0;y<sizeof(names);y++){
            if(sscanf(names[y], "%s.o", name) != 1)
                continue;
            ob = new("/daemon/localpost");
            ob->load_post_box(name);
            data = ob->query_post_data(name);
            info = data["box info"];
            ob->remove();
            for(i=0;i<sizeof(info);i++){
                if(mapp(info[i]))
                    ids += ({ info[i]["id"] });
            }
        }
    }
    dirs = get_dir("/adm/save/letters/");
    for(x=0;x<sizeof(dirs);x++){
        names = get_dir("/adm/save/letters/"+dirs[x]+"/");
        for(y=0;y<sizeof(names);y++){
            if(sscanf(names[y], "%s.o", name) != 1)
                continue;
            if(member_array(name, ids) == -1){
                removed++;
                rm("/adm/save/letters/"+dirs[x]+"/"+names[y]);
            }
        }
    }
    seteuid(getuid());
    return removed;
}    

void create() { 
    string *tmp;
    int i;

    seteuid(UID_POSTALSAVE); 
    __Letter = ""; 
    __Undeleted = ({}); 
    __LettersDir = ({}); 
    __PostalDir = ({}); 
    __LetterPtr = 0; 
    __PostalPtr = 0; 
    for(i=0; i<10; i++) { 
        if(file_size(DIR_LETTERS+"/"+i) == -2)  
          __LettersDir += get_dir(DIR_LETTERS+"/"+i+"/"); 
      } 
    i = sizeof(tmp = get_dir(DIR_POSTAL+"/")); 
    while(i--) { 
        if(file_size(DIR_POSTAL+"/"+tmp[i]) == -2) 
          __PostalDir += get_dir(DIR_POSTAL+"/"+tmp[i]+"/"); 
      } 
    seteuid(getuid()); 
    manage_letters(); 
    call_out("manage_postal", 60);
    call_out("clean_up_names", 100);
    call_out("clean_up_mudmails", 130);
  } 
 
private int valid_access(object ob) { 
    if(base_name(previous_object()) == OB_POSTAL) return 1;
    return (geteuid(ob) == UID_POSTAL || geteuid(ob) == UID_ROOT); 
  } 
 
string create_letter(string str, string *who) { 
    int tmp; 
    string id, dir; 
 
    if(!valid_access(previous_object())) return 0; 
    id = sprintf("%d", tmp = time()); 
    dir = DIR_LETTERS+"/"+(tmp%10); 
    seteuid(UID_POSTALSAVE); 
    if(file_size(dir+"/") != -2) { 
        mkdir(dir); 
        seteuid(getuid()); 
    } 
    while(file_size(dir+"/"+id+".o") != -1) id = sprintf("%d", tmp += 10); 
    seteuid(getuid()); 
    __Letter = str; 
    __Undeleted = who; 
    save_letter(dir+"/"+id); 
    return id; 
} 
 
void reassign_letter(string id, mapping forwards) { 
    string *old; 
    int i, x; 
 
    if(!valid_access(previous_object()) || !restore_letter(id)) return; 
    i = sizeof(old = keys(forwards)); 
    while(i--) { 
        if((x=member_array(old[i], __Undeleted)) < 0) continue; 
        __Undeleted[x] = forwards[old[i]]; 
    } 
    save_letter(DIR_LETTERS+"/"+id[strlen(id)-1..strlen(id)-1]+"/"+id);
}         
 
private void save_letter(string str) { 
    seteuid(UID_POSTALSAVE); 
    save_object(str); 
    seteuid(getuid()); 
  } 
 
private int restore_letter(string id) { 
    string dir; 
    int x; 
 
    if(!id || !strlen(id)) return 0;
    x = strlen(id) - 1; 
    dir = DIR_LETTERS+"/"+id[x..x]; 
    seteuid(UID_POSTALSAVE); 
    if(file_size(dir+"/"+id+".o") < 0) return 0; 
    restore_object(dir+"/"+id); 
    seteuid(getuid()); 
    return 1; 
} 
 
string query_letter(string id) { 
    if(!valid_access(previous_object())) return "Illegal access.\n"; 
    if(!restore_letter(id)) return "Invalid message.\n"; 
    return __Letter; 
  } 
 
void delete_letter(string id, string who) { 
    string *tmp; 
    int i; 
 
    if(!valid_access(previous_object())) return; 
    if(!restore_letter(id)) return; 
    __Undeleted -= ({ who }); 
    i = sizeof(tmp = __Undeleted); 
    seteuid(UID_POSTALSAVE);
    while(i--) { 
        if(!user_exists(tmp[i])) { 
            __Undeleted -= ({ tmp[i] }); 
            seteuid(UID_POSTALSAVE); 
            rm(DIR_POSTAL+"/"+tmp[i][0..0]+"/"+tmp[i]+".o"); 
            seteuid(getuid()); 
	  } 
        else if(!file_exists(DIR_POSTAL+"/"+tmp[i][0..0]+"/"+tmp[i]+".o"))
          __Undeleted -= ({ tmp[i] });
      } 
    if(!sizeof(__Undeleted))  
      rm(DIR_LETTERS+"/"+id[strlen(id)-1..strlen(id)-1]+"/"+id+".o"); 
    else save_letter(DIR_LETTERS+"/"+id[strlen(id)-1..strlen(id)-1]+"/"+id); 
    seteuid(getuid());
}
 
void manage_letters() { 
    string str, ext; 
    string *tmp; 
    int i; 
 
    if(!pointerp(__LettersDir) || !sizeof(__LettersDir)) return; 
    if(__LetterPtr >= sizeof(__LettersDir)) __LetterPtr = 0; 
    sscanf(__LettersDir[__LetterPtr], "%s\\.%s", str, ext); 
    __LetterPtr++;
    if(!restore_letter(str)) { 
        call_out("manage_letters", 120); 
        return; 
      } 
    i = sizeof(tmp = __Undeleted); 
    seteuid(UID_POSTALSAVE);
    while(i--) { 
        if(!user_exists(tmp[i])) { 
            __Undeleted -= ({ tmp[i] }); 
            seteuid(UID_POSTALSAVE); 
            rm(DIR_POSTAL+"/"+tmp[i][0..0]+"/"+tmp[i]+".o"); 
            seteuid(getuid()); 
        } 
        else if(!file_exists(DIR_POSTAL+"/"+tmp[i][0..0]+"/"+tmp[i]+".o"))
          __Undeleted -= ({ tmp[i] });
    } 
    if(!sizeof(__Undeleted)) { 
        rm(DIR_LETTERS+"/"+str[strlen(str)-1..strlen(str)-1]+"/"+str+".o");  
        seteuid(getuid()); 
    } 
    else save_letter(DIR_LETTERS+"/"+str[strlen(str)-1..strlen(str)-1]+"/"+str); 
    call_out("manage_letters", 120); 
} 
    
protected void manage_postal() { 
    string pl, ext; 
 
    if(!pointerp(__PostalDir) || !sizeof(__PostalDir)) return; 
    if(__PostalPtr >= sizeof(__PostalDir)) __PostalPtr = 0; 
    sscanf(__PostalDir[__PostalPtr], "%s\\.%s", pl, ext); 
    seteuid(UID_POSTALSAVE); 
    if(pl && !file_exists(DIR_POSTAL+"/"+pl[0..0]+"/"+pl+".o")) { 
        call_out("manage_postal", 120); 
        seteuid(getuid()); 
        return; 
      } 
    if(pl && !user_exists(pl)) rm(DIR_POSTAL+"/"+pl[0..0]+"/"+pl+".o"); 
    seteuid(getuid()); 
    call_out("manage_postal", 120); 
    __PostalPtr++;
   } 
