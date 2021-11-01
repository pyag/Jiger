#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#ifndef DIV
#define DIV

class Div {
public:
  float width, height;
  bool percentage;
  sf::RectangleShape divBox;

  Div () {}
  void setSize(float, float, bool percentage = false);
  void setPosition(float, float);
  sf::Vector2f getPosition();
  void fillColor(const sf::Color &);
  void drawOnScreen(sf::RenderWindow &);
};

#endif
