#include <string>

#include "DataNode.h"

#ifndef FILE_OPERATIONS
#define FILE_OPERATIONS

std::string readFile (const std::string &);
void populateDataNode (const std::string &, DataNode *, std::vector <std::string> &);

void nixSystemFileBrowse (const std::string &, DataNode *, std::vector <std::string> &);
void windowsSystemFileBrowse (const std::string &, DataNode *, std::vector <std::string> &);

#endif
