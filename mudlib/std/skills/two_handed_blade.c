inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("dexterity");
    set_dev_cost(8);
    set_fast_dev_cost(22);
}

void info() {
message("help","This is skill with two-handed bladed weapons."
	, this_player());
}

