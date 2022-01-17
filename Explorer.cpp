#include "Explorer.h"

Explorer::Explorer (GlobalConfig &config, sf::RenderWindow *window): Div(window) {
  this->config = config;
}

void Explorer::pollUserEvents (sf::Event &event) {
  Div::pollEvents(event);

    // Resizing Div on window
  if (event.type == sf::Event::Resized) {
    // Adjust editor size
    float newExplorerWidth = this->config.getEditorXPos();
    float newExplorerHeight = event.size.height;

    this->config.setExplorerXSize(newExplorerWidth);
    this->config.setExplorerYSize(newExplorerHeight);

    this->setSize(newExplorerWidth, newExplorerHeight);

    // Adjusting Div view
    sf::View explorerView = this->getWatchableView();

    explorerView.reset(sf::FloatRect(
      this->config.getExplorerXPos(),
      this->config.getExplorerYPos(),
      this->config.getExplorerXSize(),
      this->config.getExplorerYSize()
    ));

    explorerView.setViewport(sf::FloatRect(
      this->config.getExplorerXPos() / event.size.width,
      this->config.getExplorerYPos() / event.size.height,
      this->config.getExplorerXSize() / event.size.width,
      this->config.getExplorerYSize() / event.size.height
    ));

    this->setWatchableView(explorerView);
    return;
  }


  // Mouse Scroll Events on Explorer
  if (event.type == sf::Event::MouseWheelScrolled) {
    // For now do nothing
    return;
  }
}


void Explorer::setWatchableView (sf::View &view) {
  this->watchableView = view;
}

sf::View &Explorer::getWatchableView () {
  return this->watchableView;
}
