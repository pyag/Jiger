#include "Explorer.h"
#include "FileOperations.h"

Explorer::Explorer (GlobalConfig *config, sf::RenderWindow *window): Div(window) {
  this->config = config;
  this->workplace = NULL;
  this->activeDataNodeId = -1;

  this->tabTray = new TabTray(config, Div::getWindow());
  this->tabTray->loadConfigs();
  this->tabTray->registerExplorerActiveDnId(&this->activeDataNodeId);

  this->excludedFilePatterns.push_back(".");
  this->dataNodeId = 0;
}

void Explorer::pollUserEvents (sf::Event &event) {
  Div::pollEvents(event);

  for (int i = 0; i < this->fileDivs.size(); i++) {
    this->fileDivs[i]->pollUserEvents(event);
    this->fileDivs[i]->onHover(&this->getWatchableView());

    // Checks onclick event
    if (this->fileDivs[i]->onClick(event, &this->getWatchableView())) {
      // Open editor according to the Data Node clicked

      int clickedDnId = this->fileDivs[i]->dn->id;

      if (this->openEditors.find(clickedDnId) != this->openEditors.end()) {
        this->activeDataNodeId = clickedDnId;
        this->openEditors[clickedDnId]->loadEditorConfigs();
        this->tabTray->setActiveTab(clickedDnId);
        return;
      }

      EditorSpace *newEditor = new EditorSpace(
        this->fileDivs[i]->dn->fullpath,
        this->config,
        this->getWindow()
      );
      newEditor->loadEditorConfigs();

      this->activeDataNodeId = clickedDnId;
      this->openEditors[clickedDnId] = newEditor;
      this->tabTray->push(this->fileDivs[i]->dn->filename, clickedDnId, newEditor);

      return;
    }
  }

  // Resizing Div on window
  if (event.type == sf::Event::Resized) {
    // Adjust editor size
    float newExplorerWidth = this->config->getEditorXPos();
    float newExplorerHeight = event.size.height;

    this->config->setExplorerXSize(newExplorerWidth);
    this->config->setExplorerYSize(newExplorerHeight);

    this->setSize(newExplorerWidth, newExplorerHeight);

    this->adjustView(event.size.width, event.size.height);
    return;
  }

  // Mouse Scroll Events on Editor
  if (event.type == sf::Event::MouseWheelScrolled) {
    if (!this->mouseInMyArea(
      (float)sf::Mouse::getPosition(*this->getWindow()).x,
      (float)sf::Mouse::getPosition(*this->getWindow()).y
    )) return;

    sf::View currentView = this->getWatchableView();
    float viewYPosTop = currentView.getCenter().y - currentView.getSize().y / 2.0f;
    float viewYPosBottom = currentView.getCenter().y + currentView.getSize().y / 2.0f;

    // Mouse scroll up
    if (event.mouseWheelScroll.delta > 0) {
      // Stop scroll up when reaching top of the text

      float scrollUpThreshold = this->config->getExplorerYPos();

      if (viewYPosTop - scrollUpThreshold > 40.0f) {
        currentView.move(0.f, -40.f);
      } else {
        currentView.move(0.f, scrollUpThreshold - viewYPosTop);
      }
    }

    // Mouse scroll down
    if (event.mouseWheelScroll.delta < 0) {
      // Stop scroll down below last line of text
      float scrollDownThreshold = this->config->getExplorerYPos();
      scrollDownThreshold += this->config->getExplorerYSize() + 500.0f;

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
    dn->id = this->dataNodeId++;

    DataNodeElement *displayDn = new DataNodeElement(
      dn,
      this->config,
      this->getWindow()
    );

    this->fileDivs.push_back(displayDn);
  }
}

void Explorer::drawOnScreen () {
  Div::drawOnScreen();

  if (this->workplace == NULL) {
    return;
  }

  float wordWidth = this->config->getExplorerWordWidth();
  float wordHeight = this->config->getExplorerWordHeight();

  float parentDivXPos = this->getPosition().x;
  float parentDivYPos = this->getPosition().y;

  float leftXPadding = 25.0f;
  float topYPadding = 50.0f;

  parentDivXPos += leftXPadding;
  parentDivYPos += topYPadding;

  sf::Color fontColor(
    this->config->getExplorerFontColor().r,
    this->config->getExplorerFontColor().g,
    this->config->getExplorerFontColor().b
  );
  sf::Font explorerFont = this->config->getExplorerFont();

  sf::Text word;

  word.setFont(explorerFont);
  word.setCharacterSize(this->config->getExplorerFontSize());
  word.setColor(fontColor);

  float paintXPos = 0.0f;
  float paintYPos = 0.0f;

  float heightBuf = 2.0f;

  for (int i = 0; i < this->workplace->children.size(); i++) {
    paintYPos = i * wordHeight;

    DataNodeElement *dnDiv = this->fileDivs[i];

    dnDiv->setPosition(
      this->getPosition().x,
      parentDivYPos + paintYPos
    );
    dnDiv->setSize(this->getSize().x, wordHeight);
    dnDiv->drawOnScreen();

    std::string &filename = this->workplace->children[i]->filename;

    word.setString(filename);
    word.setPosition(sf::Vector2f(
      parentDivXPos + paintXPos,
      parentDivYPos + paintYPos + heightBuf
    ));

    this->getWindow()->draw(word);
  }
}

bool Explorer::isAnyEditorActive () {
  return (this->activeDataNodeId != -1);
}
