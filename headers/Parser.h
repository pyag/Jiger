#ifndef PARSER
#define PARSER

#include <string>
#include <iostream>

#include "Buffer.h"

class Parser {
public:
  Buffer *buffer;
  int curIndex;
  int startIndex, endIndex;

  Parser ();
  Parser (Buffer *);

  void loadBuffer (Buffer *);
  void loadBuffer (Buffer *, int, int);
  bool hasNextToken ();
  std::string getToken ();
};

#endif
