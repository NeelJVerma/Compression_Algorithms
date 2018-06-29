/** 
  * Name: Neel Verma
  * Current File: encoder.cc
  * Description: Umplements the encoder
  */

#include "encoder.h"
#include <iostream>
#include <queue>
#include <regex>

const unsigned kMaxChars = 256;

/**
  * Description: Constructor for the Encoder class.
  * Parameters: const std::string& filename --> The filename that was passed
  *             back from the command line parser.
  * Returns: Nothing.
  */

Encoder::Encoder(const std::string& filename) {
  in_file_.open(filename);

  if (!in_file_) {
    std::cout << filename << " could not be opened" << std::endl;
    in_file_.close();
    std::exit(EXIT_FAILURE);
  }

  filename_ = filename;
  frequencies_ = new unsigned[kMaxChars];
  std::memset(frequencies_, 0, kMaxChars * sizeof(unsigned));
}

/**
  * Description: Destructor for the Encoder class.
  * Parameters: None.
  * Returns: Nothing.
  */

Encoder::~Encoder() {
  in_file_.close();
  delete[] frequencies_;
}

/**
  * Description: Reads the file into a buffer, parses it, and updates the
  * frequency of every character seen.
  * Parameters: None.
  * Returns: Nothing.
  */

void Encoder::FillFrequencies() {
  in_file_.seekg(0, in_file_.end);
  unsigned file_size = in_file_.tellg();
  in_file_.seekg(0, in_file_.beg);
  char* buffer = new char[file_size];
  in_file_.read(buffer, file_size);

  for (int i = 0; i < file_size; i++) {
    unsigned int_char = unsigned(buffer[i]);

    if (int_char < kMaxChars && int_char >= 0) {
      frequencies_[int_char]++;
    }
  }

  delete[] buffer;
}

/**
  * Description: Fills the symbol table, which stores each character and its
  * binary equivalent as a string.
  * Parameters: Node* root --> The root of the tree (and each subtree)
  *             std::string code --> The binary equivalent of each character.
  * Returns: Nothing.
  */

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

/**
  * Description: Builds the huffman tree based off of the frequencies that were
  * generated on the first pass of reading the file. Also fills the symbol
  * table.
  * Parameters: None.
  * Returns: Nothing.
  */

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

/**
  * Description: Creates the output file (compressed binary file).
  * Parameters: std::ofstream& out_file --> The file to be created/opened.
  * Returns: Nothing.
  */

void Encoder::CreateOutFile(std::ofstream& out_file) {
  std::regex e(".*\\/(.*)\\..*$");
  std::smatch sm;
  std::regex_match(filename_.cbegin(), filename_.cend(), sm, e);
  std::string out_filename = "../Encoded/" + std::string(sm[1]) + "-c.huf";
  out_file.open(out_filename);

  if (!out_file) {
    std::cout << "Could not create compressed file" << std::endl;
    out_file.close();
    in_file_.close();
    std::exit(EXIT_FAILURE);
  }
}

/**
  * Description: Writes the frequencies to the head of the file. We will need
  * this to reconstruct the tree when decoding.
  * Parameters: std::ofstream& out_file --> The file to be modified.
  * Returns: Nothing.
  */

void Encoder::WriteOutFileHeader(std::ofstream& out_file) {
  out_file.write((char*)&frequencies_[0], kMaxChars * sizeof(frequencies_[0]));
}

/**
  * Description: Encodes the file. First, build the huffman tree. Then, create
  * the compressed file and write the frequency header. Next, read the entire
  * file into a buffer and process it character by character into the binary
  * equivalents, and write them to the file. Then finally, close the file.
  * 
  * EXAMPLE: cabbac
  * c --> 11
  * a --> 10
  * b --> 01
  * \n --> 00
  * Original file size = 7 bytes.
  * Encoded string = 11100101101100
  * Processing in 8 bit chunks, new file size = 2 bytes (without header).
  *
  * Parameters: None.
  * Returns: Nothing.
  */

void Encoder::Encode() {
  BuildTree();
  in_file_.clear();
  in_file_.seekg(0, std::ios::beg);
  std::ofstream out_file;
  CreateOutFile(out_file);
  WriteOutFileHeader(out_file);
  in_file_.seekg(0, in_file_.end);
  unsigned file_size = in_file_.tellg();
  in_file_.seekg(0, in_file_.beg);
  char* buffer = new char[file_size];
  in_file_.read(buffer, file_size);
  unsigned char bit_buffer = 0;
  unsigned base = 1;
  unsigned current_bit = 0;

  for (int i = 0; i < file_size; i++) {
    for (int j = 0; j < symbol_table_[buffer[i]].length(); j++) {
      if (symbol_table_[buffer[i]][j] == '1') {
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
  delete[] buffer;
}