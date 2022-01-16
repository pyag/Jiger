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
  float desktopWidth = sf::VideoMode::getDesktopMode().width;
  float desktopHeight = sf::VideoMode::getDesktopMode().height;

  float editorWidth = desktopWidth - globalEditorConfig.getEditorXPos();
  float editorHeight = desktopHeight - globalEditorConfig.getEditorYPos();

  sf::RenderWindow window(sf::VideoMode(desktopWidth, desktopHeight), "Jiger");

  globalEditorConfig.setEditorXSize(editorWidth);
  globalEditorConfig.setEditorYSize(editorHeight);

  EditorSpace editor(bufferText, globalEditorConfig, &window);

  editor.setPosition(
    globalEditorConfig.getEditorXPos(),
    globalEditorConfig.getEditorYPos()
  );

  editor.setSize(window.getSize().x, window.getSize().y);
  editor.fillColor(bgColor);

  sf::View editorView = window.getView();

  editorView.reset(sf::FloatRect(
    globalEditorConfig.getEditorXPos(),
    globalEditorConfig.getEditorYPos(),
    globalEditorConfig.getEditorXSize(),
    globalEditorConfig.getEditorYSize()
  ));

  editorView.setViewport(sf::FloatRect(
    globalEditorConfig.getEditorXPos() / window.getSize().x,
    globalEditorConfig.getEditorYPos() / window.getSize().y,
    globalEditorConfig.getEditorXSize() / window.getSize().x,
    globalEditorConfig.getEditorYSize() / window.getSize().y
  ));

  editor.setWatchableView(editorView);
  window.setView(editorView);

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
