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

  {
    std::map <DataNode *, DataNodeElement *>::iterator it;
    for (it = this->fileDivs.begin(); it != this->fileDivs.end(); it++) {
      it->second->pollUserEvents(event);
      it->second->onHover(&this->getWatchableView());

      // Check onclick event
      if (it->second->onClick(event, &this->getWatchableView())) {

        // Open editor according to the Data Node
        if (it->first->isDirectory) {

          if (!it->second->isExpanded) {
            // Open file/folders inside the DataNode
            this->populateDataNode(it->first);
          } else {
            // Clear all the populated children in Datanode
            // This deallocates the memory
            // Could have used .clear(), but it does not deallocates memory
            std::vector <DataNode *> ().swap(it->first->children);
          }

          // Toggle the expanded folder (Show <-> Hide)
          it->second->isExpanded = !it->second->isExpanded;

        } else {
          this->openNewTab(it->first);
        }

        return;
      }
    }
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
  this->workplace = new DataNode();
  this->workplace->fullpath = path;
  this->populateDataNode(this->workplace);
}

void Explorer::populateDataNode (DataNode *dn) {
  dn->populate(this->excludedFilePatterns);

  for (int i = 0; i < dn->children.size(); i++) {
    DataNode *child = dn->children[i];
    child->id = this->dataNodeId++;

    DataNodeElement *displayDn = new DataNodeElement(
      child,
      this->config,
      this->getWindow()
    );

    this->fileDivs[child] = displayDn;
  }
}

void Explorer::drawOnScreen () {
  Div::drawOnScreen();

  if (this->workplace == NULL) {
    return;
  }

  float wordHeight = this->config->getExplorerWordHeight();
  float parentDivYPos = this->getPosition().y;
  float paintYPos = 0.0f;
  float topYPadding = 50.0f;

  parentDivYPos += topYPadding;

  int nodeCount = 0;
  this->drawDataNodeTree(this->workplace, nodeCount);

  // Updating Explorer size
  this->setSize(
    this->getSize().x,
    parentDivYPos + (this->dataNodeId * wordHeight) + wordHeight
  );
}

bool Explorer::isAnyEditorActive () {
  return (this->activeDataNodeId != -1);
}

void Explorer::openNewTab(DataNode *dn) {
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

void Explorer::drawDataNodeTree (DataNode *dn, int &nodeCount) {
  if (dn == NULL) {
    return;
  }

  for (int i = 0; i < dn->children.size(); i++) {
    DataNode *child = dn->children[i];
    DataNodeElement *dnDiv = this->fileDivs[child];

    dnDiv->setPosition(
      this->getPosition().x,
      this->getPosition().y + nodeCount * this->config->getExplorerWordHeight()
    );

    dnDiv->setSize(
      this->getSize().x,
      this->config->getExplorerWordHeight()
    );

    if (dnDiv->dn->id == this->activeDataNodeId) {
      dnDiv->fillColor(sf::Color(80, 80, 80, 100));
    }

    dnDiv->drawOnScreen();
    nodeCount++;

    if (child->isDirectory && child->children.size()) {
      this->drawDataNodeTree(child, nodeCount);
    }
  }
}
