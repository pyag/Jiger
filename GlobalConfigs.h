#include <string>
#include <iostream>
#include <cstdlib>

#include <SFML/Graphics.hpp>

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
  int cursorBlinkTimeInSeconds;

  float cursorHeight, cursorWidth;

  ColorComponent fontColor;
  ColorComponent bgColor;

  sf::Font font;

  GlobalConfig ();

  // Setters
  void setFontSize (int);
  void setWordHeight (float);
  void setWordWidth (float);
  void setCursorBlinkTimeInSeconds (int);
  void setCursorHeight (float);
  void setCursorWidth (float);
  void setFontColor (int, int, int);
  void setBgColor (int, int, int);
  void setFont (std::string);

  // Getters
  int getFontSize ();
  float getWordHeight ();
  float getWordWidth ();
  int getCursorBlinkTimeInSeconds ();
  float getCursorHeight ();
  float getCursorWidth ();
  ColorComponent getFontColor ();
  ColorComponent getBgColor ();
  sf::Font& getFont ();
};

#endif