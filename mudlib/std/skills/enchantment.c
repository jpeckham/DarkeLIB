inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("intelligence");
    set_dev_cost(15);
    set_fast_dev_cost(46);
}

void info() {
message("help",
"This spell is used to cast the powerful spells of the enchanter.",
this_player());
}

