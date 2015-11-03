CC=gcc

LIBS=-ldcam

test: test.c
	gcc -O2 -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ test 
install:
	cp ./libdcam.so /usr/lib
