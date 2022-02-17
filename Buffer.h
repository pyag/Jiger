#ifndef BUFFER
#define BUFFER

#include <string>
#include <vector>

// For now it is only all long string from source
class Buffer {
public:
  std::string source;
  std::string fileLoc;

  std::vector <int> lineStartsWith;

  Buffer (std::string &);
  void loadBuffer ();
  void syncLineBreaks ();

  void insert (std::string &, int);
  void remove (int, int);
  std::string & getSource ();

  int getBufferLength ();
  int getLineNumberByPos (int);
  char getCharAtPos (int);
  int getLineStartPos (int);
  int getLineCount();
  std::string getSourceByPositions (int, int);
  bool empty ();

  void write ();
};

#endif
