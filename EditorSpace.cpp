#include "EditorSpace.h"

EditorSpace::EditorSpace (std::string &bufferText, GlobalConfig &config) {
  this->bufferText = bufferText;
  this->cursorIndex = 0;
  this->showCursor = true;

  this->curLine = 0;
  this->wordsInLineBeforeCursor = 0;

  this->config = config;
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

  // Return
  if (unicode == 13) {
    int index = 0;
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

void EditorSpace::pollMouse () {
  return;
}

void EditorSpace::pollUserEvents (sf::Event &event) {
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

    if (this->curLine == this->wordsInLine.size() - 1) {
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

  this->pollMouse();
  return;
}

std::string EditorSpace::getBufferText () {
  return this->bufferText;
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

  int xWordPosition, yWordPosition, xCursorPosition, yCursorPosition;
  int charIndex, lineCount;
  std::string wordText;

  xWordPosition = 0;
  yWordPosition = 0;
  charIndex = 0;
  lineCount = 0;

  // Cursor display settings
  xCursorPosition = 0;
  yCursorPosition = 0;

  word.setFont(editorFont);
  word.setCharacterSize(this->config.getFontSize());
  word.setColor(fontColor);

  sf::Vector2f parentDivPos = Div::getPosition();

  this->wordsInLine.clear();
  Parser parser(this->bufferText);
  while (parser.hasNextToken()) {
    wordText = parser.getToken();

    if (wordText == "int" || wordText == "string" || wordText == "std" || wordText == "return") {
      word.setColor(sf::Color::Black);
    } else {
      word.setColor(fontColor);
    }

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
      charIndex++;
      word.setString(wordText[i]);
      word.setPosition(
        sf::Vector2f(
          parentDivPos.x + (float)(xWordPosition++) * this->config.getWordWidth(), 
          parentDivPos.y + (float)yWordPosition
        )
      );
      
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
    parentDivPos.x + (float)(xCursorPosition) * this->config.getWordWidth(),
    parentDivPos.y + (float)yCursorPosition + 2.0f
  );

  if (this->clock.getElapsedTime() > sf::milliseconds(this->config.getCursorBlinkTimeInSeconds())) {
    this->showCursor = !this->showCursor;
    this->clock.restart();
  }

  if (this->showCursor) {
    this->cursor.drawOnScreen(window);
  }
}
