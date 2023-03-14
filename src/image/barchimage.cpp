#include "barchimage.h"

#include <algorithm>
#include <fstream>

#include <QDebug>

#define WHITE_PIXEL_TAG 0b0
#define BLACK_PIXEL_TAG 0b10
#define COLOR_PIXEL_TAG 0b11

#define WHITE_PIXEL 0xFF
#define BLACK_PIXEL 0

#define WHITE_ROW_FLAG 1

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

    auto iter = mData.begin();
    for (int row = 0; row < mEmptyRows.size(); ++row)
    {
        if (mEmptyRows[row] == WHITE_ROW_FLAG)
        {
            bmpData.insert(bmpData.end(), mHeader.width, WHITE_PIXEL);
        }
        else
        {
            int index = 0;
            while (index < mHeader.width)
            {
                if (*iter == WHITE_PIXEL_TAG)
                {
                    index+=4;
                    bmpData.insert(bmpData.end(), 4, WHITE_PIXEL);
                }
                else if (*iter == BLACK_PIXEL_TAG)
                {
                    index+=4;
                    bmpData.insert(bmpData.end(), 4, BLACK_PIXEL);
                }
                else if (*iter == COLOR_PIXEL_TAG) // all other colors should be tagged with it
                {
                    int shift = 4;
                    if ((index + shift) > mHeader.width)
                    {
                        shift = mHeader.width - index;
                    }
                    for (int i = 0; i < shift; ++i)
                    {
                        iter = std::next(iter);
                        bmpData.push_back(*iter);
                        index+=1;
                    }
                }
                else // TODO: don't need this else
                {
                    ++index;
                    bmpData.push_back(*iter);
                }

                if (iter != mData.end())
                {
                    iter = std::next(iter);
                }
                else
                {
                    break;
                }
            }
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
    mEmptyRows.resize(mHeader.height); // TODO:

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
            mEmptyRows[i] = WHITE_ROW_FLAG;
            continue;
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

void BarchImage::readPixels(std::ifstream &stream)
{
    mEmptyRows.resize(mHeader.height);
    stream.read(reinterpret_cast<char*>(mEmptyRows.data()), mEmptyRows.size());

    const int beginDataPos = stream.tellg();
    stream.seekg(0, stream.end);
    const int endDataPos = stream.tellg();
    stream.seekg(beginDataPos);
    mData.resize(endDataPos - beginDataPos);
    stream.read(reinterpret_cast<char*>(mData.data()), mData.size());
}

bool BarchImage::writePixels(std::ofstream &stream)
{
    stream.write(reinterpret_cast<char*>(mEmptyRows.data()), mEmptyRows.size());
    stream.write(reinterpret_cast<char*>(mBinaryData.data()), mBinaryData.size());
    return true;
}

int BarchImage::paddingSize() const
{
    return 0;
}
