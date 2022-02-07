#include "GlobalConfigs.h"

ColorComponent::ColorComponent () {}

ColorComponent::ColorComponent (int r, int g, int b) {
  this->r = r;
  this->g = g;
  this->b = b;
}

GlobalConfig::GlobalConfig () {
  // Global constructor sets up the default values
  this->fontSize = 15;
  this->wordHeight = 19.0f;
  this->wordWidth = 8.8f;

  this->explorerFontSize = 13.3f;
  this->explorerWordHeight = 22.0f;
  this->explorerWordWidth = 8.0f;

  this->breakPointMarkWidth = 2.0f;
  this->blockFoldingMarkWidth = 3.0f;

  this->lineNumberThresholdWidth = 3;

  this->editorXPos = 270.0f;
  this->editorYPos = 37.0f;

  this->explorerXPos = 0.0f;
  this->explorerYPos = 0.0f;

  this->tabXPadding = 10.0f;
  this->tabTrayHeight = this->editorYPos;

  this->cursorBlinkTimeInSeconds = 550;

  this->cursorHeight = this->wordHeight;
  this->cursorWidth = 2.0f;

  this->setFontColor(200, 200, 200);
  this->setBgColor(29, 29, 29);
  this->setExplorerColor(34, 34, 36);
  this->setLineNumberColor(120, 120, 120);
  this->setCurLineNumberColor(200, 200, 200);
  this->setExplorerFontColor(160, 160, 160);

  this->setFont("consola");
  this->setExplorerFont("monaco");
}

// Setters
void GlobalConfig::setFontSize (int size) {
  this->fontSize = size;
}

void GlobalConfig::setWordHeight (float height) {
  this->wordHeight = height;
}

void GlobalConfig::setWordWidth (float width) {
  this->wordWidth = width;
}

void GlobalConfig::setExplorerFontSize (float size) {
  this->explorerFontSize = size;
}

void GlobalConfig::setExplorerWordHeight (float height) {
  this->explorerWordHeight = height;
}

void GlobalConfig::setExplorerWordWidth (float width) {
  this->explorerWordWidth = width;
}

void GlobalConfig::setBreakPointMarkWidth (float width) {
  this->breakPointMarkWidth = width;
}

void GlobalConfig::setBlockFoldingMarkWidth (float width) {
  this->blockFoldingMarkWidth = width;
}

void GlobalConfig::setLineNumberThresholdWidth (int width) {
  this->lineNumberThresholdWidth = width;
}

void GlobalConfig::setEditorXPos (float xPos) {
  this->editorXPos = xPos;
}

void GlobalConfig::setEditorYPos (float yPos) {
  this->editorYPos = yPos;
}

void GlobalConfig::setEditorXSize (float xSize) {
  this->editorXSize = xSize;
}

void GlobalConfig::setEditorYSize (float ySize) {
  this->editorYSize = ySize;
}

void GlobalConfig::setExplorerXPos (float xPos) {
  this->explorerXPos = xPos;
}

void GlobalConfig::setExplorerYPos (float yPos) {
  this->explorerYPos = yPos;
}

void GlobalConfig::setExplorerXSize (float xSize) {
  this->explorerXSize = xSize;
}

void GlobalConfig::setExplorerYSize (float ySize) {
  this->explorerYSize = ySize;
}

void GlobalConfig::setTabXPadding (float xPadding) {
  this->tabXPadding = xPadding;
}

void GlobalConfig::setTabTrayHeight (float height) {
  this->tabTrayHeight = height;
}

void GlobalConfig::setCursorBlinkTimeInSeconds (int seconds) {
  this->cursorBlinkTimeInSeconds = seconds;
}

void GlobalConfig::setCursorHeight (float height) {
  this->cursorHeight = height;
}

void GlobalConfig::setCursorWidth (float width) {
  this->cursorWidth = width;
}

void GlobalConfig::setFontColor (int r, int g, int b) {
  this->fontColor.r = r;
  this->fontColor.g = g;
  this->fontColor.b = b;
}

void GlobalConfig::setBgColor (int r, int g, int b) {
  this->bgColor.r = r;
  this->bgColor.g = g;
  this->bgColor.b = b;
}

void GlobalConfig::setExplorerColor (int r, int g, int b) {
  this->explorerColor.r = r;
  this->explorerColor.g = g;
  this->explorerColor.b = b;  
}

void GlobalConfig::setLineNumberColor (int r, int g, int b) {
  this->lineNumberColor.r = r;
  this->lineNumberColor.g = g;
  this->lineNumberColor.b = b;
}

void GlobalConfig::setCurLineNumberColor (int r, int g, int b) {
  this->curLineNumberColor.r = r;
  this->curLineNumberColor.g = g;
  this->curLineNumberColor.b = b;
}

void GlobalConfig::setExplorerFontColor (int r, int g, int b) {
  this->explorerFontColor.r = r;
  this->explorerFontColor.g = g;
  this->explorerFontColor.b = b;  
}

void GlobalConfig::setFont (std::string fontName) {
  std::string fontPath = "./fonts/consolas/" + fontName + ".ttf";
  if (!this->font.loadFromFile(fontPath)) {
    std::cout << "Font cannot be loaded!\n";
    exit(1);
  }

  // To remove blurry font in SFML
  // Refer: https://en.sfml-dev.org/forums/index.php?topic=21058.0
  sf::Texture &fontTexture = const_cast <sf::Texture &> (this->font.getTexture(this->getFontSize()));
  fontTexture.setSmooth(false);
}

void GlobalConfig::setExplorerFont (std::string fontName) {
  std::string fontPath = "./fonts/monaco/" + fontName + ".ttf";
  if (!this->explorerFont.loadFromFile(fontPath)) {
    std::cout << "Font cannot be loaded!\n";
    exit(1);
  }

  // To remove blurry font in SFML
  // Refer: https://en.sfml-dev.org/forums/index.php?topic=21058.0
  sf::Texture &fontTexture = const_cast <sf::Texture &> (this->explorerFont.getTexture(this->getExplorerFontSize()));
  fontTexture.setSmooth(false);
}

// Getter
int GlobalConfig::getFontSize () {
  return this->fontSize;
}

float GlobalConfig::getWordHeight () {
  return this->wordHeight;
}

float GlobalConfig::getWordWidth () {
  return this->wordWidth;
}

float GlobalConfig::getExplorerFontSize () {
  return this->explorerFontSize;
}

float GlobalConfig::getExplorerWordHeight () {
  return this->explorerWordHeight;
}

float GlobalConfig::getExplorerWordWidth () {
  return this->explorerWordWidth;
}

float GlobalConfig::getBreakPointMarkWidth () {
  return this->breakPointMarkWidth;
}

float GlobalConfig::getBlockFoldingMarkWidth () {
  return this->blockFoldingMarkWidth;
}

int GlobalConfig::getLineNumberThresholdWidth () {
  return this->lineNumberThresholdWidth;
}

float GlobalConfig::getEditorXPos () {
  return this->editorXPos;
}

float GlobalConfig::getEditorYPos () {
  return this->editorYPos;
}

float GlobalConfig::getEditorXSize () {
  return this->editorXSize;
}

float GlobalConfig::getEditorYSize () {
  return this->editorYSize;
}

float GlobalConfig::getExplorerXPos () {
  return this->explorerXPos;
}

float GlobalConfig::getExplorerYPos () {
  return this->explorerYPos;
}

float GlobalConfig::getExplorerXSize () {
  return this->explorerXSize;
}

float GlobalConfig::getExplorerYSize () {
  return this->explorerYSize;
}

float GlobalConfig::getTabXPadding () {
  return this->tabXPadding;
}

float GlobalConfig::getTabTrayHeight () {
  return this->tabTrayHeight;
}

int GlobalConfig::getCursorBlinkTimeInSeconds () {
  return this->cursorBlinkTimeInSeconds;
}

float GlobalConfig::getCursorHeight () {
  return this->cursorHeight;
}

float GlobalConfig::getCursorWidth () {
  return this->cursorWidth;
}

ColorComponent GlobalConfig::getFontColor () {
  return this->fontColor;
}

ColorComponent GlobalConfig::getBgColor () {
  return this->bgColor;
}

ColorComponent GlobalConfig::getExplorerColor () {
  return this->explorerColor;
}

ColorComponent GlobalConfig::getLineNumberColor () {
  return this->lineNumberColor;
}

ColorComponent GlobalConfig::getCurLineNumberColor () {
  return this->curLineNumberColor;
}

ColorComponent GlobalConfig::getExplorerFontColor () {
  return this->explorerFontColor;
}

sf::Font& GlobalConfig::getFont () {
  return this->font;
}

sf::Font& GlobalConfig::getExplorerFont() {
  return this->explorerFont;
}
