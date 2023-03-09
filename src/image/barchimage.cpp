#include "barchimage.h"

#include <algorithm>
#include <fstream>

#include <QDebug>

#define WHITE_PIXEL_TAG 0
#define BLACK_PIXEL_TAG 10
#define COLOR_PIXEL_TAG 11

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
                else if (*iter == COLOR_PIXEL_TAG)
                {
                    index+=4;
                    iter = std::next(iter);
                    bmpData.insert(bmpData.end(), 4, *iter);
                }
                else
                {
                    ++index;
                    bmpData.push_back(*iter);
                }

                if (iter != mData.end())
                {
                    iter = std::next(iter);
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

    mData.clear();
    mEmptyRows.clear();
    mEmptyRows.resize(mHeader.height);

    std::vector<uint8_t> compressedChunk;
    compressedChunk.reserve(mHeader.width);

    for (int i = 0; i < mHeader.height; ++i)
    {
        bool whiteRow = true;
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
                        compressedChunk.push_back(WHITE_PIXEL_TAG);
                    }
                    else if (firstPixel == BLACK_PIXEL)
                    {
                        whiteRow = false;
                        compressedChunk.push_back(BLACK_PIXEL_TAG);
                    }
                    else
                    {
                        whiteRow = false;
                        compressedChunk.push_back(COLOR_PIXEL_TAG);
                        compressedChunk.push_back(firstPixel);
                    }
                }
                else
                {
                    whiteRow = false;
                    compressedChunk.push_back(firstPixel);
                    compressedChunk.push_back(secondPixel);
                    compressedChunk.push_back(thirdPixel);
                    compressedChunk.push_back(fourthPixel);
                }
                j+=3;
            }
            else
            {
                const auto pixel = bmpData[i * mHeader.width + j];
                if (pixel != WHITE_PIXEL)
                {
                    whiteRow = false;
                }
                compressedChunk.push_back(pixel);
            }
        }

        if (whiteRow)
        {
            mEmptyRows[i] = WHITE_ROW_FLAG;
            compressedChunk.clear();
        }
        else
        {
            mData.insert(mData.end(), compressedChunk.begin(), compressedChunk.end());
            compressedChunk.clear();
        }
    }

//    const auto emptyRowsCount = std::count_if(mEmptyRows.begin(), mEmptyRows.end(), [](bool value){
//        return value;
//    });
//    qDebug() << "Skipped "<< emptyRowsCount << ", with size: " << emptyRowsCount * mHeader.width;
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
    stream.write(reinterpret_cast<char*>(mData.data()), mData.size());
    return true;
}

int BarchImage::paddingSize() const
{
    return 0;
}
