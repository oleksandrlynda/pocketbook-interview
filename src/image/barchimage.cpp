#include "barchimage.h"

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
    for (int i = 0; i < mData.size(); i++)
    {
        mData[i] = 255 - mData[i];
    }
    return *this;
}

void BarchImage::fromBmp(const BmpImage &bmp)
{
    mHeader = bmp.header();
    mData = bmp.data();

    for (int i = 0; i < mData.size(); i++)
    {
        mData[i] = 255 - mData[i];
    }
}

int BarchImage::paddingSize() const
{
    return 0;
}
