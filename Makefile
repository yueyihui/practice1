all: withoutlock mutex pv

withoutlock: practice1_withoutlock.c
	gcc -Wall -pthread -o $@ $^

mutex: practice1_mutex.c
	gcc -Wall -pthread -o $@ $^

pv: practice1_pv.c
	gcc -Wall -pthread -o $@ $^

.PHONY: clean

clean:
	find -type f -executable -delete
	find -type f -name *.o -delete
