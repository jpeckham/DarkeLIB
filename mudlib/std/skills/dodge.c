inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("dexterity");
    set_dev_cost(21);
    set_fast_dev_cost(60);
}

void info() {
    message("help", "This skill allows you to dodge attacks."
	, this_player());
}

