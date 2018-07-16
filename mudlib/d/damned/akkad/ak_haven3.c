inherit "std/room";


void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit("/d/damned/akkad/ak_haven2", "west");
    add_exit("/d/damned/akkad/ak_haven4", "east");
    set("short","Outskirts of Market Square");
    set("long","You are on the western outskirts of Market Square. "+
        "Before you lie the many shops of Akkad, where "+
	"almost anything can be bought or sold.");

}
