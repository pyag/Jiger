#include <string>
#include <iostream>
#include <cstdlib>

#include <SFML/Graphics.hpp>

#ifndef PROG_LANG
#define PROG_LANG

enum ProgLang {
  PlainText,
  Cpp,
  Java,
  Python,
  JavaScript
};

#endif

#ifndef COLOR_COMPONENT
#define COLOR_COMPONENT

struct ColorComponent {
  int r, g, b;

  ColorComponent ();
  ColorComponent (int, int, int);
};

#endif

#ifndef GLOBAL_CONFIG
#define GLOBAL_CONFIG

struct GlobalConfig {
  int fontSize;
  float wordHeight, wordWidth;
  int breakPointMarkWidth, blockFoldingMarkWidth;
  int lineNumberThresholdWidth;

  int cursorBlinkTimeInSeconds;

  float cursorHeight, cursorWidth;

  ColorComponent fontColor;
  ColorComponent bgColor;
  ColorComponent lineNumberColor;

  sf::Font font;

  GlobalConfig ();

  // Setters
  void setFontSize (int);
  void setWordHeight (float);
  void setWordWidth (float);
  void setBreakPointMarkWidth (int);
  void setBlockFoldingMarkWidth (int);
  void setLineNumberThresholdWidth (int);
  void setCursorBlinkTimeInSeconds (int);
  void setCursorHeight (float);
  void setCursorWidth (float);
  void setFontColor (int, int, int);
  void setBgColor (int, int, int);
  void setLineNumberColor (int, int, int);
  void setFont (std::string);

  // Getters
  int getFontSize ();
  float getWordHeight ();
  float getWordWidth ();
  int getBreakPointMarkWidth ();
  int getBlockFoldingMarkWidth ();
  int getLineNumberThresholdWidth ();
  int getCursorBlinkTimeInSeconds ();
  float getCursorHeight ();
  float getCursorWidth ();
  ColorComponent getFontColor ();
  ColorComponent getBgColor ();
  ColorComponent getLineNumberColor ();
  sf::Font& getFont ();
};

#endif