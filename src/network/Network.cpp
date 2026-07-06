#include "Network.h"

void Network::addNode(const Node& node)
{
    _nodeIdToIndex[node.getId()] = static_cast<int>(_nodes.size());
    _nodes.push_back(node);
}

void Network::addBranch(const Branch& branch)
{
    _branches.push_back(branch);
}

int Network::getNodeIndexById(int nodeId) const
{
    auto it = _nodeIdToIndex.find(nodeId);
    if (it == _nodeIdToIndex.end())
        return -1;
    return it->second;
}

dense::DblMatrix Network::buildIncidenceMatrix() const
{
    td::UINT4 nBranches = static_cast<td::UINT4>(_branches.size());
    td::UINT4 nNodes = static_cast<td::UINT4>(_nodes.size());

    dense::DblMatrix A(nBranches, nNodes, nullptr, true); // true = inicijalizuj na nule
    dense::MatrixIO<double> io = A.getManipulator();

    for (td::UINT4 k = 0; k < nBranches; ++k)
    {
        const Branch& branch = _branches[k];
        int fromIdx = getNodeIndexById(branch.getFromNodeId());
        int toIdx = getNodeIndexById(branch.getToNodeId());

        if (fromIdx >= 0)
            io(k, static_cast<td::UINT4>(fromIdx)) = 1.0;
        if (toIdx >= 0)
            io(k, static_cast<td::UINT4>(toIdx)) = -1.0;
    }

    return A;
}

sparse::ICmplxMatrix* Network::buildYbus() const
{
    int n = static_cast<int>(_nodes.size());

    // Procjena broja ne-nula elemenata: dijagonala (n) + 2 po grani (van-dijagonalni parovi)
    int nz = n + 2 * static_cast<int>(_branches.size());

    sparse::ICmplxMatrix* pYbus = sparse::createCmplxMatrix(n, n, nz, sparse::Symmetry::NonSymmetric);

    for (const auto& branch : _branches)
    {
        int i = getNodeIndexById(branch.getFromNodeId());
        int j = getNodeIndexById(branch.getToNodeId());

        if (i < 0 || j < 0)
            continue; // grana referencira nepostojeci cvor - preskoci

        std::complex<double> ySeries = branch.getSeriesAdmittance();
        std::complex<double> yShunt = branch.getShuntAdmittance();

        // dijagonalni elementi: serijska admitansa + polovina shant admitanse na svakom kraju
        pYbus->addTriple(i, i, ySeries + yShunt / 2.0);
        pYbus->addTriple(j, j, ySeries + yShunt / 2.0);

        // van-dijagonalni elementi: -serijska admitansa
        pYbus->addTriple(i, j, -ySeries);
        pYbus->addTriple(j, i, -ySeries);
    }

    return pYbus;
}
