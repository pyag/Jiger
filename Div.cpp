#include "Div.h"

Div::Div (sf::RenderWindow *window) {
  this->window = window;
}

void Div::pollEvents (sf::Event &event) {
  // Resizing Div on window
  if (event.type == sf::Event::Resized) {
    // Adjust Div size
    this->setSize(this->getWindow()->getSize().x, this->getWindow()->getSize().y);

    // Adjusting Div view
    sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
    this->getWindow()->setView(sf::View(visibleArea));
    return;
  }

  // Mouse Scroll Events on Div in window
  if (event.type == sf::Event::MouseWheelScrolled) {

    // Mouse scroll up
    if (event.mouseWheelScroll.delta > 0) {
      sf::View currentView = this->getWindow()->getView();
      currentView.move(0.f, -40.f);
      this->getWindow()->setView(currentView);
    }

    // Mouse scroll up
    if (event.mouseWheelScroll.delta < 0) {
      sf::View currentView = this->getWindow()->getView();
      currentView.move(0.f, 40.f);
      this->getWindow()->setView(currentView);
    }

  }
}

void Div::setSize (float width, float height, bool percentage) {
  this->width = width;
  this->height = height;
  this->percentage = percentage;

  this->divBox.setSize(sf::Vector2f(this->width, this->height));
}

void Div::setPosition (float x, float y) {
  this->divBox.setPosition(sf::Vector2f(x, y));
}

sf::Vector2f Div::getPosition () {
  return this->divBox.getPosition();
}

void Div::fillColor (const sf::Color &divColor) {
  this->divBox.setFillColor(divColor);
}

void Div::drawOnScreen (sf::RenderWindow &window) {
  window.draw(this->divBox);
}

sf::RenderWindow *Div::getWindow () {
  return this->window;
}
