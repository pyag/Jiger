#ifndef BUFFER
#define BUFFER

#include <string>
#include <vector>

// For now it is array of strings per line from source
class Buffer {
public:
  std::string fileLoc;

  std::vector <std::string> lineSource;
  std::vector <int> lineStartsWith;

  Buffer (std::string &);
  void loadBuffer ();
  void syncLineBreaks ();

  void insert (std::string &, int);
  void remove (int, int);
  std::string getSource ();

  int getBufferLength ();
  int getLineNumberByPos (int);
  char getCharAtPos (int);
  int getLineStartPos (int);
  int getLineCount();
  std::string getSourceByPositions (int, int);
  bool empty ();

  void write ();
  void clearBuffer();

  void cleanUp ();
  ~Buffer ();

};

#endif
