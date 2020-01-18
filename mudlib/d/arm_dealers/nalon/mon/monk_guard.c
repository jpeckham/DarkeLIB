#include <std.h>
#include "/d/arms_dealers/nalon/monk.h"
#include <nevin.h>
inherit MONSTER;

void create() {
       string var;
       object ob;
       int wc;
      int HPBASE, LEV;

      HPBASE = 110;

       ::create();
     seteuid(geteuid());
       set("id", ({"monk", "guard"}) );
     set_name("monk guard");
      set_level(12);
      set("short", "monk guard");
      set("long",
"This monk guard stands at his post.  His job is to watch "
"the people entering his village, and if needed, stop "
"some from entering.  He looks at you suspiciously as you pass "
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
     set_languages(({"common", "treefolk", "elvish"}));
set_skill("martial arts", 60 + (5*(LEV-5)) + random(LEV) );
       set_skill("kick", LEV * 3);
       add_money("silver", 20 + random(40) );
      new(MONKOBJ+"robe.c")->move(this_object());
      force_me("wear robe");
       set_emotes(6,
    ({
"The guard say in Common: You must respect my fellow monks or keep far from Nalon.",
"The guard says in Common: Welcome to Nalon, may your visit be uplifting.",
"The guard says in Common: Only those with peacefull intentions are welcome here.",
"The guard shifts to battle ready position.",
"The guard stretches his muscles for a moment.",
"The guard carefully looks you over, almost as if sizing you up.",
      }), 0);
}
void catch_tell(string str) {
       string a;
       if(sscanf(str, "%sattacks Monk woman!", a) == 1) {
               call_out("kill_them", 1, a);
             return;
       }
       if(sscanf(str, "%sattacks Robed monk!", a) == 1) {
               call_out("kill_them", 1, a);
             return;
       if(sscanf(str, "%sattacks monk guard!", a) == 1) {
               call_out("kill_them", 1, a);
              return;
       }
       }
}
void kill_them(string who) {
       string str;
       str = "This is a peacefull village, i cannot allow such violence!"
" May your death bring us peace...";
       force_me("speak common");
      force_me("say "+str);
      force_me("grin");
      force_me("kill "+who);
  force_me("use kick at"+who);
       return;
}
