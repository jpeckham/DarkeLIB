//	Combat damage daemon for DarkeMUD combat system.  Reads text files
//	out of DIR_DAEMONS_DATA, specifically critical_<damage-type>.db
//	and damage.db, see specific files for format.
//	DarkeLIB 0.1
//	Diewarzau 3/26/94

#include <daemons.h>
#include <std.h>
#include <dirs.h>

class table {
    string table_name;
    string *idx;
    string *lines;
    string *verbs;
    mixed *results;
    int *start_idx;
    int *last_idx;
}

class d_table {
    string *m_idx;
    string *mesgs;
    string *res_idx;
    string *results;
}

nosave mixed *cont_damage;
nosave int hb_on;
class d_table dmg_table;
class table *crit_tables = ({ });
mapping alpha = ([ "A" : 0, "B" : 1, "C" : 2, "D" : 3, "E" : 4 ]);

int find_table(string type);
private void initialize_dmg_table();
private void initialize_criticals();
string *parse_message(string line, string a_name, string t_name, string verb);

void create() {
    seteuid(getuid());
    cont_damage = ({ });
    hb_on = 0;
    initialize_dmg_table();
    initialize_criticals();
}

void init_cont_damage(object who, int h_beats, int amt, string mess) {
    if(!objectp(who)) return;
    if(!cont_damage) cont_damage = ({ });
    cont_damage += ({ ({ who, h_beats, amt, mess }) });
    set_heart_beat(hb_on = 1);
    return;
}

void stop_cont_damage(object who) {
    int i;
    mixed tmp;

    if(!cont_damage) return;
    tmp = ({});
    i = sizeof(cont_damage);
    while(i--)
	if(cont_damage[i][0] != who) tmp += ({ cont_damage[i] });
    cont_damage = copy(tmp);
    return;
}

int damage_filter(mixed *args) {
    if(sizeof(args) != 4) return 0;
    if(!objectp(args[0])) return 0;
    if(!living(args[0])) return 0;
    if(args[0]->query_ghost()) return 0;
    if((int)args[1] <= 0) return 0;
    return 1;
}

void heart_beat() {
    int i;
    mixed *tmp;
    object *already;

    cont_damage = filter_array(cont_damage, "damage_filter", this_object());
    if(!sizeof(cont_damage)) {
	set_heart_beat(hb_on = 0);
	return;
    }
    i = sizeof(cont_damage);
    already = ({});
    while(i--) {
	tmp = cont_damage[i];
	if(!pointerp(tmp)) continue;
	tmp[1] = (int)tmp[1] - 1;
	((object)tmp[0])->add_hp(-1 * (int)tmp[2]);
	if(((object)tmp[0]->query_current_attacker() ||
	    !random(4)) && member_array(tmp[0], already) < 0)
	    message("my_combat", (string)tmp[3],
	      (object)tmp[0]);
	already = distinct_array(already + ({ tmp[0] }) );
	cont_damage[i] = tmp;
    }
    return;
}

private void initialize_dmg_table() {
    string *damage, *line;
    int i, sz;

    if(!file_exists(DIR_DAEMONS_DATA+"/damage.db")) {
	shout("BUG in damage daemon!  Damage.db not found.");
	return;
    }
    damage = read_database(DIR_DAEMONS_DATA+"/damage.db");
    dmg_table = new(class d_table);
    sz = sizeof(damage);
    dmg_table->res_idx = allocate(sz);
    dmg_table->results = allocate(sz);
    for(i=0;i<sz;i++) {
	line = explode(damage[i],":");
	dmg_table->res_idx[i] = line[0];
	dmg_table->results[i] = line[1];
    }
    damage = read_database(DIR_DAEMONS_DATA+"/damage_msg.db");
    sz = sizeof(damage);
    dmg_table->m_idx = allocate(sz);
    dmg_table->mesgs = allocate(sz);
    for(i=0;i<sizeof(damage);i++) {
	line = explode(damage[i],":");
	dmg_table->m_idx[i] = line[0];
	dmg_table->mesgs[i] = line[1];
    }
    return;
}

private void initialize_criticals() {
    string *table_dir, *table, *line;
    string name, crit_type;
class table tmp_table;
    int i,j, p, sz;

    table_dir = get_dir(DIR_DAEMONS_DATA+"/critical_*.db");
    i = sizeof(table_dir);
    while(i--) {
	if(sscanf(table_dir[i],"critical_%s.db",name) == 1)
	    tmp_table = new(class table);
	tmp_table->table_name = name;
	table = read_database(DIR_DAEMONS_DATA+"/"+table_dir[i]);
	sz = sizeof(table)-5;
	tmp_table->lines = allocate(sz);
	tmp_table->idx = allocate(sz);
	tmp_table->verbs = allocate(sz);
	tmp_table->results = allocate(sz);
	tmp_table->start_idx = allocate(5);
	tmp_table->last_idx = allocate(5);
	p = 0;
	for(j=0;j<sizeof(table);j++) {
	    if(sscanf(table[j],"(%s):",name) == 1) {
		crit_type = name;
		tmp_table->start_idx[alpha[name]] = p;
		if(alpha[name] > 0 && p > 0) tmp_table->last_idx[alpha[name]-1] = p-1;
		continue;
	    }
	    line = explode(table[j], ":");
	    if(sizeof(line) < 3) continue;
	    tmp_table->idx[p] = line[0];
	    sz = sizeof(line) - 1;
	    tmp_table->lines[p] = line[1];
	    tmp_table->verbs[p] = line[2];
	    if(sz >= 3) tmp_table->results[p] = line[3..sz];
	    else tmp_table->results[p] = ({});
	    p++;
	}
	tmp_table->last_idx[4] = p-1;
	crit_tables += ({ tmp_table });
    }
    return;
}

string *attack_message(string arg) {
    int roll, hi, low, i, sz;
    string t_name, a_name, line, verb;

    if(sscanf(arg,"%d %s %s:%s",roll, verb, a_name, t_name) != 4)
	return ({ "BUG", "BUG", "BUG" });
    roll = (roll < 0)? 0 : roll;
    sz = sizeof(dmg_table->m_idx);
    for(i=0;i<sz;i++) {
	sscanf(dmg_table->m_idx[i],"%d..%d",low,hi);
	if((roll <= hi && roll >= low) || i == (sz-1)) {
	    line = dmg_table->mesgs[i];
	    return parse_message(line,a_name,t_name,verb);
	}
    }
    line = dmg_table->mesgs[sz-1];
    return parse_message(line,a_name,t_name,verb);
}

string attack_result(string arg) {
    int roll, mod, hi, low, i, sides, dice, rem, ac, j, sz;

    if(sscanf(arg,"%d %d %d",roll,mod, ac) != 3)
	return "DAMAGE 0";
    if(mod < 0) { i = -1; mod *= -1; }
    else i = 1;
    switch(mod) {
    case 0..32:
	sides = 6;
    case 33..100:
	sides = 10;
    default:
	sides = 20;
    }
    dice = mod / sides;
    rem = mod%sides;
    mod = 0;
    for(j=1;j<=dice;j++)
	mod += random(sides)+1;
    mod += rem;
    mod *= i;
    roll += mod;
    if(ac < 0) { i = -1; ac *= -1; }
    else i = 1;
    switch(ac) {
    case 0..32:
	sides = 6;
    case 33..100:
	sides = 10;
    default:
	sides = 20;
    }
    if(ac > 500) {
	ac = ac *2 /3;
    } else {
	dice = ac / sides;
	rem = ac%sides;
	ac = 0;
	for(j=1;j<=dice;j++)
	    ac += random(sides)+1;
	ac += rem;
    }
    ac *= i;
    roll -= ac;
    roll = (roll < 0)? 0:roll;
    sz = sizeof(dmg_table->res_idx);
    for(i=0;i<sz;i++) {
	sscanf(dmg_table->res_idx[i],"%d..%d",low,hi);
	if((roll <= hi && roll >= low) || i == (sz-1))
	    return dmg_table->results[i];
    }
    return "DAMAGE 0";
}

int find_table(string type) {
    int i;

    if(!type) return 0;
    i = sizeof(crit_tables);
    while(i--)
	if(crit_tables[i]->table_name == type) return i;
    return -1;
}

string *query_msg(string type, int roll, string names) {
    string a_name, t_name, crit_type, tbl_name, verb, line;
    int i, hi, low, f, l, cidx;

    sscanf(type,"%s %s",tbl_name,crit_type);
    sscanf(names,"%s:%s",a_name,t_name);
    crit_type = capitalize(crit_type);
    hi = sizeof(crit_tables);
    cidx = find_table(tbl_name);
    if(cidx < 0)
	return ({ "BUG", "BUG", "BUG" });
    f = crit_tables[cidx]->start_idx[alpha[crit_type]];
    l = crit_tables[cidx]->last_idx[alpha[crit_type]];
    roll = (roll > 100)? 100:roll;
    roll = (roll < 0)? 0:roll;
    for(i=f;i<=l;i++) {
	if(sscanf(crit_tables[cidx]->idx[i],"%d..%d",low,hi) != 2) continue;
	if((roll <= hi && roll >= low)) {
	    line = crit_tables[cidx]->lines[i];
	    verb = crit_tables[cidx]->verbs[i];
	    return parse_message(line,a_name,t_name,verb);
	}
    }
    return ({ "critical hit BUG with "+tbl_name+" table.", 
      "critical hit BUG with "+tbl_name+" table.",
      "critical hit BUG with "+tbl_name+" table." });
}

string *query_result(string type, int roll) {
    string crit_type, type_c, tbl_name;
    string *rolls;
    int i, hi, low, f, l, cidx;
    class table my_table;

    sscanf(type,"%s %s",tbl_name,crit_type);
    cidx = find_table(tbl_name);
    if(cidx < 0)
	return ({ "NO_EFFECT" });
    f = crit_tables[cidx]->start_idx[alpha[crit_type]];
    l = crit_tables[cidx]->last_idx[alpha[crit_type]];
    roll = (roll > 100)? 100:roll;
    roll = (roll < 0)? 0:roll;
    for(i=f;i<=l;i++) {
	if(sscanf(crit_tables[cidx]->idx[i],"%d..%d",low,hi) != 2) continue;
	if((roll <= hi && roll >= low))
		{
		mixed *critsults = ({""});
		string *strsults = ({""});
		class table tmp_table2;
		tmp_table2 = crit_tables[cidx];
		critsults = tmp_table2->results;
		strsults = (string *)critsults[i];
                 //we need to return this - crit_tables[cidx]->results[i]
		return strsults; 
 	        
		}//this was all wacked out - still broke - that was a ugly. Cast this! .!..(><)..!.
    }
    return ({ "NO_EFFECT" });
}


string *parse_message(string line, string a_name, string t_name, string verb)
{
    string *message, a_cap_name, t_cap_name, combatcol;
    int i;
    object att, targ;

    
    a_name = lower_case(a_name); t_name = lower_case(t_name);
    att = find_living(a_name);
    targ = find_living(t_name);
    if(att) a_cap_name = (string)att->query_cap_name();
    else a_cap_name = capitalize(a_name);
    if(targ) t_cap_name = (string)targ->query_cap_name();
    else t_cap_name = capitalize(t_name);
    message = allocate(3);
    message[0] = replace_string(line,"$V",verb);
    message[0] = replace_string(message[0],"$(VT)", pluralize(verb));
    message[1] = replace_string(line,"$V",pluralize(verb));
    message[1] = replace_string(message[1],"$(VT)", pluralize(verb));
    message[2] = replace_string(line,"$V",pluralize(verb));
    message[2] = replace_string(message[2],"$(VT)",verb);
    for(i=0;i<3;i++) {
	if(i==0) {
	    message[i] = replace_string(message[i],"$Aself","yourself");
	    message[i] = replace_string(message[i],"$A","you");
	    message[i] = replace_string(message[i],"$(AP)","your");
	    message[i] = replace_string(message[i],"$(AO)","you");
	    message[i] = replace_string(message[i],"$(AN)","you");
	    message[i] = replace_string(message[i],"$(APP)","your");
	}
	if(i==0 || i ==1) {
	    message[i] = replace_string(message[i],"$Tself",
	      objective(targ)+"self");
	    message[i] = replace_string(message[i],"$T",
	      t_cap_name);
	    message[i] = replace_string(message[i],"$(TP)",
	      t_cap_name+"'s");
	    message[i] = replace_string(message[i],"$(TO)",
	      objective(targ));
	    message[i] = replace_string(message[i],"$(TN)",
	      nominative(targ));
	    message[i] = replace_string(message[i],"$(TPP)",
	      possessive(targ));

	}
	if(i==1 || i==2) {
	    message[i] = replace_string(message[i],"$Aself",
	      objective(att)+"self");
	    message[i] = replace_string(message[i],"$A",
	      a_cap_name);
	    message[i] = replace_string(message[i],"$(AP)",
	      a_cap_name+"'s");
	    message[i] = replace_string(message[i],"$(AO)",
	      objective(att));
	    message[i] = replace_string(message[i],"$(AN)",
	      nominative(att));
	    message[i] = replace_string(message[i],"$(APP)",
	      possessive(att));
	}
	if(i==2) {
	    message[i] = replace_string(message[i],"$Tself","yourself");
	    message[i] = replace_string(message[i],"$T","you");
	    message[i] = replace_string(message[i],"$(TP)","your");
	    message[i] = replace_string(message[i],"$(TO)","you");
	    message[i] = replace_string(message[i],"$(TN)","you");
	    message[i] = replace_string(message[i],"$(TPP)","your");
	}
	message[i] = capitalize(message[i]);
        if(att && (combatcol = (string)att->query_property("combat color")))
	message[i] = combatcol + message[i] + "%^RESET%^";
    }

    return message;
}
