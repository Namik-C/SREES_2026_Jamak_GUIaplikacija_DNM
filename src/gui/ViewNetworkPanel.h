#pragma once

#include <gui/View.h>
#include <gui/Label.h>
#include <gui/ComboBox.h>
#include <gui/NumericEdit.h>
#include <gui/Button.h>
#include <gui/TextEdit.h>
#include <gui/HorizontalLayout.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>

#include "../CatalogModel/EquipmentCatalog.h"
#include "ViewNetworkCanvas.h"

// ViewNetworkPanel - bocni panel (mod rada, izbor tipa grane iz kataloga) +
// canvas za crtanje topologije mreze.
class ViewNetworkPanel : public gui::View
{
public:
    explicit ViewNetworkPanel(EquipmentCatalog& catalog);

protected:
    void refreshBranchItemsCombo();
    void applyBranchType();
    void showYbus();

private:
    EquipmentCatalog& _catalog;

    gui::Label _lblMode;
    gui::ComboBox _cbMode; // "Dodaj cvor" / "Poveži granom"

    gui::Label _lblKind;
    gui::ComboBox _cbKind; // "Vod" / "Transformator"

    gui::Label _lblItem;
    gui::ComboBox _cbItem; // konkretan tip iz kataloga

    gui::Label _lblLength;
    gui::NumericEdit _length; // duzina voda [km]

    gui::HorizontalLayout _hlButtons;
    gui::Button _btnApply;         // "Primijeni tip grane"
    gui::Button _btnRefreshCatalog;// "Osvjezi katalog"
    gui::Button _btnClearNetwork;  // "Obrisi mrezu"
    gui::Button _btnShowYbus;      // "Prikazi Ybus"

    gui::TextEdit _status;
    ViewNetworkCanvas _canvas;

    gui::GridLayout _gl;
};
