#include "worker.h"
#include "bmpimage.h"

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
    }
    else if (mSuffix.contains("barch", Qt::CaseInsensitive))
    {
        emit progress(mFileName, "Decoding barch");
    }

    BmpImage image(mFilePath.toStdString());
    if (!image.load())
    {
        emit error(QString::fromStdString(image.errorString()));
    }
    else
    {
        qDebug("Loaded");
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
