#pragma once

#include <string>

// TransformerType - predstavlja jedan tip transformatora u katalogu opreme.
class TransformerType
{
public:
    TransformerType() = default;

    TransformerType(const std::string& name,
                      double snRatedKva,
                      double uPrimaryKv,
                      double uSecondaryKv,
                      double ukPercent,
                      double prCuKw,
                      double poFeKw,
                      const std::string& connectionGroup)
        : _name(name)
        , _snRatedKva(snRatedKva)
        , _uPrimaryKv(uPrimaryKv)
        , _uSecondaryKv(uSecondaryKv)
        , _ukPercent(ukPercent)
        , _prCuKw(prCuKw)
        , _poFeKw(poFeKw)
        , _connectionGroup(connectionGroup)
    {
    }

    // --- getteri ---
    const std::string& getName() const { return _name; }
    double getSnRatedKva() const { return _snRatedKva; }
    double getUPrimaryKv() const { return _uPrimaryKv; }
    double getUSecondaryKv() const { return _uSecondaryKv; }
    double getUkPercent() const { return _ukPercent; }
    double getPrCuKw() const { return _prCuKw; }
    double getPoFeKw() const { return _poFeKw; }
    const std::string& getConnectionGroup() const { return _connectionGroup; }

    // --- setteri ---
    void setName(const std::string& name) { _name = name; }
    void setSnRatedKva(double value) { _snRatedKva = value; }
    void setUPrimaryKv(double value) { _uPrimaryKv = value; }
    void setUSecondaryKv(double value) { _uSecondaryKv = value; }
    void setUkPercent(double value) { _ukPercent = value; }
    void setPrCuKw(double value) { _prCuKw = value; }
    void setPoFeKw(double value) { _poFeKw = value; }
    void setConnectionGroup(const std::string& group) { _connectionGroup = group; }

private:
    std::string _name;             // naziv tipa, npr. "DTR 400kVA 10/0.4kV"
    double _snRatedKva = 0.0;       // nazivna prividna snaga [kVA]
    double _uPrimaryKv = 0.0;       // nazivni napon primara [kV]
    double _uSecondaryKv = 0.0;     // nazivni napon sekundara [kV]
    double _ukPercent = 0.0;        // napon kratkog spoja [%] - koristi se za reaktansu
    double _prCuKw = 0.0;           // gubici u bakru (nazivno opterecenje) [kW]
    double _poFeKw = 0.0;           // gubici u gvozdju (prazan hod) [kW]
    std::string _connectionGroup;   // grupa spoja, npr. "Dyn5"
};
