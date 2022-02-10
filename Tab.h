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

  GlobalConfig *config;

  Tab (std::string &, int, EditorSpace *, GlobalConfig *, sf::RenderWindow *);

  void pollUserEvents (sf::Event &);
  void drawOnScreen();
};

#endif
