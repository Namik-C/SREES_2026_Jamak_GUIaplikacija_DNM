#pragma once
#include <gui/Window.h>
#include <gui/ActionItem.h>
#include "MenuBar.h"
#include "MainTabView.h"

class MainWindow : public gui::Window
{
protected:
    MenuBar _mainMenuBar;
    MainTabView _mainView;

public:
    MainWindow()
    : gui::Window(gui::Size(900, 700))
    {
        setTitle("Distribution Network Modeler - Katalog opreme");
        _mainMenuBar.setAsMain(this);
        setCentralView(&_mainView);
    }

    ~MainWindow()
    {
    }

    bool shouldClose() override
    {
        return true;
    }

    void onClose() override
    {
    }

    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        return false;
    }
};
