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

    // temp
    BmpImage image(mPath.toStdString() + "\\test-image-2-gs.bmp");
    if (!image.load())
    {
        qDebug("Not loaded %s", image.errorString().data());
    }
    else
    {
        qDebug("Loaded");
    }
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
