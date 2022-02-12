#include "TabTray.h"

TabTray::TabTray (GlobalConfig *config, sf::RenderWindow *window) : Div(window) {
  this->config = config;
  this->tabs.clear();
  this->activeDataNodeId = -1;
  this->openTabWidth = 0.0f;
}

void TabTray::push (std::string &name, int dnId, EditorSpace *editor) {
  Tab *newTab = new Tab(name, dnId, editor, this->config, this->getWindow());

  this->openTabWidth += newTab->width + 1.0f;
  if (this->openTabWidth > this->getSize().x) {
    this->setSize(this->openTabWidth, this->getSize().y);
  }

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

  this->adjustView(this->getWindow()->getSize().x, this->getWindow()->getSize().y);
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

  if (this->activeDataNodeId == this->tabs[x]->dnId) {
    this->tabs[x]->fillColor(sf::Color(
      this->config->getBgColor().r,
      this->config->getBgColor().g,
      this->config->getBgColor().b
    ));
  } else {
    this->tabs[x]->fillColor(sf::Color(50, 50, 51));
  }

  this->tabs[x]->drawOnScreen();
}

void TabTray::pollUserEvents (sf::Event &event) {
  Div::pollEvents(event);

  if (event.type == sf::Event::Resized) {
    this->config->setTabTrayWidth(this->config->getEditorXSize());
    this->config->setTabTrayHeight(this->config->getTabTrayHeight());

    this->setSize(
      this->config->getEditorXSize(),
      this->config->getTabTrayHeight()
    );

    this->adjustView(event.size.width, event.size.height);
  }

  if (event.type == sf::Event::MouseWheelScrolled) {
    if (!this->mouseInMyArea(
      (float)sf::Mouse::getPosition(*this->getWindow()).x,
      (float)sf::Mouse::getPosition(*this->getWindow()).y
    )) return;

    sf::View view = this->getWatchableView();
    float leftXPos = view.getCenter().x - view.getSize().x / 2.0f;
    float rightXPos = view.getCenter().x + view.getSize().x / 2.0f;

    // Scroll up will be used for scolling to left
    if (event.mouseWheelScroll.delta > 0) {
      float scrollLeftThresh = this->getPosition().x;

      if (leftXPos - scrollLeftThresh > 40.0f) {
        view.move(-40.0f, 0.0f);
      } else {
        view.move(scrollLeftThresh - leftXPos, 0.0f);
      }
    }

    // Scroll down will be used for scolling to right
    if (event.mouseWheelScroll.delta < 0) {
      float scrollRightThresh = this->getPosition().x + this->getSize().x + 200.0f;

      if (scrollRightThresh - rightXPos > 40.0f) {
        view.move(40.0f, 0.0f);
      } else {
        view.move(scrollRightThresh - rightXPos, 0.f);        
      }
    }

    this->setWatchableView(view);
    return;
  }

  for (int i = 0; i < (int) this->tabs.size(); i++) {
    this->tabs[i]->pollUserEvents(event);

    if (this->tabs[i]->onClick(event, &this->getWatchableView())) {
      this->tabs[i]->editor->loadEditorConfigs();
      this->activeDataNodeId = this->tabs[i]->dnId;
      *(this->explorerActiveDnId) = this->activeDataNodeId;
    }
  }
}

void TabTray::registerExplorerActiveDnId (int *explorerActiveDnId) {
  this->explorerActiveDnId = explorerActiveDnId;
}
