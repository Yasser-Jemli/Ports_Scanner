CC=g++
CFLAGS=-Wall -pthread -Wextra -Werror -std=c++11

test : 
	$(CC) test.cpp -o test $(CFLAGS)

ps : 
	$(CC) pscan.cpp -o ps $(CFLAGS)

clean:
	@rm `find . -type f -executable `

	