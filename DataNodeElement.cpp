#include <iostream>

#include "DataNodeElement.h"

DataNodeElement::DataNodeElement (DataNode *dn, GlobalConfig *cfg, sf::RenderWindow *window): Div(window) {
  this->dn = dn;
  this->config = cfg;
}

void DataNodeElement::pollUserEvents (sf::Event &event) {
  Div::pollEvents(event);
}

void DataNodeElement::onHover (sf::View *view) {
  if (Div::onhover(view)) {
    this->fillColor(sf::Color(60, 60, 60));
  } else {
    this->fillColor(sf::Color(
      this->config->getExplorerColor().r,
      this->config->getExplorerColor().g,
      this->config->getExplorerColor().b
    ));
  }
}
