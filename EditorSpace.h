#include <iostream>
#include <cstdlib>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Div.h"
#include "Parser.h"
#include "GlobalConfigs.h"
#include "langHighlight/highlight.h"

#ifndef EDITOR_SPACE
#define EDITOR_SPACE

class EditorSpace: public Div {
public:
  std::string bufferText;
  int cursorIndex;
  int curLine, wordsInLineBeforeCursor;
  std::vector <int> wordsInLine;
  // Don't worry ProgLang is just an enum
  ProgLang langSelected;

  Div cursor;
  bool showCursor;
  sf::Clock clock;
  bool hideLineNumber;

  GlobalConfig config;

  sf::View watchableView;

  EditorSpace(std::string &, GlobalConfig &, sf::RenderWindow *);
  std::string getBufferText();
  void setLanguage(ProgLang);
  void pollUserEvents(sf::Event &);
  void pollKeyboard(int);
  void drawOnScreen(sf::RenderWindow &);
  float getXTextOffset();
  void displayLineNumber(int);
  int getTotalLineCount();

  void setWatchableView (sf::View &);
  sf::View &getWatchableView ();

  void loadEditorConfigs();
};

#endif
