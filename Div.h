#ifndef DIV
#define DIV

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Div {
public:
  float width, height;
  bool percentage;
  sf::RectangleShape divBox;

  bool mousePressedState;
  sf::Cursor::Type mousePointerType;
  sf::View watchableView;

  float viewLeftDiff, viewTopDiff;

  sf::RenderWindow *window;

  Div ();
  Div (sf::RenderWindow *);
  
  void setSize(float, float, bool percentage = false);
  void setPosition(float, float);
  sf::Vector2f getPosition();
  sf::Vector2f getSize();

  void fillColor(const sf::Color &);
  void drawOnScreen();

  void pollEvents(sf::Event &);

  bool clicked (sf::Event &, float x, float y);
  bool onClick (sf::Event &);
  bool onClick (sf::Event &, sf::View *);

  bool onhover ();
  bool onhover (sf::View *);

  void setWatchableView (sf::View &);
  sf::View &getWatchableView ();

  void setWindow (sf::RenderWindow *);
  sf::RenderWindow *getWindow ();

  bool mouseInMyArea(float, float);
  void adjustView (float, float);

  void setCursorPointer (sf::Cursor::Type);
  void mouseCursorOnHover ();
};

#endif
