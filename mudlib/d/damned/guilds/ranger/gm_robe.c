#include <move.h>
#include <std.h>

inherit "/std/armour";
inherit AUTOLOAD;

nosave private object worn_by;

int query_always_keep() { return 1; }
int query_worn();

int query_worn() {
  if(worn_by && worn_by == environment()) return 1;
  return 0;
}
  
create() {
    ::create();
    set("id", ({ "cloak" }));
    set_name("cloak");
    set("short", "Cloak of the Ranger Guildmaster");
    set("long","This cloak is made out of the finest leather found in the
realm, afforfing not only good protection, but a perfitc fit as well.  It
is addorned with jewls and runes smithe by the finest tinkers of the realm,
many of which have since passed on.  Desipte it's somewhat wor look from
age, this is a very fine robe indeed.\n");
    set_limbs(({"head", "torso", "left arm", "right arm", "left leg",
"right leg"}));
    set_type("robe");
    set_weight(25);
    set_ac(40);
    set_ac(40, "cold");
    set_wear((: call_other,this_object(), "make_me_gm" :));
    set_ac(20, "disruption");
    set_ac(55, "electricity");
    set_ac(20, "holy");
    set_ac(40, "fire");
    set_ac(20, "vacuum");
    set_ac(30, "stress");
    set_enh_critical(-2);
    set_value(250000);
   return;
}

int make_me_gm() {
object ob;

  seteuid(getuid());
  ob= new("d/damned/guilds/ranger/rg_obj");
  ob->make_me_master(this_player());
  return 1;
}
