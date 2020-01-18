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
       set("id", ({"monk", "herbalist", "meditator", "meditating", "healer"}) );
     set_name("monk");
      set_level(8 + random(2));
       switch(random(3)) {
               case 0: var = "A meditating monk"; break;
               case 1: var = "A monk healer"; break;
               case 2: var = "A monk herbalist"; break;
      }
       this_object()->set("short", var);
       set("long",
"This monk is one of the villages spiritual monks.  He "
"studies in the arts of certain spell casting, and "
"deep meditation.  While his body is not the strongest, his "
"power comes from his mind."
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
       set_skill("perception", LEV * 12);
set_skill("melee", 75 + (5*(LEV-5)) + random(LEV) );
set_skill("martial arts", 60 + (5*(LEV-5)) + random(LEV) );
set_skill("dodge", 40 + (4*(LEV-5)) + random(LEV) );
      set_skill("prayer", LEV * 10);
        set_combat_chance(120);
      set_skill("conjuration", LEV * 9);
      set_spell_level("cure serious wounds", 5);;
      add_spell("cure serious wounds", "$(ME)");
      set_spell_level("shocking grasp", 5);
      add_spell("shocking grasp", "$(ME)");
      set_skill("concentrate", LEV * 8);
     force_me("use concentrate");
       add_money("silver", 20 + random(40) );
      new(MONKOBJ+"robe.c")->move(this_object());
      force_me("wear robe");
}
