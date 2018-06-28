/** 
  * Name: Neel Verma
  * Current File: decoder.h
  * Description: Decodes a file.
  */

#ifndef _DECODER_H_
#define _DECODER_H_

#include "node.h"
#include <string>
#include <fstream>

class Decoder {
 public:
  Decoder(const std::string& filename);
  ~Decoder();
  void Decode();

 private:
  unsigned* frequencies_;
  std::string filename_;
  std::ifstream in_file_;
  void FillFrequencies();
  Node* BuildTree();
  void CreateOutFile(std::ofstream& out_file);
};

#endif