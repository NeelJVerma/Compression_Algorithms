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