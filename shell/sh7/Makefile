# Makefile for shell exercise

include object_defs

target :  shell

shell : $(SHELLOBJ)
	$(CC) -o $@ $(SHELLOBJ)

$(SHELLOBJ) : shell.h

# some useful chores here

clean :
	rm -f *.o

print :
	pr Makefile object_defs shell.h $(SHELLOBJ:.o=.c) | lp -onobanner
