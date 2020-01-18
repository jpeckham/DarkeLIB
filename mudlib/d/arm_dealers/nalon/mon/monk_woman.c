#include <std.h>
#include <nevin.h>
#include "/d/arms_dealers/nalon/monk.h"
inherit MONSTER;

void create() {
       string var;
       object ob;
       int wc;
      int HPBASE, LEV;

      HPBASE = 90;
       ::create();
     seteuid(geteuid());
       set("id", ({"monk", "woman", "villager", "wife", "servant"}) );
      set_name("monk woman");
      set_level(5 + random(2));
       switch(random(3)) {
               case 0: var = "A monk wife"; break;
               case 1: var = "A monk villager"; break;
               case 2: var = "A monk servant"; break;
      }
       this_object()->set("short", var);
       set("long",
"A pretty monk woman, she spends her hours cleaning up "
"after the monks.  Her life is bland and boring, "
"doing the same work day after endless day. "
  );
       switch(random(4)) {
               case 0: var = "high-man"; break;
               case 1: var = "high-elf"; break;
               case 2: var = "wood-elf"; break;
               case 3: var = "ent"; break;
       }
       this_object()->set("race", var);
       set_gender("female");
       set_body_type("human");
       set_wielding_limbs( ({"right hand", "left hand"}) );
       set_class("fighter");
       set_hp(HPBASE * (int)this_player()->query_level());
       set_max_hp(HPBASE * (int)this_player()->query_level());

       LEV = (int)this_player()->query_level();
       set_skill("martial arts", LEV * 7);
   set_emotes(5,
                ({
                  "The woman eyes you wearily.",
                  "The woman works tirelessly.",
                  "The woman sits down and begins to meditate.",
                  "The woman moves away from you suspicously.",
                  "The woman says, 'Get out of here.'",
                }), 0);
   set_achats(30,
               ({
                  "The woman swings her arms wildly.",
                  "The woman tries to cry for help.",
                  "The woman attempts to kick you, but misses.",
               }) );
   set_wimpy(10);
       add_money("silver", 20 + random(40) );
}
