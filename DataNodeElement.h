#ifndef DATA_NODE_ELEMENT
#define DATA_NODE_ELEMENT

#include "DataNode.h"
#include "GlobalConfigs.h"
#include "Div.h"

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
