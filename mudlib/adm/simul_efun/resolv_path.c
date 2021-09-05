/*
// Thanks to Huthar for this!
// Bug in "here" option fixed by Pallando 93-05-26
// "cwd" option added by Pallando 93-05-30
*/

#define ROOT ({ "wizards", "d" })

string resolv_path(string curr, string newpath) {
    int i;
    string *tmp;
    string t1;

    if(newpath) {
       tmp = explode(newpath, "/");
       if(sizeof(tmp) && member_array(tmp[0], ROOT) >= 0 && newpath[0] != '/')
          newpath = "/"+newpath;
    }
    if(curr && (curr == "cwd") && this_player())
      curr = (string)this_player()->get_path();
    if(!newpath || newpath == ".") return curr;
    if( (newpath == "here") && this_player() )
    {
        return file_name(environment(this_player())) + ".c";
    }
    if(newpath == "~" || newpath == "~/" )
      newpath = user_path((string)this_player()->query_name());
    if(sscanf(newpath,"~/%s",t1))
      newpath = user_path((string)this_player()->query_name()) + t1;
    else if(sscanf(newpath,"~%s",t1))
      newpath = user_path(t1); 
    else if(newpath[0] != '/')
      newpath = curr + "/" + newpath;

    if(newpath[strlen(newpath) - 1] != '/')
        newpath += "/";
    tmp = explode(newpath,"/");
    if (!tmp) tmp = ({"/"});
    for(i = 0; i < sizeof(tmp); i++)
        if(tmp[i] == "..") {
            if(sizeof(tmp) > 2) {
                tmp = tmp[0..(i-2)] + tmp[(i+1)..(sizeof(tmp)-1)];
                i -= 2;
            } else {
                tmp = tmp[2 ..(sizeof(tmp)-1)];
                i = 0;
            }
        }
     newpath = "/" + implode(tmp,"/");
     if(newpath == "//") newpath = "/";
     return newpath;
}
