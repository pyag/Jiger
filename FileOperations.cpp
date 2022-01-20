#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "FileOperations.h"

// For Windows
#if defined(_WIN32)
// the windows implementation is yet to be done
#endif

// For Linux and Machintosh, dirent.h should work
#if defined(__APPLE__) || defined(__MACH__) || defined(__linux__)
#include <dirent.h>
#endif

std::string readFile (const std::string &fileLocation) {
  FILE *file = fopen(fileLocation.c_str(), "r");
  if (file == NULL) {
    std::cout << "File cannot be opened!\n";
    exit(0);
  }

  char ch;
  std::string fileText;
  
  while ((ch = fgetc(file)) != EOF) {
    fileText += ch;
  }

  fclose(file);
  return fileText;
}

void populateDataNode (const std::string &path, DataNode *dn, std::vector <std::string> &excluders) {
  #if defined(_WIN32)
  windowsSystemFileBrowse(path, dn, excluders);
  #endif

  #if defined(__APPLE__) || defined(__MACH__) || defined(__linux__)
  nixSystemFileBrowse(path, dn, excluders);
  #endif
}

void windowsSystemFileBrowse (const std::string &path, DataNode *dn, std::vector <std::string> &excluders) {
  // Yet to design a function for Windows system file browsing
  return;
}

void nixSystemFileBrowse(const std::string &path, DataNode *dn, std::vector <std::string> &excluders) {
  DIR *dir;
  struct dirent *entry;

  if ((dir = opendir(path.c_str())) == NULL) {

    dn->isDirectory = false;
    std::cout << "Error occured while reading file system!\n";
    return;

  } else {

    dn->isDirectory = true;

    while ((entry = readdir(dir)) != NULL) {

      std::string filename(entry->d_name);

      bool skip = false;
      for (int i = 0; i < excluders.size(); i++) {
        if (filename.rfind(excluders[i], 0) == 0) {
          skip = true;
          break;
        }
      }

      if (skip) continue;

      std::string fullpath = path + "/" + filename;

      DataNode *childDn = new DataNode();
      childDn->fullpath = fullpath;
      childDn->filename = filename;
      childDn->isDirectory = false;

      DIR *childRefDir;
      if ((childRefDir = opendir(fullpath.c_str())) != NULL) {
        childDn->isDirectory = true;
        closedir(childRefDir);
      }

      dn->children.push_back(childDn);

    }

    closedir(dir);
  }
}
