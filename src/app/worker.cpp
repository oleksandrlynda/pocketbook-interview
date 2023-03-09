#include "worker.h"
#include "barchimage.h"
#include "bmpimage.h"

#include <QFileInfo>

Worker::Worker(QObject *parent)
    : QObject{parent}
{

}

Worker::~Worker()
{

}

void Worker::process()
{
    if (mSuffix.contains("bmp", Qt::CaseInsensitive))
    {
        emit progress(mFileName, "Encoding bmp");
        convertBmpToBarch();
    }
    else if (mSuffix.contains("barch", Qt::CaseInsensitive))
    {
        emit progress(mFileName, "Decoding barch");
        convertBarchToBmp();
    }
    else
    {
        emit error(mFileName + ": Unsupported format");
    }

    emit progress(mFileName, {});
    emit finished();
}

void Worker::setSuffix(const QString &newSuffix)
{
    mSuffix = newSuffix;
}

void Worker::setFilePath(const QString &newFilePath)
{
    mFilePath = newFilePath;
}

void Worker::setFileName(const QString &newFileName)
{
    mFileName = newFileName;
}

void Worker::convertBmpToBarch()
{
    BmpImage image(mFilePath.toStdString());
    if (!image.load())
    {
        emit error(mFileName + ": " + QString::fromStdString(image.errorString()));
        return;
    }

    QFileInfo info(mFilePath);
    const auto newFileName = info.baseName() + "_packed.barch";
    const auto newImagePath = info.absolutePath() + "/" + newFileName;

    BarchImage convertedImage;
    convertedImage.fromBmp(image);
    if (!convertedImage.save(newImagePath.toStdString()))
    {
        emit error(newFileName + ": " + QString::fromStdString(convertedImage.errorString()));
        return;
    }
}

void Worker::convertBarchToBmp()
{
    BarchImage image(mFilePath.toStdString());
    if (!image.load())
    {
        emit error(mFileName + ": " + QString::fromStdString(image.errorString()));
        return;
    }

    QFileInfo info(mFilePath);
    const auto newFileName = info.baseName() + "_unpacked.bmp";
    const auto newImagePath = info.absolutePath() + "/" + newFileName;

    BmpImage convertedImage = image.toBmp();
    if (!convertedImage.save(newImagePath.toStdString()))
    {
        emit error(newFileName + ": " + QString::fromStdString(convertedImage.errorString()));
        return;
    }
}
