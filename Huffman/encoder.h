/** 
  * Name: Neel Verma
  * Current File: encoder.h
  * Description: Encodes a file.
  */

#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "node.h"
#include <string>
#include <fstream>
#include <map>

class Encoder {
 public:
  Encoder(const std::string& filename);
  ~Encoder();
  void Encode();

 private:
  unsigned* frequencies_;
  std::string filename_;
  std::ifstream in_file_;
  std::map<char, std::string> symbol_table_;
  void FillFrequencies();
  void FillSymbolTable(Node* root, std::string code);
  void BuildTree();
  void CreateOutFile(std::ofstream& out_file);
  void WriteOutFileHeader(std::ofstream& out_file);
};

#endif