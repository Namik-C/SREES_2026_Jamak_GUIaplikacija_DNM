#pragma once

#include <complex>
#include "../CatalogModel/LineType.h"
#include "../CatalogModel/TransformerType.h"

enum class BranchType
{
    Line,
    Transformer
};

// Branch - grana mreze koja povezuje dva cvora. Moze biti vod (referencira
// LineType + duzinu) ili transformator (referencira TransformerType).
// Racuna seriju/shunt admitanse potrebne za formiranje Ybus matrice.
class Branch
{
public:
    Branch() = default;

    static Branch makeLine(int fromNodeId, int toNodeId, const LineType& lineType, double lengthKm)
    {
        Branch b;
        b._fromNodeId = fromNodeId;
        b._toNodeId = toNodeId;
        b._type = BranchType::Line;
        b._lineType = lineType;
        b._lengthKm = lengthKm;
        return b;
    }

    static Branch makeTransformer(int fromNodeId, int toNodeId, const TransformerType& transformerType)
    {
        Branch b;
        b._fromNodeId = fromNodeId;
        b._toNodeId = toNodeId;
        b._type = BranchType::Transformer;
        b._transformerType = transformerType;
        return b;
    }

    int getFromNodeId() const { return _fromNodeId; }
    int getToNodeId() const { return _toNodeId; }
    BranchType getType() const { return _type; }
    const LineType& getLineType() const { return _lineType; }
    const TransformerType& getTransformerType() const { return _transformerType; }
    double getLengthKm() const { return _lengthKm; }

    // Serijska admitansa grane (1/Z), koristi se za Ybus: Y(i,i)+=y, Y(j,j)+=y, Y(i,j)-=y, Y(j,i)-=y
    std::complex<double> getSeriesAdmittance() const
    {
        if (_type == BranchType::Line)
        {
            double r = _lineType.getR1PerKm() * _lengthKm;
            double x = _lineType.getX1PerKm() * _lengthKm;
            std::complex<double> z(r, x);
            if (r == 0.0 && x == 0.0)
                return std::complex<double>(0.0, 0.0);
            return 1.0 / z;
        }
        else
        {
            // Transformator: impedansa iz Uk% i nazivnih velicina (referirano na sekundarnu stranu)
            double snMva = _transformerType.getSnRatedKva() / 1000.0;
            double uSecKv = _transformerType.getUSecondaryKv();
            if (snMva <= 0.0 || uSecKv <= 0.0)
                return std::complex<double>(0.0, 0.0);

            double zBaseOhm = (uSecKv * uSecKv) / snMva;
            double zMagnitudeOhm = (_transformerType.getUkPercent() / 100.0) * zBaseOhm;

            // Rk (pu) priblizno iz gubitaka u bakru: PrCu(kW) / Sn(kVA)
            double rPu = _transformerType.getPrCuKw() / _transformerType.getSnRatedKva();
            double zPu = _transformerType.getUkPercent() / 100.0;
            double xPu = (zPu > rPu) ? std::sqrt(zPu * zPu - rPu * rPu) : 0.0;

            double rOhm = rPu * zBaseOhm;
            double xOhm = xPu * zBaseOhm;

            std::complex<double> z(rOhm, xOhm);
            if (rOhm == 0.0 && xOhm == 0.0)
                return std::complex<double>(0.0, 0.0);
            return 1.0 / z;
        }
    }

    // Shant admitansa na krajevima voda (ukupna, dijeli se na pola po cvoru). 0 za transformator (pojednostavljeno).
    std::complex<double> getShuntAdmittance() const
    {
        if (_type == BranchType::Line)
        {
            double b = _lineType.getB1PerKm() * _lengthKm;
            return std::complex<double>(0.0, b);
        }
        return std::complex<double>(0.0, 0.0);
    }

private:
    int _fromNodeId = -1;
    int _toNodeId = -1;
    BranchType _type = BranchType::Line;
    LineType _lineType;
    TransformerType _transformerType;
    double _lengthKm = 0.0;
};
