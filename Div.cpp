#include "Div.h"

#include <iostream>

Div::Div (sf::RenderWindow *window) {
  this->window = window;
}

void Div::pollEvents (sf::Event &event) {

}

void Div::fillColor (const sf::Color &divColor) {
  this->divBox.setFillColor(divColor);
}

void Div::drawOnScreen (sf::RenderWindow &window) {
  window.draw(this->divBox);
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
