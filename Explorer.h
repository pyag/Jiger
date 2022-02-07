#ifndef EXPLORER
#define EXPLORER

#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Div.h"
#include "GlobalConfigs.h"
#include "DataNode.h"
#include "DataNodeElement.h"
#include "EditorSpace.h"
#include "TabTray.h"

class Explorer: public Div {
public:
  GlobalConfig config;
  sf::View watchableView;
  DataNode *workplace;
  std::map <int, EditorSpace *> openEditors;
  std::vector <DataNodeElement *> fileDivs;
  int dataNodeId;

  int activeDataNodeId;
  TabTray *tabTray;
  std::vector <std::string> excludedFilePatterns;

  Explorer (GlobalConfig &, sf::RenderWindow *);

  bool isAnyEditorActive ();
  void pollUserEvents (sf::Event &);

  void setWatchableView (sf::View &);
  sf::View &getWatchableView ();

  void loadWorkPlace (const std::string &);

  void drawOnScreen(sf::RenderWindow &);
};

#endif
