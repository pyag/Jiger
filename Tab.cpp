#include "Tab.h"

Tab::Tab (std::string &name, int dnId, EditorSpace *editor, sf::RenderWindow *window) : Div(window) {
  this->dnId = dnId;
  this->name = name;
  this->editor = editor;

  this->mousePressedState = false;
  this->width =
    name.length() * this->config.getWordWidth()
    + 2.0f * this->config.getTabXPadding();
}

void Tab::pollUserEvents (sf::Event &event) {
  Div::pollEvents(event);

  this->onHover();

  if (event.type == sf::Event::Resized) {
    // Adjust tab size
    float tabWidth = 126.0f;
    float tabHeight = this->config.getEditorYPos();

    // Adjusting Tab view
    sf::View tabView = this->getWatchableView();

    tabView.reset(sf::FloatRect(
      this->config.getEditorXPos(),
      this->config.getExplorerYPos(),
      tabWidth,
      tabHeight
    ));

    tabView.setViewport(sf::FloatRect(
      this->config.getEditorXPos() / event.size.width,
      this->config.getExplorerYPos() / event.size.height,
      tabWidth / event.size.width,
      tabHeight / event.size.height
    ));

    this->setWatchableView(tabView);
    return;
  }
}

void Tab::drawOnScreen (sf::RenderWindow &window) {
  Div::drawOnScreen(window);

  sf::Color fontColor(
    this->config.getFontColor().r,
    this->config.getFontColor().g,
    this->config.getFontColor().b
  );
  sf::Font explorerFont = this->config.getExplorerFont();

  sf::Text word;

  word.setFont(explorerFont);
  word.setCharacterSize(this->config.getExplorerFontSize());
  word.setColor(fontColor);

  word.setString(this->name);
  word.setPosition(sf::Vector2f(
    this->getPosition().x + this->config.getTabXPadding(),
    this->getPosition().y + this->getSize().y / 3.6f
  ));

  window.draw(word);
}

void Tab::setWatchableView (sf::View &view) {
  this->watchableView = view;
}

sf::View &Tab::getWatchableView () {
  return this->watchableView;
}

bool Tab::onClick (sf::Event &event) {
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
