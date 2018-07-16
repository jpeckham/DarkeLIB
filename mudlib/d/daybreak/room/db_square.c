#include "../daybreak.h"
inherit "std/room";

void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit(ROOMS+"dbns3", "south");
    add_exit(ROOMS+"dbns4","north");
    add_exit(ROOMS+"dbew5", "east");
    add_exit(ROOMS+"dbew6","west");
 
    set("short","Daybreak Square");
    set("long", "Daybreak Square is situated at the intersection of Daybreak Valley Road and Main Street.  A large fountain with a statue of Duke Traer of Daybreak Ridge spouts out water.  The Square has similar hedge rows to the rest of town, but you notice that there are no doors or windows on any of the walls that line the square. Main Street Begins here and runs east and west towards more residential areas of Daybreak Ridge while Daybreak Valley Road runs north and south and heads towards the outskirts of town.");
}

void reset(){
  ::reset();
    if(!present("fountain")){
    new(OBJ+"fountain.c")->move(this_object());
    }
}
