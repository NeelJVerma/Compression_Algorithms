#include "cmdlineparser.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

CmdLineParser::CmdLineParser(int argc, char* argv[]) {
  if (!ValidateInput(argc, argv)) {
    std::exit(EXIT_FAILURE);
  }

  filename_ = argv[1];
  compress_ = argv[2][1] == 'c' ? true : false;
}

std::string CmdLineParser::Filename() const {
  return filename_;
}

bool CmdLineParser::IsCompress() const {
  return compress_;
}

bool CmdLineParser::ValidateInput(int argc, char* argv[]) const {
  if (argc != 3) {
    std::cout << "Format must be <filename> -<c/d>" << std::endl;
    return false;
  }

  if ((strlen(argv[2]) > 2 || argv[2][0] != '-') ||
      (argv[2][1] != 'c' && argv[2][1] != 'd')) {
    std::cout << "Second arguement must be -<c/d>" << std::endl;
    return false;
  }

  return true;
}