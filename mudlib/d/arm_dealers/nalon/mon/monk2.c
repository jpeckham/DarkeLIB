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
       set("id", ({"monk", "trainee", "hooded", "solemn", "stocky"}) );
      set_name("monk trainee");
      set_level(7 + random(2));
       switch(random(3)) {
               case 0: var = "A hooded monk"; break;
               case 1: var = "A solemn monk"; break;
               case 2: var = "A stocky monk"; break;
      }
       this_object()->set("short", var);
       set("long",
"The monk's body is extremely tough, his knuckles scarred and "
"toughened from training his whole life.  He looks at you "
"with suspicious eyes, not liking strangers in his village. "
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
      set_languages(({"common", "treefolk", "elvish"}));
       set_wielding_limbs( ({"right hand", "left hand"}) );
       set_class("fighter");
       set_hp(HPBASE * (int)this_player()->query_level());
       set_max_hp(HPBASE * (int)this_player()->query_level());

       LEV = (int)this_player()->query_level();
  set_skill("martial arts", 60 + (4*(LEV-5) ) );
       add_money("silver", 20 + random(40) );
       new(MONKOBJ+"robe.c")->move(this_object());
       force_me("wear robe");
}
void catch_tell(string str) {
       string a;
       if(sscanf(str, "%sattacks Monk woman!", a) == 1) {
               call_out("kill_them", 1, a);
             return;
      }
}
void kill_them(string who) {
      string str;
      str = "How could you attack such a delightful woman?! You must pay "
"with your life!";
      force_me("speak common");
       force_me("say "+str);
     force_me("comfort woman");
      force_me("kill "+who);
      return;
}
