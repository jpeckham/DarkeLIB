inherit "std/room";


void create() {
  ::create();
    set_property("light",3);
    set_property("night light", 2);
    add_exit("/d/damned/akkad/ak_tavern2", "north");
    add_exit("/d/damned/akkad/mine_shop", "east");
    add_exit("/d/damned/guilds/join_rooms/child_join", "west");
    add_exit("/d/damned/akkad/ak_haven1", "south");
    set("short","On Tavern Street north of Haven");
    set("long","You are on Tavern Street, the drinking and sleeping quarter" +
	" of the city.  Immediately south of you is an intersection with Haven" +
	" Street.  Further south, this street continues on as Research Parkway" +
        " where mages reside."
        "\nTo the east is the Mine Builders Shop.");
    
}

