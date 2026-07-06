#include "ViewNetworkCanvas.h"

ViewNetworkCanvas::ViewNetworkCanvas()
: gui::Canvas({gui::InputDevice::Event::PrimaryClicks})
, _symbolBus(":bus")
, _symbolXfmr(":Xfmr2")
{
}

void ViewNetworkCanvas::setMode(Mode mode)
{
    _mode = mode;
    _pendingFromNodeId = -1;
    reDraw();
}

void ViewNetworkCanvas::setPendingLineBranch(const LineType& lineType, double lengthKm)
{
    _pendingIsLine = true;
    _pendingLineType = lineType;
    _pendingLengthKm = lengthKm;
    _pendingBranchSet = true;
}

void ViewNetworkCanvas::setPendingTransformerBranch(const TransformerType& transformerType)
{
    _pendingIsLine = false;
    _pendingTransformerType = transformerType;
    _pendingBranchSet = true;
}

void ViewNetworkCanvas::clearNetwork()
{
    _network = Network();
    _pendingFromNodeId = -1;
    reDraw();
}

void ViewNetworkCanvas::notifyStatus(const std::string& msg)
{
    if (onStatusMessage)
        onStatusMessage(msg);
}

void ViewNetworkCanvas::onDraw(const gui::Rect& rect)
{
    // grane (linije) ispod cvorova
    for (const auto& branch : _network.getBranches())
    {
        int iFrom = _network.getNodeIndexById(branch.getFromNodeId());
        int iTo = _network.getNodeIndexById(branch.getToNodeId());
        if (iFrom < 0 || iTo < 0)
            continue;

        const Node& nFrom = _network.getNodes()[iFrom];
        const Node& nTo = _network.getNodes()[iTo];

        gui::Point p1((gui::CoordType)nFrom.getX(), (gui::CoordType)nFrom.getY());
        gui::Point p2((gui::CoordType)nTo.getX(), (gui::CoordType)nTo.getY());

        if (branch.getType() == BranchType::Line)
        {
            gui::Shape::drawLine(p1, p2, td::ColorID::DarkBlue, 2.0f);
        }
        else
        {
            gui::Shape::drawLine(p1, p2, td::ColorID::DarkOrange, 2.0f, td::LinePattern::Dash);
            gui::Point mid((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
            _symbolXfmr.draw(mid);
        }
    }

    // cvorovi (bus simbol) iznad grana
    for (const auto& node : _network.getNodes())
    {
        gui::Point center((gui::CoordType)node.getX(), (gui::CoordType)node.getY());
        _symbolBus.draw(center);

        if (node.getId() == _pendingFromNodeId)
        {
            gui::Rect highlight(center.x - 20, center.y - 20, center.x + 20, center.y + 20);
            gui::Shape::drawRect(highlight, td::ColorID::Red, 2, td::LinePattern::Dash);
        }
    }
}

void ViewNetworkCanvas::onPrimaryButtonPressed(const gui::InputDevice& inputDevice)
{
    if (!inputDevice.isModelPointAvailable())
        return;

    const gui::Point& pt = inputDevice.getModelPoint();

    if (_mode == Mode::AddNode)
    {
        int newId = _network.addNodeAt("Bus", 20.0, pt.x, pt.y);
        notifyStatus("Dodan cvor ID=" + std::to_string(newId));
        reDraw();
        return;
    }

    // Mode::ConnectBranch
    int hitIdx = _network.findNodeIndexNear(pt.x, pt.y, kHitRadius);
    if (hitIdx < 0)
        return; // klik u prazno - ignorisi

    const Node& hitNode = _network.getNodes()[hitIdx];

    if (_pendingFromNodeId < 0)
    {
        _pendingFromNodeId = hitNode.getId();
        reDraw();
        return;
    }

    if (hitNode.getId() == _pendingFromNodeId)
    {
        _pendingFromNodeId = -1; // klik na isti cvor ponovo - otkazi odabir
        reDraw();
        return;
    }

    if (!_pendingBranchSet)
    {
        notifyStatus("Prvo primijeni tip grane (Vod ili Transformator) u panelu.");
        _pendingFromNodeId = -1;
        reDraw();
        return;
    }

    if (_pendingIsLine)
        _network.addBranch(Branch::makeLine(_pendingFromNodeId, hitNode.getId(), _pendingLineType, _pendingLengthKm));
    else
        _network.addBranch(Branch::makeTransformer(_pendingFromNodeId, hitNode.getId(), _pendingTransformerType));

    notifyStatus("Dodana grana izmedju cvorova.");
    _pendingFromNodeId = -1;
    reDraw();
}
