#include "../headers/EditorSpace.h"

EditorSpace::EditorSpace (std::string &fileLoc, GlobalConfig *config, sf::RenderWindow *window): Div(window) {
  this->fileLoc = fileLoc;

  this->buf = new Buffer(this->fileLoc);
  this->buf->loadBuffer();

  this->cursor = new Div(window);
  this->cursorIndex = 0;
  this->curLine = 0;

  this->mousePointerType = sf::Cursor::Text;

  this->showCursor = true;

  this->config = config;
  this->langSelected = PlainText;
  this->hideLineNumber = false;

  this->leftMouseClicked = false;
  this->selectionStartIndex = -1;
  this->selectionEndIndex = -1;

  this->isPasted = false;
  this->isCopied = false;
  this->isSaved = false;

  this->setPosition(
    this->config->getEditorXPos(),
    this->config->getEditorYPos()
  );
}

void EditorSpace::pollKeyboard (int unicode) {
  // Backspace
  if (unicode == 8) {

    if (this->isAnythingSelected()) {
      this->buf->remove(
        this->getSelectionLower(),
        this->getSelectionHigher()
      );

      this->cursorIndex = this->getSelectionLower();
      this->buf->syncLineBreaks();
    } else {
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
    }

    return;
  }

  // Tab - For now its fixed for two spaces
  if (unicode == 9) {

    std::string tabs = "  ";

    if (this->isAnythingSelected()) {
      this->buf->remove(
        this->getSelectionLower(),
        this->getSelectionHigher()
      );
      this->cursorIndex = this->getSelectionLower();
    }

    this->buf->insert(tabs, this->cursorIndex);
    this->cursorIndex += 2;
    return;
  }

  // Return
  if (unicode == 13) {
    std::string returnChar = "\n";

    if (this->isAnythingSelected()) {
      this->buf->remove(
        this->getSelectionLower(),
        this->getSelectionHigher()
      );
      this->cursorIndex = this->getSelectionLower();
    }

    this->buf->insert(returnChar, this->cursorIndex);
    this->buf->syncLineBreaks();
    
    this->cursorIndex++;
    return;
  }

  if (unicode >= 32 && unicode <= 126) {
    std::string newChar;
    newChar = (char)(unicode);

    if (this->isAnythingSelected()) {
      this->buf->remove(
        this->getSelectionLower(),
        this->getSelectionHigher()
      );
      this->cursorIndex = this->getSelectionLower();
    }

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
      if (!this->leftMouseClicked) {

        sf::Vector2i coord = sf::Mouse::getPosition(*this->getWindow());
        sf::Vector2f mPos = this->getWindow()
          ->mapPixelToCoords(coord, this->getWatchableView());

        if (!this->mouseInMyArea(mPos.x, mPos.y)) return;
        this->cursorIndex = this->getCursorIndexByLocation(mPos.x, mPos.y);

        this->leftMouseClicked = true;
        this->selectionStartIndex = this->cursorIndex;
        this->selectionEndIndex = this->cursorIndex;

      }
    
    }

    return;
  }

  if (event.type == sf::Event::MouseButtonReleased) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      if (this->leftMouseClicked) {
        this->leftMouseClicked = false;
      }
    }

    return;
  }

  if (event.type == sf::Event::MouseMoved) {
    if (this->leftMouseClicked) {
      this->showCursor = true;
      this->clock.restart();

      sf::Vector2i coord(event.mouseMove.x, event.mouseMove.y);
      sf::Vector2f mPos = this->getWindow()
        ->mapPixelToCoords(coord, this->getWatchableView());
      
      this->cursorIndex = this->getCursorIndexByLocation(mPos.x, mPos.y);
      this->selectionEndIndex = this->cursorIndex;
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

  // Ctrl + C - Copy Selected text
  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
    || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
    && sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {

    if (!this->isCopied) {
      if (this->isAnythingSelected()) {
        this->isCopied = true;
        const std::string &selectedText = this->buf->getSourceByPositions(
          getSelectionLower(),
          getSelectionHigher()
        );

        sf::Clipboard::setString(selectedText);
      }
    }

    return;
  }

  // Ctrl + V - Paste Selected text
  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
    || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
    && sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {

    if (!this->isPasted) {
      this->isPasted = true;
      std::string copiedText = sf::Clipboard::getString();

      if (this->isAnythingSelected()) {
        this->buf->remove(
          this->getSelectionLower(),
          this->getSelectionHigher()
        );
        this->cursorIndex = this->getSelectionLower();
      }

      this->buf->insert(copiedText, this->cursorIndex);
      this->cursorIndex += copiedText.length();

      this->removeSelection();
    }

    return;
  }

  // Ctrl + A - Select All
  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
    || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
    && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    this->selectionStartIndex = 0;
    this->selectionEndIndex = this->buf->getBufferLength();
    return;
  }

  // Ctrl + S - Save
  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
    || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
    && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    if (!this->isSaved) {
      std::cout << "Is Saved Called!!\n";
      this->isSaved = true;
      this->save();
    }
    return;
  }

  // Ctrl + Right - Hop next word
  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
    || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
    && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {

    this->showCursor = true;
    this->clock.restart();

    std::cout << "Clicked left + ctrl\n";
    this->cursorIndex = this->hoppingAhead();
    return;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    this->showCursor = true;
    this->clock.restart();

    if (this->isAnythingSelected()) {
      this->cursorIndex = this->getSelectionLower();
    } else {
      if (this->cursorIndex > 0) {
        this->cursorIndex--;
      }
    }

    this->removeSelection();
    return;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    this->showCursor = true;
    this->clock.restart();

    if (this->isAnythingSelected()) {
      this->cursorIndex = this->getSelectionHigher();
    } else {
      if (this->cursorIndex < this->buf->getBufferLength()) {
        this->cursorIndex++;
      }
    }

    this->removeSelection();
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
    this->removeSelection();
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

    this->removeSelection();
    return;
  }

  if (event.type == sf::Event::KeyReleased) {
    if (event.key.code == sf::Keyboard::V) {
      this->isPasted = false;
      return;
    }

    if (event.key.code == sf::Keyboard::C) {
      this->isCopied = false;
      return;
    }

    if (event.key.code == sf::Keyboard::S) {
      this->isSaved = false;
    }

    return;
  }

  if (event.type == sf::Event::TextEntered) {
    this->showCursor = true;
    this->clock.restart();
    this->pollKeyboard(event.text.unicode);
    this->removeSelection();
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
  word.setFillColor(fontColor);

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
  parser->loadBuffer(this->buf, startIndex, endIndex);

  int runningIndex = startIndex;

  while (parser->hasNextToken()) {
    wordText = parser->getToken();

    ColorComponent colorRgb = wordHighlighter(wordText, languageSelected);
    sf::Color curWordColor(colorRgb.r, colorRgb.g, colorRgb.b);
    word.setFillColor(curWordColor);

    if (wordText == "\n") {
      selectIfRequired(
        runningIndex,
        XResetPos + (float)(xWordPosition) * this->config->getWordWidth(),
        (float)yWordPosition
      );
      runningIndex++;
      xWordPosition = 0;
      yWordPosition += this->config->getWordHeight();
      continue;
    }

    for (int i = 0; i < wordText.length(); i++) {
      float x = XResetPos + (float)(xWordPosition++) * this->config->getWordWidth();
      float y = (float)yWordPosition;

      word.setString(wordText[i]);
      word.setPosition(sf::Vector2f(x, y));

      selectIfRequired(runningIndex, x, y);
      this->getWindow()->draw(word);

      runningIndex++;
    }
  }

  delete parser;

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

  this->highlightCurLine(lineTop, lineBottom);

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
    xNumPosPadding = lineNumThresh - lineStr.length() > 0 
      ? lineNumThresh - lineStr.length() 
      : 0;

    if (line == this->curLine) {
      num.setFillColor(curLineNumberColor);
    } else {
      num.setFillColor(lineNumberColor);
    }

    for (int i = 0; i < lineStr.length(); i++) {
      num.setString(lineStr[i]);
      num.setPosition(
        sf::Vector2f(
          parentDivPos.x 
            +(float)(xNumPosPadding + xNumPos++) * this->config->getWordWidth(),
          parentDivPos.y + (float)yNumPos
        )
      );

      this->getWindow()->draw(num);
    }
  }
}

void EditorSpace::loadEditorConfigs() {
  float windowWidth = this->getWindow()->getSize().x;
  float windowHeight = this->getWindow()->getSize().y;

  float newEditorWidth = windowWidth - this->config->getEditorXPos();
  float newEditorHeight = windowHeight - this->config->getEditorYPos();

  this->config->setEditorXSize(newEditorWidth);
  this->config->setEditorYSize(newEditorHeight);
  this->setSize(newEditorWidth, newEditorHeight);

  this->adjustView(windowWidth, windowHeight);
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

bool EditorSpace::insideSelectionArea (int index) {
  int low = this->selectionStartIndex;
  int high = this->selectionEndIndex;

  if (low == high) return false;
  if (low > high) {
    std::swap(low, high);
  }

  return (low <= index && index < high);
}

void EditorSpace::selectIfRequired (int index, float x, float y) {
  if (this->insideSelectionArea(index)) {
    sf::RectangleShape background(
      sf::Vector2f(this->config->getWordWidth(), this->config->getWordHeight())
    );
    background.setPosition(sf::Vector2f(x, y));
    background.setFillColor(sf::Color(65, 105, 225, 90));

    this->getWindow()->draw(background);
  }
}

void EditorSpace::removeSelection () {
  this->selectionStartIndex = this->selectionEndIndex = -1;
}

bool EditorSpace::isAnythingSelected () {
  int low = this->selectionStartIndex;
  int high = this->selectionEndIndex;

  if (low > high) {
    std::swap(low, high);
  }

  return (low < high && low >= 0 && high <= this->buf->getBufferLength());
}

int EditorSpace::getSelectionLower () {
  return std::min(this->selectionStartIndex, this->selectionEndIndex);
}

int EditorSpace::getSelectionHigher () {
  return std::max(this->selectionStartIndex, this->selectionEndIndex);
}

void EditorSpace::highlightCurLine (int lineTop, int lineBottom) {
  if ((this->curLine >= lineTop && this->curLine <= lineBottom)
    && !this->isAnythingSelected()) {
    float highlightLineY = Div::getPosition().y +
      this->curLine * this->config->getWordHeight();

    Div hightlighCurLinetDiv;
    hightlighCurLinetDiv.setWindow(this->getWindow());

    hightlighCurLinetDiv.setSize(
      this->getSize().x,
      this->config->getWordHeight()
    );

    hightlighCurLinetDiv.setPosition(
      this->getPosition().x,
      highlightLineY
    );

    hightlighCurLinetDiv.fillColor(sf::Color(
      this->config->getHightlightCurLineColor().r,
      this->config->getHightlightCurLineColor().g,
      this->config->getHightlightCurLineColor().b,
      this->config->getHightlightCurLineColor().opacity
    ));

    hightlighCurLinetDiv.drawOnScreen();
  }
}

/*
 *  Hop ahead hops cursor to next group of
 *  word.
 *  Groups are:
 *  -> Numbers + Alphabet
 *  -> Whitespace + Return(Enter Key)
 *  -> Special chars
 */
int EditorSpace::hoppingAhead () {
  int index = this->cursorIndex;
  int len = this->buf->getBufferLength();

  if (this->buf->empty()) {
    return 0;
  }

  char curChar = this->buf->getCharAtPos(index);

  if (this->isAlphaNumeric(curChar)) {
    while ((index < len) && this->isAlphaNumeric(curChar)) {
      index++;
      curChar = this->buf->getCharAtPos(index);
    }

    return index;
  }

  if (this->isSpaceOrReturn(curChar)) {
    while ((index < len) && this->isSpaceOrReturn(curChar)) {
      index++;
      curChar = this->buf->getCharAtPos(index);
    }

    return index;
  }

  while ((index < len)
    && !this->isAlphaNumeric(curChar)
    && !this->isSpaceOrReturn(curChar)) {
    index++;
    curChar = this->buf->getCharAtPos(index);
  }

  return index;
}

int EditorSpace::hoppingBehind () {

}

bool EditorSpace::isAlphaNumeric (char ch) {
  return ((ch >= 'a' && ch <= 'z') 
    || (ch >= 'A' && ch <= 'Z')
    || (ch >= '0' && ch <= '9'));
}

bool EditorSpace::isSpaceOrReturn (char ch) {
  return (ch == ' ' || ch == '\n');
}

void EditorSpace::cleanUp () {
  delete this->buf;
  delete this->cursor;
}

EditorSpace::~EditorSpace () {
  this->cleanUp();
}
