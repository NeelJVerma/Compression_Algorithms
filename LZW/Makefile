executable: lzw

lzw: cmdlineparser.o encoder.o decoder.o lzwtest.o
	g++ -o lzw cmdlineparser.o encoder.o decoder.o lzwtest.o

cmdlineparser.o: ../cmdlineparser.cc
	g++ -c -g -std=c++14 -I./ -I../ ../cmdlineparser.cc

encoder.o: encoder.cc
	g++ -c -g -std=c++14 -I./ encoder.cc

decoder.o: decoder.cc
	g++ -c -g -std=c++14 -I./ decoder.cc

huffmantest.o: lzwtest.cc
	g++ -c -g -std=c++14 -I./ lzwtest.cc