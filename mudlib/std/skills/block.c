inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("strength");
    set_dev_cost(11);
    set_fast_dev_cost(28);
}

void info() {
    message("help", "This skill allows you to block attacks with your shield."
	, this_player());
}

