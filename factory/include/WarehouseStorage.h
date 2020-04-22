#ifndef WAREHOUSE_STORAGE
#define WAREHOUSE_STORAGE

#define STORAGE_LOCATION "lagerbestand.txt"
#define STORAGE_SIZE 9

#include <fstream>
#include <map>
#include "utils.h"

enum class WarehouseContent
{
    NO_BOX = -1,
    EMPTY_BOX = 0,
    WHITE = 1,
    RED = 2,
    BLUE = 3
};

class IStorage
{
public:
    virtual WarehouseContent getWorkpieceAt(uint8_t) = 0;
    virtual void setWorkpieceAt(uint8_t, WarehouseContent) = 0;
    virtual int getPositionOf(WarehouseContent) = 0;
    virtual std::string getAsJson() = 0;
};

class FileStorage : public IStorage
{
    WarehouseContent values[STORAGE_SIZE];

public:
    FileStorage();
    WarehouseContent getWorkpieceAt(uint8_t) override;
    void setWorkpieceAt(uint8_t, WarehouseContent) override;
    int getPositionOf(WarehouseContent) override;
    std::string getAsJson() override;
};

#endif
