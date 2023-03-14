#include "test.h"

#include "barchimage.h"

#include <QDebug>
#include <QString>
#include <QTextStream>

#define TEST_RESULT(expectedResult, actualResult) \
    if (expectedResult != actualResult) \
    { \
        qWarning() << "\n" << __FUNCTION__; \
        qWarning().noquote() << "  actual result:" << actualResult; \
        qWarning().noquote() << "expected result:" << expectedResult; \
    }

QString generatePixelsString(const std::vector<uint8_t>& data)
{
    QString str;
    QTextStream stream(&str);
    for (const auto& pixel : data)
    {
        if (pixel < 0x10)
        {
            stream << '0';
        }
        stream << Qt::hex << pixel;
        stream << ' ';
    }
    return str;
}

QString generateBitString(BinaryData& data)
{
    QString str;
    BinaryBitIterator iter(data);
    int bitsByteCounter = 0;
    while (true)
    {
        bitsByteCounter++;
        str += iter.getBit() ? "1" : "0";
        if (iter.canGoNextBit())
        {
            if (bitsByteCounter == 8)
            {
                bitsByteCounter = 0;
                str += " ";
            }
            iter.nextBit();
        }
        else
        {
            break;
        }
    }
    return str;
}

void test::test()
{
    writeBinaryTest();
    bmpToBarchTest();
    barchToBmpTest();
}

void test::writeBinaryTest()
{
    BinaryData data;
    data.setData(0, DataType::Compressed);
    data.setData(2, DataType::Compressed);
    data.setData(3, DataType::Compressed);
    data.setData(0x01);
    data.setData(0x01);
    data.setData(0x01);
    data.setData(0x01);

    QString expectedResult("01011000 00001000 00001000 00001000 00001000");
    QString actualResult = generateBitString(data);

    TEST_RESULT(expectedResult, actualResult)
}

void test::bmpToBarchTest()
{
    BitmapHeader header;
    header.height = 2;
    header.width = 12;

    std::vector<uint8_t> data{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01};

    BmpImage image(std::move(header), std::move(data));
    BarchImage barch;
    barch.fromBmp(image);
    auto compressedData = barch.compressedData();

    QString expectedResult("01011000 00001000 00001000 00001000 00001000");
    QString actualResult = generateBitString(compressedData);
    TEST_RESULT(expectedResult, actualResult)
}

void test::barchToBmpTest()
{
    BitmapHeader header;
    header.height = 2;
    header.width = 12;

    std::vector<uint8_t> data{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01};
    QString expectedResult = generatePixelsString(data);

    BmpImage image(std::move(header), std::move(data));
    BarchImage barch;
    barch.fromBmp(image);
    BmpImage convertedImage = barch.toBmp();
    const auto convertedData = convertedImage.data();

    QString actualResult = generatePixelsString(convertedData);
    TEST_RESULT(expectedResult, actualResult)
}
