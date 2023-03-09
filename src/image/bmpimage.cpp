#include "bmpimage.h"

#include <fstream>
#include <iostream>

BmpImage::BmpImage()
{

}

BmpImage::BmpImage(BitmapHeader &&header, std::vector<uint8_t> &&data)
    : mHeader {std::move(header)}
    , mData {std::move(data)}
{

}

BmpImage::BmpImage(const std::string& path)
    : mPath{path}
{}

bool BmpImage::load()
{
    std::ifstream stream(mPath, std::ios::binary);
    if (!stream.is_open())
    {
        std::cout << "Stream error: " << strerror(errno) << std::endl;
        mError = "Cannot open file.";
        return false;
    }

    if (!readHeader(stream))
    {
        return false;
    }
    stream.seekg(mHeader.offsetData);
    readPixels(stream);

    return true;
}

bool BmpImage::save(const std::string& path)
{
    std::ofstream stream(path, std::ios::binary);

    writeHeader(stream);
    return writePixels(stream);
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

    int8_t unusedByte;
    while (file.tellg() != mHeader.offsetData)
    {
        file.read(reinterpret_cast<char*>(&unusedByte), sizeof unusedByte);
        mHeader.unusedBytes.push_back(unusedByte);
    }

    return true;
}
void BmpImage::readPixels(std::ifstream &file)
{
    mData.reserve(mHeader.width * mHeader.height);
    for (int i = 0; i < mHeader.height; ++i)
    {
        for (int j = 0; j < mHeader.width; ++j)
        {
            uint8_t pixel;
            file.read(reinterpret_cast<char*>(&pixel), sizeof pixel);
            mData.push_back(pixel);
        }

        int32_t padding = 0;
        file.read(reinterpret_cast<char*>(&padding), paddingSize());
    }
}

void BmpImage::writeHeader(std::ofstream &stream)
{
    stream.write(reinterpret_cast<char*>(&mHeader.fileType), sizeof mHeader.fileType);
    stream.write(reinterpret_cast<char*>(&mHeader.fileSize), sizeof mHeader.fileSize);
    stream.write(reinterpret_cast<char*>(&mHeader.reserved1), sizeof mHeader.reserved1);
    stream.write(reinterpret_cast<char*>(&mHeader.reserved2), sizeof mHeader.reserved2);
    stream.write(reinterpret_cast<char*>(&mHeader.offsetData), sizeof mHeader.offsetData);
    stream.write(reinterpret_cast<char*>(&mHeader.headerSize), sizeof mHeader.headerSize);
    stream.write(reinterpret_cast<char*>(&mHeader.width), sizeof mHeader.width);
    stream.write(reinterpret_cast<char*>(&mHeader.height), sizeof mHeader.height);
    stream.write(reinterpret_cast<char*>(&mHeader.planes), sizeof mHeader.planes);
    stream.write(reinterpret_cast<char*>(&mHeader.bitsPerPixel), sizeof mHeader.bitsPerPixel);

    stream.write(mHeader.unusedBytes.data(), mHeader.unusedBytes.size());
}

bool BmpImage::writePixels(std::ofstream &stream)
{
    int validationCounter = 0;
    for (int i = 0; i < mHeader.height; ++i)
    {
        for (int j = 0; j < mHeader.width; ++j)
        {
            stream.write(reinterpret_cast<char*>(&mData[i * mHeader.width + j]), sizeof(uint8_t));
            validationCounter++;
        }

        uint32_t padding = 0;
        stream.write(reinterpret_cast<char*>(&padding), paddingSize());
    }
    if (validationCounter > mData.size())
    {
        mError = "Image is saved incorrectly. Extra bytes: " + std::to_string(mData.size() - validationCounter);
        return false;
    }
    else if (validationCounter < mData.size())
    {
        mError = "Image is saved incorrectly. Missed bytes: " + std::to_string(mData.size() - validationCounter);
        return false;
    }

    return true;
}

int BmpImage::paddingSize() const
{
    const auto pixelsRowWithPaddings = (mHeader.width + 3) / 4 * 4;
    return  pixelsRowWithPaddings - mHeader.width;
}

std::vector<uint8_t> BmpImage::data() const
{
    return mData;
}

BitmapHeader BmpImage::header() const
{
    return mHeader;
}
