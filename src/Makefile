CC=g++
CFLAGS=-c -std=c++11

all: crypto

crypto: anagram.o analyze.o crypto_utils.o Main.o substitution.o wordlist.o
	$(CC) anagram.o analyze.o crypto_utils.o Main.o substitution.o wordlist.o -o crypto

anagram.o: anagram.cpp
	$(CC) $(CFLAGS) anagram.cpp

analyze.o: analyze.cpp
	$(CC) $(CFLAGS) analyze.cpp

crypto_utils.o: crypto_utils.cpp
	$(CC) $(CFLAGS) crypto_utils.cpp

Main.o: Main.cpp
	$(CC) $(CFLAGS) Main.cpp

substitution.o: substitution.cpp
	$(CC) $(CFLAGS) substitution.cpp

wordlist.o: wordlist.cpp
	$(CC) $(CFLAGS) wordlist.cpp

clean:
	rm -rf *o
