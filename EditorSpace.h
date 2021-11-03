#include <iostream>
#include <cstdlib>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Div.h"
#include "Parser.h"

#ifndef EDITOR_SPACE
#define EDITOR_SPACE

class EditorSpace: public Div {
public:
  std::string bufferText;
  int cursorIndex;

  Div cursor;
  bool showCursor;
  sf::Clock clock;

  EditorSpace(std::string &);
  std::string getBufferText();
  void pollUserEvents(sf::Event &);
  void pollKeyboard(int);
  void pollMouse();
  void drawOnScreen(sf::RenderWindow &);
};

#endif
