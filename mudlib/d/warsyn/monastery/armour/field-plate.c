inherit "std/armour";

void create() {
   ::create();
   set_name("field-plate");
   set("id", ({ "plate", "field-plate", "suit", "armor" }) );
   set("short", "A suit of  field-plate armor");
   set("long",
        "This is a beautiful set of field-plate armour.  It gleams "+
        "and sparkles as it is well kept and polished.  Someone "+
        "took great care of this armor.");
   set_type("body armour");
   set_ac( 40, ({ "crushing", "cutting", "impaling",
                         "impact", "strike" }) );
   set_limbs( ({ "torso", "right arm", "left arm",
                        "left leg", "right leg" }) );
   set_weight(1200);
   set_value(2000);
}
