#include <iostream>

#include "DataNodeElement.h"

DataNodeElement::DataNodeElement (DataNode *dn, GlobalConfig &cfg, sf::RenderWindow *window): Div(window) {
  // this->parentExplorer = exp;
  this->dn = dn;
  this->mousePressedState = false;

  this->config = cfg;
}

void DataNodeElement::pollUserEvents (sf::Event &event) {
  Div::pollEvents(event);
  this->onHover();
}

void DataNodeElement::onHover () {

  float mouseX = (float)sf::Mouse::getPosition(*Div::getWindow()).x;
  float mouseY = (float)sf::Mouse::getPosition(*Div::getWindow()).y;

  if (Div::mouseInMyArea(mouseX, mouseY)) {
    this->fillColor(sf::Color(60, 60, 60));
  } else {
    this->fillColor(sf::Color(
      this->config.getExplorerColor().r,
      this->config.getExplorerColor().g,
      this->config.getExplorerColor().b
    ));
  }

}

bool DataNodeElement::onClick (sf::Event &event) {
  sf::Vector2i mPos = sf::Mouse::getPosition(*Div::getWindow());

  if (event.type == sf::Event::MouseButtonPressed) {
    if (!this->mouseInMyArea(mPos.x, mPos.y)) return false;

    if (this->mousePressedState == false) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        this->mousePressedState = true;
      }
    }

    return false;
  }

  if (event.type == sf::Event::MouseButtonReleased) {
    this->mousePressedState = false;

    if (!this->mouseInMyArea(mPos.x, mPos.y)) {
      return false;
    };

    return true;
  }

  return false;
}
