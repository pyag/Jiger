#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "EditorSpace.h"
#include "FileOperations.h"

sf::Color bgColor(18, 18, 18);
sf::Color fontColor(235, 238, 235);
sf::Font editorFont;
sf::Text buffer;

int fontSize;
std::string bufferText;

int main() {
  if (!editorFont.loadFromFile("./fonts/consolas/consola.ttf")) {
    std::cout << "Font cannot be loaded!\n";
    return 1;
  }

  fontSize = 18;
  buffer.setFont(editorFont);
  buffer.setCharacterSize(fontSize);
  buffer.setColor(fontColor);

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

    window.clear(bgColor);
    window.draw(buffer);
    window.display();
  }

  return 0;
}
