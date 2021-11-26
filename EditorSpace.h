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

  GlobalConfig config;

  EditorSpace(std::string &, GlobalConfig &);
  std::string getBufferText();
  void setLanguage(ProgLang);
  void pollUserEvents(sf::Event &);
  void pollKeyboard(int);
  void pollMouse();
  void drawOnScreen(sf::RenderWindow &);
};

#endif
