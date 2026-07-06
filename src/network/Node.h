#pragma once

#include <string>

// Node - predstavlja cvor (sabirnicu) u distributivnoj mrezi.
class Node
{
public:
    Node() = default;

    Node(int id, const std::string& name, double nominalVoltageKv, double x = 0.0, double y = 0.0)
        : _id(id)
        , _name(name)
        , _nominalVoltageKv(nominalVoltageKv)
        , _x(x)
        , _y(y)
    {
    }

    int getId() const { return _id; }
    const std::string& getName() const { return _name; }
    double getNominalVoltageKv() const { return _nominalVoltageKv; }
    double getX() const { return _x; }
    double getY() const { return _y; }

    void setId(int id) { _id = id; }
    void setName(const std::string& name) { _name = name; }
    void setNominalVoltageKv(double value) { _nominalVoltageKv = value; }
    void setPosition(double x, double y) { _x = x; _y = y; }

private:
    int _id = -1;
    std::string _name;
    double _nominalVoltageKv = 0.0;
    double _x = 0.0;
    double _y = 0.0;
};
