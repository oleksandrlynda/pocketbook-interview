#ifndef BINARYDATA_H
#define BINARYDATA_H

#include <vector>

enum DataType { Compressed, Normal };

class BinaryData;

class BinaryBitIterator
{
public:
    BinaryBitIterator(BinaryData& data);

    bool canGoNextBit() const;
    void nextBit();

    void setBit(bool flag);
    bool getBit() const;

    void reset();

private:
    int mArrayIndex = 0;
    int mBitIndex = 0;

    BinaryData& mData;
};

class BinaryData
{
public:
    BinaryData();
    void setData(uint8_t byte, DataType type = Normal);

    uint8_t* data();
    const size_t size() const;
    void clear();

private:
    std::vector<uint8_t> vector;

    BinaryBitIterator mBitIterator;
    friend class BinaryBitIterator;
};

#endif // BINARYDATA_H
