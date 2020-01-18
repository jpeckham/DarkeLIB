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
       set("id", ({"monk", "wisened", "man"}) );
      set_name("wisened monk");
     set_level(13);
       set("short", "A wisened old monk");
       set("long",
"A wisened old monk, he is the eldest among the village. "
"His face is creased with age and wisdom, his knowledge "
"of the passed is superb.  He walks like that of a dying "
"man, yet carries himself with the pride of a man who "
"knows how to defend himself. "
  );
       set_gender("male");
       set("race", "high-man");
       set_body_type("human");
       set_wielding_limbs( ({"right hand", "left hand"}) );
       set_class("fighter");
       set_hp(HPBASE * (int)this_player()->query_level());
       set_max_hp(HPBASE * (int)this_player()->query_level());

       LEV = (int)this_player()->query_level();
  set_skill("martial arts", 60 + (4*(LEV-5) ) );
   set_emotes(5,
                ({
"The old man mutters something about his back",
"The old man slowly starts a journey across the room.",
"The wisened monks eyes shine with pride as he looks over his shelves",
"The old mans back seems to creak as he sits down",
"The old man looks at you with a gleam in his eyes.",
                }), 0);
   set_achats(30,
               ({
"The old man displays amazing knowledge of pressure points in his fighting",
"Although old and slow, the monk seems to know what he's doing!",
"The old man yells, 'Get out of here!'",
               }) );
       add_money("silver", 20 + random(40) );
}
