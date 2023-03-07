#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include <string>
#include <vector>

struct BitmapHeader
{
    uint16_t fileType{}; // File type always BM which is 0x4D42
    uint32_t fileSize{}; // Size of the file (in bytes)
    uint16_t reserved1{}; // Reserved, always 0
    uint16_t reserved2{}; // Reserved, always 0
    uint32_t offsetData{}; // Start position of pixel data (bytes from the beginning of file)
    uint32_t headerSize{}; // Size of header (bytes)
    int32_t width{}; // Width of the image (pixels)
    int32_t height{}; // Height of the image (pixels)
    uint16_t planes{}; // Number of color planes, always 1
    uint16_t bitsPerPixel{}; // Number of bits per pixel
};

class BmpImage
{
public:
    BmpImage(const std::string& path);

    bool load();
    std::string errorString() const;

protected:
    bool readHeader(std::ifstream& file);

private:
    std::string mPath;
    std::string mError;
    BitmapHeader mHeader;
    std::vector<unsigned char> mPixels;
};

#endif // BMPIMAGE_H
