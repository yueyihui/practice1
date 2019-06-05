all: withoutlock mutex pv

withoutlock: practice2_withoutlock.c
	gcc -Wall -pthread -o $@ $^

mutex: practice2_mutex.c
	gcc -Wall -pthread -o $@ $^

pv: practice2_pv.c
	gcc -Wall -pthread -o $@ $^

.PHONY: clean

clean:
	find -type f -executable -delete
	find -type f -name *.o -delete
