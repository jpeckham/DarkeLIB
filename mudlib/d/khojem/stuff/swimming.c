inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("constitution");
    set_dev_cost(7);
    set_fast_dev_cost(19);
}

void info() {
message("help",
"This skill allows you to cross water without drowning.",
this_player());
}

