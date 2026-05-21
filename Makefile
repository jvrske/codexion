all:
	cc -Wall -Wextra -Werror -pthread */*.c *.c -o codexion

clean:
	@rm codexion