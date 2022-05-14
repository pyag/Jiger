#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "headers/EditorSpace.h"
#include "headers/Explorer.h"
#include "headers/FileOperations.h"
#include "headers/GlobalConfigs.h"

std::string bufferText;
GlobalConfig globalEditorConfig;

sf::Color bgColor(
  globalEditorConfig.getBgColor().r,
  globalEditorConfig.getBgColor().g,
  globalEditorConfig.getBgColor().b
);

sf::Color blue(0, 0, 255);

sf::Color tabTrayBgColor (
  globalEditorConfig.getTabTrayBgColor().r,
  globalEditorConfig.getTabTrayBgColor().g,
  globalEditorConfig.getTabTrayBgColor().b
);

sf::Color explorerColor(
  globalEditorConfig.getExplorerColor().r,
  globalEditorConfig.getExplorerColor().g,
  globalEditorConfig.getExplorerColor().b
);

int main() {
  float desktopWidth = sf::VideoMode::getDesktopMode().width;
  float desktopHeight = sf::VideoMode::getDesktopMode().height;

  // Fonts are not clear when opening app in desktop width mode
  // Everything scales well after resizing, but strangely does
  // not work well when opening in full width and height
  // So for now, opening the editor in small width and height
  desktopWidth *= 0.7f;
  desktopHeight *= 0.7f;

  float editorWidth = desktopWidth - globalEditorConfig.getEditorXPos();
  float editorHeight = desktopHeight - globalEditorConfig.getEditorYPos();

  globalEditorConfig.setEditorXSize(editorWidth);
  globalEditorConfig.setEditorYSize(editorHeight);

  sf::RenderWindow window(sf::VideoMode(desktopWidth, desktopHeight), "Jiger");

  globalEditorConfig.setExplorerXSize(globalEditorConfig.getEditorXPos());
  globalEditorConfig.setExplorerYSize(desktopHeight);

  /** Explorer configuration starts **/

  Explorer explorer(&globalEditorConfig, &window);

  explorer.setPosition(
    globalEditorConfig.getExplorerXPos(),
    globalEditorConfig.getExplorerYPos()
  );

  explorer.setSize(
    globalEditorConfig.getExplorerXSize(),
    globalEditorConfig.getExplorerYSize()
  );

  explorer.adjustView(desktopWidth, desktopHeight);

  /** Explorer configuration ends **/

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      // Editor Space/Box polling events.
      if (explorer.isAnyEditorActive()) {
        int id = explorer.activeDataNodeId;
        explorer.openEditors[id]->pollUserEvents(event);
      }

      explorer.pollUserEvents(event);
      explorer.tabTray->pollUserEvents(event);
    }

    window.clear(bgColor);

    if (explorer.isAnyEditorActive()) {
      int id = explorer.activeDataNodeId;
      window.setView(explorer.openEditors[id]->getWatchableView());
      explorer.openEditors[id]->fillColor(bgColor);
      explorer.openEditors[id]->drawOnScreen();
    }

    window.setView(explorer.getWatchableView());
    explorer.fillColor(explorerColor);
    explorer.drawOnScreen();

    window.setView(explorer.tabTray->getWatchableView());
    explorer.tabTray->fillColor(tabTrayBgColor);
    if (explorer.tabTray->empty()) {
      explorer.tabTray->fillColor(bgColor);
    }
    explorer.tabTray->drawOnScreen();

    window.display();
  }

  return 0;
}
