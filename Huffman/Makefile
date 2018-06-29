executable: huffman

huffman: cmdlineparser.o encoder.o decoder.o huffmantest.o
	g++ -o huffman cmdlineparser.o encoder.o decoder.o huffmantest.o

cmdlineparser.o: ../cmdlineparser.cc
	g++ -c -g -std=c++14 -I./ -I../ ../cmdlineparser.cc

encoder.o: encoder.cc
	g++ -c -g -std=c++14 -I./ encoder.cc

decoder.o: decoder.cc
	g++ -c -g -std=c++14 -I./ decoder.cc

huffmantest.o: huffmantest.cc
	g++ -c -g -std=c++14 -I./ huffmantest.cc