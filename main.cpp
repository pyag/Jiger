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

  // Fonts are not clear when opening app in desktop width mode
  // Everything scales well after resizing, but strangely does
  // not work well when opening in full width and height
  // So for now, opening the editor in small width and height
  desktopWidth *= 0.7f;
  desktopHeight *= 0.7f;

  globalEditorConfig.setEditorXSize(editorWidth);
  globalEditorConfig.setEditorYSize(editorHeight);

  sf::RenderWindow window(sf::VideoMode(desktopWidth, desktopHeight), "Jiger");

  EditorSpace editor(bufferText, globalEditorConfig, &window);

  editor.setPosition(
    globalEditorConfig.getEditorXPos(),
    globalEditorConfig.getEditorYPos()
  );

  editor.setSize(
    globalEditorConfig.getEditorXSize(),
    globalEditorConfig.getEditorYSize()
  );

  editor.fillColor(bgColor);

  sf::View editorView = window.getView();

  editorView.reset(sf::FloatRect(
    globalEditorConfig.getEditorXPos(),
    globalEditorConfig.getEditorYPos(),
    globalEditorConfig.getEditorXSize(),
    globalEditorConfig.getEditorYSize()
  ));

  editorView.setViewport(sf::FloatRect(
    globalEditorConfig.getEditorXPos() / desktopWidth,
    globalEditorConfig.getEditorYPos() / desktopHeight,
    globalEditorConfig.getEditorXSize() / desktopWidth,
    globalEditorConfig.getEditorYSize() / desktopHeight
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
