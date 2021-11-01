#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "EditorSpace.h"
#include "FileOperations.h"

sf::Color bgColor(18, 18, 18);
sf::Color blue(0, 0, 255);

std::string bufferText;

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Jiger");
  EditorSpace editor(bufferText);
  editor.setPosition(10.0f, 10.0f);
  editor.setSize(700.0f, 200.0f);
  editor.fillColor(blue);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      // Editor Space/Box polling events.
      editor.pollUserEvents(event);
    }

    window.clear(bgColor);
    editor.drawOnScreen(window);
    window.display();
  }

  return 0;
}
