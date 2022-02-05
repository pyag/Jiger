#ifndef PARSER
#define PARSER

#include <string>
#include <iostream>

class Parser {
public:
  std::string buffer;
  int curIndex;
  int startIndex, endIndex;

  Parser ();
  Parser (const std::string &);

  void loadBuffer (const std::string &);
  void loadBuffer (const std::string &, int, int);
  bool hasNextToken ();
  std::string getToken ();
};

#endif
