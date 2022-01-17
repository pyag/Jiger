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
  float breakPointMarkWidth, blockFoldingMarkWidth;
  int lineNumberThresholdWidth;

  float editorXPos, editorYPos;
  float editorXSize, editorYSize;

  float explorerXPos, explorerYPos;
  float explorerXSize, explorerYSize;

  int cursorBlinkTimeInSeconds;

  float cursorHeight, cursorWidth;

  ColorComponent fontColor;
  ColorComponent bgColor;
  ColorComponent explorerColor;
  ColorComponent lineNumberColor;
  ColorComponent curLineNumberColor;

  sf::Font font;

  GlobalConfig ();

  // Setters
  void setFontSize (int);
  void setWordHeight (float);
  void setWordWidth (float);
  void setBreakPointMarkWidth (float);
  void setBlockFoldingMarkWidth (float);
  void setLineNumberThresholdWidth (int);

  void setEditorXPos (float);
  void setEditorYPos (float);
  void setEditorXSize (float);
  void setEditorYSize (float);

  void setExplorerXPos (float);
  void setExplorerYPos (float);
  void setExplorerXSize (float);
  void setExplorerYSize (float);

  void setCursorBlinkTimeInSeconds (int);
  void setCursorHeight (float);
  void setCursorWidth (float);
  void setFontColor (int, int, int);
  void setBgColor (int, int, int);
  void setExplorerColor (int, int, int);
  void setLineNumberColor (int, int, int);
  void setCurLineNumberColor (int, int, int);
  void setFont (std::string);

  // Getters
  int getFontSize ();
  float getWordHeight ();
  float getWordWidth ();
  float getBreakPointMarkWidth ();
  float getBlockFoldingMarkWidth ();
  int getLineNumberThresholdWidth ();

  float getEditorXPos ();
  float getEditorYPos ();
  float getEditorXSize ();
  float getEditorYSize ();

  float getExplorerXPos ();
  float getExplorerYPos ();
  float getExplorerXSize ();
  float getExplorerYSize ();

  int getCursorBlinkTimeInSeconds ();
  float getCursorHeight ();
  float getCursorWidth ();
  ColorComponent getFontColor ();
  ColorComponent getBgColor ();
  ColorComponent getExplorerColor ();
  ColorComponent getLineNumberColor ();
  ColorComponent getCurLineNumberColor ();
  sf::Font& getFont ();
};

#endif