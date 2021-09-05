//      /std/room/exits.c
//      from the Nightmare Mudlib
//      functions handling room exits
//      created by Descartes of Borg 18 june 1993

#include <rooms.h>

int skip_obvious;
private mapping destinations;
private mapping invis_exits;
private mapping pre_exit_func;
private mapping post_exit_func;
private mixed *tracks;

int invis_exit(string str);

#define MAX_TRACKS  10

#define NUMBER   ({ "two", "three", "four", "five", "six", "seven", "eight",\
                  "nine", "ten", "many" })

#define TMPLONG  ({ "north", "northeast", "east", "southeast", "south",\
                  "southwest", "west", "northwest", "up", "down", "in",\
                  "out", "enter","exit",})

#define TMPSHORT ({ "n", "ne", "e", "se", "s", "sw", "w", "nw", "u", "d",\
                  "in","out", "enter","exit",})

void create() {
    destinations = ([]);
    invis_exits = ([]);
    pre_exit_func = ([]);
    post_exit_func = ([]);
    tracks = ({ });
    skip_obvious = 0;
}

string query_exit(string str) {
    if(str && destinations && destinations[str]) return destinations[str];
    else return ROOM_VOID;
}

string query_direction(string str) {
    int i;

    if(str && destinations && 
      (i=member_array(str, values(destinations))) != -1)
        return keys(destinations)[i];
    else return ROOM_VOID;
}

protected void perform_post_exits(string str) {
    if(!str || !post_exit_func || !post_exit_func[str]) return;
    call_other(this_object(), post_exit_func[str]);
}

protected int perform_pre_exits(string str) {
    if(!str || !pre_exit_func || !pre_exit_func[str]) return 1;
    return (int)call_other(this_object(), pre_exit_func[str]);
}

// tracking function added by Valodin
// August 10, 1993
//
// the first track is the most recent one
// after MAX_TRACKS tracks then the oldest ones get taken off
// each track is an array ({ player_name, exit_name, time })

void add_tracks(string verb, object ob)
{
  mixed *new_entry;
  int szt;

  new_entry = ({ 0, 0, 0});
  new_entry[0] = ob->query_cap_name();
  new_entry[1] = verb;
  new_entry[2] = time();
  if((szt = sizeof(tracks)) >= MAX_TRACKS)
    tracks -= ({ tracks[(szt - 1)] });
    tracks += ({ new_entry });
}

mixed *query_tracks() { return tracks; }

string *query_track_makers()
{
  string *ret;
  int i, sztr;

  ret = ({});
  for(i = 0, sztr = sizeof(tracks); i < sztr; i++)
    ret += ({ (string)tracks[i][0] });
  return ret;
}

int use_exit() {
    string verb;
    string *my_limbs;
    int i, flag;
    
    if(this_player()->query_paralyzed()) {
        notify_fail("You are unable to move.\n");
        return 0;
    }
    if( this_player()->query_disable() &&
        sizeof(this_player()->query_attackers()) ) {
       write("You can not exit while doing something else.");
       return 1;
    }
    if(pointerp(this_player()->query_limbs()) &&
       pointerp(this_player()->query_wielding_limbs())) {
      my_limbs = (string *)this_player()->query_limbs();
      i = sizeof(my_limbs);
      flag = 0;
      while(i--)
        if(member_array(my_limbs[i],
          (string *)this_player()->query_wielding_limbs()) < 0)
            flag = 1;
      if(!flag) {
        message("my_action", "You cannot move with no mobility limbs!",
                    this_player());
        return 1;
      }
    }
    if(!(verb = query_verb())) return 0;
    if(query_exit(verb) == ROOM_VOID) {
        write("Error:  Set to VOID.  Notify creator of room.\n");
        return 1;
    }
    if(!perform_pre_exits(verb)) return 1;
//   add_tracks(verb, this_player());
    if(!wizardp(this_player()) &&
      !find_object_or_load(query_exit(verb))) {
      message("info",
              "This room is not available yet.  It should be ready "
              "in a week or two.  If it is not up by then, notify "
              "an arch.", this_player());
      return 1;
    }
    this_player()->move_player(query_exit(verb), verb);
    perform_post_exits(verb);
    return 1;
}

protected void initiate_exits() {
    string *borg;
    int i;

    i = sizeof(TMPLONG);
    while(i--) add_action("use_stupid_exit", TMPLONG[i]);
    if(!destinations) return;
    i = sizeof(borg = keys(destinations));
    while(i--) add_action("use_exit", borg[i]);

}


varargs protected void set_exits(mixed dests, string *dirs) {
    int i;
    string *d_keys;

    if(mapp(dests)) {
        d_keys = keys(dests);
        i = sizeof(d_keys);
        destinations = ([]);
        while(i--)
          this_object()->add_exit(dests[d_keys[i]], d_keys[i]);
        return;
    }
    if(!dests || !dirs || ((i = sizeof(dests)) != sizeof(dirs))) return;
    destinations = ([]);
    while(i--) this_object()->add_exit(dests[i], dirs[i]);
}

void add_exit(string dest, string dir) {
    string path;
  
    if(!dest || !dir) return;
    if(!destinations) destinations = ([]);
    path = "/"+implode(exclude_array(explode(base_name(this_object()), "/"),
            sizeof(explode(base_name(this_object()), "/")) - 1), "/");
    dest = resolv_path(path, dest);
    destinations[dir] = dest;
    this_object()->reinitiate();
}

protected void remove_exit(string dir) {
    if(!dir || !destinations || !destinations[dir]) return;
    map_delete(destinations, dir);
    this_object()->reinitiate();
}

string *query_exits() { return (destinations ? keys(destinations) : ({})); }

string query_full_direction(string str) {
    int i;

    if((i=member_array(str, TMPSHORT)) == -1) return str;
    else return TMPLONG[i];
}

string *query_destinations() {
    return (destinations ? values(destinations) : ({}));
}

string *query_obvious_exits() {
    string *exits, *sorties;
    int i;

    if(!destinations) return ({});
    else i = sizeof(sorties = keys(destinations));
    exits = ({});
    while(i--)
      if(!invis_exit(sorties[i])) exits += ({ sorties[i] });
    return (exits);
}

string query_long_exits() {
    int i, max;
    string str;
    string *sorties;

    if(skip_obvious) return "";
    if(!(i=sizeof(sorties = query_obvious_exits())))
      return "There are no obvious exits.";
    if(i==1)
      return "The only obvious exit is "+sorties[0]+".";
    if((max=i) > sizeof(NUMBER) + 1) max = sizeof(NUMBER)+1;
    str = ("There are " + NUMBER[max - 2] + " obvious exits: ");
    for(max = i, i = 0; i<max; i++) {
	if(i == max-1) str += "and ";
	str += sorties[i];
	if(i == max -1) str += ".";
	else if(max > 2) str += ", ";
	else str += " ";
    }
    return str;
}

string query_short_exits() {
    int i, x, max;
    string s;
    string *lx, *sx;

    s = "";
    sx = allocate(max = sizeof(lx = query_obvious_exits()));
    for(x=0; x< max; x++) {
        if( (i=member_array(lx[x], TMPLONG)) != -1)
          sx[x] = TMPSHORT[i];
        else sx[x] = lx[x];
    }
    if(!sizeof(sx)) return "[no exits]";
    if(max == 1) return "[1: "+sx[0]+"]";
    s = ("[" + max + ": ");
    for(x=0; x<max; x++) {
        if(x) s += ", ";
        s += sx[x];
    }
    return s + "]";
}

protected void set_post_exit_functions(string *dirs, string *funcs) {
    int i;

    if(!dirs || !funcs || ((i=sizeof(dirs)) != sizeof(funcs))) return;
    post_exit_func = ([]);
    while(i--) post_exit_func[dirs[i]] = funcs[i];
}

protected void add_post_exit_function(string dir, string func) {
    if(!post_exit_func) post_exit_func = ([]);
    post_exit_func[dir] = func;
    this_object()->reinitiate();
}

protected void remove_post_exit_function(string dir) {
    if(!dir || !post_exit_func || !post_exit_func[dir]) return;
    map_delete(post_exit_func, dir);
    this_object()->reinitiate();
}

protected void set_pre_exit_functions(string *dirs, string *funcs) {
    int i;

    if(!dirs || ! funcs || (sizeof(dirs) != sizeof(funcs))) return;
    if(!pre_exit_func) pre_exit_func = ([]);
    i = sizeof(dirs);
    while(i--) pre_exit_func[dirs[i]] = funcs[i];
}

protected void add_pre_exit_function(string dir, string func) {
    if(!dir || !func) return;
    if(!pre_exit_func) pre_exit_func = ([]);
    pre_exit_func[dir] = func;
    this_object()->reinitiate();
}

protected void remove_pre_exit_function(string dir) {
    if(!pre_exit_func || !pre_exit_func[dir]) return;
    map_delete(pre_exit_func, dir);
    this_object()->reinitiate();
}

void set_invis_exits(string *dirs) {
    int i;

    invis_exits = ([]);
    i = sizeof(dirs);
    while(i--) invis_exits[dirs[i]] = 1;
}

void add_invis_exit(string dir) {
    if(!invis_exits) invis_exits = ([]);
    invis_exits[dir] = 1;
}

void remove_invis_exit(string dir) {
    if(!invis_exits) return;
    if(!invis_exits[dir]) return;
    map_delete(invis_exits, dir);
}

int invis_exit(string dir) {
    if(!invis_exits) return 0;
    else return invis_exits[dir];
}

void skip_obvious() { skip_obvious = 1; }

int use_stupid_exit() {
    write("You cannot go that way.\n");
    return 1;
}

string *query_pre_exit_functions() {
    if(pre_exit_func) return keys(pre_exit_func);
    else return ({});
}

string query_pre_exit_function(string sortie) {
    if(!pre_exit_func) return 0;
    else return pre_exit_func[sortie];
}

string *query_post_exit_functions() {
    if(!post_exit_func) return ({});
    else return keys(post_exit_func);
}

string query_post_exit_function(string sortie) {
    if(!post_exit_func) return 0;
    else return post_exit_func[sortie];
}
