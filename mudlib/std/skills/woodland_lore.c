inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("intelligence");
    set_dev_cost(14);
    set_fast_dev_cost(43);
}

void info() {
message("help",
"This is the Ranger's Primary casting skill.",
this_player());
}
