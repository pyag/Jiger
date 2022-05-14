#include "headers/Buffer.h"
#include "headers/FileOperations.h"
#include <iostream>

Buffer::Buffer (std::string &loc) {
  this->fileLoc = loc;
  this->source = "";
  this->lineStartsWith.clear();
}

void Buffer::loadBuffer () {
  this->source = readFile(this->fileLoc);
  this->syncLineBreaks();
}

void Buffer::syncLineBreaks () {
  this->lineStartsWith.clear();
  this->lineStartsWith.push_back(0);
  for (int i = 0; i < this->source.length(); i++) {
    if (this->source[i] == '\n') {
      this->lineStartsWith.push_back(i + 1);
    }
  }
}

void Buffer::insert (std::string &add, int pos) {
  int sourceLen = this->getBufferLength();

  if (this->source.empty()) {
    this->source = add;
    return;
  }

  this->source = 
    this->source.substr(0, pos)
    + add
    + this->source.substr(pos, sourceLen);

  this->syncLineBreaks();
}

void Buffer::remove (int from, int to) {
  int sourceLen = this->getBufferLength();

  if (from >= to) return;

  from = from < 0 ? 0 : from;
  to = to >= sourceLen ? sourceLen : to;

  this->source =
    this->source.substr(0, from) +
    (to == sourceLen ? "" : this->source.substr(to, sourceLen));

  this->syncLineBreaks();
}

std::string & Buffer::getSource () {
  return this->source;
}

int Buffer::getBufferLength () {
  return this->source.length();
}

int Buffer::getLineNumberByPos (int pos) {
  int line = 0;
  for (int i = 0; i < this->source.length(); i++) {
    if (i == pos) return line;
    if (this->source[i] == '\n') line++;
  }

  return line;
}

char Buffer::getCharAtPos (int pos) {
  if (!this->source.empty() && pos >= 0 && pos < this->source.length())
    return this->source[pos];

  char empty = '\0';
  return empty;
}

int Buffer::getLineStartPos (int line) {
  if (line >= this->getLineCount()) {
    return this->lineStartsWith[(int)this->lineStartsWith.size() - 1];
  }

  if (line < 0) {
    return this->lineStartsWith[0];
  }

  return this->lineStartsWith[line];
}

int Buffer::getLineCount () {
  return (int)this->lineStartsWith.size();
}

std::string Buffer::getSourceByPositions (int from, int to) {
  if (from > to) std::swap(from, to);
  return this->source.substr(from, to - from);
}

bool Buffer::empty () {
  return this->source.empty();
}

void Buffer::write () {
  writeFile(this->fileLoc, this->source);
}

void Buffer::clearBuffer () {
  // Clear the string and relaim its memory
  std::string().swap(this->source);
}

void Buffer::cleanUp () {
  this->clearBuffer();
  std::vector <int> ().swap(this->lineStartsWith);
}

Buffer::~Buffer () {
  this->cleanUp();
}
