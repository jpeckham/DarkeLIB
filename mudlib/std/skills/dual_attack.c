inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("dexterity");
    set_dev_cost(25);
    set_fast_dev_cost(70);
}

void info() {
message("help","This skill helps offset the penalty which the player "+
"incurs while attacking with more than one weapon.", this_player());
}

