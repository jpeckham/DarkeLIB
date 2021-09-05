//      /std/money.c

//      from the Nigttmare mudlib

//      an inheritable money object, for hard and soft currencies

//      created by Descartes of Borg 24 april 1994



#include <daemons.h>



private mapping money;

private nosave object coins_ob;



string *query_currencies();



int query_money(string str) {

    if(!money) money = ([]);

    if(!str || !stringp(str)) return 0;

    return money[str];

}



void remove_coins_ob() { coins_ob = 0; }



int add_money(string str, int amount) {

    string logging;



    if(!str || !stringp(str) || !intp(amount)) return 0;

    if(!money) money = ([]);

    if(this_object()->is_player() && !wizardp(this_object())

       && member_array(str, (string *)ECONOMY_D->__QueryCurrencies()) >= 0) {

      if(!this_object()->add_encumbrance(to_int((float)ECONOMY_D->

						__Query(str, "weight") *

       to_float(amount))) && amount > 0) {

	if(!coins_ob) {

	  message("my_action", "You cannot carry all of your money, and you "+

		  "drop some on the ground.", this_object());

	  message("other_action", (string)this_object()->query_cap_name() +

		  " fumbles and drops some money on the ground.",

		  all_inventory(environment()),

		  ({ this_object() }));

	  seteuid(getuid(this_object()));

	  coins_ob = new("/std/obj/coins");

	}

	coins_ob->add_money(str, amount);

	if(environment())

	  coins_ob->move(environment());

	remove_call_out("remove_coins_ob");

	call_out("remove_coins_ob", 2);

	return 0;

      }

    }

    if(!money[str]) money[str] = amount;

    else money[str] += amount;

    if(money[str] < 1) map_delete(money, str);

    if(wizardp(this_object()) || !this_object()->is_player()) return 1;

    switch(str) {

        case "platinum": amount *=10; break;

        case "gold": break;

        case "silver": amount /=10; break;

        case "electrum": amount /=50; break;

        case "copper": amount /= 100; break;

        default: return 1;

    }

    if(amount < 100) return 1;

    if(!this_player() || this_player() == this_object())

      logging = "EUID ("+

      geteuid(previous_object())+")"+

      ":"+file_name(previous_object());



    else logging = (string)this_player()->query_name();



    log_file("money", this_object()->query_name()+":received"+

      ":"+amount+":in:"+str+":from:"+logging+":"+ctime(time())+"\n");

    return 1;

}



void set_money(string type, int amount) {

    if(this_object()->is_player()) return;

    if(amount < 0) return;

    if(!type || !stringp(type) || !intp(amount)) return;

    if(!money) money = ([]);

    if(amount == 0 && !undefinedp(money[type])) map_delete(money, type);

    money[type] = amount;

}



protected void reset_money() {

    string *currs;

    int i, tmp;



    if(!money) return;

    currs = keys(money);

    i = sizeof(currs);

    while(i--) {

      tmp = money[currs[i]];

      money[currs[i]] = 0;

      this_object()->add_money(currs[i], tmp);

    }

    return;

}



string *query_currencies() { 

    if(!money) return ({});

    else return keys(money);

}



float query_money_value() {

  float val;

  int i;

  string *monies;

  

  if(!money) return 0.0;

  monies = keys(money);

  i = sizeof(monies);

  val = 0.0;

  while(i--) {

    if(member_array(monies[i], (string *)ECONOMY_D->

                    __QueryCurrencies()) < 0) continue;

    val += to_float(money[monies[i]]) /

      (float)ECONOMY_D->__Query(monies[i], "rate");

  }

  return val;

}





