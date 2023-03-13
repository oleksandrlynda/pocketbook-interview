#ifndef BARCHIMAGE_H
#define BARCHIMAGE_H

#include <vector>

#include "bmpimage.h"

/* TODO:
 * 1. use bitset for empty rows
 * 2. write mData pixels as bits not bytes
 *
 */

enum DataType { Tag, Pixel };


/* TODO:
 * Maybe use vector<uint_8> and add next method that will add more uint8_t if needed.
 * or next method should point on bit in the array and add more uint8_t if needed.
 */
struct BarchData
{
    void setData(uint8_t byte, DataType type = Pixel);
    void addPadding();

    std::vector<bool> vector;
};

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
    std::vector<bool> mBinaryData;
    std::vector<uint8_t> mEmptyRows; // bitset instead of vector, serialize ulong
};

#endif // BARCHIMAGE_H
