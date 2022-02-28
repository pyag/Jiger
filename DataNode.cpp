#include <algorithm>

#include "FileOperations.h"
#include "DataNode.h"

std::string toLower (const std::string &str) {
  std::string lowerCaseStr;
  for (int i = 0; i < str.length(); i++) {
    lowerCaseStr += (char) std::tolower(str[i]);
  }

  return lowerCaseStr;
}

bool compareFilenames (DataNode *dn1, DataNode *dn2) {
  if (dn1->isDirectory && !dn2->isDirectory) return true;
  if (!dn1->isDirectory && dn2->isDirectory) return false;

  return toLower(dn1->filename) < toLower(dn2->filename);
}

DataNode::DataNode () {}

DataNode::DataNode (std::string &path) {
  this->fullpath = path;
}

void DataNode::populate (std::vector <std::string> &excluders) {
  if (this->fullpath.length() > 0) {
    populateDataNode(this->fullpath, this, excluders);
  }

  this->sortChildrenByFilename();
}

void DataNode::sortChildrenByFilename () {
  if (this->children.size()) {
    std::sort(this->children.begin(), this->children.end(), compareFilenames);
  }
}
