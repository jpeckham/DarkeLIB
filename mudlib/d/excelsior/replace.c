#pragma no_warnings

inherit DAEMON;
inherit "/std/Object";

mapping flags;
string *Virtualext;
mixed statarray;

void start(mixed intor, mixed files, mixed fileext, mixed instrang, mixed utstrang);

void create() {
	create();
    Virtualext = ({ ".v.c",".o",".r.c" });
    statarray = ({ "y", "n", "a", "e", "r", "R", "A" });
    seteuid(getuid());
}

void init() {
	::init();
	add_action("cmd_replace","replace");
}

string usage() 
{  return "replace [-hi][-n <fileextension>]\n"
    "<-d <delimiter><oldstring><delimiter><newstring><delimiter>|\n"
    "<oldstring> <newstring>> <files>\n";
}

/*Used to sort of the flags in the commmand*/
protected void take_out_arg(string tmp) {
    for(;strlen(tmp) > 0;tmp = tmp[1..-1]) flags[tmp[0..0]] = 1;
}

int cmd_replace(mixed arg, mixed utst, mixed inst) {
    object obj;
    string fileext;
    string mcom, tmp;
    string *array, *files;
    string utstrang,instrang, del;
    int i,j;
    mixed path;

    seteuid(getuid(TP));
    for(flags = ([]), mcom = arg; mcom&&(sscanf(mcom, "-%s %s", tmp, mcom) == 2
	|| (sscanf(mcom, "-%s", tmp) == 1 && !(mcom = 0)));take_out_arg(tmp));
    if (flags["h"]) {
	if (utst || inst)  {
	    write(previous_object()->usage());
	    return 1;
	}
	write(usage());
	write("\n-h displays this text.\n");
	write("-i interactive, will prompt for every match.\n");
	write("-n <file extension> the changed file will be saved with the file extension <file extension>\n");
	write("-d <del><oldstring><del><newstring><del> is way to replace strings "
	  "that has <space> in them. Works like ed's 's'-command.\n");
	write("<files> kan be one or several, wildcards works, but directories "
	  "must use * to work.\n");
	return 1;
    }

    if (!stringp(mcom)) {
	notify_fail("Too few arguments.\n");
	return 0;
    }
    if (inst && utst) 
	mcom = sprintf("%s %s %s",utst,inst,mcom);

    array = explode(mcom," ");
    if (sizeof(array) < 2) {
	notify_fail("Too few arguments.\n");
	return 0;
    }
    if (flags["n"]) {
	fileext = array[0];
	if (fileext == "") {
	    notify_fail("Illegal fileextension.\n");
	    return 0;
	}
	mcom = implode(array[1..100000]," ");
    }
    if (flags["d"]) {
	del = mcom[0..0];
	array = explode(mcom[1..1000000],mcom[0..0]);
    }
    else
    {
	del = " ";
	array = explode(mcom," ");
    }
    if (sizeof(array) < 3) { 
	notify_fail("Too few arguments.\n");
	return 0;
    }
    utstrang = array[0];
    instrang = array[1];
    if (utstrang == instrang) {
	notify_fail("Can't substitute a string for itself.\n");
	return 0;
    }
    if (!stringp(utstrang) || !strlen(utstrang)) {
	notify_fail("String that should be replaced may not be empty.\n");
	return 0;
    }

    mcom = implode(array[2..1000000],del);
    mcom = replace_string(mcom,"  ","");
    array = explode(mcom," ");
    array -= ({ "" }); /*Should not be needed*/
    files = ({ });
    i = sizeof(array);
    while(--i >= 0) {
	if (objectp(obj = find_object(array[i]))) {
	    path = file_name(obj);
	    if (virtualp(obj)) {
		for(j=0;j<sizeof(Virtualext);j++)
		    if ((file_size(tmp = (path+Virtualext[j]))) > 0) {
			files += ({ tmp });
			break;
		    }
		if (j > sizeof(Virtualext)) 
		    printf("Couldn't find file for: %s\n",array[i]);
		continue;
	    }
	    if (clonep(obj)) {
		printf("Cannot replace strings for clones: %s\n",array[i]);
		continue;
	    }
	    path = sprintf("%s.c",path);
	}
	else
	    path = sprintf("%s/%s", (string)TP->get_path(), resolv_path(array[i]));
	switch (file_size(path)) 
	{
	case -1: /*Probably wildcards or no file at all*/
	    tmp = resolv_path(path);
	    path = get_dir(path);
	    if (pointerp(path) && sizeof(path-=({ ".",".."}))) 
		for (j=0;j<sizeof(path);j++)
		    files += ({ sprintf("%s%s",tmp,path[j]) });
	    else
		printf("replace: No files matched: %s\n",array[i]);
		fileext = ".c";
	    break;
	case -2: 
	    write("replace: Must use wildcards for directories.\n");
	    break;
	default:
	    files += ({ path });
	}
    }
    if (!sizeof(files)) {
	write("No files matched.\n");
	return 1;
    }
    start(flags["i"],files,fileext,instrang,utstrang);
    return 1;
}

/*Here starts the real work, all upto now have been prosessing arguments*/

mixed start(mixed intor, mixed files, mixed fileext, mixed instrang, mixed utstrang) {
    int i;
    if (!intor) {
	for (i = 0; i < sizeof(files); i++) 
	    TO->go_file(0,fileext,files[i],instrang,utstrang);
	TO->destroy();
	return;
    }

    while(sizeof(files)) {
	if (TO->go_file(1,fileext, files[0],instrang,utstrang,files=files[1..100000])) 
	    return;
    }
    TO->destroy();
    return;
}

skriv_fraga(array,fil,instrang,utstrang) {
    string strang,strang2;
    int radnr;

    if (sscanf(array[0],"%s"+utstrang+"%s",strang,array[0])) {
	array[1] += strang;
	strang = explode(array[1],"\n");/*Can optimize*/
	radnr = sizeof(strang);
	printf("Found match on line %d in file %s\n%s\n%s<<%s>>"
	  ,radnr,fil,((radnr > 1) ? strang[radnr-2] : "")
	  ,(strang[radnr-1] ? strang[radnr -1] : ""),
	  bold(utstrang));
	strang2 = 0;
	if (sscanf(array[0],"%s\n%s\n",strang,strang2))
	    if (!strang2 /*|| utstrang[strlen(utstrang)-1] == '\n'*/) 
		printf("%s\n",strang);
	    else
		printf("%s\n%s\n",strang,strang2);
	else
	    printf("%s\n",array[0]);
	printf("Change to: <<%s>> (yes/no/abort/exit/run/Run/Abort)?",
	  bold(instrang));
	return 1;
    }
    return 0;
}

go_file(intor,fileext,fil,instrang,utstrang,files) {
    string str2,res;
    string *array;
    str2 = read_file(fil); /*If file is bigger then max size then this will not work*/
    res = "";
    if (!str2) {
	printf("Empty or to big file: %s\n",fil);
	return 0;
    }
    if (intor)
	if (skriv_fraga((array = ({str2,""})),fil,instrang,utstrang)) {
	    input_to("ask",0,array,fileext,fil,instrang,utstrang,files);
	    return 1;
	}
	else
	    return 0;
    res = replace_string(str2,utstrang,instrang);
    if (str2 == res) 
	return 0;
    save_file(fil,fileext,res);
    return 0;
}

save_file(fil,fileext,resultat)
{
    if (fileext) {
	rm(sprintf("%s%s",fil,fileext));
	printf("replace: %s is changed and new file is %s\n",fil,
	  sprintf("%s%s",fil,fileext));
	write_file(sprintf("%s%s",fil,fileext),resultat);
    }
    else {
	rm(fil);
	printf("replace: %s is modified.\n",fil);
	write_file(fil,resultat);
    }
    return 1;
}

ask(str,array,fileext,fil,instrang,utstrang,files) {
    string strang;

    if (!str) {
	str = this_player()->query_property("default_answer");
    }

    if (!stringp(str) || !strlen(str) || member_array(str[0],statarray) == -1) {
	printf("Please answer abort, exit, no, run or yes: ");
	input_to("ask",0,array,fileext,fil,instrang,utstrang,files);
	return 1;
    }

    if (str[0] == 'A')
	return destroy();
    if (str[0] == 'a')
	return;
    if (str[0] == 'y' || str[0] == 'r' || str[0] == 'R') 
	array[1] = array[1]+instrang;
    else
	array[1] = array[1]+utstrang;

    if ((str[0] != 'e' && str[0] != 'r' && str[0] != 'R') && 
      skriv_fraga(array,fil,instrang,utstrang)) {
	input_to("ask",0,array,fileext,fil,instrang,utstrang,files);
	return 1;
    }

    if (stringp(array[0])) {
	if (str[0] == 'r' || str[0] == 'R')
	    array[0] = replace_string(array[0],utstrang,instrang);
	save_file(fil,fileext,array[1]+array[0]);
    }
    else
	save_file(fil,fileext,array[1]); 

    start(str[0] != 'R',files,fileext,instrang,utstrang);
}

destroy() {
    write("Done.\n");
}

void help() {
    message("help", usage() + "\nReplace occurences of a string "
      "in a file.", TP);
}
