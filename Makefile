executable: huffman

huffman: cmdlineparser.o encoder.o decoder.o huffmantest.o
	g++ -o huffman cmdlineparser.o encoder.o decoder.o huffmantest.o

cmdlineparser.o: cmdlineparser.cc
	g++ -c -g -std=c++14 -I./ cmdlineparser.cc

encoder.o: Huffman/encoder.cc
	g++ -c -g -std=c++14 -I./ -I./Huffman Huffman/encoder.cc

decoder.o: Huffman/decoder.cc
	g++ -c -g -std=c++14 -I./ -I./Huffman Huffman/decoder.cc

huffmantest.o: huffmantest.cc
	g++ -c -g -std=c++14 -I./ huffmantest.cc