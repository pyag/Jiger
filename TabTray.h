#ifndef TABTRAY
#define TABTRAY

#include <vector>

#include "GlobalConfigs.h"
#include "Div.h"
#include "Tab.h"
#include "EditorSpace.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class TabTray : public Div {
  /*
   * Below is explorer's Active Data Node ID
   * DO NOT CHANGE THIS. NEVER MESS WITH THIS
   * THIS IS ONLY TO SYNC EXPLORER WITH TABTRAY
   * - ONLY ONCLICK EVENT CHANGES THIS.
   */ 
  int *explorerActiveDnId;

public:
  std::vector <Tab *> tabs;
  int activeDataNodeId;

  GlobalConfig *config;

  sf::View watchableView;

  TabTray (GlobalConfig *, sf::RenderWindow *);

  void push (std::string &, int, EditorSpace *);
  void remove (int);

  bool empty ();
  void setActiveTab (int);
  void registerExplorerActiveDnId (int *);
  void loadConfigs ();

  void setWatchableView (sf::View &);
  sf::View &getWatchableView ();

  void pollUserEvents (sf::Event &);
  void drawOnScreen(sf::RenderWindow &);

  void drawTabs (int, float &);
};

#endif
