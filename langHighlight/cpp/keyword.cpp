#include "keyword.h"

std::vector <std::string> cppDataTypes {
  "char", "double", "float",
  "int", "long", "register",
  "short", "signed", "unsigned",
  "volatile"
};

std::vector <std::string> otherCppKeywords {
  "asm", "auto", "break", "case", "catch", "class",
  "const", "continue", "default", "delete", "do", "else",
  "enum", "extern", "for", "friend", "goto", "if", "inline",
  "new", "operator", "private", "protected", "public", "sizedof",
  "static", "struct", "switch", "template", "this", "throw",
  "try", "typedef", "union", "virtual", "void", "while"
};

ColorComponent CppHighlighter (std::string &word) {
  for (int i = 0; i < cppDataTypes.size(); i++) {
    if (cppDataTypes[i] == word) {
      return ColorComponent(100, 149, 237);
    }
  }

  for (int i = 0; i < otherCppKeywords.size(); i++) {
    if (otherCppKeywords[i] == word) {
      return ColorComponent(251, 236, 93);
    }
  }

  return ColorComponent(235, 238, 235);
}
