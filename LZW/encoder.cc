#include "encoder.h"
#include <regex>
#include <iostream>

const unsigned kMaxChars = 256;

Encoder::Encoder(const std::string& filename) {
  in_file_.open(filename);

  if (!in_file_) {
    std::cout << filename << " could not be opened" << std::endl;
    in_file_.close();
    std::exit(EXIT_FAILURE);
  }

  filename_ = filename;
}

Encoder::~Encoder() {
  in_file_.close();
}

void Encoder::CreateOutFile(std::ofstream& out_file) {
  std::regex e(".*\\/(.*)\\..*$");
  std::smatch sm;
  std::regex_match(filename_.cbegin(), filename_.cend(), sm, e);
  std::string out_filename = "../Encoded/" + std::string(sm[1]) + "-c.lzw";
  out_file.open(out_filename);

  if (!out_file) {
    std::cout << "Could not create compressed file" << std::endl;
    out_file.close();
    in_file_.close();
    std::exit(EXIT_FAILURE);
  }
}

void Encoder::FillSymbolTable() {
  for (int i = 0; i < kMaxChars; i++) {
    symbol_table_[std::string(1, char(i))] = i;
  }
}

void Encoder::Encode() {
  FillSymbolTable();
  unsigned code = kMaxChars;
  in_file_.seekg(0, in_file_.end);
  unsigned file_size = in_file_.tellg();
  in_file_.seekg(0, in_file_.beg);
  char* buffer = new char[file_size];
  in_file_.read(buffer, file_size);
  std::ofstream out_file;
  CreateOutFile(out_file);
  std::string code_word(1, buffer[0]);

  for (int i = 1; i < file_size; i++) {
    char c = buffer[i];

    if (symbol_table_.find(code_word + c) != symbol_table_.end()) {
      code_word += c;
    } else {
      unsigned st_code = symbol_table_[code_word];
      out_file.write((char*)&st_code, sizeof(st_code));
      symbol_table_[code_word + c] = code++;
      code_word = std::string(1, c);
    }
  }

  if (code_word.length()) {
    unsigned st_code = symbol_table_[code_word];
    out_file.write((char*)&st_code, sizeof(st_code));
  }

  out_file.close();
  delete[] buffer;
}