#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#ifndef EDITOR_SPACE
#define EDITOR_SPACE

class EditorSpace {
private:
  std::string bufferText;
  int cursorIndex;

public:
  EditorSpace(std::string &);
  std::string getBufferText();
  void pollUserEvents(sf::Event &);
  void pollKeyboard(int);
  void pollMouse();
};

#endif
