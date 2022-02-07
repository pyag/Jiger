#include "TabTray.h"

TabTray::TabTray (GlobalConfig *config, sf::RenderWindow *window) : Div(window) {
  this->config = config;
  this->tabs.clear();
  this->activeDataNodeId = -1;
}

void TabTray::push (std::string &name, int dnId, EditorSpace *editor) {
  Tab *newTab = new Tab(name, dnId, editor, Div::getWindow());

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

void TabTray::loadConfigs() {
  this->setPosition(
    this->config->getEditorXPos(),
    this->config->getExplorerYPos()
  );

  this->setSize(
    this->config->getEditorXSize(),
    this->config->getTabTrayHeight()
  );

  sf::View tabTrayView = Div::getWindow()->getView();

  tabTrayView.reset(sf::FloatRect(
    this->config->getEditorXPos(),
    this->config->getExplorerYPos(),
    this->config->getEditorXSize(),
    this->config->getTabTrayHeight()
  ));

  tabTrayView.setViewport(sf::FloatRect(
    this->config->getEditorXPos() / Div::getWindow()->getSize().x,
    this->config->getExplorerYPos() / Div::getWindow()->getSize().y,
    this->config->getEditorXSize() / Div::getWindow()->getSize().x,
    this->config->getTabTrayHeight() / Div::getWindow()->getSize().y
  ));

  this->setWatchableView(tabTrayView);
}

void TabTray::drawOnScreen (sf::RenderWindow &window) {
  Div::drawOnScreen(window);

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
    tabXPos / Div::getWindow()->getSize().x,
    tabYPos / Div::getWindow()->getSize().y,
    tabXSize / Div::getWindow()->getSize().x,
    tabYSize / Div::getWindow()->getSize().y
  ));

  this->setWatchableView(tabView);
  Div::getWindow()->setView(this->getWatchableView());

  if (this->activeDataNodeId == this->tabs[x]->dnId) {
    this->tabs[x]->fillColor(sf::Color(29, 29, 29));      
  } else {
    this->tabs[x]->fillColor(sf::Color(39, 39, 41));
  }

  this->tabs[x]->drawOnScreen(*Div::getWindow());
}

void TabTray::setWatchableView (sf::View &view) {
  this->watchableView = view;
}

sf::View &TabTray::getWatchableView () {
  return this->watchableView;
}

void TabTray::pollUserEvents (sf::Event &event) {
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

