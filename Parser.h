#include <string>
#include <iostream>

#ifndef PARSER
#define PARSER

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
