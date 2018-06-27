#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <string>
#include <fstream>
#include <map>

struct Node {
  char data;
  unsigned frequency;
  Node* left;
  Node* right;

  Node(const char& data, const unsigned& frequency) {
    left = right = NULL;
    this->data = data;
    this->frequency = frequency;
  }
};

struct Compare {
  bool operator()(const Node* left, const Node* right) {
    return left->frequency > right->frequency;
  }
};

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
  void ZeroOutFrequencies();
  void FillFrequencies();
  void FillSymbolTable(Node* root, std::string code);
  void BuildTree();
  std::ofstream CreateOutFile();
  void WriteOutFileHeader(std::ofstream& out_file);
};

#endif