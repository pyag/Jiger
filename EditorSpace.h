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

  Div cursor;
  bool showCursor;
  sf::Clock clock;
  bool hideLineNumber;

  GlobalConfig config;

  sf::View watchableView;

  EditorSpace(std::string &, GlobalConfig &, sf::RenderWindow *);
  void setLanguage(ProgLang);
  void pollUserEvents(sf::Event &);
  void pollKeyboard(int);
  void drawOnScreen(sf::RenderWindow &);
  float getXTextOffset();
  void displayLineNumber(int, int);
  int getTotalLineCount();

  void setWatchableView (sf::View &);
  sf::View &getWatchableView ();

  void loadEditorConfigs();
  void updateEditorSize();

  void scrollUpByLines(int);
  void scrollDownByLines(int);
};

#endif
