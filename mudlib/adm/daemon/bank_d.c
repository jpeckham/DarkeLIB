//	/adm/daemon/bank_d.c
//	from the Nightmare mudlib
//	a daemon to keep track of all player accounts
//	created by Descartes of Borg 28 february 1993 (not a leap year)

#include <security.h>
#include <dirs.h>
#include <money.h>
#include <bank.h>

mapping account;
nosave string current;

int valid_access(string who);
int account_exists(string who);

void create() {
    seteuid(getuid());
    current = 0;
    account = ([]);
}

int valid_access(string who) {
    if(!who) return 0;
    if(geteuid(previous_object()) == UID_ROOT) return 1;
    if(!this_player()) return 0;
    if((string)this_player()->query_name() != who) return 0;
    return 1;
}

mapping query_balance(string who, string bank) {
    mapping ret;

    if(!who || !bank) return 0;
    if(!valid_access(who)) return 0;
    if(current != who) {
	current = who;
	if(!account_exists(current)) {
	    current = 0;
	   account = 0;
	    return 0;
	}
	else restore_object(DIR_ACCOUNTS+"/"+current);
    }
    if(!account) return 0;
    if(!account[bank]) return 0;
    else ret = account[bank];
    account[bank]["transaction"] = 0;
    account[bank]["time"] = time();
    save_object(DIR_ACCOUNTS+"/"+current);
    return ret;
}

int deposit(string who, string bank, int amount, string type) {
    if(!who || !bank || !amount || !type) return BAD_DATA;
    if(!valid_access(who)) return BAD_ACCESS;
    if(member_array(bank, LISCENSED_BANKS) == -1) return BAD_BANK;
    if(member_array(type, HARD_CURRENCIES) == -1) return BAD_MONEY;
    if(amount < 1) return BAD_DATA;
    if(current != who) {
	current = who;
	if(!account_exists(current)) {
	    current = 0;
	    account = ([]);
	    return NO_ACCOUNT;
	}
	else restore_object(DIR_ACCOUNTS+"/"+current);
    }
    if(!account) return NO_ACCOUNT;
    if(!account[bank]) return NO_ACCOUNT;
    if(!account[bank][type]) account[bank][type] = amount;
    else account[bank][type] += amount;
    account[bank]["transaction"] = amount;
    account[bank]["time"] = time();
    log_file("bank", who+" deposited "+amount+" coins in "+bank+": "+ctime(time())+"\n");
    save_object(DIR_ACCOUNTS+"/"+current);
    return TRANSACTION_OK;
}

int withdraw(string who, string bank, int amount, string type) {
    if(!who || !bank || !amount || !type) return BAD_DATA;
    if(!valid_access(who)) return BAD_ACCESS;
    if(member_array(bank, LISCENSED_BANKS) == -1) return BAD_BANK;
    if(amount < 1) return BAD_DATA;
    if(member_array(type, HARD_CURRENCIES) == -1) return BAD_MONEY;
    if(current != who) {
	current = who;
	if(!account_exists(current)) {
	    current = 0;
	    account = ([]);
	    return NO_ACCOUNT;
	}
	else restore_object(DIR_ACCOUNTS+"/"+current);
    }
    if(!account) return NO_ACCOUNT;
    if(!account[bank]) return NO_ACCOUNT;
    if(!account[bank][type] || account[bank][type] < amount)
	return INSF;
    else account[bank][type] -= amount;
    account[bank]["transaction"] = -amount;
    account[bank]["time"] = time();
    log_file("bank", who+" withdrew "+amount+" coins in "+bank+": "+ctime(time())+"\n");
    save_object(DIR_ACCOUNTS+"/"+current);
    return TRANSACTION_OK;
}

int open_account(string who, string bank) {
    if(!who || !bank) return BAD_DATA;
    if(!valid_access(who)) return BAD_ACCESS;
    if(member_array(bank, LISCENSED_BANKS) == -1) return BAD_BANK;
    if(current != who) {
	current = who;
	if(!account_exists(current)) account = ([]);
	else restore_object(DIR_ACCOUNTS+"/"+current);
    }
    if(!account) account = ([]);
    if(account) {
	if(account[bank] && account[bank] != ([]))
	  return ALREADY_EXISTS;
    }
    account[bank] = ([ "transaction":0, "time":0, "platinum":0, "gold":0, "electrum":0, "silver":0, "copper":0 ]);
    save_object(DIR_ACCOUNTS+"/"+current);
    return TRANSACTION_OK;
}

int close_account(string who, string bank) {
    if(!who || !bank) return BAD_DATA;
    if(!valid_access(who)) return BAD_ACCESS;
    if(member_array(bank, LISCENSED_BANKS) ==-1) return BAD_BANK;
    if(current != who) {
	current = who;
	if(!account_exists(current)) {
	    current = 0;
	    account = ([]);
	    return NO_ACCOUNT;
	}
	else restore_object(DIR_ACCOUNTS+"/"+current);
    }
    if(!account) return NO_ACCOUNT;
    if(!account[bank]) return NO_ACCOUNT;
    map_delete(account, bank);
    save_object(DIR_ACCOUNTS+"/"+current);
    return TRANSACTION_OK;
}

int account_exists(string who) {
    if(file_size(DIR_ACCOUNTS+"/"+who+".o") > -1) return 1;
    else return 0;
}

string *query_bank_accounts(string who) {
    if(geteuid(previous_object()) != UID_ROOT) return 0;
    if(current != who) {
	current = who;
	if(!account_exists(current)) {
	    account = ([]);
	    current = 0;
	    return ({});
	}
	else restore_object(DIR_ACCOUNTS+"/"+current);
    }
    if(!account) return ({});
    return keys(account);
}
