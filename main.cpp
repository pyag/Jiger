#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "EditorSpace.h"

sf::Color bgColor(18, 18, 18);
sf::Font editorFont;
sf::Text buffer;

int fontSize;
std::string bufferText;

void keyPressEvents (int unicode) {
  if (unicode == 8) {
    bufferText = bufferText.substr(0, bufferText.length() - 1);
    return;
  }

  if (unicode == 10) {
    bufferText += "\n";
    return;
  }

  if (unicode >= 32 && unicode <= 126) {
    bufferText += (char)(unicode);
    return;
  }

  return;
}

int main() {
  if (!editorFont.loadFromFile("./fonts/consolas/consola.ttf")) {
    std::cout << "Font cannot be loaded!\n";
    return 1;
  }

  fontSize = 18;
  buffer.setFont(editorFont);

  sf::RenderWindow window(sf::VideoMode(800, 600), "Jiger");
  EditorSpace editor(bufferText);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      /***
       * Editor Space/Box polling events.
       **/
      editor.pollUserEvents(event);
    }

    buffer.setString(editor.getBufferText());
    buffer.setCharacterSize(fontSize);

    window.clear(bgColor);
    window.draw(buffer);
    window.display();
  }

  return 0;
}
