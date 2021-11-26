#include "highlight.h"

ColorComponent wordHighlighter (std::string &word, ProgLang lang) {
  return CppHighlighter(word);
  // switch (lang) {
  //   case Cpp:
  //     return CppHighlighter(word);
  // }
}
