#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Div.h"
#include "GlobalConfigs.h"
#include "DataNode.h"

#ifndef EXPLORER
#define EXPLORER

class Explorer: public Div {
public:
  GlobalConfig config;
  sf::View watchableView;
  DataNode *workplace;

  std::vector <std::string> excludedFilePatterns;

  Explorer (GlobalConfig &, sf::RenderWindow *);

  void pollUserEvents (sf::Event &);

  void setWatchableView (sf::View &);
  sf::View &getWatchableView ();

  void loadWorkPlace (const std::string &);
};

#endif