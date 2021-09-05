//  A Power Stone.....veeeeeeery cool.

inherit "/std/Object";

int max_points;
int mp_left;
int activated;
nosave int time_to_regen;
nosave int hb_on;

void create() {
  ::create();
  max_points = 10;
  time_to_regen = 30;
  set_name("powerstone");
  set_id(({ "powerstone", "stone", "power stone" }));
  set_long("This is a power stone.  You may borrow mp from it with "
  "borrow <amount> from <stone> (example: 'borrow 45 from stone 2').  "
  "The stone will regenerate automatically.");
  set_weight(5);
  set_value(1000);
  set_heart_beat(1);
  hb_on = 1;
  return;
}

int is_active() { return activated; }

string query_long(string null) {
  string res;

  res = ::query_long(null);
  if(!activated) {
    return "This is a blank power stone for use with the enchanter spell "+
      "'power stone'.  As is, it is little more than a pretty jewel."+
      sprintf("\nThe capacity of this stone is %d mp.", max_points);
  }
  res += "\n\n";
  res += sprintf("This stone has %d (%d) mp available.", mp_left, max_points);
  return res;
}

string query_short() {
  if(!activated) return sprintf("A blank %d-point powerstone", max_points);
  else return sprintf("A %d-point powerstone", max_points);
}

void init() {
  ::init();
  add_action("borrow_mp", "borrow");
  return;
}

int borrow_mp(string str) {
  string me;
  int amt;

  if(sscanf(str, "%d from %s", amt, me) != 2) return 0;
  if(present(me, this_player()) != this_object())
    return notify_fail("There is no "+me+" here.\n");
  if(!activated) {
    write("That stone is still blank.");
    return 1;
  }
  if(amt > mp_left) {
    write("Sorry, only "+sprintf("%d mp %s", mp_left, ((mp_left==1)?"is":"are"))+
      " available.");
    return 1;
  }
  mp_left -= amt;
  this_player()->add_mp(amt);
  if(!hb_on) {
    hb_on = 1;
    set_heart_beat(1);
  }
  write("Ok.");
  write(sprintf("You now have %d (%d) mp.", (int)this_player()->query_mp(),
      (int)this_player()->query_max_mp()));
  return 1;
}

int query_size() { return max_points; }

void heart_beat() {
  if(!activated || mp_left >= max_points) {
    hb_on = 0;
    set_heart_beat(0);
    return;
  }
  time_to_regen--;
  if(time_to_regen <= 0) {
    mp_left += max_points / 20 + 1;
    time_to_regen = 30;
    if(mp_left >= max_points) {
      mp_left = max_points;
      hb_on = 0;
      set_heart_beat(0);
    }
  }
  return;
}

int can_exceed_max_mp() { return 1; }

void set_max(int x) {
  max_points = x;
  set_value(10 * x * x/2);
  return;
}

void activate_me() {
  mp_left = max_points;
  activated = 1;
  if(!hb_on) {
    set_heart_beat(1);
    hb_on = 1;
  }
  return;
}

