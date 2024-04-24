CC = gcc
CCFLAGS = -Wall

tweets: tweets_generator.c markov_chain.o
	$(CC) $(CCFLAGS) -o tweets_generator tweets_generator.c markov_chain.o linked_list.o

snake: snakes_and_ladders.c markov_chain.o
	$(CC) $(CCFLAGS) -o snakes_and_ladders snakes_and_ladders.c markov_chain.o linked_list.o

markov_chain.o: markov_chain.c markov_chain.h linked_list.c linked_list.h
	$(CC) -c markov_chain.c linked_list.c
clean:
	rm *.o snakes_and_ladders tweets_generator