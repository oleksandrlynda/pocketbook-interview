#include "barchimage.h"

#include <algorithm>
#include <fstream>

#define WHITE_PIXEL 0xFF
#define BLACK_PIXEL 0

#define WHITE_ROW_FLAG 1
#define NOT_WHITE_ROW_FLAG 0

BarchImage::BarchImage()
    : BmpImage()
{

}

BarchImage::BarchImage(const std::string &path)
    : BmpImage(path)
{

}

BmpImage BarchImage::toBmp()
{
    std::vector<uint8_t> bmpData;
    bmpData.reserve(mHeader.width * mHeader.height);
    mBinaryData.resetIterator();

    auto rowsIter = BinaryBitIterator(mEmptyRows);
    for (int row = 0; row < mHeader.height; ++row)
    {
        if (rowsIter.getBit() == WHITE_ROW_FLAG)
        {
            bmpData.insert(bmpData.end(), mHeader.width, WHITE_PIXEL);
        }
        else
        {
            int pixelIndex = 0;
            while (pixelIndex < mHeader.width)
            {
                uint8_t byte = mBinaryData.getByte(Compressed);
                if (byte == WHITE_PIXEL_TAG)
                {
                    pixelIndex += 4;
                    bmpData.insert(bmpData.end(), 4, WHITE_PIXEL);
                }
                else if (byte == BLACK_PIXEL_TAG)
                {
                    pixelIndex += 4;
                    bmpData.insert(bmpData.end(), 4, BLACK_PIXEL);
                }
                else if (byte == COLOR_PIXEL_TAG)
                {
                    int shift = 4;
                    if ((pixelIndex + shift) > mHeader.width)
                    {
                        shift = mHeader.width - pixelIndex;
                    }
                    for (int i = 0; i < shift; ++i)
                    {
                        bmpData.push_back(mBinaryData.getByte());
                        pixelIndex += 1;
                    }
                }
            }
        }
        if (rowsIter.canGoNextBit())
        {
            rowsIter.nextBit();
        }
    }

    return BmpImage(std::move(mHeader), std::move(bmpData));
}

void BarchImage::fromBmp(const BmpImage &bmp)
{
    mHeader = bmp.header();
    const auto bmpData = bmp.data();

    mBinaryData.clear();
    mData.clear();
    mEmptyRows.clear();

    for (int i = 0; i < mHeader.height; ++i)
    {
        bool whiteRow = true;
        for (int j = 0; j < mHeader.width; ++j)
        {
            const auto pixel = bmpData[i * mHeader.width + j];
            if (pixel != WHITE_PIXEL)
            {
                whiteRow = false;
                break;
            }
        }
        if (whiteRow)
        {
            mEmptyRows.setData(WHITE_ROW_FLAG, Compressed);
            continue;
        }
        else
        {
            mEmptyRows.setData(NOT_WHITE_ROW_FLAG, Compressed);
        }

        for (int j = 0; j < mHeader.width; ++j)
        {
            if ((j + 3) < mHeader.width)
            {
                const auto firstPixel = bmpData[i * mHeader.width + j];
                const auto secondPixel = bmpData[i * mHeader.width + j + 1];
                const auto thirdPixel = bmpData[i * mHeader.width + j + 2];
                const auto fourthPixel = bmpData[i * mHeader.width + j + 3];

                if ((firstPixel == secondPixel) && (firstPixel == thirdPixel) && (firstPixel == fourthPixel))
                {
                    if (firstPixel == WHITE_PIXEL)
                    {
                        mBinaryData.setData(WHITE_PIXEL_TAG, Compressed);
                    }
                    else if (firstPixel == BLACK_PIXEL)
                    {
                        mBinaryData.setData(BLACK_PIXEL_TAG, Compressed);
                    }
                    else
                    {
                        mBinaryData.setData(COLOR_PIXEL_TAG, Compressed);
                        mBinaryData.setData(firstPixel);
                        mBinaryData.setData(secondPixel);
                        mBinaryData.setData(thirdPixel);
                        mBinaryData.setData(fourthPixel);
                    }
                }
                else
                {
                    mBinaryData.setData(COLOR_PIXEL_TAG, Compressed);
                    mBinaryData.setData(firstPixel);
                    mBinaryData.setData(secondPixel);
                    mBinaryData.setData(thirdPixel);
                    mBinaryData.setData(fourthPixel);
                }
                j+=3;
            }
            else
            {
                mBinaryData.setData(COLOR_PIXEL_TAG, Compressed);
                int shift = mHeader.width - j;
                for (int h = 0; h < shift; ++h)
                {
                    const auto pixel = bmpData[i * mHeader.width + j + h];
                    mBinaryData.setData(pixel);
                }
                j += shift - 1;
            }
        }
    }
}

BinaryData BarchImage::compressedData() const
{
    return mBinaryData;
}

void BarchImage::readPixels(std::ifstream &stream)
{
    mEmptyRows.resize((mHeader.height + 7) / 8);
    stream.read(reinterpret_cast<char*>(mEmptyRows.data()), mEmptyRows.size());

    const int beginDataPos = stream.tellg();
    stream.seekg(0, stream.end);
    const int endDataPos = stream.tellg();
    stream.seekg(beginDataPos);
    mBinaryData.resize(endDataPos - beginDataPos);
    stream.read(reinterpret_cast<char*>(mBinaryData.data()), mBinaryData.size());
}

bool BarchImage::writePixels(std::ofstream &stream)
{
    stream.write(reinterpret_cast<char*>(mEmptyRows.data()), mEmptyRows.size());
    stream.write(reinterpret_cast<char*>(mBinaryData.data()), mBinaryData.size());
    return true;
}
