#include "../headers/Div.h"

#include <iostream>

Div::Div () {}

Div::Div (sf::RenderWindow *window) {
  this->window = window;
  this->mousePressedState = false;

  this->viewLeftDiff = -1.0f;
  this->viewTopDiff = -1.0f;

  this->mousePointerType = sf::Cursor::Arrow;
}

void Div::pollEvents (sf::Event &event) {
  this->mouseCursorOnHover();
}

void Div::setWatchableView (sf::View &view) {
  this->watchableView = view;
}

sf::View &Div::getWatchableView () {
  return this->watchableView;
}

void Div::fillColor (const sf::Color &divColor) {
  this->divBox.setFillColor(divColor);
}

void Div::drawOnScreen () {
  this->window->draw(this->divBox);
}

// Setters
void Div::setSize (float width, float height, bool percentage) {
  this->width = width;
  this->height = height;
  this->percentage = percentage;

  this->divBox.setSize(sf::Vector2f(this->width, this->height));
}

void Div::setPosition (float x, float y) {
  this->divBox.setPosition(sf::Vector2f(x, y));
}

// Getters
sf::Vector2f Div::getPosition () {
  return this->divBox.getPosition();
}

sf::Vector2f Div::getSize () {
  return this->divBox.getSize();
}

void Div::setWindow (sf::RenderWindow *window) {
  this->window = window;
}

sf::RenderWindow *Div::getWindow () {
  return this->window;
}

bool Div::mouseInMyArea(float mx, float my) {
  float x = this->getPosition().x;
  float X = x + this->width;

  float y = this->getPosition().y;
  float Y = y + this->height;

  return !(mx < x || mx > X || my < y || my > Y);
}

bool Div::onClick (sf::Event &event) {
  sf::Vector2i mPos = sf::Mouse::getPosition(*this->getWindow());

  return clicked(event, mPos.x, mPos.y);
}

bool Div::onClick (sf::Event &event, sf::View *view) {
  sf::Vector2i coord = sf::Mouse::getPosition(*this->getWindow());
  sf::Vector2f mPos = this->getWindow()
    ->mapPixelToCoords(coord, *view);

  return clicked(event, mPos.x, mPos.y);
}

bool Div::clicked (sf::Event &event, float x, float y) {
  if (event.type == sf::Event::MouseButtonPressed) {
    if (!this->mouseInMyArea(x, y)) return false;

    if (this->mousePressedState == false) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        this->mousePressedState = true;
      }
    }

    return false;
  }

  if (event.type == sf::Event::MouseButtonReleased) {
    if (this->mouseInMyArea(x, y) && this->mousePressedState) {
      this->mousePressedState = false;
      return true;      
    }
  }

  return false; 
}

bool Div::onhover () {
  sf::Vector2i mPos = sf::Mouse::getPosition(*this->getWindow());
  return this->mouseInMyArea(mPos.x, mPos.y);
}

bool Div::onhover (sf::View *view) {
  sf::Vector2i coord = sf::Mouse::getPosition(*this->getWindow());
  sf::Vector2f mPos = this->getWindow()
    ->mapPixelToCoords(coord, *view);

  return this->mouseInMyArea(mPos.x, mPos.y);
}

void Div::adjustView (float width, float height) {
  // Adjusting Div view
  sf::View view = this->getWatchableView();

  if (viewLeftDiff == -1.0f && viewTopDiff == -1.0f) {

    view.reset(sf::FloatRect(
      this->getPosition().x,
      this->getPosition().y,
      this->getSize().x,
      this->getSize().y
    ));

    this->viewLeftDiff = 0.0f;
    this->viewTopDiff = 0.0f;

  } else {
    float leftPos = view.getCenter().x - view.getSize().x / 2.0f;
    float topPos = view.getCenter().y - view.getSize().y / 2.0f;
    
    this->viewLeftDiff = leftPos - this->getPosition().x;
    this->viewTopDiff = topPos - this->getPosition().y;

    view.reset(sf::FloatRect(
      this->getPosition().x + this->viewLeftDiff,
      this->getPosition().y + this->viewTopDiff,
      this->getSize().x,
      this->getSize().y
    ));
  }

  view.setViewport(sf::FloatRect(
    this->getPosition().x / width,
    this->getPosition().y / height,
    this->getSize().x / width,
    this->getSize().y / height
  ));

  this->setWatchableView(view);
}

void Div::setCursorPointer (sf::Cursor::Type type) {
  sf::Cursor systemCursor;

  if (systemCursor.loadFromSystem(type)) {
    this->getWindow()->setMouseCursor(systemCursor);
  } else {
    std::cout << "Cannot load cursor\n";
  }
}

void Div::mouseCursorOnHover () {
  if (this->onhover(&this->getWatchableView())) {

    // By default set cursor pointer as Arrow
    this->setCursorPointer(this->mousePointerType);
  }
}
