/*
 *	/std/move.c
 *	from the Nightmare mudlib
 *	handles the moving and removing of objects
 *	created by Descarte of Borg 18 february 1993
 *      modified by Geldron to support funky things
 *      modified by Geldron 031396 to fix desting ppl in room's bugs
 */

#include <move.h>
#include <daemons.h>
#include <rooms.h>

nosave private int no_clean;
private int encumbrance;
nosave private object last_location;

void set_weight(int x);
void set_no_clean(int x);
int clean_up();
void set_last_location(object ob);
int query_weight();
int move(mixed dest);
int remove();

int move(mixed dest) {


  object ob, spell;
  string str;
  
  if(!this_object()) return MOVE_DESTRUCTED;
  if(stringp(dest)) {
    ob = find_object(dest);
    if(!ob) {
      str = catch(call_other(dest, "???"));
      if(str) {
	write(str+"\n");
	return MOVE_NO_DEST;
      }
      ob = find_object(dest);
    }
  }
  else ob = dest;
  if(!ob) return MOVE_NOT_ALLOWED;
  if(!ob->receive_objects()) return MOVE_NOT_ALLOWED;
  /* Modified by Geldron 030696 to allow players to go into other 
   * living/nonliving inventories
   */
  if(living(this_object()) && !interactive(this_object()) &&
     living(ob) && !( (int)ob->move_ok() )) return MOVE_NOT_ALLOWED;
  if((interactive(this_object()) && interactive(ob)) ||
     (interactive(this_object()) && environment(this_object()) &&
      !environment(this_object())->query_room())) {
  } else {
    if(!ob->add_encumbrance((int)this_object()->query_weight()))
      return MOVE_NO_ROOM;
    if(environment(this_object()))
      environment(this_object())->add_encumbrance(-1 *
						  (int)this_object()->query_weight());
  }
  if(!this_object()) return MOVE_OK;
  set_last_location(environment(this_object()));
  if(living(this_object()) && (spell = (object)this_object()->
			       query_casting()) &&
     !spell->move_while_casting(this_object())) {
    message("info","%^CYAN%^You interrupt your spell.",
	    this_object());
    message("info","%^CYAN%^"+(string)this_object()->query_cap_name()+
	    " interrupts "+possessive(this_object()) + " spell.",
	    environment(),({ this_object() }));
    this_object()->set_casting(0);
    spell->remove();
  }
  move_object(ob);
  if(last_location && file_name(last_location) != ROOM_VOID && 
     ob->query_is_locker_room() &&
     this_object()->query_is_locker() && !this_object()->is_pet())
    SAVE_ITEMS_D->update_lockers(ob);
  if(ob && file_name(ob) == ROOM_VOID) return MOVE_OK;
  ob = last_location;
  if(objectp(ob) && ob->query_is_locker_room() &&
     this_object()->query_is_locker() && !this_object()->is_pet())
    SAVE_ITEMS_D->update_lockers(ob);
  return MOVE_OK;
}

int remove() {
  object env;
  object *inv;
    int i;

    env = environment(this_object());
    if(env) {
	env->add_encumbrance(-1 * (int)this_object()->query_weight());
	i= sizeof(inv = all_inventory(this_object()));
	while(i--) inv[i]->move(env);
    }
      this_object()->unequip();
    if(this_object()->query_is_locker())
      this_object()->move(load_object(ROOM_VOID));
    destruct(this_object());
    return 1;
}

/*
 * The new clean_up function.  Only destructs an object if
 * it has no environment.  Otherwise, leaves it alone.
 *
 * Diewarzau 7/22/96
 */

int clean_up() {
  object ob;

  if(!environment()) {
    catch(this_object()->remove());
    if(this_object()) destruct(this_object());
    return 0;
  }
  ob = this_object();
  if(ob->is_player()) return 0;
  if(ob->query("no clean up")) return 0;
  if(ob->is_pet() && stringp(ob->query_owner()) &&
     find_player((string)ob->query_owner())) return 1;
  if(living(ob) && ((int)ob->query_max_hp() /
		    (int)ob->query_hp()) > 2) return 1;
  ob = environment();
  while(ob) {
    if(living(ob)) return 1;
    if(ob->query_property("storage room")) return 1;
    ob = environment(ob);
  }
  catch(this_object()->remove());
  return 0;
}

/*
 * Old clean_up, just in case new one doesn't work out...
 */

#if 0
int clean_up() {
    object ob, env;
    object *inv;
    int i, flag;

    if(virtualp(this_object())) flag = 1;
    else flag = 0;

    /* If the object is set not to be cleaned, forget about it */
    if(no_clean) return 0;

    /* If the object is autoloading, forget about it */
    if(!(ob = this_object()) || ob->query_auto_load()) return 0;

    /* Wait on anything in a bag or in a storage room */
    env = environment(ob);
    if(env) if(env->is_bag()) return 1;
    if(this_object()->is_player()) return 0;
    if(env) if(env->query_property("storage room")) return 1;
    if(this_object()->query_property("storage room")) return 0;
    if(query_heart_beat(this_object())) return 1;


    /* NEVER clean a pet */
    if(this_object()->is_pet()) return 0;
    /* Get the deep inventory of the object an setup an index counter */
    if(!objectp(ob)) return 0;
    i = sizeof(inv = deep_inventory(ob));

    /* Wait on anything with players inside it */
    while(i--) {
	if(inv[i]->is_player()) return 1;
	if(interactive(inv[i]) && query_idle(inv[i]) < 1800) return 1;
	if(query_heart_beat(inv[i])) return 1;
	if(inv[i]->is_pet()) return 1;
    }

    /* No environment! Get rid of it and everything in it! */
    if(!env) {
	if(i = sizeof(inv)) 
	    while(i--) {
		inv[i]->remove();
		if(inv[i]) destruct(inv[i]);
	    }
	if(ob) ob->remove();
	if(ob) destruct(ob);
	if(flag)
	    return 1;
	else
	    return 0;
    }

    /* Wait for anything inside a player */
    if(living(env) && userp(env)) return 1;

    /* Wait on any player who is not idle more than 30 minutes */
    if(userp(ob)) {
	if(!interactive(ob)) return 1;
	else if(query_idle(ob) < 1800) return 1;
    }

    /* Let the environment make sure everything else is ok to go.
       It will get rid of this object in its version of the if(!env) above */
    env->clean_up();
    if(ob) return 1;
    if(flag)
	return 1;
    else
	return 0;
}
#endif

void set_last_location(object ob) { 
    if(!ob || !objectp(ob)) return;
    last_location = ob;
}

object query_last_location() { return last_location; }

void set_weight(int x) { encumbrance = x; }

int query_weight() { return encumbrance; }

void set_no_clean(int x) { no_clean = x; }

int move_or_destruct(object env_of_destee) {
    if(!TO || !objectp(TO))
	return 0;
    if(interactive(TO)) {
	message("environment", "%^RED%^You are transferred to the Akkad Church...%^RESET%^",
	  TO);
	TO->move_player(ROOM_START);
    }
    else if(env_of_destee)
	return move(env_of_destee);
    return 0;
}
