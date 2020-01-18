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
       set("id", ({"monk", "warrior"}) );
     set_name("warrior monk");
      set_level(8 + random(2));
       switch(random(3)) {
               case 0: var = "A warrior monk"; break;
               case 1: var = "A warrior monk"; break;
               case 2: var = "A warrior monk"; break;
      }
       this_object()->set("short", var);
       set("long",
"A fierce warrior-monk, he has spent his whole life "
"training in the arts of hand-to-hand combat.  He is fully "
"aware of his surroundings, looking at you as he would a "
"bug in his house.  He doesnt like strangers." );
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
       set_skill("perception", LEV * 12);
set_skill("martial arts", 60 + (5*(LEV-5)) + random(LEV) );
       add_money("silver", 20 + random(40) );
       new(MONKOBJ+"robe.c")->move(this_object());
       force_me("wear robe");
}
