/** 
  * Name: Neel Verma
  * Current File: decoder.cc
  * Description: Implements the decoder.
  */

#include "decoder.h"
#include <iostream>
#include <queue>
#include <regex>

const unsigned kMaxChars = 256;

/**
  * Description: Constructor for the Decoder class.
  * Parameters: const std::string& filename --> The filename that was passed
  *             back from the command line parser.
  * Returns: Nothing.
  */

Decoder::Decoder(const std::string& filename) {
  in_file_.open(filename);

  if (!in_file_) {
    std::cout << filename << " could not be opened" << std::endl;
    in_file_.close();
    std::exit(EXIT_FAILURE);
  }

  filename_ = filename;
  frequencies_ = new unsigned[kMaxChars];
}

/**
  * Description: Destructor for the Encoder class.
  * Parameters: None.
  * Returns: Nothing.
  */

Decoder::~Decoder() {
  in_file_.close();
  delete[] frequencies_;
}

/**
  * Description: Reads the compressed file header and recoonstructs the
  * frequencies array.
  * Parameters: None.
  * Returns: Nothing.
  */

void Decoder::FillFrequencies() {
  in_file_.read((char*)frequencies_, kMaxChars * sizeof(unsigned));
}

/**
  * Description: Builds the huffman tree based off of the frequencies that were
  * generated on the first pass of reading the file.
  * Parameters: None.
  * Returns: The root of the final constructed tree. Used when decoding
  */

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

/**
  * Description: Creates the output file (decompressed text file).
  * Parameters: std::ofstream& out_file --> The file to be created/opened.
  * Returns: Nothing.
  */

void Decoder::CreateOutFile(std::ofstream& out_file) {
  std::regex e(".*\\/(.*)\\..*$");
  std::smatch sm;
  std::regex_match(filename_.cbegin(), filename_.cend(), sm, e);
  std::string string_sm = std::string(sm[1]);
  string_sm = string_sm.substr(0, string_sm.length() - 2);
  std::string out_filename = "../Decoded/" + string_sm + "-huf-d.txt";
  out_file.open(out_filename);

  if (!out_file) {
    std::cout << "Could not create decompressed file" << std::endl;
    out_file.close();
    in_file_.close();
    std::exit(EXIT_FAILURE);
  }
}

/**
  * Description: Decodes the compressed binary file. First, reconstruct the
  * huffman tree from the frequencies read in the header. Then, create the
  * output file and read the input file into a buffer. Next, process every bit
  * of the compressed file and when we get to a leaf of the tree, print its
  * character to the output file.
  *
  * EXAMPLE: cabbac
  * c --> 11
  * a --> 10
  * b --> 01
  * \n --> 00
  * Above is the tree generated by the frequencies.
  * Encoded string = 11100101101100
  * As we are parsing, we see that 11 is c, 10 is a, 01 is b, and so on. So, we
  * are able to reconstruct our file from the compressed one.
  *
  * Parameters: None.
  * Returns: Nothing.
  */

void Decoder::Decode() {
  Node* root = BuildTree();
  Node* back_to_root = root;
  std::ofstream out_file;
  CreateOutFile(out_file);
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

  delete[] bit_buffer;
}