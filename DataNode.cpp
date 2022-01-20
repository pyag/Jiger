#include "FileOperations.h"
#include "DataNode.h"

DataNode::DataNode () {}

DataNode::DataNode (std::string &path) {
  this->fullpath = path;
}

void DataNode::populate (std::vector <std::string> &excluders) {
  if (this->fullpath.length() > 0) {
    populateDataNode(this->fullpath, this, excluders);
  }
}
