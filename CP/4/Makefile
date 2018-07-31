PROGS = prodsim janken
all: $(PROGS)

prodsim: prodsim.c
	gcc -o prodsim prodsim.c

janken: janken.c
	gcc -o janken janken.c -lpthread

clean:
	rm -f $(PROGS) *~
