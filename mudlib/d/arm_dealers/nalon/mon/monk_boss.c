#include <std.h>
#include <nevin.h>
#include "/d/arms_dealers/nalon/monk.h"
inherit MONSTER;

void create() {
       string var;
       object ob;
       int wc;
      int HPBASE, LEV;

      HPBASE = 125;
       ::create();
     seteuid(geteuid());
      set("id", ({"monk", "boss", "man", "regear", "leader"}) );
      set_name("regear");
      set_level(14);
       set("short", "Regear, the warrior-monks training leader");
       set("long",
"Regear is the leader of the warrior-monk trainers.  He "
"coordinates the training program, making sure it is run "
"efficiently, and all are trained as they should be.  He "
"is one of the strongest of the warrior-monks, second "
"only to the Guildmaster!"
  );
       set_gender("male");
       set("gang bang",1);
       set("race", "high-man");
       set_body_type("human");
       set_wielding_limbs( ({"right hand", "left hand"}) );
       set_class("fighter");
       set_hp(HPBASE * (int)this_player()->query_level());
       set_max_hp(HPBASE * (int)this_player()->query_level());

       LEV = (int)this_player()->query_level();
       set_skill("melee", LEV * 7);
       set_skill("martial arts", LEV * 5);
       set_skill("dodge", LEV * 4);
   set_emotes(5,
                ({
"Regear looks you over, pondering the situation.",
"Regear sneers at you disdainfully.",
"Regear says in Common: You are disturbing my meditation, leave immediately!",
"A monk child peaks in through a crack in the wall, Regear turns toward the child's peering eyes and smiles.",
"Regear adjusts his belt, never taking his eyes off you."
                }), 0);
   set_achats(30,
               ({
"With a few swift motions Regear feels body for weaknesses.",
"Regear booms: You should leave now before i have removed your limbs...",
"Regear leaps in the air over your head, flicks your forehead, and lands behind you."
               }) );
      set_skill("resist stun", 30);
      set_skill("reverse stroke", 30);
      set_skill("perception", 120);
       add_money("silver", 20 + random(40) );
      new(MONKOBJ+"robe.c")->move(this_object());
       force_me("wear robe");
      new(MONKOBJ+"sandals.c")->move(this_object());
      force_me("wear sandals");
      new(MONKOBJ+"belt.c")->move(this_object());
      force_me("wear belt");
}
