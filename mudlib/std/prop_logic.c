// File:       /std/prop_logic.c
// Purpose:    generic heirachic property setting mechanism
// History:
// 92-05-01    Buddha@TMI-2        Written for object properties
// 93-03-17    Pallando@Tabor      Cleaned up code and modularised.
// 93-06-08    Pallando@Nightmare  Used for material properties

protected nomask private mixed _query( mapping map, string *parts )
{
  mixed value;
  int i, s;

  value = map;
  s = sizeof( parts );
  for( i = 0 ; i < s ; i++ )
  {
    if( undefinedp( value = value[parts[i]] ) )
      break;
    if( !mapp( value ) )
      break;
  }
  return value;
}

protected nomask private int _delete( mapping map, string *parts )
{
  if( sizeof( parts ) == 1 )
  {
    map_delete( map, parts[0] );
    return 1;
  }
  if( !map[parts[0]] || !mapp( map[parts[0]] ) )
    return 0;
  return _delete( map[parts[0]], parts[1..sizeof( parts )-1] );
}

protected nomask mixed _set( mapping map, string *parts, mixed value )
{
  mixed old_value;

  if( sizeof( parts ) == 1 )
  {
    old_value = map[parts[0]];
    map[parts[0]] = value;
    return old_value;
  }
  if( !map[parts[0]] || !mapp( map[parts[0]] ) )
    map[parts[0]] = ([ parts[1] : 0 ]);
  return _set( map[parts[0]], parts[1..sizeof( parts )-1], value );
}
