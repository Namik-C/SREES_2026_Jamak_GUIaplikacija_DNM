#include "EquipmentCatalog.h"

#include <fstream>
#include <sstream>
#include <algorithm>

// ---------------------------------------------------------------------
// Vodovi (LineType)
// ---------------------------------------------------------------------

void EquipmentCatalog::addLineType(const LineType& lineType)
{
    _lineTypes.push_back(lineType);
}

bool EquipmentCatalog::removeLineType(const std::string& name)
{
    auto it = std::find_if(_lineTypes.begin(), _lineTypes.end(),
        [&name](const LineType& lt) { return lt.getName() == name; });

    if (it == _lineTypes.end())
        return false;

    _lineTypes.erase(it);
    return true;
}

LineType* EquipmentCatalog::findLineType(const std::string& name)
{
    auto it = std::find_if(_lineTypes.begin(), _lineTypes.end(),
        [&name](const LineType& lt) { return lt.getName() == name; });

    return (it != _lineTypes.end()) ? &(*it) : nullptr;
}

// ---------------------------------------------------------------------
// Transformatori (TransformerType)
// ---------------------------------------------------------------------

void EquipmentCatalog::addTransformerType(const TransformerType& transformerType)
{
    _transformerTypes.push_back(transformerType);
}

bool EquipmentCatalog::removeTransformerType(const std::string& name)
{
    auto it = std::find_if(_transformerTypes.begin(), _transformerTypes.end(),
        [&name](const TransformerType& tt) { return tt.getName() == name; });

    if (it == _transformerTypes.end())
        return false;

    _transformerTypes.erase(it);
    return true;
}

TransformerType* EquipmentCatalog::findTransformerType(const std::string& name)
{
    auto it = std::find_if(_transformerTypes.begin(), _transformerTypes.end(),
        [&name](const TransformerType& tt) { return tt.getName() == name; });

    return (it != _transformerTypes.end()) ? &(*it) : nullptr;
}

// ---------------------------------------------------------------------
// Perzistencija - LineType CSV
// Format kolona: name,r1PerKm,x1PerKm,b1PerKm,r0PerKm,x0PerKm,iRatedA,uRatedKv
// ---------------------------------------------------------------------

bool EquipmentCatalog::saveLineTypesToCsv(const std::string& filePath) const
{
    std::ofstream file(filePath);
    if (!file.is_open())
        return false;

    file << "name,r1PerKm,x1PerKm,b1PerKm,r0PerKm,x0PerKm,iRatedA,uRatedKv\n";

    for (const auto& lt : _lineTypes)
    {
        file << lt.getName() << ','
             << lt.getR1PerKm() << ','
             << lt.getX1PerKm() << ','
             << lt.getB1PerKm() << ','
             << lt.getR0PerKm() << ','
             << lt.getX0PerKm() << ','
             << lt.getIRatedA() << ','
             << lt.getURatedKv() << '\n';
    }

    return true;
}

bool EquipmentCatalog::loadLineTypesFromCsv(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
        return false;

    _lineTypes.clear();

    std::string line;
    bool isHeader = true;

    while (std::getline(file, line))
    {
        if (isHeader) { isHeader = false; continue; } // preskoci header red
        if (line.empty())
            continue;

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ','))
            tokens.push_back(token);

        if (tokens.size() != 8)
            continue; // preskoci neispravan red

        LineType lt(
            tokens[0],
            std::stod(tokens[1]),
            std::stod(tokens[2]),
            std::stod(tokens[3]),
            std::stod(tokens[4]),
            std::stod(tokens[5]),
            std::stod(tokens[6]),
            std::stod(tokens[7])
        );

        _lineTypes.push_back(lt);
    }

    return true;
}

// ---------------------------------------------------------------------
// Perzistencija - TransformerType CSV
// Format kolona: name,snRatedKva,uPrimaryKv,uSecondaryKv,ukPercent,prCuKw,poFeKw,connectionGroup
// ---------------------------------------------------------------------

bool EquipmentCatalog::saveTransformerTypesToCsv(const std::string& filePath) const
{
    std::ofstream file(filePath);
    if (!file.is_open())
        return false;

    file << "name,snRatedKva,uPrimaryKv,uSecondaryKv,ukPercent,prCuKw,poFeKw,connectionGroup\n";

    for (const auto& tt : _transformerTypes)
    {
        file << tt.getName() << ','
             << tt.getSnRatedKva() << ','
             << tt.getUPrimaryKv() << ','
             << tt.getUSecondaryKv() << ','
             << tt.getUkPercent() << ','
             << tt.getPrCuKw() << ','
             << tt.getPoFeKw() << ','
             << tt.getConnectionGroup() << '\n';
    }

    return true;
}

bool EquipmentCatalog::loadTransformerTypesFromCsv(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
        return false;

    _transformerTypes.clear();

    std::string line;
    bool isHeader = true;

    while (std::getline(file, line))
    {
        if (isHeader) { isHeader = false; continue; }
        if (line.empty())
            continue;

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ','))
            tokens.push_back(token);

        if (tokens.size() != 8)
            continue;

        TransformerType tt(
            tokens[0],
            std::stod(tokens[1]),
            std::stod(tokens[2]),
            std::stod(tokens[3]),
            std::stod(tokens[4]),
            std::stod(tokens[5]),
            std::stod(tokens[6]),
            tokens[7]
        );

        _transformerTypes.push_back(tt);
    }

    return true;
}
