CFLAGS = -Wall -pedantic -O2 -std=c11
MCC = mpicc
NAME = broadcast

$(NAME): $(NAME).c
	$(MCC) $(CFLAGS) -o $(NAME) $<

.PHONY: clean
clean:
	rm $(NAME)
	