#ifndef DATA_NODE_ELEMENT
#define DATA_NODE_ELEMENT

#include "DataNode.h"
#include "GlobalConfigs.h"
#include "Div.h"

struct DataNodeElement : public Div {
  bool isDirectory;
  DataNode *dn;
  GlobalConfig *config;

  DataNodeElement (DataNode *, GlobalConfig *, sf::RenderWindow *);
  void pollUserEvents (sf::Event &);

  void onHover (sf::View *);
};

#endif
