inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("dexterity");
    set_dev_cost(7);
    set_fast_dev_cost(21);
}

void info() {
message("help","This is skill with two-handed staff-type weapons."
	, this_player());
}

