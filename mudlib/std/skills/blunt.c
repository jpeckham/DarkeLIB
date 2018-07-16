inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("dexterity");
    set_dev_cost(6);
    set_fast_dev_cost(18);
}

void info() {
message("help","This is skill with one-handed blunt weapons."
	, this_player());
}

