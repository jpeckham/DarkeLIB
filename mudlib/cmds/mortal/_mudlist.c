/* $Header: /mud/tkv/bin/player/RCS/_mudlist.c,v 1.2 1995/07/09 05:29:39 mudriver Exp $
 * $Log: _mudlist.c,v $
 * Revision 1.2  1995/07/09  05:29:39  mudriver
 * fixed a parse error caused by the indent sys cmd
 * on line 55 the indent caused the .. to be seperated to . .
 *
 * Revision 1.1  1995/07/08  22:26:24  mudriver
 * Initial revision
 *
 * /bin/player/_mudlist.c
 * created by Zortek
 * ported to DarkeMud by Zortek 040196
 */

#include <std.h>
#include <daemons.h>

inherit         DAEMON;

private mapping borg;

int 
cmd_mudlist(string str)
{
    string         *muds,
                   *foo,
                    color,
                    tmp,
                    lib,
                    bar;
    int             i,
                    x;


    if (!str)
        borg = (mapping) NETWORK_D->query_known_muds();
    else
        borg = (mapping) NETWORK_D->query_mud_info(str);
    if (str && !borg)
        return notify_fail(mud_name() + " is not aware of that mud.\n");
    else if (str) {
        write("----------------------------------------------");
        write("\n%^YELLOW%^" + borg["NAME"] + "%^RESET%^   " + borg["HOSTADDRESS"] + " " + borg["PORT"]);
        write("Driver:  " + (borg["DRIVER"] ? borg["DRIVER"] : "Unknown"));
        write("Mudlib:  " + (borg["MUDLIB"] ? borg["MUDLIB"] : "Unknown") + " " + (borg["VERSION"] && borg["MUDLIB"] ? borg["VERSION"] : "") + "\n");
        write("----------------------------------------------");
        } 
        else {
            foo = ({ });
            muds = keys(borg);
            muds = (sort_array(muds, "compare", this_object()));
            write("-----------------------------------------------------------------");
            write("%^BOLD%^                         COMPILED MUDLIST%^RESET%^");
            write("-----------------------------------------------------------------\n");
            bar = "";
            for (x = 0; x < sizeof(muds); x++) {
                borg = (mapping) NETWORK_D->query_mud_info(muds[x]);
                    foo += ({ borg["NAME"] });
            }
            message("info", format_page(foo, 3), this_player());
            write("-----------------------------------------------------------------");

        }
    return 1;
}

protected int 
compare(string one, string two)
{
    return strcmp(borg[one]["NAME"], borg[two]["NAME"]);
}

int 
help()
{
    write(@HELP
Syntax:  mudlist [mud]

Shows all muds that are connected by the interwiz channel or if an
optional mudname is supplied it will provide additional information
specific to that mud if it is on the intermud network.  If you have
ansi color activated, the main mudlist will be color coded for you.

See also:  mudtime
HELP
);
    return 1;
}
