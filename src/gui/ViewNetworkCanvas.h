#pragma once

#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <gui/Symbol.h>
#include <functional>
#include <string>
#include "../network/Network.h"

// ViewNetworkCanvas - graficki prikaz i interaktivna izgradnja topologije mreze.
// Klik u modu "AddNode" dodaje novi cvor na poziciju klika.
// Klik u modu "ConnectBranch" prvo bira "od" cvor, pa "do" cvor i kreira granu
// (koristeci tip grane koji je prethodno postavljen preko setPendingLineBranch/
// setPendingTransformerBranch).
class ViewNetworkCanvas : public gui::Canvas
{
public:
    enum class Mode
    {
        AddNode,
        ConnectBranch
    };

    ViewNetworkCanvas();

    void setMode(Mode mode);

    void setPendingLineBranch(const LineType& lineType, double lengthKm);
    void setPendingTransformerBranch(const TransformerType& transformerType);

    Network& getNetwork() { return _network; }
    const Network& getNetwork() const { return _network; }

    void clearNetwork();

    // Callback za obavjestavanje "roditeljskog" view-a (panela) o statusu (npr. za prikaz u tekstualnoj listi)
    std::function<void(const std::string&)> onStatusMessage;

protected:
    void onDraw(const gui::Rect& rect) override;
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;

private:
    Network _network;
    Mode _mode = Mode::AddNode;
    int _pendingFromNodeId = -1;

    bool _pendingBranchSet = false;
    bool _pendingIsLine = true;
    LineType _pendingLineType;
    double _pendingLengthKm = 1.0;
    TransformerType _pendingTransformerType;

    gui::Symbol _symbolBus;
    gui::Symbol _symbolXfmr;

    static constexpr double kHitRadius = 18.0;

    void notifyStatus(const std::string& msg);
};
