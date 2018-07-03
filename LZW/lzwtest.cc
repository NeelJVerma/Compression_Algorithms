/** 
  * Name: Neel Verma
  * Current File: lzwtest.cc
  * Description: Tests the LZW codec.
  */

#include "encoder.h"
#include "decoder.h"
#include "../cmdlineparser.h"
#include <cstdlib>

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