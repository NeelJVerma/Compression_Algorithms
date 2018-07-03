/** 
  * Name: Neel Verma
  * Current File: decoder.cc
  * Description: Implements the decoder.
  */

#include "decoder.h"
#include <iostream>
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
}

/**
  * Description: Destructor for the Encoder class.
  * Parameters: None.
  * Returns: Nothing.
  */

Decoder::~Decoder() {
  in_file_.close();
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
  std::string out_filename = "../Decoded/" + string_sm + "-lzw-d.txt";
  out_file.open(out_filename);

  if (!out_file) {
    std::cout << "Could not create decompressed file" << std::endl;
    out_file.close();
    in_file_.close();
    std::exit(EXIT_FAILURE);
  }
}

/**
  * Description: Fills the symbol table with the codes to the one byte
  * codewords.
  * Parameters: None.
  * Returns: Nothing.
  */

void Decoder::FillSymbolTable() {
  for (int i = 0; i < kMaxChars; i++) {
    symbol_table_[i] = std::string(1, char(i));
  }
}

/**
  * Description: Decodes the compressed binary file. First, fill the symbol
  * table with the one byte codes, then create/open the output file. Next,
  * decode the file in the same way that it was encoded, but in reverse.
  * Parameters: None.
  * Returns: Nothing.
  */

void Decoder::Decode() {
  FillSymbolTable();
  unsigned code = kMaxChars;
  in_file_.seekg(0, in_file_.end);
  unsigned file_size = in_file_.tellg();
  in_file_.seekg(0, in_file_.beg);
  char* buffer = new char[file_size];
  in_file_.read(buffer, file_size);
  std::ofstream out_file;
  CreateOutFile(out_file);
  unsigned* f_code = (unsigned*)buffer;
  std::string code_word = symbol_table_[*f_code++];
  std::string entry = "";
  out_file << code_word;

  while (f_code < (unsigned*)(buffer + file_size)) {
    if (symbol_table_.find(*f_code) != symbol_table_.end()) {
      entry = symbol_table_[*f_code];
    } else if (*f_code == symbol_table_.size()) {
      entry = code_word + code_word[0];
    }

    out_file << entry;
    symbol_table_[code++] = code_word + entry[0];
    code_word = entry;
    f_code++;
  }

  out_file.close();
  delete[] buffer;
}