//      /std/armour.c

//      from the Nightmare Mudlib

//      the basic inherited armour object

//      created by Descartes of Borg 25 june 1993



#include <std.h>



inherit OBJECT;



private mapping armour_save;

private nosave mapping armour_static;



void unwear();

void extinguish_glow();

void unequip();

void set_not_equipped();

void set_enh_critical(mixed x);

void set_ac(int x, string type);

void set_illuminate(int x);

void set_limbs(mixed *borg);

void set_wear(mixed val);

void set_remove(mixed val);

void set_type(string str);

int query_ac(string type);

mapping all_base_ac();

void set_short(string desc);

mapping query_all_ac();

string *query_ac_types();

mixed *query_limbs();

string query_wear_string();

object query_wear_func();

mixed query_enh_critical();

string query_remove_string();

object query_remove_func();

string query_type();

object query_worn();



void init() {

    ::init();

    if(environment(this_object()) != this_player()) return;

    add_action("wear", "wear");

    add_action("do_remove", "remove");

    add_action("illuminate", "illuminate");

}



void create() {

    ::create();

    armour_save = ([]);

    armour_static = ([]);

    set_material("metal/iron");

}



int wear(string str) {

    mixed *lim;

    string *new_limbs;

    string ret, what, where, *tmp;

    int i, j, k;

    function *funx;



    if(!str) return notify_fail("Wear what?\n");

    if(present((what = str), this_player()) != this_object() && sscanf(str, "%s on %s", what, where) != 2) {

	if(parse_objects(this_player(), str) != this_object()) {

	    notify_fail("You don't have that!\n");

	    return 0;

	}

    }

    else if(present(what, this_player()) != this_object() && parse_objects(this_player(), what) != this_object()) {

	notify_fail("You don't have that!\n");

	return 0;

    }

    if(query_worn()) {

	message("my_action", "You are already wearing that!", this_player());

	return 1;

    }

    i = sizeof(lim = query_limbs());

    new_limbs = ({});

    while(i--) {

	if(stringp(lim[i])) {

	    new_limbs += ({ lim[i] });

	    continue;

	}

	if((j=member_array(where, lim[i])) != -1) new_limbs += ({ lim[i][j] });

	else if(where) {

	    message("my_action", "You cannot wear this on your "+where+".",

	      this_player());

	    return 1;

	}

	else if(sizeof(lim) != 1) {

	    j = sizeof(lim[i]);

	    tmp = (string *)this_player()->query_limbs();

	    while(j--)

		if((k = member_array(lim[i][j], tmp)) > -1)

		    new_limbs += ({ tmp[k] });

	}

	else {

	    write("You must specify where you want to wear it.");

	    return 1;

	}

    }

    new_limbs = distinct_array(filter_array(new_limbs, "string_filter", this_object()));

    if(ret = (string)this_player()->equip_armour_to_limb(this_object(),new_limbs)) {

	if(ret == "NO") return 1;

	message("my_action", ret, this_player());

	return 1;

    }

    if(stringp(armour_static["wear"]))

	message("my_action", armour_static["wear"], this_player());

    else message("my_action", "You wear "+query_short()+".", this_player());

    message("other_action", (string)this_player()->query_cap_name()+

      " wears "+(string)this_player()->query_possessive()+" "+query_short()+

      ".", environment(this_player()), ({ this_player() }));

    armour_static["worn by"] = this_player();

    armour_static["actual limbs"] = new_limbs;

    return 1;

}



int string_filter(mixed element) { return stringp(element); }



int do_remove(string str) {

    function *funx;

    int i;



    if(!str) return notify_fail("Remove what?\n");

    if(!id(str) && parse_objects(this_player(), str) != this_object()) {

	notify_fail("You don't have that!\n");

	return 0;

    }

    if(!query_worn()) {

	message("my_action", "You are not wearing that!", this_player());

	return 1;

    }

    if(functionp(armour_static["unwear"])){

	if(!((*armour_static["unwear"])())) return 1;

    }

    if(pointerp(armour_static["unwear"])) {

	funx = armour_static["unwear"];

	i = sizeof(funx);

	while(i--) {

	    if(!functionp(funx[i])) continue;

	    if(!(*funx[i])()) return 1;

	}

    }

    unwear();

    return 1;

}



int illuminate(string str) {

    int heure;



    if(!query_property("magic item") || 

      member_array("illuminate", query_property("magic item")) == -1) return 0;

    if(!str || !id(str)) {

	notify_fail("Illuminate what?\n");

	return 0;

    }

    heure = 5*((int)this_player()->query_stats("wisdom")) +

    (int)this_player()->query_skill("conjuring");

    if(heure < 20) heure = 20;

    if((int)this_player()->query_mp() < heure/10) {

	write("Too low on magic power.\n");

	return 1;

    }

    this_player()->add_mp(-(heure/10));

    write("Your "+query_name()+" begins to glow.\n");

    say(this_player()->query_cap_name()+"'s "+query_name()+" begins to glow.\n");

    environment(this_object())->set_property("light",

      (armour_static["lit"] = 1+random(3)));

    call_out("extinguish_glow", heure);

    return 1;

}



void unwear() {

    if(stringp(armour_static["unwear"]))

	message("my_action", armour_static["unwear"], armour_static["worn by"]);

    else message("my_action", "You remove your "+query_short()+".",

	  armour_static["worn by"]);

    armour_static["worn by"]->remove_armour_from_limb(this_object(),

      armour_static["actual limbs"]);

    message("other_action", (string)armour_static["worn by"]->query_cap_name()+

      " removes "+(string)armour_static["worn by"]->query_possessive()

      +" "+query_name()+".", environment(armour_static["worn by"]),

      ({ armour_static["worn by"] }));

    map_delete(armour_static, "worn by");

    map_delete(armour_static, "actual limbs");

}



void extinguish_glow() {

    if(!this_object()) return;

    if(!armour_static["lit"]) return;

    if(living(environment(this_object()))) {

	tell_object(environment(this_object()),"The magic glow of your

"+query_name()+" fades away.\n");

        tell_room(environment(environment(this_object())),

environment(this_object())->query_cap_name()+"'s "+query_name()+

          "magic glow fades away.\n", ({environment(this_object()) }) );

    }

    else tell_room(environment(this_object()), "The magic glow of the "+

      query_name()+" fades away.\n");

    environment(this_object())->set_property("light", -armour_static["lit"]);

    map_delete(armour_static, "lit");

}

 

void unequip() { if(armour_static["worn by"]) unwear(); }

 

void set_not_equipped() {

    if(query_worn()) map_delete(armour_static, "worn by");

    if(armour_static["actual limbs"]) map_delete(armour_static, "actual limbs");}

 

int remove() {

    if(armour_static && armour_static["lit"]) {

      if(environment(this_object()))

        environment(this_object())->set_property("light",-armour_static["lit"]);

        map_delete(armour_static, "lit");

    }

    return ::remove();

}

 

int move(mixed dest) {

    int x;

 

    if(armour_static["lit"])

      environment(this_object())->set_property("light", -armour_static["lit"]);

    x = ::move(dest);

    if(armour_static["lit"])

      environment(this_object())->set_property("light", armour_static["lit"]);

    return x;

}



void set_enh_critical(mixed x) {

    if(mapp(x) || intp(x))

    	armour_save["enhance critical"] = x;

}

 

varargs void set_ac(int x, string type) {

    int i;



    if(!armour_save["ac"]) armour_save["ac"] = ([]);

    if(!type || !stringp(type)) {

	for(i=0;i<sizeof(DAMAGE_TYPES);i++) {

	    armour_save["ac"][DAMAGE_TYPES[i]] = x;

	} return; }

    if(member_array(type, DAMAGE_TYPES) < 0) return;

	else

	armour_save["ac"][type] = x;

}

 

void set_illuminate(int x) { set_property("magic item", ({ "illuminate" })); }

 

void set_limbs(mixed *limbs) {

  int i;



  if(armour_static["type"] == "ring") return;

  i = sizeof(limbs);

  while(i--) {

    if(pointerp(limbs[i]) && member_array("right arm", limbs[i]) != -1 &&

	member_array("left arm", limbs[i]) != -1)

      limbs[i] = distinct_array(limbs[i] + ({ "right arm", "left arm", "first arm", "second arm", 

		      "third arm", "fourth arm", "first branch", "second branch",

		      "third branch", "fourth branch" }));



    if(pointerp(limbs[i]) && member_array("right leg", limbs[i]) != -1 &&

	member_array("left leg", limbs[i]) != -1)

      limbs[i] = distinct_array(limbs[i] + ({ "first leg", "second leg", "third leg",

		      "fourth leg", "first root", "second root", "third root", "fourth root" }));

//Added by Drizzt 10/15/96 to fix thranx feet

//    if(pointerp(limbs[i]) && member_array("right foot", limbs[i]) != -1 &&

//        member_array("left foot", limbs[i]) != -1)

//      limbs[i] = distinct_array(limbs[i] + ({ "first foot", "second foot", "third foot", "fourth foot" }));

    if(pointerp(limbs[i]) && member_array("right hand", limbs[i]) != -1 &&

       member_array("left hand", limbs[i]) != -1)

      limbs[i] = distinct_array(limbs[i] + ({ "first hand", "second hand", "third hand", "fourth hand" }));

    if(stringp(limbs[i]) && limbs[i] == "right hand")

      limbs[i] = ({ "right hand", "first hand", "second hand" });

    if(stringp(limbs[i]) && limbs[i] == "left hand")

      limbs[i] = ({ "left hand", "third hand", "fourth hand" });

    if(stringp(limbs[i]) && limbs[i] == "right arm")

      limbs[i] = ({ "right arm", "first arm", "second arm", "first branch", "second branch" });

    if(stringp(limbs[i]) && limbs[i] == "left arm")

      limbs[i] = ({ "left arm", "third arm", "fourth arm", "third branch", "fourth branch" });

    if(stringp(limbs[i]) && limbs[i] == "right leg")

      limbs[i] = ({ "right leg", "first leg", "second leg", "first root", "second root" });

    if(stringp(limbs[i]) && limbs[i] == "left leg")

      limbs[i] = ({ "left leg", "third leg", "fourth leg", "third root", "fourth root" });

  }

  armour_static["possible limbs"] = limbs; 

}



void set_wear(mixed val) {

    if(!functionp(val) && !stringp(val) && !pointerp(val)) return 0;

    armour_static["wear"] = val;

}

 

void set_remove(mixed val) {

    if(!functionp(val) && !stringp(val) && !pointerp(val)) return;

    armour_static["unwear"] = val;

}

 

void set_type(string str) {

  armour_static["type"] = str;

  if(str == "ring") {

    armour_static["possible limbs"] =

      ({ ({ "right hand", "left hand", "first branch", "second branch",

            "third branch", "fourth branch " }) });

  }

  return;

}

 

int query_ac(string type) {

    string *dmg_types;

    int tmp;

    mapping extra;



    if(!armour_save["ac"]) return 0;

    dmg_types = keys(armour_save["ac"]);

    if(!pointerp(dmg_types)) return 0;

    if(!type || !stringp(type)) type = dmg_types[0];

    tmp = armour_save["ac"][type] + props["enchantment"];

    if(intp(this_object()->prop("armour bonus")))

	tmp += (int)this_object()->prop("armour bonus");

	  if(mapp(extra=query_property("extra ac")))

	    if(extra[type]) tmp += extra[type];

	  if(mapp(extra=prop("extra ac")))

	    if(extra[type]) tmp += extra[type];

    return tmp;

}



mixed query_enh_critical() {

  int i;

  string *e_keys;

  mapping tmp;



  if(!prop("enhance criticals"))

    return armour_save["enhance critical"];

  else if(intp(armour_save["enhance critical"]))

    return armour_save["enhance critical"] - prop("enhance criticals");

  else if(mapp(armour_save["enhance critical"])) {

    e_keys = DAMAGE_TYPES;

    i = sizeof(e_keys);

    tmp = ([]);

    while(i--) {

      if(!armour_save["enhance critical"][e_keys[i]])

        tmp[e_keys[i]] = -1 * prop("enhance criticals");

      else

        tmp[e_keys[i]] = -1 * prop("enhance criticals") +

          armour_save["enhance critical"][e_keys[i]];

    }

    return tmp;

  }

  return 0;

}



mapping all_base_ac() {

  return armour_save["ac"];

}



mapping query_all_ac() {

    int i, tmp;

    string *types;

    mapping ret_val, extra;



    if(!armour_save["ac"]) return ([]);

    i = sizeof((types = query_ac_types()));

    if(i) ret_val = allocate_mapping(i);

    if(!mapp(extra=query_property("extra ac")))

      extra = ([]);

    if(prop("extra ac") && mapp(prop("extra ac")))

    	extra += prop("extra ac");

    while(i--) {

	tmp = armour_save["ac"][types[i]] + props["enchantment"];

	if(extra[types[i]]) tmp += extra[types[i]];

	if(prop("armour bonus")) tmp += (int)prop("armour bonus");

	ret_val[types[i]] = tmp; }

    return ret_val;

}



string *query_ac_types() {

  string *ret;

  

    if(!armour_save["ac"]) return ({});

    ret = keys(armour_save["ac"]);

    if(mapp(query_property("extra ac")))

      ret += keys(query_property("extra ac"));

    if(prop("extra ac") && mapp(prop("extra ac"))) ret += keys(prop("extra ac"));

    return distinct_array(ret);

}

 

mixed *query_limbs() { return armour_static["possible limbs"]; }

 

mixed query_wear() { return armour_static["wear"]; }

 

mixed query_unwear() { return armour_static["unwear"]; }

 

string query_type() { return armour_static["type"]; }

 

object query_worn() { return armour_static["worn by"]; }

 

 

string query_short() {

    if(!armour_static["worn by"]) return ::query_short();

    else return ::query_short()+" (worn)";

}



string actual_short_desc() {

  if(armour_save["short desc"])

  return armour_save["short desc"];

  else return query("short desc");

}



void set_short(string desc) {

  armour_save["short desc"] = desc;

  set("short desc", desc);

  ::set_short((: call_other, this_object(), "actual_short_desc" :));

}



string query_long(string str) {

    string ret;

    int i;

    mixed tmp;

    

    if(!armour_static["worn by"]) ret = ::query_long(str);

    else {

        ret = ::query_long(str);

        i = sizeof(armour_static["actual limbs"]);

        ret += "\nWorn on: "+armour_static["actual limbs"][--i];

        if(i>-1) while(i--) ret += ", "+armour_static["actual limbs"][i];

        ret += ".\n";

    }

    if(query_property("material_name")) {

      ret += "\nThis item is made of "+

        (string)query_property("material_name")+".";

    }

    if(tmp=query_property("extra long")) {

      if(stringp(tmp)) ret += "\n"+tmp;

      else if(pointerp(tmp)) {

        i = sizeof(tmp);

        while(i--) {

          if(stringp(tmp[i]))

            ret += "\n"+tmp[i];

          else if(pointerp(tmp[i]) && sizeof(tmp[i]) == 2) {

            if(random(100) < (int)this_player()->query_skill(tmp[i][1]))

              ret += "\n"+tmp[i][0];

          }

        }

      }

    }

    return ret;

}

 

string *query_actual_limbs() { return armour_static["actual limbs"]; }

 

int is_armour() { return 1; }

 

void set_struck(mixed val) {

    if(functionp(val) && geteuid(this_object()) != geteuid(val[0])) return;

    armour_static["struck"] = val;

}



mixed query_struck() { return armour_static["struck"]; } 

