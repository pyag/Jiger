#ifndef TAB
#define TAB

#include "GlobalConfigs.h"
#include "Div.h"
#include "EditorSpace.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Tab : public Div {
public:
  std::string name;
  int dnId;
  EditorSpace *editor;

  int width;
  bool mousePressedState;

  GlobalConfig config;

  sf::View watchableView;

  Tab (std::string &, int, EditorSpace *, sf::RenderWindow *);
  void setWatchableView (sf::View &);
  sf::View &getWatchableView ();

  void pollUserEvents (sf::Event &);
  void drawOnScreen(sf::RenderWindow &);

  bool onClick (sf::Event &);
};

#endif
