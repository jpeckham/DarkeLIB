inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("intelligence");
    set_dev_cost(11);
    set_fast_dev_cost(35);
}

void info() {
message("help",
"This is a skill with nature magic, mostly practiced by druids.",
this_player());
}

