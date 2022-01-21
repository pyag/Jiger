#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#ifndef DIV
#define DIV

class Div {
public:
  float width, height;
  bool percentage;
  sf::RectangleShape divBox;

  sf::RenderWindow *window;

  Div () {}
  Div (sf::RenderWindow *);
  
  void setSize(float, float, bool percentage = false);
  void setPosition(float, float);
  sf::Vector2f getPosition();
  sf::Vector2f getSize();

  void fillColor(const sf::Color &);
  void drawOnScreen(sf::RenderWindow &);

  void pollEvents(sf::Event &);

  sf::RenderWindow *getWindow ();

  bool mouseInMyArea(float, float);
};

#endif
