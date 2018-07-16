inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("intelligence");
    set_dev_cost(12);
    set_fast_dev_cost(35);
}

void info() {
message("help",
"This skill is used to cast spells that manipulate the very nature of energy itself!",
this_player());
}

