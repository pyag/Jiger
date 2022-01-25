#ifndef DATANODE
#define DATANODE

#include <string>
#include <vector>

struct DataNode {
  std::string fullpath;
  std::string filename;
  bool isDirectory;

  std::vector <DataNode *> children;

  DataNode();
  DataNode(std::string &);
  void populate(std::vector <std::string> &);
};

#endif
