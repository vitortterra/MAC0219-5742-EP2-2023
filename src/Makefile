CFLAGS = -Wall -pedantic -O2 -std=c11
MCC = mpicc

bcast: bcast.c
	$(MCC) $(CFLAGS) -o bcast $<

.PHONY: clean
clean:
	rm bcast