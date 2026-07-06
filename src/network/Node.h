#pragma once

#include <string>

// Node - predstavlja cvor (sabirnicu) u distributivnoj mrezi.
class Node
{
public:
    Node() = default;

    Node(int id, const std::string& name, double nominalVoltageKv)
        : _id(id)
        , _name(name)
        , _nominalVoltageKv(nominalVoltageKv)
    {
    }

    int getId() const { return _id; }
    const std::string& getName() const { return _name; }
    double getNominalVoltageKv() const { return _nominalVoltageKv; }

    void setId(int id) { _id = id; }
    void setName(const std::string& name) { _name = name; }
    void setNominalVoltageKv(double value) { _nominalVoltageKv = value; }

private:
    int _id = -1;
    std::string _name;
    double _nominalVoltageKv = 0.0;
};
