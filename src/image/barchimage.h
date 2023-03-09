#ifndef BARCHIMAGE_H
#define BARCHIMAGE_H

#include <vector>

#include "bmpimage.h"

/* TODO:
 * 1. use bitset for empty rows
 * 2. write mData pixels as bits not bites
 *
 */

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
    std::vector<uint8_t> mEmptyRows; // bitset instead of vector, serialize ulong
};

#endif // BARCHIMAGE_H
