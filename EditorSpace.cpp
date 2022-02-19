#include "EditorSpace.h"

EditorSpace::EditorSpace (std::string &fileLoc, GlobalConfig *config, sf::RenderWindow *window): Div(window) {
  this->fileLoc = fileLoc;
  
  this->buf = new Buffer(this->fileLoc);
  this->buf->loadBuffer();

  this->cursor = new Div(window);
  this->cursorIndex = 0;
  this->curLine = 0;

  this->showCursor = true;

  this->config = config;
  this->langSelected = PlainText;
  this->hideLineNumber = false;

  this->mousePressedFirstTime = false;
  this->selectionStartIndex = -1;
  this->selectionEndIndex = -1;
}

void EditorSpace::pollKeyboard (int unicode) {
  // Backspace
  if (unicode == 8) {
    bool syncLines = false;
    int to = this->cursorIndex;
    if (this->buf->getCharAtPos(to - 1) == '\n') {
      syncLines = true;
    }

    this->buf->remove(to - 1, to);

    if (syncLines) {
      this->buf->syncLineBreaks();
    }

    if (this->cursorIndex > 0) this->cursorIndex--;
    return;
  }

  // Tab - For now its fixed for two spaces
  if (unicode == 9) {

    std::string tabs = "  ";
    this->buf->insert(tabs, this->cursorIndex);

    this->cursorIndex += 2;
    return;
  }

  // Return
  if (unicode == 13) {
    std::string returnChar = "\n";
    this->buf->insert(returnChar, this->cursorIndex);
    this->buf->syncLineBreaks();
    
    this->cursorIndex++;
    return;
  }

  if (unicode >= 32 && unicode <= 126) {
    std::string newChar;
    newChar = (char)(unicode);
    this->buf->insert(newChar, this->cursorIndex);

    this->cursorIndex++;
    return;
  }

  return;
}

void EditorSpace::pollUserEvents (sf::Event &event) {
  Div::pollEvents(event);

  /**
   * Below are Editor specific user events
   **/

  // OnClick Event
  if (event.type == sf::Event::MouseButtonPressed) {
    
    if (event.mouseButton.button == sf::Mouse::Left) {
      
      sf::Vector2i coord = sf::Mouse::getPosition(*this->getWindow());
      sf::Vector2f mPos = this->getWindow()->mapPixelToCoords(coord, this->getWatchableView());

      if (!this->mouseInMyArea(mPos.x, mPos.y)) return;
      this->cursorIndex = this->getCursorIndexByLocation(mPos.x, mPos.y);

      if (!this->mousePressedFirstTime) {
        this->mousePressedFirstTime = true;
        this->selectionStartIndex = this->cursorIndex;
        this->selectionEndIndex = this->cursorIndex;
      } else {
        this->selectionEndIndex = this->cursorIndex;
      }
    
    }

    return;
  }

  // Resizing Div on window
  if (event.type == sf::Event::Resized) {
    // Adjust editor size
    float newEditorWidth = event.size.width - this->config->getEditorXPos();
    float newEditorHeight = event.size.height - this->config->getEditorYPos();
    this->config->setEditorXSize(newEditorWidth);
    this->config->setEditorYSize(newEditorHeight);

    this->setSize(newEditorWidth, newEditorHeight);

    this->adjustView(event.size.width, event.size.height);
    return;
  }

  // Mouse Scroll Events on Editor
  if (event.type == sf::Event::MouseWheelScrolled) {
    if (!Div::mouseInMyArea(
      (float)sf::Mouse::getPosition(*Div::getWindow()).x,
      (float)sf::Mouse::getPosition(*Div::getWindow()).y
    )) return;

    // Mouse scroll up
    if (event.mouseWheelScroll.delta > 0) {
      this->scrollUpByLines(3);
      return;
    }

    // Mouse scroll down
    if (event.mouseWheelScroll.delta < 0) {
      this->scrollDownByLines(3);
      return;
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    this->showCursor = true;
    this->clock.restart();

    if (this->cursorIndex > 0) {
      this->cursorIndex--;
    }
    
    return;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    this->showCursor = true;
    this->clock.restart();

    if (this->cursorIndex < this->buf->getBufferLength()) {
      this->cursorIndex++;
    }

    return;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    this->showCursor = true;
    this->clock.restart();

    if (this->curLine == 0) {
      this->cursorIndex = 0;
      return;
    }

    int startPosInCurLine = this->buf->getLineStartPos(this->curLine);
    int wordGapInCurLine = this->cursorIndex - startPosInCurLine;

    int startPosInLineAbove = this->buf->getLineStartPos(this->curLine - 1);
    if (startPosInLineAbove + wordGapInCurLine > startPosInCurLine) {
      this->cursorIndex = startPosInCurLine - 1;
    } else {
      this->cursorIndex = startPosInLineAbove + wordGapInCurLine;
    }

    this->curLine--;
    return;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    this->showCursor = true;
    this->clock.restart();

    if (this->curLine == this->buf->getLineCount() - 1) {
      this->cursorIndex = this->buf->getBufferLength();
      return;
    }

    int startPosInCurLine = this->buf->getLineStartPos(this->curLine);
    int wordGapInCurLine = this->cursorIndex - startPosInCurLine;

    int startPosInLineBelow = this->buf->getLineStartPos(this->curLine + 1);
    int startPosInLineBelowBelow;

    if (this->curLine + 2 < this->buf->getLineCount()) {
      startPosInLineBelowBelow = this->buf->getLineStartPos(this->curLine + 2);
    } else {
      startPosInLineBelowBelow = this->buf->getBufferLength();
    }

    if (startPosInLineBelow + wordGapInCurLine > startPosInLineBelowBelow) {
      this->cursorIndex = startPosInLineBelowBelow - 1;
    } else {
      this->cursorIndex = startPosInLineBelow + wordGapInCurLine;
    }

    this->curLine++;

    return;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
    && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    this->save();
    return;
  }

  if (event.type == sf::Event::TextEntered) {
    this->showCursor = true;
    this->clock.restart();
    this->pollKeyboard(event.text.unicode);  
  }

  return;
}

void EditorSpace::setLanguage (ProgLang lang) {
  this->langSelected = lang;
}

void EditorSpace::drawOnScreen () {
  Div::drawOnScreen();

  sf::Color fontColor(
    this->config->getFontColor().r,
    this->config->getFontColor().g,
    this->config->getFontColor().b
  );

  sf::Font editorFont = this->config->getFont();
  sf::Text word;
  ProgLang languageSelected = this->langSelected;

  int xWordPosition, yWordPosition;
  float XResetPos;
  std::string wordText;

  XResetPos = Div::getPosition().x;

  xWordPosition = 0;
  yWordPosition = Div::getPosition().y;

  this->curLine = this->buf->getLineNumberByPos(this->cursorIndex);

  word.setFont(editorFont);
  word.setCharacterSize(this->config->getFontSize());
  word.setColor(fontColor);

  sf::View view = this->getWatchableView();
  float viewTop = view.getCenter().y - (view.getSize().y / 2.0f);
  float viewBottom = view.getCenter().y + (view.getSize().y / 2.0f);

  int lineTop = this->getTopLineByView(this->getWatchableView());
  int lineBottom = this->getBottomLineByView(this->getWatchableView());

  int startIndex = this->buf->getLineStartPos(lineTop);
  int endIndex = this->buf->getLineStartPos(lineBottom);

  endIndex =
    (lineBottom == this->buf->getLineCount())
    ? this->buf->getBufferLength()
    : endIndex;

  if (!(this->hideLineNumber)) {
    XResetPos += this->getXTextOffset() * this->config->getWordWidth();
    this->displayLineNumber(lineTop, lineBottom);
  }

  yWordPosition += lineTop * this->config->getWordHeight();
  Parser *parser = new Parser();
  parser->loadBuffer(this->buf->getSource(), startIndex, endIndex);

  while (parser->hasNextToken()) {
    wordText = parser->getToken();

    ColorComponent colorRgb = wordHighlighter(wordText, languageSelected);
    sf::Color curWordColor(colorRgb.r, colorRgb.g, colorRgb.b);
    word.setColor(curWordColor);

    if (wordText == "\n") {
      xWordPosition = 0;
      yWordPosition += this->config->getWordHeight();
      continue;
    }

    for (int i = 0; i < wordText.length(); i++) {
      float x = XResetPos + (float)(xWordPosition++) * this->config->getWordWidth();
      float y = (float)yWordPosition;

      word.setString(wordText[i]);
      word.setPosition(sf::Vector2f(x, y));

      this->getWindow()->draw(word);
    }
  }

  this->cursor->setSize(
    this->config->getCursorWidth(),
    this->config->getCursorHeight()
  );

  this->cursor->fillColor(fontColor);

  float cursorX = XResetPos +
    (this->cursorIndex - this->buf->getLineStartPos(this->curLine)) *
    this->config->getWordWidth();

  float cursorY = Div::getPosition().y +
    this->curLine * this->config->getWordHeight() +
    1.0f;

  this->cursor->setPosition(cursorX, cursorY);

  if (
    this->clock.getElapsedTime() >
    sf::milliseconds(this->config->getCursorBlinkTimeInSeconds())
  ) {
    this->showCursor = !this->showCursor;
    this->clock.restart();
  }

  if (
    (this->curLine >= lineTop && this->curLine <= lineBottom)
    && this->showCursor
  ) {
    this->cursor->drawOnScreen();
  }

  this->updateEditorSize();
}

float EditorSpace::getXTextOffset () {
  int lineCount = this->buf->getLineCount();
  int lineCountDigits = 0;

  while (lineCount) {
    lineCountDigits++;
    lineCount /= 10;
  }

  if (lineCountDigits < this->config->getLineNumberThresholdWidth()) {
    lineCountDigits = this->config->getLineNumberThresholdWidth();
  }

  // These two features below may be used in future.
  float breakPointMarkWidth = this->config->getBreakPointMarkWidth();
  float blockFoldingMarkWidth = this->config->getBlockFoldingMarkWidth();

  return (lineCountDigits + breakPointMarkWidth + blockFoldingMarkWidth);
}

void EditorSpace::displayLineNumber (int start, int end) {
  float xNumPos, yNumPos, xNumPosPadding;
  sf::Vector2f parentDivPos = Div::getPosition();
  sf::Text num;
  sf::Font editorFont = this->config->getFont();

  sf::Color lineNumberColor(
    this->config->getLineNumberColor().r,
    this->config->getLineNumberColor().g,
    this->config->getLineNumberColor().b
  );

  sf::Color curLineNumberColor(
    this->config->getCurLineNumberColor().r,
    this->config->getCurLineNumberColor().g,
    this->config->getCurLineNumberColor().b
  );

  int lineNumThresh = this->config->getLineNumberThresholdWidth();

  num.setFont(editorFont);
  num.setCharacterSize(this->config->getFontSize());

  int digitCount = 0;
  for (int tmpNum = this->buf->getLineCount(); tmpNum;) {
    digitCount++;
    tmpNum /= 10;
  }

  if (digitCount > lineNumThresh) {
    lineNumThresh = lineNumThresh;
  }

  for (int line = start; line < end; line++) {
    
    std::string lineStr = std::to_string(line + 1);
    xNumPos = this->config->getBreakPointMarkWidth();
    yNumPos = line * this->config->getWordHeight();
    xNumPosPadding = lineNumThresh - lineStr.length() > 0 ? lineNumThresh - lineStr.length() : 0;

    if (line == this->curLine) {
      num.setColor(curLineNumberColor);
    } else {
      num.setColor(lineNumberColor);
    }

    for (int i = 0; i < lineStr.length(); i++) {
      num.setString(lineStr[i]);
      num.setPosition(
        sf::Vector2f(
          parentDivPos.x + (float)(xNumPosPadding + xNumPos++) * this->config->getWordWidth(),
          parentDivPos.y + (float)yNumPos
        )
      );

      this->getWindow()->draw(num);
    }
  }
}

void EditorSpace::loadEditorConfigs() {
  this->setPosition(
    this->config->getEditorXPos(),
    this->config->getEditorYPos()
  );

  this->setSize(
    this->config->getEditorXSize(),
    this->config->getEditorYSize()
  );

  this->adjustView(this->getWindow()->getSize().x, this->getWindow()->getSize().y);
}

void EditorSpace::updateEditorSize () {
  // Updating Editor Space Div size, if required
  int newXSize = this->getSize().x;
  int newYSize = this->buf->getLineCount() * this->config->getWordHeight();

  if (newYSize < this->getWindow()->getSize().y) {
    newYSize = this->getWindow()->getSize().y;
  }

  this->setSize(newXSize, newYSize);
}

void EditorSpace::scrollUpByLines (int lines) {
  sf::View view = this->getWatchableView();

  float viewYPosTop = view.getCenter().y - view.getSize().y / 2.0f;
  float scrollDelta = lines * this->config->getWordHeight();
  float scrollUpThreshold = this->config->getEditorYPos();

  // Stop scroll up when reaching top of the text
  if (viewYPosTop - scrollUpThreshold > scrollDelta) {
    view.move(0.f, -scrollDelta);
  } else {
    view.move(0.f, scrollUpThreshold - viewYPosTop);
  }

  this->setWatchableView(view);
}

void EditorSpace::scrollDownByLines (int lines) {
  sf::View view = this->getWatchableView();

  float viewYPosBottom = view.getCenter().y + view.getSize().y / 2.0f;
  
  float scrollDownThreshold = this->config->getEditorYPos();
  scrollDownThreshold += 
    ((this->buf->getLineCount() - 1) * this->config->getWordHeight());
  scrollDownThreshold += this->config->getEditorYSize();

  float scrollDelta = lines * this->config->getWordHeight();

  // Stop scroll down below last line of text
  if (scrollDownThreshold - viewYPosBottom > scrollDelta) {
    view.move(0.f, scrollDelta);
  } else {
    view.move(0.f, scrollDownThreshold - viewYPosBottom);        
  }

  this->setWatchableView(view);
}

void EditorSpace::save () {
  this->buf->write();
}

int EditorSpace::getTopLineByView (sf::View &view) {
  float viewTop = view.getCenter().y - (view.getSize().y / 2.0f);

  return (viewTop - Div::getPosition().y) / this->config->getWordHeight();
}

int EditorSpace::getBottomLineByView (sf::View &view) {
  float viewBottom = view.getCenter().y + (view.getSize().y / 2.0f);

  int lineBottom =
    (viewBottom - Div::getPosition().y) / this->config->getWordHeight();
  lineBottom =
    lineBottom > this->buf->getLineCount()
    ? this->buf->getLineCount()
    : lineBottom;

  return lineBottom;
}

int EditorSpace::getCursorIndexByLocation(float x, float y) {
  sf::View view = this->getWatchableView();
  int lineTop = this->getTopLineByView(view);

  float viewTopY = view.getCenter().y - view.getSize().y / 2.0f;

  int lineClicked = lineTop + (y - viewTopY) / this->config->getWordHeight();
  if (lineClicked > this->buf->getLineCount()) {
    lineClicked = this->buf->getLineCount();
  }

  int lineStartIndex = this->buf->getLineStartPos(lineClicked);
  int nextLineStartIndex = this->buf->getLineStartPos(lineClicked + 1);

  float lineNumberOffsetX = this->getXTextOffset();

  float xPos = (x
    - this->getXTextOffset() * this->config->getWordWidth()
    - this->getPosition().x);

  float wordGap = xPos / this->config->getWordWidth();
  
  bool shouldAddOneInIndex = false;
  if (0.5f < wordGap - (int)wordGap) {
    shouldAddOneInIndex = true;
  }

  int index = lineStartIndex + wordGap;

  // Rounding index
  if (shouldAddOneInIndex) index++;

  if (index >= nextLineStartIndex) {
    index = nextLineStartIndex - 1;
  }

  return index;
}
