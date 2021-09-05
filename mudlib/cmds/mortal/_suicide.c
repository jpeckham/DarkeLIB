//      /cmds/mortal/_suicide.c 
                     
#include <std.h>
#include <daemons.h>
#include <security.h>
#include <bank.h>
#include <dirs.h>

inherit DAEMON;

protected void my_choice(string str) {
    object ob;
    string name;
    string em;
    string *inv,file;
    int i;

    ob = this_player();
    name = TP->query_name();
    em = TP->query_email();
    if(!TP || !name) {
            return 0;
    }
    name = lower_case(name);
    switch(str) {
        case "yes":       
           if(!ob->query_ghost())
             ob->die();
           "/daemon/save_all_d"->add_crash_items(this_player());
           if(ob)
             ob->remove();
           if(ob) {
             seteuid(UID_DESTRUCT);  
             destruct(ob);
             seteuid(getuid());
           }
           seteuid(UID_USERSAVE);
//            if(sscanf(read_file(DIR_USERS+"/"+name[0..0]+"/"+name+".o"), "%*semail\"%s\"%*s", em) == 3)
             EMAIL_D->unregister_email(em);
             rename(DIR_USERS+"/"+name[0..0]+"/"+name+".o", DIR_USERS+"/suicide/"+name+".o");
           if(file_size(ACCOUNTS_DIR+name+".o") > -1)
             rm(ACCOUNTS_DIR+name+".o");
             inv = get_dir("/adm/save/objects/saveall/"+explode(name,"")[0]+"/"+name+"/*");
    for(i=0;i<sizeof(inv);i++){
        file = "/adm/save/objects/saveall/"+explode(name,"")[0]+"/"+name+"/"+inv[i];
        rm(file);
        rmdir("/adm/save/objects/saveall/"+explode(name, "")[0]+"/"+name);
    }

             write(capitalize(name)+" is removed from"+mud_name()+".\n");
             seteuid(UID_LOG);
             log_file("suicide", name+" suicided on"+ctime(time())+"\n");
             seteuid(getuid());
             return;
    }
    write("You quickly change your mind.");
    return;
}

int cmd_suicide(string str) {
    string name;
    string em;

    
    if(wizardp(TP)){
        return notify_fail("Hrm...Ask a admin to get rid of ya if its that
bad...\n");
    }
    write("Are you sure you wish to commit suicide??(yes/no)");
    input_to("my_choice");
    return 1;
}

void help() {
    write( @EndText
        Syntax: suicide
    Effect: Deletes, nukes, wipes out and annhilates you!!
EndText
  );
}
