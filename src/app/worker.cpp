#include "worker.h"
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

    // temp bmp to bmp
    QFileInfo info(mFilePath);
    const auto newFileName = info.baseName() + "_packed.bmp";
    const auto newImagePath = info.absolutePath() + "/" + newFileName;
    qDebug(qPrintable(newImagePath));
    if (!image.save(newImagePath.toStdString()))
    {
        emit error(newFileName + ": " + QString::fromStdString(image.errorString()));
        return;
    }
}

void Worker::convertBarchToBmp()
{

}
