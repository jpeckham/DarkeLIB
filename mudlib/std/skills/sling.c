inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("dexterity");
    set_dev_cost(4);
    set_fast_dev_cost(10);
}

void info() {
    message("help", "This is skill with slings and staff-slings."
	, this_player());
}

