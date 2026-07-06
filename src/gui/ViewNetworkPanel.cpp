#include "ViewNetworkPanel.h"
#include <sstream>

ViewNetworkPanel::ViewNetworkPanel(EquipmentCatalog& catalog)
: _catalog(catalog)
, _lblMode("Nacin rada:")
, _lblKind("Tip grane:")
, _lblItem("Katalog:")
, _lblLength("Duzina [km]:")
, _length(td::real8)
, _hlButtons(10)
, _btnApply(tr("Primijeni tip grane"), tr("Postavlja odabrani tip grane za sljedece povezivanje"))
, _btnRefreshCatalog(tr("Osvjezi katalog"), tr("Ucitava najnovije stanje kataloga u liste"))
, _btnClearNetwork(tr("Obrisi mrezu"), tr("Brise sve cvorove i grane"))
, _btnShowYbus(tr("Prikazi Ybus"), tr("Gradi i prikazuje Ybus matricu trenutne mreze"))
, _gl(6, 4)
{
    _cbMode.addItem("Dodaj cvor (klik na platno)");
    _cbMode.addItem("Povezi granom (klik-klik na 2 cvora)");
    _cbMode.selectIndex(0, false);

    _cbKind.addItem("Vod");
    _cbKind.addItem("Transformator");
    _cbKind.selectIndex(0, false);

    _length.setValue(1.0);

    gui::GridComposer gc(_gl);
    gc.appendRow(_lblMode) << _cbMode;
    gc.appendRow(_lblKind) << _cbKind << _lblItem << _cbItem;
    gc.appendRow(_lblLength) << _length;

    _hlButtons << _btnApply << _btnRefreshCatalog << _btnClearNetwork << _btnShowYbus;
    gc.appendRow(_hlButtons, 0);

    gc.appendRow(_status, 0);
    gc.appendRow(_canvas, 0);

    setLayout(&_gl);

    refreshBranchItemsCombo();

    _cbMode.onChangedSelection([this]()
        {
            bool isAddNode = (_cbMode.getSelectedIndex() == 0);
            _canvas.setMode(isAddNode ? ViewNetworkCanvas::Mode::AddNode : ViewNetworkCanvas::Mode::ConnectBranch);
        });

    _cbKind.onChangedSelection([this]()
        {
            refreshBranchItemsCombo();
        });

    _btnRefreshCatalog.onClick([this]()
        {
            refreshBranchItemsCombo();
            _status.appendString("Katalog osvjezen.\n");
        });

    _btnApply.onClick([this]()
        {
            applyBranchType();
        });

    _btnClearNetwork.onClick([this]()
        {
            _canvas.clearNetwork();
            _status.appendString("Mreza obrisana.\n");
        });

    _btnShowYbus.onClick([this]()
        {
            showYbus();
        });

    _canvas.onStatusMessage = [this](const std::string& msg)
        {
            std::string line = msg + "\n";
            _status.appendString(line.c_str());
        };
}

void ViewNetworkPanel::refreshBranchItemsCombo()
{
    _cbItem.clean();

    bool isLine = (_cbKind.getSelectedIndex() == 0);
    if (isLine)
    {
        for (const auto& lt : _catalog.getLineTypes())
            _cbItem.addItem(lt.getName().c_str());
    }
    else
    {
        for (const auto& tt : _catalog.getTransformerTypes())
            _cbItem.addItem(tt.getName().c_str());
    }

    if ((isLine && _catalog.lineTypeCount() > 0) || (!isLine && _catalog.transformerTypeCount() > 0))
        _cbItem.selectIndex(0, false);
}

void ViewNetworkPanel::applyBranchType()
{
    int idx = _cbItem.getSelectedIndex();
    if (idx < 0)
    {
        _status.appendString("Nema odabranog tipa u katalogu (dodaj ga na tabu Katalog opreme).\n");
        return;
    }

    bool isLine = (_cbKind.getSelectedIndex() == 0);
    if (isLine)
    {
        const LineType& lt = _catalog.getLineTypes()[idx];
        double lengthKm = _length.getValue().r8Val();
        _canvas.setPendingLineBranch(lt, lengthKm);
        std::string msg = "Postavljen tip grane: vod '" + lt.getName() + "', duzina=" + std::to_string(lengthKm) + "km\n";
        _status.appendString(msg.c_str());
    }
    else
    {
        const TransformerType& tt = _catalog.getTransformerTypes()[idx];
        _canvas.setPendingTransformerBranch(tt);
        std::string msg = "Postavljen tip grane: transformator '" + tt.getName() + "'\n";
        _status.appendString(msg.c_str());
    }
}

void ViewNetworkPanel::showYbus()
{
    const Network& net = _canvas.getNetwork();

    if (net.nodeCount() == 0)
    {
        _status.appendString("Mreza je prazna - dodaj cvorove i grane na platnu.\n");
        return;
    }

    std::ostringstream oss;
    oss << "\n=== Mreza: " << net.nodeCount() << " cvora, " << net.branchCount() << " grane ===\n";
    oss << "(trofazni ABC model - svaki cvor ima 3 reda/kolone: faze A, B, C)\n";

    dense::DblMatrix incidence = net.buildIncidenceMatrix();
    dense::MatrixIO<double> incReader = incidence.getManipulator();
    oss << "--- Matrica incidencije (" << incidence.getNoOfRows() << "x" << incidence.getNoOfCols() << ") ---\n";
    for (td::UINT4 r = 0; r < incidence.getNoOfRows(); ++r)
    {
        for (td::UINT4 c = 0; c < incidence.getNoOfCols(); ++c)
            oss << incReader(r, c) << "\t";
        oss << "\n";
    }

    sparse::ICmplxMatrix* pYbus = net.buildYbus3Phase();
    sparse::CmplxMatrixReleaser releaser(pYbus);

    oss << "\n--- Trofazna Ybus matrica (" << pYbus->getNoOfRows() << "x" << pYbus->getNoOfCols()
        << ", nonzero=" << pYbus->getNoOfNonZero() << ") ---\n";
    pYbus->serialize("Ybus_ABC", oss, sparse::Format::Matlab);
    oss << "\n";

    _status.appendString(oss.str().c_str());
}
