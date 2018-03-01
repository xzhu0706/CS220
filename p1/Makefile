main = test
CC = g++
CFLAGS = -Wall

$(main) : $(main).c
	$(CC) $(CFLAGS) $< -o $(main)

.PHONY : clean
clean :
	rm -f $(main)
