#include "decoder.h"
#include "../cmdlineparser.h"
#include <iostream>
#include <cstdlib>
#include <queue>
#include <regex>

const unsigned kMaxChars = 256;

Decoder::Decoder(const std::string& filename) {
  in_file_.open(filename);

  if (!in_file_) {
    std::cout << filename << " could not be opened" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  filename_ = filename;
  frequencies_ = new unsigned[kMaxChars];
}

Decoder::~Decoder() {
  in_file_.close();
  delete[] frequencies_;
}

void Decoder::FillFrequencies() {
  for (int i = 0; i < kMaxChars; i++) {
    in_file_.read((char*)&frequencies_[i], sizeof(unsigned));
  }
}

Node* Decoder::BuildTree() {
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

  return pq.top();
}

std::ofstream Decoder::CreateOutFile() {
  std::regex e(".*\\/(.*)\\..*$");
  std::smatch sm;
  std::regex_match(filename_.cbegin(), filename_.cend(), sm, e);
  std::string string_sm = std::string(sm[1]);
  string_sm = string_sm.substr(0, string_sm.length() - 2);
  std::string out_filename = "../Decoded/" + string_sm + "-d.txt";
  std::ofstream out_file;
  out_file.open(out_filename);

  if (!out_file) {
    std::cout << "Could not create compressed file" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  return out_file;
}

void Decoder::Decode() {
  Node* root = BuildTree();
  Node* back_to_root = root;
  std::ofstream out_file = CreateOutFile();
  in_file_.seekg(0, in_file_.end);
  unsigned file_size = in_file_.tellg();
  file_size -= 1024;
  in_file_.seekg(1024, in_file_.beg);
  char* bit_buffer = new char[file_size];
  in_file_.read(bit_buffer, file_size);

  for (int i = 0; i < file_size; i++) {
    for (int j = 0; j < 8; j++) {
      if ((bit_buffer[i] >> j) & 1) {
        root = root->right;
      } else {
        root = root->left;
      }

      if (root->data != '\0') {
        out_file << root->data;
        root = back_to_root;
      }
    }
  }
}