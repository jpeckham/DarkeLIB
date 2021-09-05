/*
** socket.c
**
** This object represents an open UDP/TCP socket using the MudOS
** socket facilities.
**
** 09-Feb-95. Deathblade. Created.
*/

#include <mudlib.h>
#include <socket.h>
#include <driver/socket_err.h>
#include <i3.h>

#define MUSTLOG(x,y)	write_file("/open/sktlog",sprintf("%s: %O\n",x,y))
//#define SKTLOG(x,y)	MUSTLOG(x,y)
#define SKTLOG(x,y)

nosave private int	style;
nosave private int	fdOwned = -1;	/* no socket yet */
nosave private function	read_func;
nosave private function	close_func;

nosave private mixed *	write_queue = ({ });
nosave private int	blocked;

/* For debug purposes only */
nosave private mixed addr;
int stat_me()
{
    switch ( style )
    {
    case SKT_STYLE_LISTEN:
	printf("%O: listening at %O\n", this_object(), addr);
	printf("    read_func=%O  close_func=%O\n", read_func, close_func);
	break;

    case SKT_STYLE_CONNECT:
	printf("%O: connected to %O\n", this_object(), addr);
	printf("    read_func=%O  close_func=%O\n", read_func, close_func);
	break;

    case SKT_STYLE_UDP:
	printf("%O: UDP at %O\n", this_object(), addr);
	printf("    read_func=%O\n", read_func);
	break;

    case SKT_STYLE_LISTEN_M:
	printf("%O: (mud) listening at %O\n", this_object(), addr);
	printf("    read_func=%O  close_func=%O\n", read_func, close_func);
	break;

    case SKT_STYLE_CONNECT_M:
	printf("%O: (mud) connected to %O\n", this_object(), addr);
	printf("    read_func=%O  close_func=%O\n", read_func, close_func);
	break;

    case SKT_STYLE_INT_ACQUIRE:
	printf("%O: accepted connection from %s\n", this_object(),
	       socket_address(fdOwned));
	printf("    read_func=%O  close_func=%O\n", read_func, close_func);
	break;
    }

    if ( sizeof(write_queue) )
	printf("queue: %O\n", write_queue);

    return 1;
}


void create(int skt_style, mixed p1, mixed p2, mixed p3)
{
    int err;

    if ( !clonep() )
	return;

seteuid(getuid(previous_object()));

SKTLOG("create: self",this_object());
    style = skt_style;
    addr = p1;

    switch ( style )
    {
    case SKT_STYLE_LISTEN:
	read_func = p2;
	close_func = p3;
	fdOwned = socket_create(1 /* STREAM */,
				"read_callback",
				"close_callback");
	if ( fdOwned < 0 )
	    error("could not create socket: " + socket_error(fdOwned) + "\n");
	if ( (err = socket_bind(fdOwned, p1)) < 0 )
	    error("could not bind socket: " + socket_error(err) + "\n");
	if ( (err = socket_listen(fdOwned, "listen_callback")) < 0 )
	    error("could not listen to socket: " + socket_error(err) + "\n");
SKTLOG("create: SKT_STYLE_LISTEN",fdOwned);
	break;

    case SKT_STYLE_CONNECT:
	read_func = p2;
	close_func = p3;
	fdOwned = socket_create(1 /* STREAM */,
				"read_callback",
				"close_callback");
	if ( fdOwned < 0 )
	    error("could not create socket: " + socket_error(fdOwned) + "\n");
	err = socket_connect(fdOwned, p1, "read_callback", "write_callback");
	if ( err < 0 )
	    error("could not listen to socket: " + socket_error(err) + "\n");
SKTLOG("create: SKT_STYLE_CONNECT",fdOwned);
SKTLOG("create: close_func",close_func);
	break;

    case SKT_STYLE_UDP:
	read_func = p2;
	fdOwned = socket_create(2 /* DATAGRAM */, "read_udp_callback");
	if ( fdOwned < 0 )
	    error("could not create socket: " + socket_error(fdOwned) + "\n");
	if ( (err = socket_bind(fdOwned, p1)) < 0 )
	    error("could not bind socket: " + socket_error(err) + "\n");
SKTLOG("create: SKT_STYLE_UDP",fdOwned);
	break;

    case SKT_STYLE_LISTEN_M:
	read_func = p2;
	close_func = p3;
	fdOwned = socket_create(0 /* MUD */,
				"read_callback",
				"close_callback");
	if ( fdOwned < 0 )
	    error("could not create socket: " + socket_error(fdOwned) + "\n");
	if ( (err = socket_bind(fdOwned, p1)) < 0 )
	    error("could not bind socket: " + socket_error(err) + "\n");
	if ( (err = socket_listen(fdOwned, "listen_callback")) < 0 )
	    error("could not listen to socket: " + socket_error(err) + "\n");
SKTLOG("create: SKT_STYLE_LISTEN_M",fdOwned);
	break;

    case SKT_STYLE_CONNECT_M:
	read_func = p2;
	close_func = p3;
	fdOwned = socket_create(0 /* MUD */,
				"read_callback",
				"close_callback");
	if ( fdOwned < 0 )
	    error("could not create socket: " + socket_error(fdOwned) + "\n");
	err = socket_connect(fdOwned, p1, "read_callback", "write_callback");
	if ( err < 0 )
	    error("could not listen to socket: " + socket_error(err) + "\n");
SKTLOG("create: SKT_STYLE_CONNECT_M",fdOwned);
SKTLOG("create: close_func",close_func);
	break;

    case SKT_STYLE_INT_ACQUIRE:
	read_func	= p1;
	close_func	= p2;
	break;
    }
}

private nomask void listen_callback(int fd)
{
    object	s;
    int		err;

SKTLOG("listen_callback: self",this_object());
SKTLOG("listen_callback: fd",fd);
    fd = socket_accept(fd, "read_callback", "write_callback");
    s = clone_object(SOCKET, SKT_STYLE_INT_ACQUIRE,
		     read_func,
		     close_func);
SKTLOG("listen_callback: new sock",s);
    err = socket_release(fd, s, "release_callback");
SKTLOG("listen_callback: err",err);
    if ( err < 0 )
	error("could not release: " + socket_error(err) + "\n");
}

private nomask void release_callback(int fdToAcquire)
{
    int err;

SKTLOG("release_callback: self",this_object());
    fdOwned = fdToAcquire;
SKTLOG("release_callback: fdOwned",fdOwned);
    err = socket_acquire(fdOwned,
			 "read_callback",
			 "write_callback",
			 "close_callback");
SKTLOG("release_callback: err",err);

    if ( err < 0 )
	error("could not release: " + socket_error(err) + "\n");

    /*
    ** Deliver a 0 indicating a new connection (and providing self)
    */
    catch(evaluate(read_func, this_object(), 0));
}

private nomask void read_callback(int fd, mixed message)
{
    /* ### workaround an internal driver bug */
    restore_variable("0");

SKTLOG("read_callback: self",this_object());
SKTLOG("read_callback: fd",fd);
    catch(evaluate(read_func, this_object(), message));
}

private nomask void read_udp_callback(int fd, mixed message, string address)
{
SKTLOG("read_udp_callback: self",this_object());
SKTLOG("read_udp_callback: fd",fd);
SKTLOG("read_udp_callback: read_func",read_func);
    catch(evaluate(read_func, this_object(), message, address));
}

private nomask void close_callback(int fd)
{
SKTLOG("close_callback: self",this_object());
SKTLOG("close_callback: fd",fd);
SKTLOG("close_callback: close_func",close_func);
    if ( close_func )
    {
	catch(evaluate(close_func, this_object()));
    }

    destruct(this_object());
}

private nomask void write_callback(int fd)
{
SKTLOG("write_callback: self",this_object());
SKTLOG("write_callback: fd",fd);
SKTLOG("write_callback: # elem",sizeof(write_queue));

    /*
    ** No longer blocked (can accept new data).
    */
    blocked = 0;

    while ( sizeof(write_queue) > 0 )
    {
	int err;

	err = socket_write(fd, write_queue[0]);
SKTLOG("write_callback: err",err);

	/* ### workaround an internal driver bug */
	restore_variable("0");

	/* ### HACK: driver did not write anything, but it won't call again,
	** ### so force it with a call_out()
	*/
	if ( err == EEWOULDBLOCK )
	{
	    blocked = 1;
	    call_out((: write_callback, fd :), 1);
	    return;
	}

	/*
	** Remove the item from the queue.  It has been written.
	*/
	write_queue = write_queue[1..];

	if ( err == EECALLBACK )
	{
	    /* done for now... wait for the next callback */
	    blocked = 1;
	    return;
	}
	if ( err < 0 )
	    error("could not write: " + socket_error(err) + "\n");
    }
}

varargs nomask void send(mixed message, string address)
{
    int	err;

SKTLOG("send: self",this_object());
SKTLOG("send: fd",fdOwned);
SKTLOG("send: # elem",sizeof(write_queue));

    if ( address )
	err = socket_write(fdOwned, message, address);
    else if ( blocked )
    {
	/*
	** If we are blocked, then the socket doesn't want us to send
	** any more.  Place it on our queue for sending later.
	*/
	write_queue += ({ message });
    }
    else
    {
	err = socket_write(fdOwned, message);
SKTLOG("send: err",err);

	/* ### workaround an internal driver bug */
	restore_variable("0");

	/* ### note: test for EEALREADY for now... need newer driver */
	if ( err == EEALREADY )
	{
	    blocked = 1;
	    write_queue += ({ message });
	    return;
	}
	/* ### same for OWB: means the driver hasn't queued it :- */
	/* ### set a call_out() to ensure we're called to deliver it */
	if ( err == EEWOULDBLOCK )
	{
	    blocked = 1;
	    write_queue += ({ message });
	    call_out((: write_callback, fdOwned :), 1);
	    return;
	}
	if ( err == EECALLBACK )
	{
	    /*
	    ** Socket took the message but is blocked until it can
	    ** write it out.  Set a flag so that we don't write any
	    ** more until we get the callback.
	    */
	    blocked = 1;
	    return;
	}
    }

    if ( err < 0 )
	error("could not write: " + socket_error(err) + "\n");
}

void remove()
{
    int err;

    if ( fdOwned >= 0 )
    {
SKTLOG("remove: self",this_object());
SKTLOG("remove: fdOwned",fdOwned);
	err = socket_close(fdOwned);
SKTLOG("remove: err",err);
	if ( err < 0 )
	    error("could not close: " + socket_error(err) + "\n");
    }

    destruct(this_object());
}

nomask mixed *address()
{
    string tmp;
    string host;
    int port;

    tmp = socket_address(fdOwned);
SKTLOG("address: fdOwned", fdOwned);
SKTLOG("address: tmp", tmp);
    sscanf(tmp, "%s %d", host, port);
    return ({ host, port });
}
