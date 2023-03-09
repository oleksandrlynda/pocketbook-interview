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
    void readPixels(std::ifstream &stream) override;
    bool writePixels(std::ofstream &stream) override;
    int paddingSize() const override;

private:
    std::vector<uint8_t> mEmptyRows;
};

#endif // BARCHIMAGE_H
