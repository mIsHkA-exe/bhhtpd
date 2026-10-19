CC=gcc
CFLAGS=-Wall
EXEC=bhttpd

~/bhttpd/src/$(EXEC): ~/bhttpd/src/bhttpd.o
	$(CC) $(CFLAGS) $< -o $@

~/bhttpd/src/bhttpd.o: ~/bhttpd/src/bhttpd.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean install

clean:
	rm -f ~/bhttpd/src/*.o ~/bhttpd/src/$(EXEC)

install: ~/bhttpd/src/$(EXEC)
	cp $< ~/opt/bhttpd/sbin/$(EXEC)
	chmod +x ~/opt/bhttpd/sbin/$(EXEC)
