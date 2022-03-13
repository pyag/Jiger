#ifndef DATANODE
#define DATANODE

#include <string>
#include <vector>

#include "GlobalConfigs.h"
#include "Div.h"

struct DataNode : public Div {
  int id;
  std::string fullpath;
  std::string filename;

  bool isDirectory;
  bool isExpanded;

  GlobalConfig *config;
  std::vector <DataNode *> children;

  // This holds text UI style
  sf::Text fileText;

  DataNode ();
  DataNode (const std::string &, GlobalConfig *, sf::RenderWindow *);
  void populate (std::vector <std::string> &);
  void sortChildrenByFilename ();

  void pollUserEvents (sf::Event &);

  void onHover (sf::View *);
  void drawOnScreen (int);

  void setFileTextStyle ();
};

#endif
