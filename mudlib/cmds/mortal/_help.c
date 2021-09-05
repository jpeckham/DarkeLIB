//      /bin/user/_help.c
//      from the Nightmare mudlib
//      the help command
//      based on the help created by Wayfarer and Mrx
//      written by Descartes of Borg

#include <std.h>
#include <daemons.h>
#include <dirs.h>

inherit DAEMON;

string alig_str(string str, int x);
int list_cmds(string str);
int skill_help(string skill);
int spell_help(string spell);


void write_banner(string group, string topic) {
    message("help", sprintf("--=== Help for %s (%s) ===--\n", topic, group),
      this_player());
}

int call_help (string path, string topic) {
  string group, file;
  object ob;

  file = path+"/_"+topic;

  if ((ob = find_object(file)) || (file_size(file+".c") >= 0 ) ||
    (file_size(path + "/" + topic) >=0)) {
      switch (path)
        {
        case DIR_USER_HELP :
          group = "General Help";
          write_banner(group, topic);
                   this_player()->more(path+"/"+topic, "help");
          return 1;
          break;
        case DIR_CREATOR_HELP :
          group = "Developer Help";
          write_banner(group, topic);
          this_player()->more(path+"/"+topic, "help");
          return 1;
          break;
        case DIR_MORTAL_CMDS :
          group = "Player Command";
          break;
        case DIR_HM_CMDS:
           group = "High Mortal Command";
            break;
        case DIR_CREATOR_CMDS :
          group = "Developer Command";
          break;
	case DIR_SYSTEM_CMDS :
	  group = "System Command";
	  break;
	case DIR_AMBASSADOR_CMDS :
	  group = "Ambassador Command";
	  break;
        default:
          group = path;
          break;
        }
      message("help", sprintf("--=== Help for %s (%s) ===--", topic, group),
        this_player());


      if (!ob)
        ob = find_object_or_load (file);
      if (!ob)
        {
          message("help", "That command is not loading properly, please "
            "report it using the \"bug\" command.", this_player());
          return 1;
        }
      if (function_exists("help",ob))
        {
          call_other(file,"help");
          return 1;
        }
      else
        {
          message("help", "That command exists, but has no help.  "
            "Please use the bug command to report it.", this_player());
          return 1;
        }
    }
  else
    return 0;
}

int cmd_help(string topic) {
  string *search_path;
  int i;
  string tmp1, tmp2;
  object ob, act_ob;

  act_ob = previous_object();

    seteuid(getuid(previous_object()));
  if(stringp(topic) && sscanf(topic,"%s %s",tmp1,tmp2) == 2) {
	if(tmp1 == "spell") return spell_help(tmp2);
	else if(tmp1 == "skill") return skill_help(tmp2); }
  if (!topic)
    {
     topic = "help";
    }
  if(topic != "I")   /* patch to allow "help I"  by Valodin Jan 93 */
    topic = lower_case (topic);

    if(topic == "commands" || topic == "feelings") {
    return list_cmds(topic);
  }
  if (call_help(DIR_USER_HELP, topic)) return 1;
  if (wizardp(act_ob))
      if (call_help(DIR_CREATOR_HELP, topic)) return 1;

  search_path = (string *)act_ob->query_search_path();
  if (search_path) {
      for (i = 0; i<sizeof(search_path); i++)
              if (call_help(search_path[i],topic)) return 1;
  }

  if((ob = present(topic,act_ob)) || (ob = present(topic,environment(act_ob))))    {
      if(function_exists("help",ob))
        {
        message("help", sprintf("--=== Help for %s (Object Help) ===--",
          topic), this_player());
          ob->help();
          return 1;
        }
    }
  message("help", "There is no help available for that topic.", this_player());
  return 1;
}

int list_cmds(string str) {
    string borg;
    string *comms, *paths;
    int i;

    borg = "\n";
    if(str == "feelings") comms = (string *)SOUL_D->query_feelings();
    else {
        comms = ({});
        i = sizeof(paths = (string *)this_player()->query_search_path());
        while(i--) comms += (string *)CMD_D->query_commands(paths[i]);
    }
    comms = sort_array(comms, "alpha", this_object());
    for(i=0; i <sizeof(comms); i+=5) {
        borg += alig_str(comms[i], 13);
        if(i+1<sizeof(comms)) borg += alig_str(comms[i+1], 13);
        if(i+2<sizeof(comms)) borg += alig_str(comms[i+2], 13);
        if(i+3<sizeof(comms)) borg += alig_str(comms[i+3], 13);
        if(i+4<sizeof(comms)) borg += alig_str(comms[i+4], 13);
        borg += "\n";
    }
    this_player()->more(explode(borg, "\n"), "help");
    return 1;
}

string alig_str(string str, int x) {

    int i, y;

    if(!str) str = "";
    y = strlen(str);
    x -= y;
    if(x < 1) return str;
    for(i=0; i<= x; i++) {
        str += " ";
    }
    return str;
}

void help() {
    write("Syntax: <help [topic]>\n\n"+
        "Gives you help ony any subject for which help is available.\n"+
        "Help exists for all commands.  For a list of what topics are\n"+
        "available, type <help topics>.\n"
    );
}

int alpha(string a, string b) { return strcmp(a, b); }

int skill_help(string skill) {
    string file;

    file = "/std/skills/"+replace_string(skill," ","_");
    if(!file_exists(file+".c"))
	return notify_fail("There is no such skill.\n");
    write("%^CYAN%^Skill Help: %^RESET%^"+capitalize(skill));
    write("%^CYAN%^Development Cost: %^RESET%^"+(int)file->query_dev());
    write("%^CYAN%^Fast Development Cost: %^RESET%^"+(int)file->query_fast_dev());
if(file->query_property("prereq"))
    write("%^CYAN%^Prerequisite Skill: %^RESET%^"+file->query_property("prereq") );
    write("%^CYAN%^Stat: %^RESET%^"+(string)file->query_stat());
if(file->query_property("no crosstrain"))
    write("%^CYAN%^Can Crosstrain: %^RESET%^no");
    write("%^CYAN%^------------------------------------------------------------------------%^RESET%^");
    if(undefinedp(file->info()))
	return notify_fail("This skill has no help!  Tell a wiz.\n");
    return 1;
}

int spell_help(string spell) {
    string file, dur;
    int tmp;

    file = "/std/spells/"+replace_string(spell," ","_");
    if(!file_exists(file+".c"))
        return notify_fail("There is no such spell.\n");
    write("%^GREEN%^Spell Help: %^RESET%^"+capitalize(spell));
    write("%^GREEN%^Development Cost: %^RESET%^"+(int)file->query_property("dev cost"));
    write("%^GREEN%^Fast Development Cost: %^RESET%^"+(int)file->query_property("fast dev cost"));
    write("%^GREEN%^Base mp Cost: %^RESET%^"+(int)file->query_property("base mp cost"));
    if(file->query_property("casting time"))
    write("%^GREEN%^Casting Time: %^RESET%^"+(int)file->query_property("casting time"));
    else
    write("%^GREEN%^Casting Time: %^RESET%^Instantaneous");
    write("%^GREEN%^Moon: %^RESET%^"+(string)file->query_property("moon"));
    write("%^GREEN%^Spell Level: %^RESET%^"+(int)file->query_property("spell level"));
    if(!file->query_property("duration"))
      dur = "n/a";
    else if(stringp(file->query_property("duration")))
      dur = (string)file->query_property("duration");
    else if(intp(file->query_property("duration"))) {
      tmp = (int)file->query_property("duration");
      switch(tmp) {
      case 0..120:
	dur = sprintf("%d rounds", tmp / 2);
	break;
      case 121..3599:
	dur = sprintf("%d minutes", tmp / 60);
	break;
      default:
	dur = sprintf("%d hours, %d minutes", tmp / 3600, (tmp%3600)/60);
      }
    }
    else
      dur = "n/a";
    write("%^GREEN%^Duration: %^RESET%^"+dur);
    write("%^GREEN%^Casting Skill: %^RESET%^"+(string)file->query_property("skill") );
if(file->query_property("prereq"))
    write("%^GREEN%^Prerequisite Spell: %^RESET%^"+(string)file->query_property("prereq") );
if(file->query_property("no crosstrain"))
    write("%^GREEN%^Can Crosstrain: %^RESET%^no");
    write("%^GREEN%^------------------------------------------------------------------------%^RESET%^");
    if(undefinedp(file->info()))
	return notify_fail("No help for this spell.  Tell a wiz!\n");
    return 1;
}
