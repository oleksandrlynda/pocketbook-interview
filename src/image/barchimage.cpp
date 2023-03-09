#include "barchimage.h"

#include <algorithm>
#include <fstream>

#include <QDebug>

#define WHITE_PIXEL_TAG 0
#define BLACK_PIXEL_TAG 10
#define COLOR_PIXEL_TAG 11

#define WHITE_PIXEL 0xFF
#define BLACK_PIXEL 0

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

    for (int i = 0; i < mData.size(); i++)
    {
        mData[i] = 255 - mData[i];
    }

    return *this;
}

void BarchImage::fromBmp(const BmpImage &bmp)
{
    mHeader = bmp.header();
    const auto bmpData = bmp.data();
    mData.clear();

    std::vector<uint8_t> emptyRows;
    emptyRows.resize(mHeader.height);

    std::vector<uint8_t> compressedData;
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
            emptyRows[i] = 1;
            compressedChunk.clear();
        }
        else
        {
            compressedData.insert(compressedData.end(), compressedChunk.begin(), compressedChunk.end());
            compressedChunk.clear();
        }
    }

    const auto emptyRowsCount = std::count_if(emptyRows.begin(), emptyRows.end(), [](bool value){
        return value;
    });
    qDebug() << "Skipped "<< emptyRowsCount << ", with size: " << emptyRowsCount * mHeader.width;

    mData.reserve(emptyRows.size() + compressedData.size());
//    mData.push_back(0xaa); // TODO: remove padding
    mData.insert(mData.end(), emptyRows.begin(), emptyRows.end());
//    mData.push_back(0xbb); // TODO: remove padding
    mData.insert(mData.end(), compressedData.begin(), compressedData.end());
}

void BarchImage::readPixels(std::ifstream &stream)
{
    mData.resize(mHeader.height * mHeader.width);
    stream.read(reinterpret_cast<char*>(mData.data()), mData.size());
}

bool BarchImage::writePixels(std::ofstream &stream)
{
    stream.write(reinterpret_cast<char*>(mData.data()), mData.size());
    return true;
}

int BarchImage::paddingSize() const
{
    return 0;
}
