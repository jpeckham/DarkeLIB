inherit "std/armour";



void create() {

   ::create();

   set_name("captain-plate");

   set("id", ({ "plate", "full plate", "plate armour", "suit", "armour" }) );

   set("short", "suit of full plate armour");

   set("long",@TEXT

This is a suit of masterfully forged plate armour.  While it is

basically forged of laen, there appears to be an inlay design of

a black sun rising on the central breast plate.

TEXT

);

   set_material("/metal/laen");

   set_type("body armour");

   set_ac( 40, ({ "crushing",

                  "cutting",

                  "impaling",

                  "impact", 

                  "strike",

                  "disruption",

                  "infernal",

                  "holy",

               }) );

   set_limbs( ({ "torso", 

                 "right arm",

                 "left arm",

                 "left leg",

                 "right leg",

                 "right foot",

                 "left foot",

              }) );

   set_weight(1200);

   set_value(22000);

}

