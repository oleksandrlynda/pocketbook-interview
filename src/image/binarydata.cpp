#include "binarydata.h"

BinaryData::BinaryData()
    : mBitIterator(*this)
{}

void BinaryData::setData(uint8_t byte, DataType type)
{
    // compress tag
    int shift = 0;
    if (type == Compressed)
    {
        while ((byte & 0b10000000) == 0)
        {
            if (shift == 7)
            {
                break;
            }

            shift++;
            byte <<= 1;
        }
    }

    // data push per bit
    for (int i = 0; i < (8 - shift); ++i, byte <<= 1)
    {
        mBitIterator.nextBit();
        mBitIterator.setBit(byte & 0b10000000);
    }
}

uint8_t BinaryData::getByte(DataType type)
{
    const auto iterateToNext = [this]() {
        if (mBitIterator.canGoNextBit())
        {
            mBitIterator.nextBit();
        }
    };

    if (type == Compressed)
    {
        if (mBitIterator.getBit() == WHITE_PIXEL_TAG)
        {
            iterateToNext();
            return WHITE_PIXEL_TAG;
        }
        else
        {
            iterateToNext();
            if (mBitIterator.getBit())
            {
                iterateToNext();
                return COLOR_PIXEL_TAG;
            }
            else
            {
                iterateToNext();
                return BLACK_PIXEL_TAG;
            }
        }
    }
    else
    {
        uint8_t byte = 0;
        for (int i = 0; i < 8; ++i)
        {
            if (mBitIterator.getBit())
            {
                const auto mask = 0b10000000 >> i;
                byte |= mask;
            }
            iterateToNext();
        }
        return byte;
    }
}

uint8_t* BinaryData::data()
{
    return vector.data();
}

const size_t BinaryData::size() const
{
    return vector.size();
}

void BinaryData::resetIterator()
{
    mBitIterator.reset();
}

void BinaryData::resize(int size)
{
    vector.resize(size);
}

void BinaryData::clear()
{
    vector.clear();
}

BinaryBitIterator::BinaryBitIterator(BinaryData& data)
    : mData{data}
{}

bool BinaryBitIterator::canGoNextBit() const
{
    return ((mArrayIndex + 1) < mData.vector.size()) || (((mArrayIndex + 1) == mData.vector.size()) && (mBitIndex < 7));
}

void BinaryBitIterator::nextBit()
{
    if (!canGoNextBit())
    {
        mData.vector.push_back({});
        mArrayIndex = mData.vector.size() - 1;
        mBitIndex = 0;
        return;
    }

    if (mBitIndex < 8)
    {
        mBitIndex++;
    }
    if (mBitIndex == 8)
    {
        mArrayIndex++;
        mBitIndex = 0;
    }
}

void BinaryBitIterator::setBit(bool flag)
{
    if (flag)
    {
        const auto mask = 0b10000000 >> mBitIndex;
        mData.vector[mArrayIndex] |= mask;
    }
}

bool BinaryBitIterator::getBit() const
{
    const auto mask = 0b10000000 >> mBitIndex;
    return mData.vector[mArrayIndex] & mask;
}

void BinaryBitIterator::reset()
{
    mArrayIndex = 0;
    mBitIndex = 0;
}
