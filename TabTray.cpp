#include "TabTray.h"

TabTray::TabTray (GlobalConfig *config, sf::RenderWindow *window) : Div(window) {
  this->config = config;
  this->tabs.clear();
  this->activeDataNodeId = -1;
}

void TabTray::push (std::string &name, int dnId, EditorSpace *editor) {
  Tab *newTab = new Tab(name, dnId, editor, this->config, this->getWindow());

  this->tabs.push_back(newTab);
  this->activeDataNodeId = dnId;
}

void TabTray::remove (int id) {
  // For now its just iterates and match and delete the tab;
  for (int i = 0; i < (int) this->tabs.size(); i++) {
    if (this->tabs[i]->dnId == id) {
      this->tabs.erase( this->tabs.begin() + i );
      return;
    }
  }
}

bool TabTray::empty () {
  return this->tabs.empty();
}

void TabTray::setActiveTab (int id) {
  this->activeDataNodeId = id;
}

void TabTray::loadConfigs () {
  this->setPosition(
    this->config->getEditorXPos(),
    this->config->getExplorerYPos()
  );

  this->setSize(
    this->config->getEditorXSize(),
    this->config->getTabTrayHeight()
  );

  this->adjustView(Div::getWindow()->getSize().x, Div::getWindow()->getSize().y);
}

void TabTray::drawOnScreen () {
  Div::drawOnScreen();

  float runningX = 0;
  // Just display all tabs
  for (int i = 0; i < (int) this->tabs.size(); i++) {
    this->drawTabs(i, runningX);
  }
}

void TabTray::drawTabs (int x, float &xPos) {
  float tabXPos = this->getPosition().x + xPos;
  float tabYPos = this->getPosition().y;

  float tabXSize = this->tabs[x]->width;
  float tabYSize = this->getSize().y;
  xPos += tabXSize + 1.0f;

  this->tabs[x]->setPosition(tabXPos, tabYPos);
  this->tabs[x]->setSize(tabXSize, tabYSize);

  sf::View &tabView = this->tabs[x]->getWatchableView();

  tabView.reset(sf::FloatRect(tabXPos, tabYPos, tabXSize, tabYSize));

  tabView.setViewport(sf::FloatRect(
    tabXPos / this->getWindow()->getSize().x,
    tabYPos / this->getWindow()->getSize().y,
    tabXSize / this->getWindow()->getSize().x,
    tabYSize / this->getWindow()->getSize().y
  ));

  this->setWatchableView(tabView);
  this->getWindow()->setView(this->getWatchableView());

  if (this->activeDataNodeId == this->tabs[x]->dnId) {
    this->tabs[x]->fillColor(sf::Color(29, 29, 29));      
  } else {
    this->tabs[x]->fillColor(sf::Color(39, 39, 41));
  }

  this->tabs[x]->drawOnScreen();
}

void TabTray::pollUserEvents (sf::Event &event) {
  Div::pollEvents(event);

  for (int i = 0; i < (int) this->tabs.size(); i++) {
    this->tabs[i]->pollUserEvents(event);

    if (this->tabs[i]->onClick(event)) {
      this->tabs[i]->editor->loadEditorConfigs();
      this->activeDataNodeId = this->tabs[i]->dnId;
      *(this->explorerActiveDnId) = this->activeDataNodeId;
    }

  }
}

void TabTray::registerExplorerActiveDnId (int *explorerActiveDnId) {
  this->explorerActiveDnId = explorerActiveDnId;
}
