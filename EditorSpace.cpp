#include "EditorSpace.h"

EditorSpace::EditorSpace (std::string &bufferText, GlobalConfig &config, sf::RenderWindow *window): Div(window) {
  this->bufferText = bufferText;
  this->cursorIndex = 0;
  this->showCursor = true;

  this->curLine = 0;
  this->wordsInLineBeforeCursor = 0;

  this->config = config;
  this->langSelected = PlainText;
  this->hideLineNumber = false;
}

void EditorSpace::pollKeyboard (int unicode) {
  // Backspace
  if (unicode == 8) {
    if (this->cursorIndex == this->bufferText.length()) {
      this->bufferText = this->bufferText.substr(0, this->bufferText.length() - 1);
    } else {
      if (this->cursorIndex > 0) {
        this->bufferText = this->bufferText.substr(0, this->cursorIndex - 1)
                          + this->bufferText.substr(this->cursorIndex, this->bufferText.length());
      }
    }

    if (this->cursorIndex > 0) this->cursorIndex--;
    return;
  }

  // Tab - For now its fixed for two spaces
  if (unicode == 9) {
    if (this->cursorIndex == this->bufferText.length()) {
      this->bufferText += "  ";
    } else if (this->cursorIndex == 0) {
      this->bufferText = "  " + this->bufferText;
    } else {
      this->bufferText = this->bufferText.substr(0, this->cursorIndex)
                          + "  "
                          + this->bufferText.substr(this->cursorIndex, this->bufferText.length());
    }

    this->cursorIndex += 2;
    return;
  }

  // Return
  if (unicode == 13) {
    if (this->cursorIndex == this->bufferText.length()) {
      this->bufferText += "\n"; 
    } else if (this->cursorIndex == 0) {
      this->bufferText = "\n" + this->bufferText;
    } else {
      this->bufferText = this->bufferText.substr(0, this->cursorIndex) 
                          + "\n" 
                          + this->bufferText.substr(this->cursorIndex, this->bufferText.length());
    }
    
    this->cursorIndex++;
    return;
  }

  if (unicode >= 32 && unicode <= 126) {
    if (this->cursorIndex == this->bufferText.length()) {
      this->bufferText += (char)(unicode); 
    } else if (this->cursorIndex == 0) {
      this->bufferText = (char)(unicode) + this->bufferText;
    } else {
      this->bufferText = this->bufferText.substr(0, this->cursorIndex)
                          + (char)(unicode)
                          + this->bufferText.substr(this->cursorIndex, this->bufferText.length());
    }

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

  // Resizing Div on window
  if (event.type == sf::Event::Resized) {
    // Adjust editor size
    float newEditorWidth = event.size.width - this->config.getEditorXPos();
    float newEditorHeight = event.size.height - this->config.getEditorYPos();
    this->config.setEditorXSize(newEditorWidth);
    this->config.setEditorYSize(newEditorHeight);

    this->setSize(newEditorWidth, newEditorHeight);

    // Adjusting Div view
    sf::View editorView = this->getWatchableView();

    editorView.reset(sf::FloatRect(
      this->config.getEditorXPos(),
      this->config.getEditorYPos(),
      this->config.getEditorXSize(),
      this->config.getEditorYSize()
    ));

    editorView.setViewport(sf::FloatRect(
      this->config.getEditorXPos() / event.size.width,
      this->config.getEditorYPos() / event.size.height,
      this->config.getEditorXSize() / event.size.width,
      this->config.getEditorYSize() / event.size.height
    ));

    this->setWatchableView(editorView);
    return;
  }

  // Mouse Scroll Events on Editor
  if (event.type == sf::Event::MouseWheelScrolled) {
    sf::View currentView = this->getWatchableView();
    float viewYPosTop = currentView.getCenter().y - currentView.getSize().y / 2.0f;
    float viewYPosBottom = currentView.getCenter().y + currentView.getSize().y / 2.0f;

    // Mouse scroll up
    if (event.mouseWheelScroll.delta > 0) {
      // Stop scroll up when reaching top of the text

      float scrollUpThreshold = this->config.getEditorYPos();

      if (viewYPosTop - scrollUpThreshold > 40.0f) {
        currentView.move(0.f, -40.f);
      } else {
        currentView.move(0.f, scrollUpThreshold - viewYPosTop);
      }
    }

    // Mouse scroll down
    if (event.mouseWheelScroll.delta < 0) {
      // Stop scroll down below last line of text
      float scrollDownThreshold = this->config.getEditorYPos();
      scrollDownThreshold += ((this->getTotalLineCount() - 1) * this->config.getWordHeight());
      scrollDownThreshold += this->config.getEditorYSize();

      if (scrollDownThreshold - viewYPosBottom > 40.0f) {
        currentView.move(0.f, 40.f);
      } else {
        currentView.move(0.f, scrollDownThreshold - viewYPosBottom);        
      }
    }

    this->setWatchableView(currentView);
    return;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    this->showCursor = true;
    this->clock.restart();
    if (this->cursorIndex > 0) this->cursorIndex--;
    return;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    this->showCursor = true;
    this->clock.restart();
    if (this->cursorIndex < this->bufferText.length()) this->cursorIndex++;
    return;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    this->showCursor = true;
    this->clock.restart();

    if (this->curLine == 0) {
      this->cursorIndex = 0;
      return;
    }

    int wordsInLineAbove = this->wordsInLine[this->curLine - 1];
    int wordsTillCursor = this->wordsInLineBeforeCursor;
    this->cursorIndex = this->cursorIndex - wordsTillCursor;
    if (wordsInLineAbove - wordsTillCursor > 0) {
      this->cursorIndex = this->cursorIndex - (wordsInLineAbove - wordsTillCursor);
    }
    this->cursorIndex--;
    this->curLine--;

    return;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    this->showCursor = true;
    this->clock.restart();

    if (this->curLine == this->getTotalLineCount() - 1) {
      this->cursorIndex = this->bufferText.length();
      return;
    }

    int wordsInLineBelow = this->wordsInLine[this->curLine + 1];
    int wordsTillCursor = this->wordsInLineBeforeCursor;
    int wordsInCurLine = this->wordsInLine[this->curLine];
    this->cursorIndex = this->cursorIndex + (wordsInCurLine - wordsTillCursor);
    
    this->cursorIndex += wordsTillCursor < wordsInLineBelow ? wordsTillCursor : wordsInLineBelow;

    this->cursorIndex++;
    this->curLine++;

    return;
  }

  if (event.type == sf::Event::TextEntered) {
    this->showCursor = true;
    this->clock.restart();
    this->pollKeyboard(event.text.unicode);  
  }

  return;
}

std::string EditorSpace::getBufferText () {
  return this->bufferText;
}

void EditorSpace::setLanguage (ProgLang lang) {
  this->langSelected = lang;
}

void EditorSpace::drawOnScreen (sf::RenderWindow &window) {
  Div::drawOnScreen(window);

  sf::Color fontColor(
    this->config.getFontColor().r,
    this->config.getFontColor().g,
    this->config.getFontColor().b
  );
  sf::Font editorFont = this->config.getFont();
  sf::Text word;
  ProgLang languageSelected = this->langSelected;

  int xWordPosition, yWordPosition, xCursorPosition, yCursorPosition;
  float XResetPos;
  int charIndex, lineCount;
  std::string wordText;

  XResetPos = Div::getPosition().x;

  if (!(this->hideLineNumber)) {
    XResetPos += this->getXTextOffset() * this->config.getWordWidth();
    this->displayLineNumber(this->getTotalLineCount());
  }

  xWordPosition = 0;
  yWordPosition = Div::getPosition().y;
  charIndex = 0;
  lineCount = 0;

  // Cursor display settings
  xCursorPosition = 0;
  yCursorPosition = Div::getPosition().y;

  word.setFont(editorFont);
  word.setCharacterSize(this->config.getFontSize());
  word.setColor(fontColor);

  this->wordsInLine.clear();
  Parser parser(this->bufferText);
  while (parser.hasNextToken()) {
    wordText = parser.getToken();

    ColorComponent colorRgb = wordHighlighter(wordText, languageSelected);
    sf::Color curWordColor(colorRgb.r, colorRgb.g, colorRgb.b);
    word.setColor(curWordColor);

    if (wordText == "\n") {
      charIndex++;
      lineCount++;
      this->wordsInLine.push_back(xWordPosition);

      xWordPosition = 0;
      yWordPosition += this->config.getWordHeight();

      // Setting cursor to newline
      if (this->cursorIndex == charIndex) {
        xCursorPosition = xWordPosition;
        yCursorPosition = yWordPosition;
        this->curLine = lineCount;
        this->wordsInLineBeforeCursor = xCursorPosition;
      }

      continue;
    }

    for (int i = 0; i < wordText.length(); i++) {
      float x = XResetPos + (float)(xWordPosition++) * this->config.getWordWidth();
      float y = (float)yWordPosition;
      charIndex++;
      word.setString(wordText[i]);
      word.setPosition(sf::Vector2f(x, y));
      
      //Setting cursor position
      if (this->cursorIndex == charIndex) {
        xCursorPosition = xWordPosition;
        yCursorPosition = yWordPosition;
        this->curLine = lineCount;
        this->wordsInLineBeforeCursor = xCursorPosition;
      }

      window.draw(word);
    }
  }

  // the line below captures last line words
  this->wordsInLine.push_back(xWordPosition);

  this->cursor.setSize(this->config.getCursorWidth(), this->config.getCursorHeight());
  this->cursor.fillColor(fontColor);
  this->cursor.setPosition(
    XResetPos + (float)(xCursorPosition) * this->config.getWordWidth(),
    (float)yCursorPosition + 1.0f
  );

  if (this->clock.getElapsedTime() > sf::milliseconds(this->config.getCursorBlinkTimeInSeconds())) {
    this->showCursor = !this->showCursor;
    this->clock.restart();
  }

  if (this->showCursor) {
    this->cursor.drawOnScreen(window);
  }

  // Updating Editor Space Div size, if required
  int totalLines = this->getTotalLineCount();

  int newXSize = Div::getSize().x;
  int newYSize = totalLines * this->config.getWordHeight();

  if (newYSize < Div::getWindow()->getSize().y) {
    newYSize = Div::getWindow()->getSize().y;
  }

  Div::setSize(newXSize, newYSize);
}

float EditorSpace::getXTextOffset () {
  int lineCount = this->getTotalLineCount();
  int lineCountDigits = 0;

  while (lineCount) {
    lineCountDigits++;
    lineCount /= 10;
  }

  if (lineCountDigits < this->config.getLineNumberThresholdWidth()) {
    lineCountDigits = this->config.getLineNumberThresholdWidth();
  }

  // These two features below may be used in future.
  float breakPointMarkWidth = this->config.getBreakPointMarkWidth();
  float blockFoldingMarkWidth = this->config.getBlockFoldingMarkWidth();

  return (lineCountDigits + breakPointMarkWidth + blockFoldingMarkWidth);
}

void EditorSpace::displayLineNumber (int lineCount) {
  float xNumPos, yNumPos, xNumPosPadding;
  sf::Vector2f parentDivPos = Div::getPosition();
  sf::Text num;
  sf::Font editorFont = this->config.getFont();

  sf::Color lineNumberColor(
    this->config.getLineNumberColor().r,
    this->config.getLineNumberColor().g,
    this->config.getLineNumberColor().b
  );

  sf::Color curLineNumberColor(
    this->config.getCurLineNumberColor().r,
    this->config.getCurLineNumberColor().g,
    this->config.getCurLineNumberColor().b
  );

  int lineNumThresh = this->config.getLineNumberThresholdWidth();

  num.setFont(editorFont);
  num.setCharacterSize(this->config.getFontSize());

  int digitCount = 0;
  for (int tmpNum = lineCount; tmpNum;) {
    digitCount++;
    tmpNum /= 10;
  }

  if (digitCount > lineNumThresh) {
    lineNumThresh = lineNumThresh;
  }

  for (int line = 0; line < lineCount; line++) {
    
    std::string lineStr = std::to_string(line + 1);
    xNumPos = this->config.getBreakPointMarkWidth();
    yNumPos = line * this->config.getWordHeight();
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
          parentDivPos.x + (float)(xNumPosPadding + xNumPos++) * this->config.getWordWidth(),
          parentDivPos.y + (float)yNumPos
        )
      );

      Div::getWindow()->draw(num);
    }
  }
}

int EditorSpace::getTotalLineCount () {
  return this->wordsInLine.size();
}

void EditorSpace::setWatchableView (sf::View &view) {
  this->watchableView = view;
}

sf::View &EditorSpace::getWatchableView () {
  return this->watchableView;
}
