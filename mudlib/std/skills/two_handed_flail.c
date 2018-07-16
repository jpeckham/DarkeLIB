inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("dexterity");
    set_dev_cost(7);
    set_fast_dev_cost(20);
}

void info() {
message("help","This is skill with flail and chain-type two-handed weapons."
	, this_player());
}
