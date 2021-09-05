//    Harming Glyph
//    Based on Diewarzau's Rune Code
inherit "/wizards/zortek/canyon/obj/glyph";

nosave int hb_on;
nosave int heal_status;

int living_filter(object who) { return living(who); }

void create() {
  ::create();
  set_property("non-combat", 1);
  heal_status = 6;
  set_id(({ "harm glyph", "healing glyph" }));
  set_short("A %^RESET%^ORANGE%^glyph of decay%^RESET%^%^MAGENTA%^%^BOLD%^");
  set_long(@GLYPH
This shimmering glyph is floating in the air. Potential
power, hidden within the meaning of its shape and movement,
reveal no hints to its function or power.
GLYPH
);  set("function", (: this_object(), "do_harm" :));
  return;
}

void do_harm(object who) {
  if(!hb_on) {
    hb_on = 1;
    set_heart_beat(1);
  }
  return;
}

void heart_beat() {
  object *inv;
  int i;

  inv = filter_array(all_inventory(environment()), "living_filter",
      this_object());
  if(!inv || !sizeof(inv)) {
    hb_on = 0;
    set_heart_beat(0);
    return;
  }
  heal_status--;
  if(heal_status > 0)
    return;
  heal_status = 6;
  i = sizeof(inv);
  while(i--) {
    message("info",
      "%^ORANGE%^You feel faint and smell the strong odor of decay.", inv[i]);
    inv[i]->do_damage("torso", query_level()*10);
  }
  return;
}

