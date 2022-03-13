#ifndef EXPLORER
#define EXPLORER

#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Div.h"
#include "GlobalConfigs.h"
#include "DataNode.h"
#include "EditorSpace.h"
#include "TabTray.h"

class Explorer: public Div {
public:
  GlobalConfig *config;
  DataNode *workplace;
  std::map <int, EditorSpace *> openEditors;
  int dataNodeId;
  int globalDataNodeIteratorIndex;

  int activeDataNodeId;
  TabTray *tabTray;
  std::vector <std::string> excludedFilePatterns;

  Explorer (GlobalConfig *, sf::RenderWindow *);

  bool isAnyEditorActive ();
  void pollUserEvents (sf::Event &);

  void loadWorkPlace (const std::string &);
  void openNewTab (DataNode *);
  void populateDataNode (DataNode *);
  void hideDataNode (DataNode *);

  void drawOnScreen ();
  void drawDataNodeTree (DataNode *);

  void iteratorDataNode (void (*callback)(Explorer *, std::vector <void *>),
    std::vector <void *> &);

  static void pollUserEventsOnSingleDn (Explorer *, std::vector <void *>);
  static void drawSingleDataNode (Explorer *, std::vector <void *>);

  void offLoadWorkPlace ();
  void cleanUp ();
  ~Explorer ();
};

#endif
