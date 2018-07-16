inherit "/std/skills/skill.c";

void create() {
    ::create();
    set_stat("dexterity");
    set_dev_cost(9);
    set_fast_dev_cost(26);
}

void info() {
    message("help", "This skill allows you to parry would-be attackers."
	, this_player());
}

