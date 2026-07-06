#include "Network.h"
#include <cmath>

void Network::addNode(const Node& node)
{
    _nodeIdToIndex[node.getId()] = static_cast<int>(_nodes.size());
    _nodes.push_back(node);
    if (node.getId() >= _nextAutoId)
        _nextAutoId = node.getId() + 1;
}

int Network::addNodeAt(const std::string& name, double nominalVoltageKv, double x, double y)
{
    int newId = _nextAutoId;
    Node node(newId, name, nominalVoltageKv, x, y);
    addNode(node);
    return newId;
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

int Network::findNodeIndexNear(double x, double y, double radius) const
{
    for (size_t i = 0; i < _nodes.size(); ++i)
    {
        double dx = _nodes[i].getX() - x;
        double dy = _nodes[i].getY() - y;
        double dist = std::sqrt(dx * dx + dy * dy);
        if (dist <= radius)
            return static_cast<int>(i);
    }
    return -1;
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

sparse::ICmplxMatrix* Network::buildYbus3Phase() const
{
    int n = static_cast<int>(_nodes.size());
    int dim = 3 * n;

    // Procjena ne-nula elemenata: dijagonalni blokovi (n*9) + van-dijagonalni parovi (2*grane*9)
    int nz = n * 9 + 2 * 9 * static_cast<int>(_branches.size());

    sparse::ICmplxMatrix* pYbus = sparse::createCmplxMatrix(dim, dim, nz, sparse::Symmetry::NonSymmetric);

    for (const auto& branch : _branches)
    {
        int i = getNodeIndexById(branch.getFromNodeId());
        int j = getNodeIndexById(branch.getToNodeId());

        if (i < 0 || j < 0)
            continue; // grana referencira nepostojeci cvor - preskoci

        dense::CmplxMatrix ySeries = branch.getPhaseSeriesAdmittance(); // 3x3
        dense::CmplxMatrix yShunt = branch.getPhaseShuntAdmittance();   // 3x3 (dijagonalno popunjeno)

        dense::MatrixIO<std::complex<double>> ys = ySeries.getManipulator();
        dense::MatrixIO<std::complex<double>> ysh = yShunt.getManipulator();

        for (td::UINT4 p = 0; p < 3; ++p)
        {
            for (td::UINT4 q = 0; q < 3; ++q)
            {
                std::complex<double> yVal = ys(p, q);
                std::complex<double> yShVal = ysh(p, q);
                std::complex<double> diagAdd = yVal + yShVal / 2.0;

                int rowI = 3 * i + static_cast<int>(p);
                int colI = 3 * i + static_cast<int>(q);
                int rowJ = 3 * j + static_cast<int>(p);
                int colJ = 3 * j + static_cast<int>(q);

                // dijagonalni blokovi (self admitansa + polovina shanta)
                pYbus->addTriple(rowI, colI, diagAdd);
                pYbus->addTriple(rowJ, colJ, diagAdd);

                // van-dijagonalni blokovi (medjusobna veza i-j)
                pYbus->addTriple(rowI, colJ, -yVal);
                pYbus->addTriple(rowJ, colI, -yVal);
            }
        }
    }

    return pYbus;
}
