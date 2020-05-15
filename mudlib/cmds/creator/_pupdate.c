/*

 * _pupdate.c

 * description: update a program

 * Author: Wayfarer

 * last modified : 2/13/92

 */



#include <std.h>



inherit DAEMON;



private int load (string str) 

{

  string res;

  res = catch (call_other(str,"???"));

  if (!res) return 1;

  write (res);

  return 1;

}



int cmd_pupdate (string str) 

{

  object ob, bag, thing, next_thing;

  int i;

  

  seteuid("TEMP");

  if (!str) {

    ob = environment (previous_object());

    if (ob && environment(ob)) 

      {

	write("Not in a room.\n");

	return 1;

      }

    str = "/" + file_name(ob);

  }

   str = resolv_path((string)this_player()->get_path(),str);

  if (!str) 

    {

      notify_fail ("*Error in update\n");

      return 0;

    }

  ob = find_object(str);

  if (!ob) 

    {

      if (file_size(str+".c") < 0) 

	{

	  write("Invalid file or directory.\n");

	  return 1;

	}

      if (!load(str)) 

	{

	  write("No such object.\n");

	}

      return 1;

    }

  /* save everything in the room first... */

  bag = clone_object("/std/container");

  thing = first_inventory(ob);

  while (thing) 

    {

      next_thing = next_inventory(thing);

      thing->move(bag);

      thing = next_thing;

    }

  destruct(ob);

  file_size(str);

  load(str);

  bag->move(str);

  ob = find_object(str);

  thing = first_inventory(bag);

  while (thing) 

    {

      next_thing = next_inventory(thing);

      if (ob) 

	thing->move(ob);

      else 

	{

	  thing->move("/d/standard/void");

	  tell_object(thing,"The room you are in dissolves into a mass of " +

		      "twisted logic and confusion.\n" +

		      "You suddenly find yourself somewhere else.\n");

	}

      thing = next_thing;

    }

  

  destruct(bag);

  if (str == master())

    tell_group 

      (CONFIG_DIR+"/admin",

       "Upadate of master.c by "+(string)previous_object()->query_cap_name()+"\n");

  return 1;

}



int help()

{

  write( @EndText

Syntax: pupdate <string>

Effect: ??? Anyone know how this differs from update?

EndText

  );

  return 1;

}

