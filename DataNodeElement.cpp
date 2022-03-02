#include <iostream>

#include "DataNodeElement.h"

DataNodeElement::DataNodeElement (DataNode *dn, GlobalConfig *cfg, sf::RenderWindow *window): Div(window) {
  this->isExpanded = false;
  this->dn = dn;
  this->config = cfg;

  this->setFileTextStyle();
}

void DataNodeElement::pollUserEvents (sf::Event &event) {
  Div::pollEvents(event);
}

void DataNodeElement::onHover (sf::View *view) {
  if (Div::onhover(view)) {
    this->fillColor(sf::Color(60, 60, 60, 100));
  } else {
    this->fillColor(sf::Color(
      this->config->getExplorerColor().r,
      this->config->getExplorerColor().g,
      this->config->getExplorerColor().b
    ));
  }
}

void DataNodeElement::drawOnScreen () {
  Div::drawOnScreen();

  float leftXPadding = 25.0f;
  float heightBuf = 2.0f;

  this->fileText.setString(this->dn->filename);

  this->fileText.setPosition(sf::Vector2f(
    this->getPosition().x + leftXPadding,
    this->getPosition().y + heightBuf
  ));

  this->getWindow()->draw(this->fileText);
}

void DataNodeElement::setFileTextStyle () {
  sf::Color fontColor(
    this->config->getExplorerFontColor().r,
    this->config->getExplorerFontColor().g,
    this->config->getExplorerFontColor().b
  );

  this->fileText.setFont(this->config->getExplorerFont());
  this->fileText.setCharacterSize(this->config->getExplorerFontSize());
  this->fileText.setFillColor(fontColor);
}
