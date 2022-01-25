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

  sf::Vector2i mPos = sf::Mouse::getPosition(*this->getWindow());

  if (event.type == sf::Event::MouseButtonPressed) {
    if (!this->mouseInMyArea(mPos.x, mPos.y)) return;

    if (this->mousePressedState == false) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        this->mousePressedState = true;
        return;
      }
    }

    return;
  }

  if (event.type == sf::Event::MouseButtonReleased) {
    this->mousePressedState = false;

    if (!this->mouseInMyArea(mPos.x, mPos.y)) return;

    // Open editor according to the Data Node clicked
    // std::string fileData = readFile(this->dn->fullpath);
    // EditorSpace *newEditor = new EditorSpace(fileData, this->config, Div::getWindow());
    // newEditor->loadEditorConfigs();
    
    // this->parentExplorer->activeEditor = newEditor;
    // this->parentExplorer->openEditors[this->dn] = newEditor;

    return;
  }
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
