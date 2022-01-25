#ifndef PARSER
#define PARSER

#include <string>
#include <iostream>

class Parser {
public:
  std::string buffer;
  int curIndex;

  Parser();
  Parser(std::string &);

  void loadBuffer(std::string &);
  bool hasNextToken();
  std::string getToken();
};

#endif
