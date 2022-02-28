#ifndef DATA_NODE_ELEMENT
#define DATA_NODE_ELEMENT

#include "DataNode.h"
#include "GlobalConfigs.h"
#include "Div.h"

struct DataNodeElement : public Div {
  bool isDirectory;
  DataNode *dn;
  GlobalConfig *config;

  // This holds text UI style
  sf::Text fileText;

  DataNodeElement (DataNode *, GlobalConfig *, sf::RenderWindow *);
  void pollUserEvents (sf::Event &);

  void onHover (sf::View *);
  void drawOnScreen ();

  void setFileTextStyle ();
};

#endif
