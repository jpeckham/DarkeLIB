#include <std.h>
inherit OBJECT;

void create() {
	::create();
	set_name("bolt of cloth");
	set_id( ({"bolt", "cloth", "bolt of cloth"}) );
	set_short("bolt of silvery cloth");
	set_long(
		"A bolt of silvery cloth."
	);
	set_weight(100);
	set_value(600);
}
