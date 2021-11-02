CC		= gcc
CFLAGS	= -Wall -Wextra -pedantic -std=c11 


link:
	$(CC) $(CFLAGS) utils.c main.c