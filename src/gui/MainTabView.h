#pragma once
#include <gui/StandardTabView.h>
#include "ViewCatalog.h"
#include "ViewNetworkPanel.h"
#include "../CatalogModel/EquipmentCatalog.h"

// MainTabView - glavni sadrzaj prozora, sa dva taba koja dijele isti katalog opreme:
//   1. Katalog opreme (unos vodova/transformatora)
//   2. Mreza (crtanje topologije, Ybus matrica)
class MainTabView : public gui::StandardTabView
{
protected:
    EquipmentCatalog _catalog; // jedini vlasnik kataloga, oba taba ga dijele preko reference
    ViewCatalog _viewCatalog;
    ViewNetworkPanel _viewNetwork;

public:
    MainTabView()
    : _viewCatalog(_catalog)
    , _viewNetwork(_catalog)
    {
        addView(&_viewCatalog, "Katalog opreme");
        addView(&_viewNetwork, "Mreza");
    }
};
