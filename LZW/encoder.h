/** 
  * Name: Neel Verma
  * Current File: encoder.h
  * Description: Encodes a file.
  */

#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <map>
#include <string>
#include <fstream>

class Encoder {
 public:
  Encoder(const std::string& filename);
  ~Encoder();
  void Encode();

 private:
  std::string filename_;
  std::ifstream in_file_;
  std::map<std::string, unsigned> symbol_table_;
  void FillSymbolTable();
  void CreateOutFile(std::ofstream& out_file);
};

#endif