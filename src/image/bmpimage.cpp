#include "bmpimage.h"

#include <fstream>

#include <QDebug>

BmpImage::BmpImage(const std::string& path)
    : mPath{path}
{}

bool BmpImage::load()
{
    std::ifstream stream(mPath, std::ios::binary);
    if (!stream.is_open())
    {
        qDebug() << "Stream error:" << strerror(errno);
        mError = "Cannot open file.";
        return false;
    }

    if (!readHeader(stream))
    {
        return false;
    }
    stream.seekg(mHeader.offsetData - 6);
    readPixels(stream);

    return true;
}

std::string BmpImage::errorString() const
{
    return mError;
}

bool BmpImage::readHeader(std::ifstream& file)
{
    file.read(reinterpret_cast<char*>(&mHeader.fileType), sizeof mHeader.fileType);
    file.read(reinterpret_cast<char*>(&mHeader.fileSize), sizeof mHeader.fileSize);
    file.read(reinterpret_cast<char*>(&mHeader.reserved1), sizeof mHeader.reserved1);
    file.read(reinterpret_cast<char*>(&mHeader.reserved2), sizeof mHeader.reserved2);
    file.read(reinterpret_cast<char*>(&mHeader.offsetData), sizeof mHeader.offsetData);
    file.read(reinterpret_cast<char*>(&mHeader.headerSize), sizeof mHeader.headerSize);
    file.read(reinterpret_cast<char*>(&mHeader.width), sizeof mHeader.width);
    file.read(reinterpret_cast<char*>(&mHeader.height), sizeof mHeader.height);
    file.read(reinterpret_cast<char*>(&mHeader.planes), sizeof mHeader.planes);
    file.read(reinterpret_cast<char*>(&mHeader.bitsPerPixel), sizeof mHeader.bitsPerPixel);

    if (mHeader.fileType != 0x4D42)
    {
        mError = "File is not BMP format";
        return false;
    }

    return true;
}

void BmpImage::readPixels(std::ifstream &file)
{
    for (int i = 0; i < mHeader.height; ++i)
    {
        for (int j = 0; j < mHeader.width; ++j)
        {
            uint8_t pixel;
            file.read(reinterpret_cast<char*>(&pixel), sizeof pixel);
            mPixels.push_back(pixel);
        }

        int32_t padding = 0;
        file.read(reinterpret_cast<char*>(&padding), sizeof padding);
    }
}
