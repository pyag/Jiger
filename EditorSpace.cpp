#include "EditorSpace.h"

EditorSpace::EditorSpace (std::string &bufferText) {
  this->bufferText = bufferText;
  this->cursorIndex = 0;
  this->showCursor = true;
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

  sf::Color fontColor(235, 238, 235);
  sf::Font editorFont;
  sf::Text word;

  int fontSize, xWordPosition, yWordPosition, xCursorPosition, yCursorPosition;
  int charIndex;
  float wordWidth, wordHeight, cursorHeight, cursorWidth;
  std::string wordText;

  if (!editorFont.loadFromFile("./fonts/consolas/consola.ttf")) {
    std::cout << "Font cannot be loaded!\n";
    exit(1);
  }

  fontSize = 18;
  xWordPosition = 0;
  yWordPosition = 0;
  wordWidth = 10.0f;
  wordHeight = 19.0f;
  charIndex = 0;

  // Cursor display settings
  cursorHeight = 22.0f;
  cursorWidth = 2.0f;
  xCursorPosition = 0;
  yCursorPosition = 0;

  word.setFont(editorFont);
  word.setCharacterSize(fontSize);
  word.setColor(fontColor);

  sf::Vector2f parentDivPos = Div::getPosition();

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
      xWordPosition = 0;
      yWordPosition += wordHeight;

      // Setting cursor to newline
      if (this->cursorIndex == charIndex) {
        xCursorPosition = xWordPosition;
        yCursorPosition = yWordPosition;
      }

      continue;
    }

    for (int i = 0; i < wordText.length(); i++) {
      charIndex++;
      word.setString(wordText[i]);
      word.setPosition(sf::Vector2f(parentDivPos.x + (float)(xWordPosition++) * wordWidth, parentDivPos.y + (float)yWordPosition));
      
      //Setting cursor position
      if (this->cursorIndex == charIndex) {
        xCursorPosition = xWordPosition;
        yCursorPosition = yWordPosition;
      }

      window.draw(word);
    }
  }

  this->cursor.setSize(cursorWidth, cursorHeight);
  this->cursor.fillColor(fontColor);
  this->cursor.setPosition(parentDivPos.x + (float)(xCursorPosition) * wordWidth, parentDivPos.y + (float)yCursorPosition + 2.0f);


  if (this->clock.getElapsedTime() > sf::milliseconds(550)) {
    this->showCursor = !this->showCursor;
    this->clock.restart();
  }

  if (this->showCursor) {
    this->cursor.drawOnScreen(window);
  }
}
