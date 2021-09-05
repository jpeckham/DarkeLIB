//  /bin/user/_speak.c
//  written by Valodin, August 1993
//  enables players to speak in racial tongues...or any
//  language for that matter
// Modified by Geldron 031196 to fix say bug

#include <std.h>
#include <daemons.h>

#define FLUBS ({ "something", "womble", "blarg", "cabbage", "blurgle", "jello", "bing", "migrated", "yellow hephalumps", "furry" })
#define FLUBNUM 10
inherit DAEMON;

void speak(string lang, string str, int prof);
void tell_em(string str, object *ob);
mapping make_speakers(object *ob, string lang);
mapping make_speech(mapping speakers, string str, int prof);

int cmd_say(string str)
{
   string lang;


   if(!str)
   {
     write("Speak what?\n");
     return 1;
   }

     if(!(lang = this_player()->query_primary_lang()))
     {
       write("You need to visit the sage to get your languages fixed.\n");
       return 1;
     }
     else
       speak(lang, str, (int)this_player()->query_lang_prof(lang));
   return 1;
}

void speak(string lang, string str, int prof)
{
   object *speakers;
   mapping speech;
   string cap_name, cap_lang, foo;
   int i, k;

if(this_player() && environment(this_player()))
   speakers = all_inventory(environment(this_player()));
else speakers = ({});
   speakers -= ({ this_player() });
   speakers = filter_array(speakers, "is_living", this_object());
  if(this_player()->is_player()) foo = "";
  else foo = "\n";
   if(prof != 10)
    str = translate(str, prof);
   cap_lang = capitalize(lang);
   write("You say in " + cap_lang + ": " + str);
   speech = make_speakers(speakers, lang);
   speech = make_speech(speech, str, prof);
   cap_name = this_player()->query_cap_name();
   if(speech[10])
     tell_em(foo+cap_name + " says in " + cap_lang + ": " + str, speech[10]);
   if(speech[0]) {
     tell_em(foo+cap_name + " says something in " + cap_lang + ".", speech[0]);
     i = sizeof(speech[0]);
     while(i--)
	if(speech[0][i]->is_player()) speech[0][i]->learn_language(lang,
	    random(4));
   }
   for(i = 1; i < 10; i++)
     if(speech[i]) {
       tell_em(foo+cap_name + " says in " + cap_lang + ": " + 
	       speech["messages"][i], speech[i]);
       k = sizeof(speech[i]);
       while(k--)
	if(speech[i][k]->is_player()) speech[i][k]->learn_language(lang,
	    random(25));
   }	
}

int is_living(object ob)
{
   return living(ob);
}

void tell_em(string str, object *ob)
{

    message("talk", str, ob);
}

mapping make_speakers(object *ob, string lang)
{
  mapping speakers;
  int i, obsz, z;

  speakers = ([]);
  for(i = 0, obsz = sizeof(ob); i < obsz; i++)
  {
    z = ob[i]->query_lang_prof(lang);
    if(!speakers[z])
      speakers[z] = ({ ob[i] });
    else
      speakers[z] += ({ ob[i] });
  }
  return speakers;
}

mapping make_speech(mapping speakers, string str, int prof)
{
  int i;

  speakers["messages"] = ([]);
  for(i = 1; i < 10; i++)
    if(speakers[i])
      speakers["messages"][i] = translate(str, i);
  return speakers;
}

int help()
{
write("Command: say\nSyntax: say <text>\n"
      "        This allows you to speak in a language other than common. \n"
       "       Only those around you who understand the language will see the real\n"
       "       text.  The others will see parts of it based on theirspeaking\n"
       "       ability.  If you have problems with languages, visit the sage and ask\n"
       "       him to fix you.  \n"
       "       NOTE: Use the 'speak' command to change your default language.");
return 1;
}
