#include "Parser.h"

Parser::Parser () {
  this->curIndex = 0;
}

Parser::Parser (std::string &buffer) {
  this->curIndex = 0;
  this->buffer = buffer;
}

void Parser::loadBuffer (std::string &buffer) {
  this->buffer = buffer;
  this->curIndex = 0;
}

bool Parser::hasNextToken () {
  return this->curIndex < this->buffer.length();
}

std::string Parser::getToken () {
  std::string token;
  bool isAlphabet = false;
  int index = this->curIndex;
  
  if (index < this->buffer.length()) {
    while((this->buffer[index] >= 'a' && this->buffer[index] <= 'z') 
      || (this->buffer[index] >= 'A' && this->buffer[index] <= 'Z')) {
      token += this->buffer[index++];
      isAlphabet = true;
    }

    this->curIndex = index;
    if (isAlphabet) {
      return token;
    }

    token = this->buffer[index++];
  }
  
  this->curIndex = index;
  return token;
}