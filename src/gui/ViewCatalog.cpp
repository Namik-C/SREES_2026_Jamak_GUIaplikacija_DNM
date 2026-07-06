#include "ViewCatalog.h"
#include <sstream>

ViewCatalog::ViewCatalog()
: _lblLineSection("--- Katalog vodova ---")
, _lblLineName("Naziv voda:")
, _lblR1("R1 [ohm/km]:")
, _r1(td::real8)
, _lblX1("X1 [ohm/km]:")
, _x1(td::real8)
, _lblB1("B1 [S/km]:")
, _b1(td::real8)
, _lblR0("R0 [ohm/km]:")
, _r0(td::real8)
, _lblX0("X0 [ohm/km]:")
, _x0(td::real8)
, _lblIRated("Irated [A]:")
, _iRated(td::real8)
, _lblURatedLine("Urated [kV]:")
, _uRatedLine(td::real8)
, _btnAddLine(tr("Dodaj vod"), tr("Dodaje novi tip voda u katalog"))
, _lblTrafoSection("--- Katalog transformatora ---")
, _lblTrafoName("Naziv trafoa:")
, _lblSn("Sn [kVA]:")
, _sn(td::real8)
, _lblUp("Up [kV]:")
, _uPrimary(td::real8)
, _lblUs("Us [kV]:")
, _uSecondary(td::real8)
, _lblUk("Uk [%]:")
, _uk(td::real8)
, _lblPrCu("Pcu [kW]:")
, _prCu(td::real8)
, _lblPoFe("Pfe [kW]:")
, _poFe(td::real8)
, _lblConnGroup("Grupa spoja:")
, _btnAddTrafo(tr("Dodaj transformator"), tr("Dodaje novi tip transformatora u katalog"))
, _hlFileButtons(10)
, _btnSaveCsv(tr("Snimi CSV"), tr("Snima katalog u CSV fajlove"))
, _btnLoadCsv(tr("Ucitaj CSV"), tr("Ucitava katalog iz CSV fajlova"))
, _gl(14, 4)
{
    gui::GridComposer gc(_gl);

    // --- sekcija vodova ---
    gc.appendRow(_lblLineSection, 0);
    gc.appendRow(_lblLineName) << _lineName;
    gc.appendRow(_lblR1) << _r1 << _lblX1 << _x1;
    gc.appendRow(_lblB1) << _b1 << _lblR0 << _r0;
    gc.appendRow(_lblX0) << _x0 << _lblIRated << _iRated;
    gc.appendRow(_lblURatedLine) << _uRatedLine << _btnAddLine;

    // --- sekcija transformatora ---
    gc.appendRow(_lblTrafoSection, 0);
    gc.appendRow(_lblTrafoName) << _trafoName;
    gc.appendRow(_lblSn) << _sn << _lblUp << _uPrimary;
    gc.appendRow(_lblUs) << _uSecondary << _lblUk << _uk;
    gc.appendRow(_lblPrCu) << _prCu << _lblPoFe << _poFe;
    gc.appendRow(_lblConnGroup) << _connGroup << _btnAddTrafo;

    // --- fajl dugmad i lista ---
    _hlFileButtons << _btnSaveCsv << _btnLoadCsv;
    gc.appendRow(_hlFileButtons, 0);
    gc.appendRow(_list, 0);

    setLayout(&_gl);

    // --- handleri ---
    _btnAddLine.onClick([this]()
        {
            td::String name = _lineName.getText();
            if (name.length() == 0)
                return;

            LineType lt(
                name.c_str(),
                _r1.getValue().r8Val(),
                _x1.getValue().r8Val(),
                _b1.getValue().r8Val(),
                _r0.getValue().r8Val(),
                _x0.getValue().r8Val(),
                _iRated.getValue().r8Val(),
                _uRatedLine.getValue().r8Val()
            );
            _catalog.addLineType(lt);
            clearLineFields();
            refreshList();
        });

    _btnAddTrafo.onClick([this]()
        {
            td::String name = _trafoName.getText();
            if (name.length() == 0)
                return;

            td::String connGroup = _connGroup.getText();

            TransformerType tt(
                name.c_str(),
                _sn.getValue().r8Val(),
                _uPrimary.getValue().r8Val(),
                _uSecondary.getValue().r8Val(),
                _uk.getValue().r8Val(),
                _prCu.getValue().r8Val(),
                _poFe.getValue().r8Val(),
                connGroup.c_str()
            );
            _catalog.addTransformerType(tt);
            clearTrafoFields();
            refreshList();
        });

    _btnSaveCsv.onClick([this]()
        {
            _catalog.saveLineTypesToCsv("lineTypes.csv");
            _catalog.saveTransformerTypesToCsv("transformerTypes.csv");
            _list.appendString("Katalog snimljen u lineTypes.csv i transformerTypes.csv\n");
        });

    _btnLoadCsv.onClick([this]()
        {
            _catalog.loadLineTypesFromCsv("lineTypes.csv");
            _catalog.loadTransformerTypesFromCsv("transformerTypes.csv");
            refreshList();
        });

    refreshList();
}

void ViewCatalog::clearLineFields()
{
    _lineName.setText("");
    _r1.toZero();
    _x1.toZero();
    _b1.toZero();
    _r0.toZero();
    _x0.toZero();
    _iRated.toZero();
    _uRatedLine.toZero();
}

void ViewCatalog::clearTrafoFields()
{
    _trafoName.setText("");
    _connGroup.setText("");
    _sn.toZero();
    _uPrimary.toZero();
    _uSecondary.toZero();
    _uk.toZero();
    _prCu.toZero();
    _poFe.toZero();
}

void ViewCatalog::refreshList()
{
    _list.clean();

    std::ostringstream oss;
    oss << "=== Tipovi vodova (" << _catalog.lineTypeCount() << ") ===\n";
    for (const auto& lt : _catalog.getLineTypes())
    {
        oss << "- " << lt.getName()
            << " | R1=" << lt.getR1PerKm() << " X1=" << lt.getX1PerKm()
            << " B1=" << lt.getB1PerKm()
            << " R0=" << lt.getR0PerKm() << " X0=" << lt.getX0PerKm()
            << " Irated=" << lt.getIRatedA() << "A Urated=" << lt.getURatedKv() << "kV\n";
    }

    oss << "\n=== Tipovi transformatora (" << _catalog.transformerTypeCount() << ") ===\n";
    for (const auto& tt : _catalog.getTransformerTypes())
    {
        oss << "- " << tt.getName()
            << " | Sn=" << tt.getSnRatedKva() << "kVA"
            << " Up=" << tt.getUPrimaryKv() << "kV Us=" << tt.getUSecondaryKv() << "kV"
            << " Uk=" << tt.getUkPercent() << "% PrCu=" << tt.getPrCuKw() << "kW"
            << " PoFe=" << tt.getPoFeKw() << "kW Grupa=" << tt.getConnectionGroup() << "\n";
    }

    _list.appendString(oss.str().c_str());
}
