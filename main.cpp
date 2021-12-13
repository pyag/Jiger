#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "EditorSpace.h"
#include "FileOperations.h"
#include "GlobalConfigs.h"


std::string bufferText;
GlobalConfig globalEditorConfig;

sf::Color bgColor(
  globalEditorConfig.getBgColor().r,
  globalEditorConfig.getBgColor().g,
  globalEditorConfig.getBgColor().b
);
sf::Color blue(0, 0, 255);

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Jiger");
  EditorSpace editor(bufferText, globalEditorConfig);
  editor.setPosition(10.0f, 10.0f);
  editor.setSize(700.0f, 200.0f);
  editor.fillColor(bgColor);

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
