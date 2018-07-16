inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("dexterity");
    set_dev_cost(17);
    set_fast_dev_cost(52);
}

void info() {
message("help","This skill helps the player overcome the penalties "+
"incurred while wielding weapons in his or her off hand(s).", this_player());
}

