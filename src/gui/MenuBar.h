#pragma once
#include <gui/MenuBar.h>

class MenuBar : public gui::MenuBar
{
private:
    gui::SubMenu _subApp;

protected:
    void populateSubAppMenu()
    {
        auto& items = _subApp.getItems();
        items[0].initAsQuitAppActionItem(tr("Quit"), "q");
    }

public:
    MenuBar()
    : gui::MenuBar(1)              // jedan meni
    , _subApp(10, "App", 1)        // App meni sa 1 stavkom
    {
        populateSubAppMenu();
        _menus[0] = &_subApp;
    }

    ~MenuBar()
    {
    }
};
