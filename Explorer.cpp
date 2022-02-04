#include "Explorer.h"
#include "FileOperations.h"

Explorer::Explorer (GlobalConfig &config, sf::RenderWindow *window): Div(window) {
  this->config = config;
  this->workplace = NULL;
  this->activeEditor = NULL;

  this->excludedFilePatterns.push_back(".");
}

void Explorer::pollUserEvents (sf::Event &event) {
  Div::pollEvents(event);

  for (int i = 0; i < this->fileDivs.size(); i++) {
    this->fileDivs[i]->pollUserEvents(event);

    // Checks onclick event
    if (this->fileDivs[i]->onClick(event)) {
      
      // Open editor according to the Data Node clicked
      std::string fileData = readFile(this->fileDivs[i]->dn->fullpath);
      EditorSpace *newEditor = new EditorSpace(
        fileData,
        this->config,
        Div::getWindow()
      );
      newEditor->loadEditorConfigs();
      
      this->activeEditor = newEditor;
      // this->openEditors[this->fileDivs[i]->dn] = newEditor;

      return;
    }
  }

  // Resizing Div on window
  if (event.type == sf::Event::Resized) {
    // Adjust editor size
    float newExplorerWidth = this->config.getEditorXPos();
    float newExplorerHeight = event.size.height;

    this->config.setExplorerXSize(newExplorerWidth);
    this->config.setExplorerYSize(newExplorerHeight);

    this->setSize(newExplorerWidth, newExplorerHeight);

    // Adjusting Div view
    sf::View explorerView = this->getWatchableView();

    explorerView.reset(sf::FloatRect(
      this->config.getExplorerXPos(),
      this->config.getExplorerYPos(),
      this->config.getExplorerXSize(),
      this->config.getExplorerYSize()
    ));

    explorerView.setViewport(sf::FloatRect(
      this->config.getExplorerXPos() / event.size.width,
      this->config.getExplorerYPos() / event.size.height,
      this->config.getExplorerXSize() / event.size.width,
      this->config.getExplorerYSize() / event.size.height
    ));

    this->setWatchableView(explorerView);
    return;
  }

  // Mouse Scroll Events on Editor
  if (event.type == sf::Event::MouseWheelScrolled) {
    if (!Div::mouseInMyArea(
      (float)sf::Mouse::getPosition(*Div::getWindow()).x,
      (float)sf::Mouse::getPosition(*Div::getWindow()).y
    )) return;

    sf::View currentView = this->getWatchableView();
    float viewYPosTop = currentView.getCenter().y - currentView.getSize().y / 2.0f;
    float viewYPosBottom = currentView.getCenter().y + currentView.getSize().y / 2.0f;

    // Mouse scroll up
    if (event.mouseWheelScroll.delta > 0) {
      // Stop scroll up when reaching top of the text

      float scrollUpThreshold = this->config.getExplorerYPos();

      if (viewYPosTop - scrollUpThreshold > 40.0f) {
        currentView.move(0.f, -40.f);
      } else {
        currentView.move(0.f, scrollUpThreshold - viewYPosTop);
      }
    }

    // Mouse scroll down
    if (event.mouseWheelScroll.delta < 0) {
      // Stop scroll down below last line of text
      float scrollDownThreshold = this->config.getExplorerYPos();
      scrollDownThreshold += this->config.getExplorerYSize() + 500.0f;

      if (scrollDownThreshold - viewYPosBottom > 40.0f) {
        currentView.move(0.f, 40.f);
      } else {
        currentView.move(0.f, scrollDownThreshold - viewYPosBottom);        
      }
    }

    this->setWatchableView(currentView);
    return;
  }
}

void Explorer::loadWorkPlace (const std::string &path) {
  this->workplace = new DataNode();
  this->workplace->fullpath = path;
  this->workplace->populate(this->excludedFilePatterns);

  for (int i = 0; i < this->workplace->children.size(); i++) {
    DataNode *dn = this->workplace->children[i];
    DataNodeElement *displayDn = new DataNodeElement(
      dn,
      this->config,
      Div::getWindow()
    );
    this->fileDivs.push_back(displayDn);
  }
}

void Explorer::setWatchableView (sf::View &view) {
  this->watchableView = view;
}

sf::View &Explorer::getWatchableView () {
  return this->watchableView;
}

void Explorer::drawOnScreen (sf::RenderWindow &window) {
  Div::drawOnScreen(window);

  if (this->workplace == NULL) {
    return;
  }

  float wordWidth = this->config.getExplorerWordWidth();
  float wordHeight = this->config.getExplorerWordHeight();

  float parentDivXPos = Div::getPosition().x;
  float parentDivYPos = Div::getPosition().y;

  float leftXPadding = 25.0f;
  float topYPadding = 50.0f;

  parentDivXPos += leftXPadding;
  parentDivYPos += topYPadding;

  sf::Color fontColor(
    this->config.getExplorerFontColor().r,
    this->config.getExplorerFontColor().g,
    this->config.getExplorerFontColor().b
  );
  sf::Font explorerFont = this->config.getExplorerFont();

  sf::Text word;

  word.setFont(explorerFont);
  word.setCharacterSize(this->config.getExplorerFontSize());
  word.setColor(fontColor);

  float paintXPos = 0.0f;
  float paintYPos = 0.0f;

  float heightBuf = 2.0f;

  for (int i = 0; i < this->workplace->children.size(); i++) {
    paintYPos = i * wordHeight;

    DataNodeElement *dnDiv = this->fileDivs[i];

    dnDiv->setPosition(
      Div::getPosition().x,
      parentDivYPos + paintYPos
    );
    dnDiv->setSize(Div::getSize().x, wordHeight);
    dnDiv->drawOnScreen(*Div::getWindow());

    std::string &filename = this->workplace->children[i]->filename;

    word.setString(filename);
    word.setPosition(sf::Vector2f(
      parentDivXPos + paintXPos,
      parentDivYPos + paintYPos + heightBuf
    ));

    window.draw(word);
  }

}
