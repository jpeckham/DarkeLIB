//      /adm/daemon/bboard_d.c
//      from the Nightmare mudlib
//      a daemon to service the new Neightmare bboard
//      created by Descartes of Borg 6 march 1993
 
#include <std.h>
#include <dirs.h>
#include <security.h>
#include <bboard.h>
 
inherit DAEMON;

mapping *posts;
mixed *read_by;
nosave string *edit_ok;
nosave string current;
 
int valid_edit(int x, string id);
string when(int x);
mixed *sub_array(mixed *arr, int x);
 
void create() {
    ::create();
    current = 0;
    edit_ok = ({});
    posts = ({});
    read_by = ({});
}
 
int remove_message(string id, int number) {
    if(!id || number < 0) return BAD_DATA;
    if(current != id) {
        if(file_size(DIR_BOARDS+"/"+id+".o") < 0) return INVALID_MSG;
        current = id;
        restore_object(DIR_BOARDS+"/"+current);
    }
    edit_ok = previous_object()->query_edit_ok();
    if(!this_player()) return ILLEGAL_ACCESS;
    if(!sizeof(posts)) return INVALID_MSG;
    if(number > sizeof(posts)-1 || number < 0) return INVALID_MSG;
    if(!valid_edit(number, geteuid(this_player())))
        return ILLEGAL_ACCESS;
    posts -= ({ posts[number] });
    read_by = sub_array(read_by, number);
    save_object(DIR_BOARDS+"/"+current);
    return BBOARD_OK;
}
 
int edit_note(string id, int number) {
    string *lines;
    string file;
    int i;

    if(!id || number < 0) return BAD_DATA;
    if(current != id) {
        if(file_size(DIR_BOARDS+"/"+id+".o") < 0) return INVALID_MSG;
        current = id;
        edit_ok = ({});
        restore_object(DIR_BOARDS+"/"+current);
    }
    if(!this_player()) return ILLEGAL_ACCESS;
    if(!sizeof(posts)) return INVALID_MSG;
    if(number > sizeof(posts)-1 || number < 0) return INVALID_MSG;
    if(!valid_edit(number, geteuid(this_player())))
        return ILLEGAL_ACCESS;
    lines = explode(posts[number]["message"], "\n");
    file = BBOARD_EDIT+posts[number]["owner"];
    rm(file);
    for(i=0; i<sizeof(lines); i++) write_file(file, lines[i]+"\n");
    posts -= ({ posts[number] });
    read_by = sub_array(read_by, number);
    return BBOARD_OK;
}
 
int post_message(string id, string title) {
    string body;
 
    if(!id || !title) return BAD_DATA;
    if(id != current) {
        if(file_size(DIR_BOARDS+"/"+id+".o") < 0) {
            posts = ({});
	    read_by = ({});
            current = id;
        }
        else {
            current = id;
            restore_object(DIR_BOARDS+"/"+current);
        }
    }
    body = read_file(BBOARD_EDIT+(string)this_player()->query_name());
    rm(BBOARD_EDIT+(string)this_player()->query_name());
    if(!body) {
        save_object(DIR_BOARDS+"/"+current);
        return EDIT_ABORT;
    }
    posts += ({ ([
        "owner": (string)this_player()->query_name(),
        "date": time(),
	"size": strlen(body),
        "title": title,
        "level": (int)this_player()->query_level(),
        "message": body,
    ]) });
     read_by += ({ ({ (string)this_player()->query_name() }) });
    save_object(DIR_BOARDS+"/"+current);
    return BBOARD_OK;
}
 
mapping get_message(string id, int number) {
    if(!id || number <0) return 0;
    if(current != id) {
        if(file_size(DIR_BOARDS+"/"+id+".o") < 0) return 0;
        current = id;
        restore_object(DIR_BOARDS+"/"+current);
    }
    if(!sizeof(posts) || sizeof(posts)-1 < number) return 0;
    return posts[number];
}
 
string display_board(string id) {
    string borg;
    int i;

    if(!id) return 0;
    if(current != id) {
        if(file_size(DIR_BOARDS+"/"+id+".o") < 0) {
            borg = ("No messages posted.\n");
	    return borg;
        }
        current = id;
        restore_object(DIR_BOARDS+"/"+current);
    }
    if(sizeof(posts) < 1) {
        borg = ("No messages posted.\n");
        return borg;
    }
    borg = "";
    for(i=0; i<sizeof(posts); i++) {
        borg +=("["+(i+1>9 ?(i+1): " "+(i+1))+"] ");
        if(member_array( (string)this_player()->query_name(), read_by[i]) == -1)
            borg +=("(new) ");
        else borg += ("      ");
        borg +=(capitalize(posts[i]["owner"])+"("+posts[i]["level"]+"): ");
        borg += (posts[i]["title"] +" ("+posts[i]["size"]+") ");
        borg += (when(posts[i]["date"])+"\n");
    }
    return borg;
}
 
int valid_edit(int x, string id) {
    string who, gm;
    int level;

    gm = "/d/damned/guilds/join_rooms/"
      +(string)this_player()->query_class()+"_join.c";
    gm = gm->query_master();
    if (!gm) gm = "None";
    who = posts[x]["owner"];
    level = posts[x]["level"];
    if(id == UID_ROOT) return 1;
/*  Root may remove any notes */
    if(id == who) return 1;
/* Guildmasters may remove notes */
    if ((string) this_player()->query_name() == gm) return 1;
/* Arches may remove any notes */
    if(this_player() && archp(this_player())) return 1;
/* You cannot remove an arch's note unless you are an arch */
    if(member_group(who, "superuser")) return 0;
    if(member_group(who, "assist")) return 0;
/* Can't remove a note of someone higher than you, no matter what */
    if(this_player() && (int)this_player()->query_level() < level) return 0;
/* Allow the board to have special priveledges */
    if(member_array(id, edit_ok) != -1) return 1;
    return 0;
}

string when(int x) {
    string date, day, mon, year, hour, ret;

    if(sscanf(ctime(x), "%s %s  %s %s %s", day, mon, date, hour, year) !=5) 
	sscanf(ctime(x), "%s %s %s %s %s", day, mon, date, hour, year);
    sscanf(hour, "%s:%s:%*s", hour, ret);
    return(hour+ret+" "+day+" "+date+" "+mon);
}

string get_title(string id, int number) {
    if(!id || number < 0) return 0;
    if(current != id) {
	if(file_size(DIR_BOARDS+"/"+id+".o") < 0) return 0;
	current = id;
	restore_object(DIR_BOARDS+"/"+current);
    }
    if(number > sizeof(posts)-1) return 0;
    return posts[number]["title"];
}

mixed *sub_array(mixed *arr, int x) {
    mixed *a;
    int i;

    for(i=0, a = ({}); i< x; i++) a += ({ arr[i] });
    for(i=x+1; i<sizeof(arr); i++) a+= ({ arr[i] });
    return a;
}

int set_read(string id, string who, int number) {
    string a;

    if(!id || !who || number < 0) return 0;
    if(sscanf(id, "%*s..%s", a) ==1) return 0;
    if(current != id) {
	if(file_size(DIR_BOARDS+"/"+id+".o") < 0) return 0;
	current = id;
	restore_object(DIR_BOARDS+"/"+current);
    }
    if(number > sizeof(read_by)-1) return 0;
    if(member_array(who, read_by[number]) != -1) return 1;
    read_by[number] += ({ who });
    save_object(DIR_BOARDS+"/"+current);
    return 1;
}

int get_num_posts(string id) {
    if(!id) return 0;
    if(current != id) {
	if(file_size(DIR_BOARDS+"/"+id+".o") < 0) return 0;
	current = id;
	restore_object(DIR_BOARDS+"/"+current);
    }
    return sizeof(posts);
}

int get_new_post(string id, string who) {
    int i;

    if(!id || !who) return -1;
    if(current != id) {
	if(file_size(DIR_BOARDS+"/"+id+".o") < 0) return -1;
	current = id;
	restore_object(DIR_BOARDS+"/"+current);
    }
    for(i=0; i<sizeof(read_by); i++)
	if(member_array(who, read_by[i]) == -1) break;
    return i;
}

void clean_up() { return; }
