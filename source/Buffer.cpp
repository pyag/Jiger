#include "../headers/Buffer.h"
#include "../headers/FileOperations.h"
#include <iostream>

Buffer::Buffer (std::string &loc) {
  this->fileLoc = loc;
  this->lineSource.clear();
  this->lineStartsWith.clear();
}

void Buffer::loadBuffer () {
  std::string original = readFile(this->fileLoc);

  std::string lineText;
  for (int i = 0; i < original.length(); i++) {
    lineText += original[i];
    if (original[i] == '\n') {
      lineSource.push_back(lineText);
      lineText = "";
    }
  }

  this->syncLineBreaks();
}

void Buffer::syncLineBreaks () {
  this->lineStartsWith.clear();

  this->lineStartsWith.push_back(0);
  int runningIndex = 0;
  for (int i = 0; i < (int) this->lineSource.size(); i++) {
    runningIndex += this->lineSource[i].length();
    this->lineStartsWith.push_back(runningIndex);
  }
}

void Buffer::insert (std::string &add, int pos) {
  int line = this->getLineNumberByPos(pos);
  int startIndex = this->lineStartsWith[line];
  int lineLength = this->lineSource[line].length();

  int addOffset = pos - startIndex;

  std::string subString1 = this->lineSource[line].substr(0, addOffset);
  std::string subString2 = this->lineSource[line].substr(addOffset, lineLength - addOffset);

  this->lineSource.erase(this->lineSource.begin() + line);

  for (int i = 0; i < add.length(); i++) {
    subString1 += add[i];
    if (add[i] == '\n') {
      this->lineSource.insert(this->lineSource.begin() + line, subString1);
      ++line;
      subString1 = "";
    }
  }

  this->lineSource.insert(this->lineSource.begin() + line, subString1 + subString2);
  this->syncLineBreaks();
}

void Buffer::remove (int from, int to) {
  if (from >= to) return;

  int sourceLength = this->getBufferLength();
  from = from < 0 ? 0 : from;
  to = to >= sourceLength ? sourceLength : to;

  int line1 = this->getLineNumberByPos(from);
  int line2 = this->getLineNumberByPos(to);

  int line1Start = this->lineStartsWith[line1];
  int line2Start = this->lineStartsWith[line2];
  int line2Length = this->lineSource[line2].length();

  int line1Offset = from - line1Start;
  int line2Offset = to - line2Start;

  std::string subString1 = this->lineSource[line1].substr(0, line1Offset);
  std::string subString2 = this->lineSource[line2].substr(line2Offset, line2Length - line2Offset);

  while (line1 <= line2) {
    this->lineSource.erase(this->lineSource.begin() + line1);
    line2--;
  }

  this->lineSource.insert(this->lineSource.begin() + line1, subString1 + subString2);

  this->syncLineBreaks();
}

std::string Buffer::getSource () {
  std::string source;
  for (int i = 0; i < this->lineSource.size(); i++)
    source += this->lineSource[i];

  return source;
}

int Buffer::getBufferLength () {
  int codeLength = 0;
  for (int i = 0; i < (int) this->lineSource.size(); i++)
    codeLength += this->lineSource[i].length();

  return codeLength;
}

int Buffer::getLineNumberByPos (int pos) {
  int runningIndex = 0;
  for (int i = 0; i < (int) this->lineSource.size(); i++) {
    if (pos < runningIndex) return i - 1;
    runningIndex += this->lineSource[i].length();
  }

  return (int) this->lineSource.size() - 1;
}

char Buffer::getCharAtPos (int pos) {
  if (!this->empty() && pos >= 0 && pos < this->getBufferLength()) {
    int line = this->getLineNumberByPos(pos);
    int reqIndex = pos - this->lineStartsWith[line];
    return this->lineSource[line][reqIndex];
  }

  return '\0';
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
  return (int) this->lineSource.size();
}

std::string Buffer::getSourceByPositions (int from, int to) {
  if (from > to) std::swap(from, to);

  int line1 = this->getLineNumberByPos(from);
  int line2 = this->getLineNumberByPos(to);

  int line1Start = this->lineStartsWith[line1];
  int line2Start = this->lineStartsWith[line2];

  int line1Offset = from - line1Start;
  int line2Offset = to - line2Start;

  std::string subString = this->lineSource[line1].substr(line1Start, line1Offset - line1Start);
  for (int i = line1 + 1; i < line2; i++) {
    subString += this->lineSource[i];
  }
  subString += this->lineSource[line2].substr(line2Start, line2Offset - line2Start);

  return subString;
}

bool Buffer::empty () {
  return ((int) this->lineSource.size() == 0);
}

void Buffer::write () {
  writeFile(this->fileLoc, this->getSource());
}

void Buffer::clearBuffer () {
  // Clear the string and relaim its memory
  std::vector < std::string >().swap(this->lineSource);
}

void Buffer::cleanUp () {
  this->clearBuffer();
  std::vector <int> ().swap(this->lineStartsWith);
}

Buffer::~Buffer () {
  this->cleanUp();
}
