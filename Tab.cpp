#include "headers/Tab.h"

Tab::Tab (std::string &name, int dnId, EditorSpace *editor, GlobalConfig *config, sf::RenderWindow *window) : Div(window) {
  this->dnId = dnId;
  this->name = name;
  this->editor = editor;
  this->config = config;

  this->width =
    name.length() * this->config->getWordWidth()
    + 2.0f * this->config->getTabXPadding();
}

void Tab::pollUserEvents (sf::Event &event) {
  Div::pollEvents(event);

  if (event.type == sf::Event::Resized) {
    // Adjust tab size
    this->adjustView(event.size.width, event.size.height);
    return;
  }
}

void Tab::drawOnScreen () {
  Div::drawOnScreen();

  sf::Color tabFontColor(180, 180, 180);
  sf::Color activeTabFontColor(200, 200, 200);
  sf::Font explorerFont = this->config->getExplorerFont();

  sf::Text word;

  word.setFont(explorerFont);
  word.setCharacterSize(this->config->getExplorerFontSize());
  word.setFillColor(tabFontColor);

  word.setString(this->name);
  word.setPosition(sf::Vector2f(
    this->getPosition().x + this->config->getTabXPadding(),
    this->getPosition().y + this->getSize().y / 3.6f
  ));

  this->getWindow()->draw(word);
}
