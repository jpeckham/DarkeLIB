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
       set("id", ({"monk", "trainer"}) );
      set_name("monk trainer");
     set_level(9 + random(3));
       set("short", "A monk trainer");
       set("long",
"This monk trains others in the arts of martial arts "
"His knowledge in this field is almost unsurpassed, "
"only topped by the warrior-monks leader.  He looks at "
"you curiously for a moment, then his curiousity melts "
"into suspicion. "
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
set_skill("melee", 75 + (5*(LEV-5)) + random(LEV) );
set_skill("martial arts", 60 + (5*(LEV-5)) + random(LEV) );
set_skill("dodge", 40 + (4*(LEV-5)) + random(LEV) );
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
       if(sscanf(str, "%sattacks Robbed monk!", a) == 1) {
               call_out("kill_themR", 1, a);
             return;
      }
       if(sscanf(str, "%sattacks Monk trainie!", a) == 1) {
               call_out("kill_themT", 1, a);
             return;
      }
}
void kill_them(string who) {
      string str;
       str = "How could attack such a delightfull woman?! You must pay "
"with your life!";
      force_me("speak common");
       force_me("say "+str);
     force_me("comfort woman");
      force_me("kill "+who);
      return;
}
void kill_themT(string who) {
     string str;
       str = "You dare attack a student of mine?! I must make an example "
"of you now...";
      force_me("speak common");
       force_me("say "+str);
     force_me("peer"+who);
      force_me("kill "+who);
      return;
}
void kill_themR(string who) {
     string str;
       str = "You must think i'm just going to sit here and let you "
"desturb my future students. Well, you are %^RED%^DEAD%^RESET%^ wrong!";
      force_me("speak common");
       force_me("say "+str);
    force_me("bonk"+who);
      force_me("kill "+who);
      return;
}
