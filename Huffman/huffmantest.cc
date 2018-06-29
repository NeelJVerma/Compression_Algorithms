/** 
  * Name: Neel Verma
  * Current File: huffmantest.cc
  * Description: Tests the huffman codec.
  */

#include "encoder.h"
#include "decoder.h"
#include "../cmdlineparser.h"

int main(int argc, char* argv[]) {
  CmdLineParser* parser = new CmdLineParser(argc, argv);

  if (parser->IsCompress()) {
    Encoder* encoder = new Encoder(parser->Filename());
    encoder->Encode();
    delete encoder;
  } else {
    Decoder* decoder = new Decoder(parser->Filename());
    decoder->Decode();
    delete decoder;
  }

  delete parser;
  return EXIT_SUCCESS;
}