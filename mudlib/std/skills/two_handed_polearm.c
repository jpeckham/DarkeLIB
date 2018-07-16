inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("dexterity");
    set_dev_cost(5);
    set_fast_dev_cost(19);
}

void info() {
message("help","This is skill with polearms such as halberds."
	, this_player());
}

