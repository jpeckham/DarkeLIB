
SRC=uids.c contrib.c sockets.c develop.c matrix.c math.c parser.c mudlib_stats.c 
OBJ=uids.$(O) contrib.$(O) sockets.$(O) develop.$(O) matrix.$(O) math.$(O) parser.$(O) mudlib_stats.$(O) 

.c.$(O):
	$(CC) $(CFLAGS) -I.. -c $*.c

all: $(OBJ)
	ar rcu packages.$(A) $(OBJ)
	$(RANLIB) packages.$(A)

clean:
	rm -f *.$(O)
	rm -f packages.$(A)
