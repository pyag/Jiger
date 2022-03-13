#include <algorithm>

#include "FileOperations.h"
#include "DataNode.h"

std::string toLower (const std::string &str) {
  std::string lowerCaseStr;
  for (int i = 0; i < str.length(); i++) {
    lowerCaseStr += (char) std::tolower(str[i]);
  }

  return lowerCaseStr;
}

bool compareFilenames (DataNode *dn1, DataNode *dn2) {
  if (dn1->isDirectory && !dn2->isDirectory) return true;
  if (!dn1->isDirectory && dn2->isDirectory) return false;

  return toLower(dn1->filename) < toLower(dn2->filename);
}

DataNode::DataNode () {}

DataNode::DataNode (const std::string &path, GlobalConfig *config, sf::RenderWindow *window)
  : Div(window) {
  this->fullpath = path;
  this->isExpanded = false;
  this->config = config;

  this->setFileTextStyle();
}

void DataNode::pollUserEvents (sf::Event &event) {
  Div::pollEvents(event);
}

void DataNode::onHover (sf::View *view) {
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

void DataNode::drawOnScreen (int depth) {
  Div::drawOnScreen();

  float leftXPadding = 25.0f;
  float heightBuf = 2.0f;

  this->fileText.setString(this->filename);

  this->fileText.setPosition(sf::Vector2f(
    this->getPosition().x + leftXPadding +
      depth * this->config->getExplorerWordWidth() * 1.25f,
    this->getPosition().y + heightBuf
  ));

  this->getWindow()->draw(this->fileText);
}

void DataNode::setFileTextStyle () {
  sf::Color fontColor(
    this->config->getExplorerFontColor().r,
    this->config->getExplorerFontColor().g,
    this->config->getExplorerFontColor().b
  );

  this->fileText.setFont(this->config->getExplorerFont());
  this->fileText.setCharacterSize(this->config->getExplorerFontSize());
  this->fileText.setFillColor(fontColor);
}

void DataNode::populate (std::vector <std::string> &excluders) {
  if (this->fullpath.length() > 0) {
    populateDataNode(this->fullpath, this, excluders);
  }

  if (this->children.size()) {
    for (int i = 0; i < (int) this->children.size(); i++) {
      this->children[i]->isExpanded = false;
      this->children[i]->config = config;
      this->children[i]->setWindow(this->getWindow());
      this->children[i]->setFileTextStyle();
    }
  }

  this->sortChildrenByFilename();
}

void DataNode::sortChildrenByFilename () {
  if (this->children.size()) {
    std::sort(this->children.begin(), this->children.end(), compareFilenames);
  }
}
