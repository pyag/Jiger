#include <cstdio>
#include <cstdlib>
#include <iostream>

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
