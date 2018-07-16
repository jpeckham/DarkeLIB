inherit "std/room";


void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit("/d/damned/akkad/ak_sgate", "south");
    add_exit("/d/damned/akkad/ak_victory3", "north");
    set("short","On Victory Street");

    set("long","You are Victory Street, north of the southern gate. "+
        "Looking to the north, you see Market Square, the business center "+
        "of Akkad.");
}
