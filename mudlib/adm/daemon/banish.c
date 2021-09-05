#include <config.h> 
#include <security.h> 
#include <objects.h>
#include <daemons.h> 
#include <save.h> 
 
string *__Names, *__Sites, *__WatchNames, *__WatchSites; 
string *__Allowed, *__Guests, *__IllegalSubStrings; 

private int valid_access(object ob);
void register_site(string str);
void unregister_site(string str);
string *query_register();
void banish_name(string str);
void unbanish_name(string str);
string *query_banished();
void watch_site(string str);
void unwatch_site(string str);
string *query_watched_sites();
void watch_name(string str);
void unwatch_name(string str);
string *query_watched_names();
void allow_name(string str);
void unallow_name(string str);
string *query_allowed();
void set_illegal_substring(string str);
void remove_illegal_substring(string str);
string *query_illegal_substrings();
void add_guest(string str);
void remove_guest(string str);
string *query_guests();
private void save_banish();
private void restore_banish();
int is_guest(string str);
int valid_name(string str);
int allow_logon(string nom, string ip);
private int match_ip(string ip, string *sites);
 
void create() { 
    seteuid(getuid()); 
    __Names = ({}); 
    __Sites = ({}); 
    __WatchNames = ({}); 
    __WatchSites = ({}); 
    __Allowed = ({}); 
    __Guests = ({}); 
    __IllegalSubStrings = ({}); 
    if(file_exists(SAVE_BANISH+".o")) restore_banish(); 
} 
 
private int valid_access(object ob) { 
    return (geteuid(ob) == UID_BANISH || geteuid(ob) == UID_ROOT); 
} 
 
void register_site(string str) { 
    if(!valid_access(previous_object())) return; 
    __Sites = distinct_array(__Sites + ({ str })); 
    save_banish(); 
} 
 
void unregister_site(string str) { 
    if(!valid_access(previous_object())) return; 
    __Sites -= ({ str }); 
    save_banish(); 
} 
 
string *query_registered() { 
    if(!valid_access(previous_object())) return ({}); 
    return __Sites; 
} 
 
void banish_name(string str) { 
    if(!valid_access(previous_object())) return; 
    __Names = distinct_array(__Names + ({ lower_case(str) })); 
    save_banish(); 
} 
 
void unbanish_name(string str) { 
    if(!valid_access(previous_object())) return; 
    __Names -= ({ lower_case(str) }); 
    save_banish(); 
} 
 
string *query_banished() { 
    if(!valid_access(previous_object())) return ({}); 
    return __Names; 
} 
 
void watch_site(string str) { 
    if(!valid_access(previous_object())) return; 
    __WatchSites = distinct_array(__WatchSites + ({ str })); 
    save_banish(); 
} 
 
void unwatch_site(string str) { 
    if(!valid_access(previous_object())) return; 
    __WatchSites -= ({ str }); 
    save_banish(); 
} 
 
string *query_watched_sites() { 
    if(!valid_access(previous_object())) return ({}); 
    return __WatchSites; 
} 
 
void watch_name(string str) { 
    if(!valid_access(previous_object())) return; 
    __WatchNames = distinct_array(__WatchNames + ({ lower_case(str) })); 
    save_banish(); 
} 
 
void unwatch_name(string str) { 
    if(!valid_access(previous_object())) return; 
    __WatchNames -= ({ lower_case(str) }); 
    save_banish(); 
} 
 
string *query_watched_names() { 
    if(!valid_access(previous_object())) return ({}); 
    return __WatchNames; 
} 
 
void allow_name(string str) { 
    if(!valid_access(previous_object())) return; 
    __Allowed = distinct_array(__Allowed + ({ lower_case(str) })); 
    save_banish(); 
} 
 
void unallow_name(string str) { 
    if(!valid_access(previous_object())) return; 
    __Allowed -= ({ lower_case(str) }); 
    save_banish(); 
} 
 
string *query_allowed() { 
    if(!valid_access(previous_object())) return ({}); 
    return __Allowed; 
} 
 
void set_illegal_substring(string str) { 
    if(!valid_access(previous_object())) return; 
    __IllegalSubStrings = distinct_array(__IllegalSubStrings + 
      ({ lower_case(str) })); 
    save_banish(); 
} 
 
void remove_illegal_substring(string str) { 
    if(!valid_access(previous_object())) return; 
    __IllegalSubStrings -= ({ lower_case(str) }); 
    save_banish(); 
} 
 
string *query_illegal_substrings() {  
    if(!valid_access(previous_object())) return ({}); 
    else return __IllegalSubStrings; 
} 
 
void add_guest(string str) { 
    if(!valid_access(previous_object())) return; 
    __Guests = distinct_array(__Guests + ({ lower_case(str) })); 
    save_banish(); 
} 
 
void remove_guest(string str) { 
    if(!valid_access(previous_object())) return; 
    __Guests -= ({ lower_case(str) }); 
    save_banish(); 
} 
 
string *query_guests() { 
    if(!valid_access(previous_object())) return ({}); 
    else return __Guests; 
} 
 
private void save_banish() { 
    seteuid(UID_SECURE_DAEMONSAVE); 
    save_object(SAVE_BANISH); 
    seteuid(getuid()); 
} 
 
private void restore_banish() { 
    seteuid(UID_SECURE_DAEMONSAVE); 
    restore_object(SAVE_BANISH); 
    seteuid(getuid()); 
} 
 
int is_guest(string str) { 
    if(base_name(previous_object()) != OB_LOGIN) return 0; 
    else return (member_array(lower_case(str), __Guests) != -1); 
} 
 
int valid_name(string str) { 
    int i, x; 
 
    if(base_name(previous_object()) != OB_LOGIN) return 0; 
    if(member_array(str, __Names) != -1) return 0; 
    i = sizeof(__IllegalSubStrings);
    while(i--) if(strsrch(str, __IllegalSubStrings[i]) != -1) return 0;
    if((x = strlen(str)) > MAX_USER_NAME_LENGTH) return 0; 
    if(x < MIN_USER_NAME_LENGTH) return 0; 
    for(i=0; i<x; i++)  
      if(str[i] < 'a' || str[i] > 'z') return 0; 
    return 1; 
} 
 
int allow_logon(string nom, string ip) { 
    if(base_name(previous_object()) != OB_LOGIN) return 0; 
    if(member_array(nom, __WatchNames) != -1) { 
        seteuid(UID_LOG); 
        log_file("watch/names", sprintf("%s from %s at %s\n", nom, ip,
          ctime(time()))); 
        seteuid(getuid()); 
    } 
    if(match_ip(ip, __WatchSites)) { 
        seteuid(UID_LOG); 
        log_file("watch/"+ip, sprintf("%s at %s\n", nom, ctime(time())));
        seteuid(getuid()); 
    } 
    if(member_array(nom, __Allowed) != -1) { 
        seteuid(UID_LOG); 
        log_file("watch/allowed", sprintf("%s from %s at %s\n", nom, ip,
          ctime(time()))); 
        seteuid(getuid()); 
        __Allowed -= ({ nom }); 
        save_banish(); 
        return 1; 
    }     
    if(match_ip(ip, __Sites)) { 
        if(user_exists(nom)) { 
            seteuid(UID_LOG); 
            log_file("watch/"+ip, sprintf("%s allowed in from %s at %s\n", nom, ip,
              ctime(time()))); 
            seteuid(getuid()); 
            return 1; 
        } 
        else { 
            seteuid(UID_LOG); 
            log_file("watch/"+ip, sprintf("%s failed from %s at %s\n", nom, ip,
              ctime(time()))); 
            seteuid(getuid()); 
            return 0; 
        } 
    } 
    return 1; 
} 
 
private int match_ip(string ip, string *sites) { 
    string a, b; 
    int i; 
 
    if(!(i = sizeof(sites))) return 0; 
    while(i--) { 
        if(sites[i] == ip) return 1; 
        if(sscanf(sites[i], "%s.*", a) && sscanf(ip, a+"%s", b)) 
          return 1; 
    } 
    return 0; 
} 
