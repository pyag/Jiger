#ifndef EDITOR_SPACE
#define EDITOR_SPACE

#include <iostream>
#include <cstdlib>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Buffer.h"
#include "Div.h"
#include "Parser.h"
#include "GlobalConfigs.h"
#include "langHighlight/highlight.h"

class EditorSpace: public Div {
public:
  Buffer *buf;
  std::string fileLoc;

  int cursorIndex, curLine;
  // Don't worry ProgLang is just an enum
  ProgLang langSelected;

  Div *cursor;
  bool showCursor;
  sf::Clock clock;
  bool hideLineNumber;

  GlobalConfig *config;

  // Onclick event attribes
  int selectionStartIndex, selectionEndIndex;
  bool leftMouseClicked;

  //Flags for KeyPress states
  bool isPasted, isCopied, isSaved;

  EditorSpace (std::string &, GlobalConfig *, sf::RenderWindow *);
  void setLanguage (ProgLang);
  void pollUserEvents (sf::Event &);
  void pollKeyboard (int);
  void drawOnScreen ();
  float getXTextOffset ();
  void displayLineNumber (int, int);
  int getTotalLineCount ();

  void loadEditorConfigs ();
  void updateEditorSize ();

  void save ();

  void scrollUpByLines (int);
  void scrollDownByLines (int);

  int getCursorIndexByLocation(float, float);

  int getTopLineByView (sf::View &);
  int getBottomLineByView (sf::View &);

  bool isAnythingSelected ();
  bool insideSelectionArea (int);
  void selectIfRequired (int, float, float);
  void removeSelection ();

  int getSelectionLower ();
  int getSelectionHigher ();

};

#endif
