#include "controller.h"
#include "bmpimage.h"
#include "filesmodel.h"

#include <QSortFilterProxyModel>

Controller::Controller(QObject *parent)
    : QObject{parent}
    , mModel{new FilesModel(parent)}
    , mFilterModel {new QSortFilterProxyModel(parent)}
{
    mFilterModel->setSourceModel(mModel);
    mFilterModel->setFilterRole(FilesModel::Suffix);
}

void Controller::init(const QString &path)
{
    mPath = path;
    mModel->setFolderPath(mPath);
}

QString Controller::imagesPath() const
{
    return mPath;
}

QAbstractItemModel *Controller::model() const
{
    return mFilterModel;
}

void Controller::filter(const QString &suffix)
{
    if (suffix == "All")
    {
        mFilterModel->setFilterFixedString("");
    }
    else
    {
        mFilterModel->setFilterFixedString(suffix);
    }
    mFilterModel->invalidate();
}

void Controller::processFile(int index)
{
    const auto fileName = mModel->data(mModel->index(index), FilesModel::Name).toString();
    const auto filePath = mPath + "\\" + fileName;

    BmpImage image(filePath.toStdString());
    if (!image.load())
    {
        emit errorOccured(QString::fromStdString(image.errorString()));
    }
    else
    {
        qDebug("Loaded");
    }
}
