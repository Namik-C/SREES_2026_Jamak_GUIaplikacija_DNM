#include "Branch.h"

namespace
{
    // Pravi 3x3 kompleksnu matricu oblika [Ys Ym Ym; Ym Ys Ym; Ym Ym Ys]
    dense::CmplxMatrix buildSymmetricPhaseMatrix(const std::complex<double>& ys, const std::complex<double>& ym)
    {
        dense::CmplxMatrix M(3, 3, nullptr, true);
        dense::MatrixIO<std::complex<double>> io = M.getManipulator();

        for (td::UINT4 p = 0; p < 3; ++p)
        {
            for (td::UINT4 q = 0; q < 3; ++q)
                io(p, q) = (p == q) ? ys : ym;
        }
        return M;
    }
}

dense::CmplxMatrix Branch::getPhaseSeriesAdmittance() const
{
    if (_type == BranchType::Line)
    {
        double r1 = _lineType.getR1PerKm() * _lengthKm;
        double x1 = _lineType.getX1PerKm() * _lengthKm;
        double r0 = _lineType.getR0PerKm() * _lengthKm;
        double x0 = _lineType.getX0PerKm() * _lengthKm;

        std::complex<double> z1(r1, x1);
        std::complex<double> z0(r0, x0);

        // ako je nulta sekvenca nepoznata (0), pretpostavi z0 = 3*z1 (tipicno za priblizne proracune)
        if (std::abs(z0) == 0.0)
            z0 = 3.0 * z1;

        std::complex<double> y1 = (std::abs(z1) > 0.0) ? (1.0 / z1) : std::complex<double>(0.0, 0.0);
        std::complex<double> y0 = (std::abs(z0) > 0.0) ? (1.0 / z0) : std::complex<double>(0.0, 0.0);

        std::complex<double> ys = (y0 + 2.0 * y1) / 3.0;
        std::complex<double> ym = (y0 - y1) / 3.0;

        return buildSymmetricPhaseMatrix(ys, ym);
    }
    else
    {
        // Transformator: pojednostavljeno - faze medjusobno nezavisne (bez sprezanja)
        std::complex<double> y = getSeriesAdmittance(); // postojeca jednofazna vrijednost
        return buildSymmetricPhaseMatrix(y, std::complex<double>(0.0, 0.0));
    }
}

dense::CmplxMatrix Branch::getPhaseShuntAdmittance() const
{
    if (_type == BranchType::Line)
    {
        double b = _lineType.getB1PerKm() * _lengthKm;
        std::complex<double> yShunt(0.0, b);
        return buildSymmetricPhaseMatrix(yShunt, std::complex<double>(0.0, 0.0));
    }

    return buildSymmetricPhaseMatrix(std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0));
}
