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
    int paddingSize() const;

private:
    std::vector<int> emptyRows;
};

#endif // BARCHIMAGE_H
