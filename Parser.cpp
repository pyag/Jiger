#include "headers/Parser.h"

Parser::Parser () {
  this->curIndex = 0;
  this->startIndex = 0;
  this->endIndex = 0;
}

Parser::Parser (Buffer *buffer) {
  this->curIndex = 0;
  this->startIndex = 0;
  this->buffer = buffer;
  this->endIndex = this->buffer->getBufferLength();
}

void Parser::loadBuffer (Buffer *buffer) {
  this->curIndex = 0;
  this->startIndex = 0;
  this->buffer = buffer;
  this->endIndex = this->buffer->getBufferLength();
}

void Parser::loadBuffer (Buffer *buffer, int start, int end) {
  this->curIndex = start;
  this->startIndex = start;
  this->endIndex = end;
  this->buffer = buffer;
}

bool Parser::hasNextToken () {
  return (this->curIndex >= this->startIndex && this->curIndex < this->endIndex);
}

std::string Parser::getToken () {
  std::string token;
  bool isAlphabet = false;
  int index = this->curIndex;
  char ch;
  
  if (index < this->endIndex) {
    ch = this->buffer->getCharAtPos(index);
    while((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '#')) {
      token += ch;
      ++index;
      ch = this->buffer->getCharAtPos(index);
      isAlphabet = true;
    }

    this->curIndex = index;
    if (isAlphabet) {
      return token;
    }

    token = this->buffer->getCharAtPos(index);
    index++;
  }
  
  this->curIndex = index;
  return token;
}
