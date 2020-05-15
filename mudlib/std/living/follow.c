//	/std/living/follow.c
//	from the Nightmare mudlib
//	code for allowing people to follow
//	created by Descartes of Borg 15 february 1993

nosave private string *allowed_to_follow;
nosave private object *followers;
nosave private object following;

object *query_followers() { return followers; }
object query_following() { return following; }

void set_following(object f) { following = f; }

int set_followers(object *f) {
    int i;

    if(!f) return 0;
    if(member_array( this_object(), f ) != -1 ) return 0;
    if(following && member_array(following, f) != -1) return 0;
    followers = f;
    for(i=0; i<sizeof(f); i++) {
	if(!f[i]) followers -= ({ f[i] });
	else f[i]->set_following(this_object());
    }
    return 1;
}

int add_follower(object f) {
    if( f == this_object() ) return 0;
    if( f == following ) return 0;
    if(!followers) {
	followers = ({ f });
	f->set_following(this_object());
	return 1;
    }
    f->set_following(this_object());
    followers = followers + ({ f });
    return 1;
}

void remove_follower(object ob) {
    if(followers && member_array(ob, followers) == -1) 
	return;
    followers -= ({ ob });
    ob->set_following(0);
}

void clear_followers() {
    int i;

    if(!followers) return;
    for(i=0; i<sizeof(followers); i++) 
	if(followers[i]) followers[i]->set_following(0);
    followers = ({});
}

void move_followers(object prev) {
    object *tmp;
    int i;
    string verb;

    tmp = ({});
    verb = query_verb();
    for(i=0; i<sizeof(followers); i++) {
        if(!objectp(followers[i])) continue;
	if(!environment(followers[i])) continue;
	if(this_object()->query_invis() && !followers[i]->query("see invis")) continue;
	if(environment(followers[i]) != prev) {
	    tell_object(this_object(), "You have lost "+followers[i]->query_cap_name()+".\n");
	    tell_object(followers[i],
		this_object()->query_cap_name()+" has ditched you.\n");
	    followers[i]->set_following(0);
	}
	else {
	    if( followers[i]->follow(verb, environment(this_object())))
        tmp += ({ followers[i] });
      else
        followers[i]->set_following(0);
	}
    }
    followers = tmp;
}

void follow_allow(string str) {
  if(!allowed_to_follow) allowed_to_follow = ({});
  allowed_to_follow = distinct_array(allowed_to_follow + ({ str }));
}

int follow_disallow(string str) {
  if(!allowed_to_follow) return 0;
  if(member_array(str, allowed_to_follow) < 0) return 0;
  allowed_to_follow -= ({ str });
}

int can_follow(string str) {
  if(!allowed_to_follow || !pointerp(allowed_to_follow)) return 0;
  return (member_array(str, allowed_to_follow) >= 0);
}

int follow(string exit, object env) {
    string cmd;
    int i;

    if(!environment(this_object())) return 0;
    if(!(cmd = exit) || !strlen(exit)) {
      if(!env) return 0;
      cmd = base_name(env);
      i = strlen(cmd) - 1;
      if(i <= 1) return 0;
      if(cmd[i-1..i] == ".c") cmd = cmd[0..i-2];
      cmd = (string)environment(this_object())->query_direction(cmd);
    }
    command(cmd);
    return 1;
}
