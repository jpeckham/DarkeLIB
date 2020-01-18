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
       set("id", ({"monk", "robed"}) );
      set_name("robed monk");
     set_level(6 + random(2));
       switch(random(3)) {
               case 0: var = "A brown robed monk"; break;
               case 1: var = "A green robed monk"; break;
               case 2: var = "A yellow robed monk"; break;
      }
       this_object()->set("short", var);
       set("long",
"The monks robes signify much of there culture, they determine "
"rank, show ability, and general standing in the community. "
"The monk looks at you with suspicion, not liking outsiders in his "
"village.  Although he is peace-loving, he will kill to defend "
"himself or his village!");
   set_moving(3);
   set_speed(60);
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
set_skill("martial arts", 60 + (5*(LEV-5)) + random(LEV) );
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
               "your life!";
       force_me("speak common");
       force_me("say "+str);
      force_me("comfort woman");
       force_me("kill "+who);
       return;
}
