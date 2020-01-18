#include <std.h>
#include <nevin.h>
#include "/d/arms_dealers/nalon/monk.h"
inherit MONSTER;

void create() {
       string var;
       object ob;
       int wc;
      int HPBASE, LEV;

      HPBASE = 110;

       ::create();
     seteuid(geteuid());
      set("id", ({"monk", "gardener"}) );
      set_name("gardener");
      set_level(7);
       set("short", "A monk gardener");
       set("long",
"This man serves his village not by training in the arts "
"combat, but by tending the plants in the village.  While he "
"might have some combat training, his expertise is in "
"keeping the village beautiful. "
  );
       switch(random(4)) {
               case 0: var = "high-man"; break;
               case 1: var = "high-elf"; break;
               case 2: var = "wood-elf"; break;
               case 3: var = "ent"; break;
       }
       this_object()->set("race", var);
       set_gender("male");
       set_body_type("human");
       set_wielding_limbs( ({"right hand", "left hand"}) );
       set_class("fighter");
       set_hp(HPBASE * (int)this_player()->query_level());
       set_max_hp(HPBASE * (int)this_player()->query_level());

       LEV = (int)this_player()->query_level();
  set_skill("martial arts", 60 + (4*(LEV-5) ) );
       add_money("silver", 20 + random(40) );
       new(MONKOBJ+"bucket.c")->move(this_object());
       set_emotes(5,
               ({
"The gardener waters the plants in the area carefully.",
"The gardener waters the plants in the area carefully.",
"A small beetle moves around slowly as the gardener shuffles the fallen leaves.",
"The gardener feeds a bird some seeds just before it takes to the air.",
"An earthworm quickly buries himself in the soil as water begins to fall from the gardeners bucket."
     }), 0);
}
