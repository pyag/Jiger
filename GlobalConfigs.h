#ifndef PROG_LANG
#define PROG_LANG

#ifndef COLOR_COMPONENT
#define COLOR_COMPONENT

#ifndef GLOBAL_CONFIG
#define GLOBAL_CONFIG

#include <string>
#include <iostream>
#include <cstdlib>

#include <SFML/Graphics.hpp>

enum ProgLang {
  PlainText,
  Cpp,
  Java,
  Python,
  JavaScript
};

struct ColorComponent {
  int r, g, b;

  ColorComponent ();
  ColorComponent (int, int, int);
};

struct GlobalConfig {
  int fontSize;
  float wordHeight, wordWidth;

  float explorerFontSize;
  float explorerWordHeight, explorerWordWidth;

  float breakPointMarkWidth, blockFoldingMarkWidth;
  int lineNumberThresholdWidth;

  float editorXPos, editorYPos;
  float editorXSize, editorYSize;

  float explorerXPos, explorerYPos;
  float explorerXSize, explorerYSize;

  float tabWidth, tabHeight, tabXPadding;
  float tabTrayWidth, tabTrayHeight;

  int cursorBlinkTimeInSeconds;

  float cursorHeight, cursorWidth;

  ColorComponent fontColor;
  ColorComponent bgColor;
  ColorComponent explorerColor;
  ColorComponent lineNumberColor;
  ColorComponent curLineNumberColor;
  ColorComponent explorerFontColor;
  ColorComponent tabTrayBgColor;

  sf::Font font;
  sf::Font explorerFont;

  GlobalConfig ();

  // Setters
  void setFontSize (int);
  void setWordHeight (float);
  void setWordWidth (float);

  void setExplorerFontSize (float);
  void setExplorerWordHeight (float);
  void setExplorerWordWidth (float);

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

  void setTabXPadding (float);
  void setTabWidth (float);
  void setTabHeight (float);

  void setTabTrayWidth (float);
  void setTabTrayHeight (float);

  void setCursorBlinkTimeInSeconds (int);
  void setCursorHeight (float);
  void setCursorWidth (float);
  void setFontColor (int, int, int);
  void setBgColor (int, int, int);
  void setExplorerColor (int, int, int);
  void setLineNumberColor (int, int, int);
  void setCurLineNumberColor (int, int, int);
  void setExplorerFontColor (int, int, int);
  void setFont (std::string);
  void setExplorerFont (std::string);
  void setTabTrayBgColor (int, int, int);

  // Getters
  int getFontSize ();
  float getWordHeight ();
  float getWordWidth ();

  float getExplorerFontSize ();
  float getExplorerWordHeight ();
  float getExplorerWordWidth ();

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

  float getTabXPadding ();
  float getTabWidth ();
  float getTabHeight ();

  float getTabTrayWidth ();
  float getTabTrayHeight ();

  int getCursorBlinkTimeInSeconds ();
  float getCursorHeight ();
  float getCursorWidth ();
  ColorComponent getFontColor ();
  ColorComponent getBgColor ();
  ColorComponent getExplorerColor ();
  ColorComponent getLineNumberColor ();
  ColorComponent getCurLineNumberColor ();
  ColorComponent getExplorerFontColor ();
  ColorComponent getTabTrayBgColor ();
  sf::Font& getFont ();
  sf::Font& getExplorerFont();
};

#endif

#endif

#endif
