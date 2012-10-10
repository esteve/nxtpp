
nxtpp: linuxcomm.o nxt++.o
	ar rcs libnxtpp.a nxt++.o linuxcomm.o
	mv libnxtpp.a /usr/lib/libnxtpp.a
linuxcomm.o: src/linuxcomm.cpp include/linuxcomm.h
	g++ -c src/linuxcomm.cpp -Wall -ggdb -Iinclude
nxt++.o: include/linuxcomm.h src/nxt++.cpp include/NXT++.h
	g++ -c src/nxt++.cpp -Wall -ggdb -Iinclude
clean:
	rm -f linuxcomm.o nxt++.o
