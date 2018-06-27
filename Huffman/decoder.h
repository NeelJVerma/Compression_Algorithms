#ifndef _DECODER_H_
#define _DECODER_H_

#include <string>
#include <fstream>

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
  std::ofstream CreateOutFile();
};

#endif