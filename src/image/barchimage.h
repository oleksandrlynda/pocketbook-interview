#ifndef BARCHIMAGE_H
#define BARCHIMAGE_H

#include <vector>

#include "binarydata.h"
#include "bmpimage.h"

class BarchImage : public BmpImage
{
public:
    BarchImage();
    BarchImage(const std::string& path);

    BmpImage toBmp();
    void fromBmp(const BmpImage& bmp);

protected:
    void readPixels(std::ifstream &stream) override;
    bool writePixels(std::ofstream &stream) override;
    int paddingSize() const override;

private:
    BinaryData mBinaryData;
    BinaryData mEmptyRows;
};

#endif // BARCHIMAGE_H
