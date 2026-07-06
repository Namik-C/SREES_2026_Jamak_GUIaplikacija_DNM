#pragma once

#include <string>

// LineType - predstavlja jedan tip voda/kabla u katalogu opreme.
// Sadrzi parametre po jedinici duzine (obicno po km), koji se koriste
// za formiranje admitantne matrice mreze (Ybus).
class LineType
{
public:
    LineType() = default;

    LineType(const std::string& name,
              double r1PerKm,
              double x1PerKm,
              double b1PerKm,
              double r0PerKm,
              double x0PerKm,
              double iRatedA,
              double uRatedKv)
        : _name(name)
        , _r1PerKm(r1PerKm)
        , _x1PerKm(x1PerKm)
        , _b1PerKm(b1PerKm)
        , _r0PerKm(r0PerKm)
        , _x0PerKm(x0PerKm)
        , _iRatedA(iRatedA)
        , _uRatedKv(uRatedKv)
    {
    }

    // --- getteri ---
    const std::string& getName() const { return _name; }
    double getR1PerKm() const { return _r1PerKm; }
    double getX1PerKm() const { return _x1PerKm; }
    double getB1PerKm() const { return _b1PerKm; }
    double getR0PerKm() const { return _r0PerKm; }
    double getX0PerKm() const { return _x0PerKm; }
    double getIRatedA() const { return _iRatedA; }
    double getURatedKv() const { return _uRatedKv; }

    // --- setteri ---
    void setName(const std::string& name) { _name = name; }
    void setR1PerKm(double value) { _r1PerKm = value; }
    void setX1PerKm(double value) { _x1PerKm = value; }
    void setB1PerKm(double value) { _b1PerKm = value; }
    void setR0PerKm(double value) { _r0PerKm = value; }
    void setX0PerKm(double value) { _x0PerKm = value; }
    void setIRatedA(double value) { _iRatedA = value; }
    void setURatedKv(double value) { _uRatedKv = value; }

private:
    std::string _name;      // naziv tipa voda, npr. "XHE 49-A 3x150mm2"
    double _r1PerKm = 0.0;   // direktna (pozitivna) otpornost [ohm/km]
    double _x1PerKm = 0.0;   // direktna reaktansa [ohm/km]
    double _b1PerKm = 0.0;   // direktna susceptansa [S/km]
    double _r0PerKm = 0.0;   // nulta otpornost [ohm/km] - bitno za nesimetricnu mrezu
    double _x0PerKm = 0.0;   // nulta reaktansa [ohm/km]
    double _iRatedA = 0.0;   // nazivna struja [A]
    double _uRatedKv = 0.0;  // nazivni napon [kV]
};
