#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/NumericEdit.h>
#include <gui/Button.h>
#include <gui/TextEdit.h>
#include <gui/HorizontalLayout.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>

#include "../CatalogModel/EquipmentCatalog.h"

// ViewCatalog - forma za unos tipova vodova i transformatora u katalog,
// sa jednostavnim tekstualnim prikazom trenutnog sadrzaja kataloga.
class ViewCatalog : public gui::View
{
protected:
    EquipmentCatalog _catalog;

    // --- sekcija: vod ---
    gui::Label _lblLineSection;
    gui::Label _lblLineName;
    gui::LineEdit _lineName;
    gui::Label _lblR1;
    gui::NumericEdit _r1;
    gui::Label _lblX1;
    gui::NumericEdit _x1;
    gui::Label _lblB1;
    gui::NumericEdit _b1;
    gui::Label _lblR0;
    gui::NumericEdit _r0;
    gui::Label _lblX0;
    gui::NumericEdit _x0;
    gui::Label _lblIRated;
    gui::NumericEdit _iRated;
    gui::Label _lblURatedLine;
    gui::NumericEdit _uRatedLine;
    gui::Button _btnAddLine;

    // --- sekcija: transformator ---
    gui::Label _lblTrafoSection;
    gui::Label _lblTrafoName;
    gui::LineEdit _trafoName;
    gui::Label _lblSn;
    gui::NumericEdit _sn;
    gui::Label _lblUp;
    gui::NumericEdit _uPrimary;
    gui::Label _lblUs;
    gui::NumericEdit _uSecondary;
    gui::Label _lblUk;
    gui::NumericEdit _uk;
    gui::Label _lblPrCu;
    gui::NumericEdit _prCu;
    gui::Label _lblPoFe;
    gui::NumericEdit _poFe;
    gui::Label _lblConnGroup;
    gui::LineEdit _connGroup;
    gui::Button _btnAddTrafo;

    // --- perzistencija i prikaz ---
    gui::HorizontalLayout _hlFileButtons;
    gui::Button _btnSaveCsv;
    gui::Button _btnLoadCsv;
    gui::TextEdit _list;

    gui::GridLayout _gl;

public:
    ViewCatalog();

protected:
    void refreshList();
    void clearLineFields();
    void clearTrafoFields();
};
