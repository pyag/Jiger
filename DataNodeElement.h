#include "DataNode.h"
// #include "Explorer.h"
#include "GlobalConfigs.h"
#include "Div.h"

#ifndef DATA_NODE_ELEMENT
#define DATA_NODE_ELEMENT

struct DataNodeElement : public Div {
  bool mousePressedState;
  bool isDirectory;
  DataNode *dn;
  // Explorer *parentExplorer;
  GlobalConfig config;

  DataNodeElement (DataNode *, GlobalConfig &, sf::RenderWindow *);
  void pollUserEvents (sf::Event &);

  void onHover ();
};

#endif
