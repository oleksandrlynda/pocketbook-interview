#ifndef BARCHIMAGE_H
#define BARCHIMAGE_H

#include <vector>

#include "bmpimage.h"

class BarchImage
{
public:
    BarchImage();

    BmpImage toBmp();
    void fromBmp(const BmpImage& bmp);

private:
    std::vector<int> emptyRows;
};

#endif // BARCHIMAGE_H
