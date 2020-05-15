/*
// This is part of the TMI distribution mudlib.
// Please retain this header for any files you may use the code in.
// written by Sulam(12-12-91)
// Help added by Brian 1/27/92
*/

#include <std.h>

inherit DAEMON;

protected void conv(string text) {
    int i;

    if( text ) {
	for(i=0; text[i] == ' '; i++) { /* empty loop */ } 
	if( text == "**" ) return;
	else if( text == "" || i == strlen(text) ) {
	    write("] ");
	    input_to("conv");
	    return;
	} 
	else say( (string) this_player()->query_cap_name() +
            " says: " + text + "\n", this_player() );
    }
    write("] ");
    input_to("conv");
    return;
}
  
int
cmd_converse(string foo) {
    write("To escape from converse type **\n");
    write("While in converse you may use ! to execute commands.\n");
    conv(foo);  /* shrug, needs some text...  */
    return 1;
}

void
create() {
    seteuid("backbone");
}

int
help() {
  write("Command: converse\nSyntax: converse (no args)\n"+
        "This command puts you into converse mode, while\n"+
        "in converse mode everything you type is automatically said.\n"+
        "You may precede any commands with a ! to have them executed.\n"+
        "To exit converse mode type ** at the beginning of any line.\n");
  return 1;
}
/* EOF */
