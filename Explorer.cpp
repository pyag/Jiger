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
  this->globalDataNodeIteratorIndex = 0;
}

// Callback for polling events on single Datanode
void Explorer::pollUserEventsOnSingleDn (Explorer *self, std::vector <void *> args) {
  DataNode *dn = (DataNode *) args[2];
  sf::Event *event = (sf::Event *) args[4];

  dn->pollUserEvents(*event);
  dn->onHover(&self->getWatchableView());

  // Check onclick event
  if (dn->onClick(*event, &self->getWatchableView())) {

    if (dn->isDirectory) {

      if (!dn->isExpanded) {
      // Open file/folders inside the DataNode
        if (!dn->children.size()) {
          self->populateDataNode(dn);
        }
        dn->isExpanded = true;
      } else {
        self->hideDataNode(dn);
      }

    } else {
      // Open editor for the DataNode
      self->openNewTab(dn);
    }
  }
}

void Explorer::pollUserEvents (sf::Event &event) {
  Div::pollEvents(event);

  {
    std::vector <void *> args;
    int depth = 0;

    args.push_back(&this->globalDataNodeIteratorIndex);
    args.push_back(this->workplace);
    args.push_back(this->workplace);
    args.push_back(&depth);
    args.push_back(&event);

    this->iteratorDataNode(this->pollUserEventsOnSingleDn, args);
    this->globalDataNodeIteratorIndex = 0;
  }

  // Resizing Div on window
  if (event.type == sf::Event::Resized) {
    // Adjust editor size
    float newExplorerWidth = this->config->getEditorXPos();
    float newExplorerHeight = event.size.height;

    this->config->setExplorerXSize(newExplorerWidth);
    this->config->setExplorerYSize(newExplorerHeight);

    float prevHeight = this->getSize().y;
    this->setSize(newExplorerWidth, newExplorerHeight);

    this->adjustView(event.size.width, event.size.height);

    this->setSize(newExplorerWidth, prevHeight);
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
      float scrollUpThreshold = this->getPosition().y;

      if (viewYPosTop - scrollUpThreshold > 40.0f) {
        currentView.move(0.f, -40.f);
      } else {
        currentView.move(0.f, scrollUpThreshold - viewYPosTop);
      }
    }

    // Mouse scroll down
    if (event.mouseWheelScroll.delta < 0) {
      float scrollDownThreshold = this->getPosition().y;
      scrollDownThreshold += this->getSize().y + 10.0f;

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
  this->workplace = new DataNode(path, this->config, this->getWindow());
  this->populateDataNode(this->workplace);

  if (this->workplace != NULL) {
    this->workplace->isExpanded = true;
  }
}

void Explorer::deleteDataNodePointers (Explorer *self, std::vector <void *> args) {
  DataNode *dn = (DataNode *) args[2];
  delete dn;
}

void Explorer::offLoadWorkPlace () {
  std::vector <void *> args;
  int depth = 0;

  args.push_back(&this->globalDataNodeIteratorIndex);
  args.push_back(this->workplace);
  args.push_back(this->workplace);
  args.push_back(&depth);

  this->iteratorDataNode(this->deleteDataNodePointers, args);
  this->globalDataNodeIteratorIndex = 0;

  delete this->workplace;
}

void Explorer::populateDataNode (DataNode *dn) {
  dn->populate(this->excludedFilePatterns);

  for (int i = 0; i < dn->children.size(); i++) {
    DataNode *child = dn->children[i];
    child->id = this->dataNodeId++;
  }
}

void Explorer::hideDataNode (DataNode *dn) {
  dn->isExpanded = false;
}

void Explorer::drawOnScreen () {
  Div::drawOnScreen();

  if (this->workplace == NULL) {
    return;
  }

  float wordHeight = this->config->getExplorerWordHeight();

  this->drawDataNodeTree(this->workplace);

  // Updating Explorer size
  this->setSize(
    this->getSize().x,
    this->getPosition().y + (this->dataNodeId * wordHeight) + wordHeight
  );
}

bool Explorer::isAnyEditorActive () {
  return (this->activeDataNodeId != -1);
}

void Explorer::openNewTab (DataNode *dn) {
  int id = dn->id;

  if (this->openEditors.find(id) != this->openEditors.end()) {
    this->activeDataNodeId = id;
    this->openEditors[id]->loadEditorConfigs();
    this->tabTray->setActiveTab(id);
    return;
  }

  EditorSpace *newEditor = new EditorSpace(
    dn->fullpath,
    this->config,
    this->getWindow()
  );
  newEditor->loadEditorConfigs();

  this->activeDataNodeId = id;
  this->openEditors[id] = newEditor;
  this->tabTray->push(dn->filename, id, newEditor);
}

void Explorer::drawSingleDataNode (Explorer *self, std::vector <void *> args) {
  DataNode *dn = (DataNode *) args[2];
  int depth = *((int *) args[3]);

  if (dn == NULL) {
    return;
  }

  dn->setPosition(
    self->getPosition().x,
    self->getPosition().y +
      self->globalDataNodeIteratorIndex * self->config->getExplorerWordHeight()
  );

  dn->setSize(
    self->getSize().x,
    self->config->getExplorerWordHeight()
  );

  if (dn->id == self->activeDataNodeId) {
    dn->fillColor(sf::Color(80, 80, 80, 100));
  }

  dn->drawOnScreen(depth);
}

void Explorer::drawDataNodeTree (DataNode *dn) {

  std::vector <void *> args;
  int depth = 0;

  args.push_back(&this->globalDataNodeIteratorIndex);
  args.push_back(this->workplace);
  args.push_back(this->workplace);
  args.push_back(&depth);


  this->iteratorDataNode(this->drawSingleDataNode, args);
  this->globalDataNodeIteratorIndex = 0;
}

/*
 * args array contains arguments for iterator
 * Indexes:
 *  - 0: index for iterator
 *  - 1: Root node for Data Node
 *  - 2: Iterator to point current Data Node
 *
 */
void Explorer::iteratorDataNode (void (*callback)(Explorer *, std::vector <void *>),
  std::vector <void *> &args) {

  int *index = (int *) args[0];
  DataNode *root = (DataNode *) args[1];
  DataNode *dn = (DataNode *) args[2];
  int depth = *((int *) args[3]);

  if (dn == NULL) {
    return;
  }

  if (dn != root) {
    callback(this, args);
  }

  (*index)++;

  if (dn->isDirectory && dn->isExpanded) {
    for (int i = 0; i < (int) dn->children.size(); i++) {
      args[2] = dn->children[i];
      *((int *) args[3]) = depth + 1;

      this->iteratorDataNode(callback, args);

      *((int *) args[3]) = depth;
    }
  }
}

void Explorer::cleanUp () {
  this->offLoadWorkPlace();
  delete this->tabTray;  
}

Explorer::~Explorer () {
  this->cleanUp();
}
