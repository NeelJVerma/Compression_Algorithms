/** 
  * Name: Neel Verma
  * Current File: cmdlineparser.cc
  * Description: Implements the command line parser.
  */

#include "cmdlineparser.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

/**
  * Description: Constructor for the CmdLineParser class.
  * Parameters: int argc --> Number of command line arguements.
  *             char* argv[] --> Array of command line arguements.
  * Returns: Nothing.
  */

CmdLineParser::CmdLineParser(int argc, char* argv[]) {
  if (!ValidateInput(argc, argv)) {
    std::exit(EXIT_FAILURE);
  }

  filename_ = argv[1];
  compress_ = argv[2][1] == 'c' ? true : false;
}

/**
  * Description: Gets the filename to be used in the encoder/decoder.
  * Parameters: None.
  * Returns: The filename.
  */

std::string CmdLineParser::Filename() const {
  return filename_;
}

/**
  * Description: Tells whether the -<c/d> option was -c or not, which is used
  * when testing.
  * Parameters: None.
  * Returns: Whehter the option was set to -c or not.
  */

bool CmdLineParser::IsCompress() const {
  return compress_;
}

/**
  * Description: Validates the command line input.
  * Parameters: int argc --> Number of command line arguements.
  *             char* argv[] --> Array of command line arguements.
  * Returns: Whether the input was valid or not.
  */

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

  std::string argv_str(argv[1]);
  std::size_t extension_idx = argv_str.find('.');

  if (argv_str[extension_idx - 1] == argv[2][1]) {
    std::cout << "Cannot compress/decompress a compressed/decomressed file" << std::endl;
    return false;
  }

  return true;
}