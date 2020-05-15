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
    set("id", ({ "widget" }));
    set_name("amulet");
    set("short", "Enchanter's Widget");
    set("long",
"This little object is fashioned to be worn as an amulet but it looks like "
"no amulet that you have ever seen.  It is a glowing object made from "
"a lightweight but durable material that you cannot identify and "
"it is twisted into a contorted shape that your eye just slides over.  "
"You try to bring the shape into focus, but you give up when it starts"
" giving you a headache.\n");
    set_limbs(({"head"}));
    set_weight(25);
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
  ob= new("d/damned/guilds/enchanter/enchanter_obj");
  ob->make_me_master(this_player());
  return 1;
}
