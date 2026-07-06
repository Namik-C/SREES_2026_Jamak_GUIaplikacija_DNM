#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <dense/Matrix.h>
#include <sparse/IMatrix.h>
#include "Node.h"
#include "Branch.h"

// Network - model topologije distributivne mreze (cvorovi + grane).
// Gradi:
//   - matricu incidencije (dense::DblMatrix)  - topologija: koja grana spaja koje cvorove
//   - Ybus admitantnu matricu (sparse, kompleksna) - iz kataloskih parametara grana
class Network
{
public:
    Network() = default;

    void addNode(const Node& node);
    void addBranch(const Branch& branch);

    size_t nodeCount() const { return _nodes.size(); }
    size_t branchCount() const { return _branches.size(); }

    const std::vector<Node>& getNodes() const { return _nodes; }
    const std::vector<Branch>& getBranches() const { return _branches; }

    // Vraca poziciju (0-based indeks) cvora sa datim ID-om unutar interne liste, ili -1 ako ne postoji.
    int getNodeIndexById(int nodeId) const;

    // Matrica incidencije: broj redova = broj grana, broj kolona = broj cvorova.
    // A(k, i) = +1 ako grana k pocinje u cvoru i, -1 ako zavrsava u cvoru i, inace 0.
    dense::DblMatrix buildIncidenceMatrix() const;

    // Ybus admitantna matrica (sparse, kompleksna, dimenzija nodeCount x nodeCount).
    // Pozivalac je odgovoran da pozove release() na vracenom pokazivacu kad zavrsi.
    sparse::ICmplxMatrix* buildYbus() const;

private:
    std::vector<Node> _nodes;
    std::vector<Branch> _branches;
    std::unordered_map<int, int> _nodeIdToIndex; // node ID -> pozicija u _nodes
};
