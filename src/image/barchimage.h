#ifndef BARCHIMAGE_H
#define BARCHIMAGE_H

#include <vector>

#include "bmpimage.h"

class BarchImage : public BmpImage
{
public:
    BarchImage();
    BarchImage(const std::string& path);

    BmpImage toBmp();
    void fromBmp(const BmpImage& bmp);

protected:
    void readPixels(std::ifstream &stream);
    bool writePixels(std::ofstream &stream);
    int paddingSize() const;
};

#endif // BARCHIMAGE_H
