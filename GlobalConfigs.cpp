#include "GlobalConfigs.h"

ColorComponent::ColorComponent () {}

ColorComponent::ColorComponent (int r, int g, int b) {
  this->r = r;
  this->g = g;
  this->b = b;
}

GlobalConfig::GlobalConfig () {
  // Global constructor sets up the default values
  this->fontSize = 18;

  this->wordHeight = 19.0f;
  this->wordWidth = 10.0f;

  this->breakPointMarkWidth = 3;
  this->blockFoldingMarkWidth = 3;

  this->lineNumberThresholdWidth = 3;

  this->cursorBlinkTimeInSeconds = 550;

  this->cursorHeight = 22.0f;
  this->cursorWidth = 2.0f;

  this->setFontColor(200, 200, 200);
  this->setBgColor(28, 28, 28);
  this->setLineNumberColor(120, 120, 120);

  this->setFont("consola");
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

void GlobalConfig::setBreakPointMarkWidth (int width) {
  this->breakPointMarkWidth = width;
}

void GlobalConfig::setBlockFoldingMarkWidth (int width) {
  this->blockFoldingMarkWidth = width;
}

void GlobalConfig::setLineNumberThresholdWidth (int width) {
  this->lineNumberThresholdWidth = width;
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

void GlobalConfig::setLineNumberColor (int r, int g, int b) {
  this->lineNumberColor.r = r;
  this->lineNumberColor.g = g;
  this->lineNumberColor.b = b;
}

void GlobalConfig::setFont (std::string fontName) {
  std::string fontPath = "./fonts/consolas/" + fontName + ".ttf";
  if (!this->font.loadFromFile(fontPath)) {
    std::cout << "Font cannot be loaded!\n";
    exit(1);
  }
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

int GlobalConfig::getBreakPointMarkWidth () {
  return this->breakPointMarkWidth;
}

int GlobalConfig::getBlockFoldingMarkWidth () {
  return this->blockFoldingMarkWidth;
}

int GlobalConfig::getLineNumberThresholdWidth () {
  return this->lineNumberThresholdWidth;
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

ColorComponent GlobalConfig::getLineNumberColor () {
  return this->lineNumberColor;
}

sf::Font& GlobalConfig::getFont () {
  return this->font;
}
