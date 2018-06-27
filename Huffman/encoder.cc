#include "encoder.h"
#include "../cmdlineparser.h"
#include <iostream>
#include <cstdlib>
#include <queue>
#include <regex>

const unsigned kMaxChars = 256;

Encoder::Encoder(const std::string& filename) {
  in_file_.open(filename);

  if (!in_file_) {
    std::cout << filename << " could not be opened" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  filename_ = filename;
  frequencies_ = new unsigned[kMaxChars];
}

Encoder::~Encoder() {
  in_file_.close();
  delete[] frequencies_;
}

void Encoder::ZeroOutFrequencies() {
  for (int i = 0; i < kMaxChars; i++) {
    frequencies_[i] = 0;
  }
}

void Encoder::FillFrequencies() {
  ZeroOutFrequencies();
  char c;

  while (in_file_.get(c)) {
    unsigned int_c = unsigned(c);

    if (int_c < kMaxChars && int_c >= 0) {
      frequencies_[int_c]++;
    }
  }
}

void Encoder::FillSymbolTable(Node* root, std::string code) {
  if (!root) {
    return;
  }

  if (root->data != '\0') {
    symbol_table_[root->data] = code;
  }

  FillSymbolTable(root->left, code + '0');
  FillSymbolTable(root->right, code + '1');
}

void Encoder::BuildTree() {
  FillFrequencies();
  std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

  for (int i = 0; i < kMaxChars; i++) {
    if (frequencies_[i] > 0) {
      pq.push(new Node(char(i), frequencies_[i]));
    }
  }

  while (pq.size() > 1) {
    Node* left = pq.top();
    pq.pop();
    Node* right = pq.top();
    pq.pop();
    Node* top = new Node('\0', left->frequency + right->frequency);
    top->left = left;
    top->right = right;
    pq.push(top);
  }

  FillSymbolTable(pq.top(), "");
}

std::ofstream Encoder::CreateOutFile() {
  std::regex e(".*\\/(.*)\\..*$");
  std::smatch sm;
  std::regex_match(filename_.cbegin(), filename_.cend(), sm, e);
  std::string out_filename = "../Encoded/" + std::string(sm[1]) + "-c.huf";
  std::ofstream out_file;
  out_file.open(out_filename);

  if (!out_file) {
    std::cout << "Could not create compressed file" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  return out_file;
}

void Encoder::WriteOutFileHeader(std::ofstream& out_file) {
  for (int i = 0; i < kMaxChars; i++) {
    out_file.write((char*)&frequencies_[i], sizeof(frequencies_[i]));
  }
}

void Encoder::Encode() {
  BuildTree();
  in_file_.clear();
  in_file_.seekg(0, std::ios::beg);
  std::ofstream out_file = CreateOutFile();
  WriteOutFileHeader(out_file);
  char c;
  unsigned char bit_buffer = 0;
  unsigned base = 1;
  unsigned current_bit = 0;

  while (in_file_.get(c)) {
    for (int i = 0; i < symbol_table_[c].length(); i++) {
      if (symbol_table_[c][i] == '1') {
        bit_buffer += base;
      }

      base <<= 1;
      current_bit++;

      if (current_bit == 8) {
        out_file << bit_buffer;
        base = 1;
        current_bit = 0;
        bit_buffer = 0;
      }
    }
  }

  if (current_bit) {
    out_file << bit_buffer;
  }

  out_file.close();
}