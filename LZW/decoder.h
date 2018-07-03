#ifndef _DECODER_H_
#define _DECODER_H_

#include <string>
#include <fstream>
#include <map>

class Decoder {
 public:
  Decoder(const std::string& filename);
  ~Decoder();
  void Decode();

 private:
  std::string filename_;
  std::ifstream in_file_;
  std::map<unsigned, std::string> symbol_table_;
  void FillSymbolTable();
  void CreateOutFile(std::ofstream& out_file);
};

#endif