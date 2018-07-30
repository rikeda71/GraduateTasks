EXEC = ProdCons ProdCons_not_debug

all: $(EXEC)
ProdCons:
	gcc -o ProdCons ProdCons.c
ProdCons_not_debug:
	gcc -DWITH_NOT_DEBUG -o ProdCons_not_debug ProdCons.c

clean:
	rm -f $(EXEC) *.o *.s *~
