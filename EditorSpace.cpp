#include "EditorSpace.h"

EditorSpace::EditorSpace (std::string &bufferText) {
  this->bufferText = bufferText;
  this->cursorIndex = 0;
}

void EditorSpace::pollKeyboard (int unicode) {
  if (unicode == 8) {
    this->bufferText = this->bufferText.substr(0, this->bufferText.length() - 1);
    return;
  }

  if (unicode == 10) {
    this->bufferText += "\n";
    return;
  }

  if (unicode >= 32 && unicode <= 126) {
    this->bufferText += (char)(unicode);
    return;
  }

  return;
}

void EditorSpace::pollMouse () {
  return;
}

void EditorSpace::pollUserEvents (sf::Event &event) {
  if (event.type == sf::Event::TextEntered) {
    this->pollKeyboard(event.text.unicode);  
  }

  this->pollMouse();
  return;
}

std::string EditorSpace::getBufferText () {
  return this->bufferText;
}

void EditorSpace::drawOnScreen (sf::RenderWindow &window) {
  Div::drawOnScreen(window);

  sf::Color fontColor(235, 238, 235);
  sf::Font editorFont;
  sf::Text word;

  int fontSize;

  if (!editorFont.loadFromFile("./fonts/consolas/consola.ttf")) {
    std::cout << "Font cannot be loaded!\n";
    exit(1);
  }

  fontSize = 18;
  word.setFont(editorFont);
  word.setCharacterSize(fontSize);
  word.setColor(fontColor);

  sf::Vector2f parentDivPos = Div::getPosition();

  for (int i = 0; i < this->bufferText.length(); i++) {
    word.setString(this->bufferText[i]);
    word.setPosition(sf::Vector2f(parentDivPos.x + (float)i * 10.0f, parentDivPos.y));
    window.draw(word);
  }

  // buffer.setString(this->bufferText);
  // window.draw(buffer);
}
