#include "keyword.h"

std::vector <std::string> cppDataTypes {
  "char", "double", "float",
  "int", "long", "bool", "string",
  "register", "short", "signed",
  "unsigned", "volatile", "using"
};

std::vector <std::string> otherCppKeywords {
  "asm", "auto", "break", "case", "catch", "class",
  "const", "continue", "default", "delete", "do", "else",
  "enum", "extern", "for", "friend", "goto", "if", "inline",
  "new", "operator", "private", "protected", "public", "sizedof",
  "static", "struct", "switch", "template", "this", "throw",
  "try", "typedef", "union", "virtual", "void", "while", "return",
  "namespace"
};

std::vector <std::string> cppDirectives {
  "#include", "#ifndef", "#define", "#if", "#else",
  "#endif"
};

ColorComponent cppDataTypesColor(130, 105, 255);
ColorComponent otherCppComponentColor(242, 140, 40);
ColorComponent defaultTextColor(200, 200, 200);
ColorComponent cppDirectivesColor(111, 143, 175);

ColorComponent CppHighlighter (std::string &word) {
  for (int i = 0; i < cppDirectives.size(); i++) {
    if (cppDirectives[i] == word) {
      return cppDirectivesColor;
    }
  }

  for (int i = 0; i < cppDataTypes.size(); i++) {
    if (cppDataTypes[i] == word) {
      return cppDataTypesColor;
    }
  }

  for (int i = 0; i < otherCppKeywords.size(); i++) {
    if (otherCppKeywords[i] == word) {
      return otherCppComponentColor;
    }
  }

  return defaultTextColor;
}
