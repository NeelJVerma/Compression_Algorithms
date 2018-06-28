/** 
  * Name: Neel Verma
  * Current File: cmdlineparser.h
  * Description: Parses the command line. Used for all compression algorithms
  * that have been and will be implemented.
  */

#ifndef _CMD_LINE_PARSER_H_
#define _CMD_LINE_PARSER_H_

#include <string>

class CmdLineParser {
 public:
  CmdLineParser(int argc, char* argv[]);
  std::string Filename() const;
  bool IsCompress() const;

 private:
  std::string filename_;
  bool compress_;
  bool ValidateInput(int argc, char* argv[]) const;
};

#endif