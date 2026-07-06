#pragma once

#include <vector>
#include <string>
#include "LineType.h"
#include "TransformerType.h"

// EquipmentCatalog - centralna kolekcija svih tipova opreme (vodovi i
// transformatori). GUI tabele (npr. za unos/izmjenu kataloga) ce raditi
// direktno nad ovom klasom.
class EquipmentCatalog
{
public:
    EquipmentCatalog() = default;

    // --- rad sa tipovima vodova ---
    void addLineType(const LineType& lineType);
    bool removeLineType(const std::string& name);
    LineType* findLineType(const std::string& name);
    const std::vector<LineType>& getLineTypes() const { return _lineTypes; }

    // --- rad sa tipovima transformatora ---
    void addTransformerType(const TransformerType& transformerType);
    bool removeTransformerType(const std::string& name);
    TransformerType* findTransformerType(const std::string& name);
    const std::vector<TransformerType>& getTransformerTypes() const { return _transformerTypes; }

    // --- perzistencija (jednostavan CSV format, po jedan fajl za svaki tip) ---
    bool saveLineTypesToCsv(const std::string& filePath) const;
    bool loadLineTypesFromCsv(const std::string& filePath);

    bool saveTransformerTypesToCsv(const std::string& filePath) const;
    bool loadTransformerTypesFromCsv(const std::string& filePath);

    size_t lineTypeCount() const { return _lineTypes.size(); }
    size_t transformerTypeCount() const { return _transformerTypes.size(); }

private:
    std::vector<LineType> _lineTypes;
    std::vector<TransformerType> _transformerTypes;
};
