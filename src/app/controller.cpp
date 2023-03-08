#include "controller.h"

#include "filesmodel.h"
#include "worker.h"

#include <QSortFilterProxyModel>
#include <QThread>

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
    updateModel();
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

    const auto sourceIndex = mFilterModel->mapToSource(mFilterModel->index(index, 0));
    const auto suffix = mModel->data(sourceIndex, FilesModel::Suffix).toString();
    const auto fileName = mModel->data(sourceIndex, FilesModel::Name).toString();
    const auto filePath = mPath + "\\" + fileName;

    Worker* worker = new Worker;
    worker->setFileName(fileName);
    worker->setFilePath(filePath);
    worker->setSuffix(suffix);

    QThread* thread = new QThread;
    worker->moveToThread(thread);
    connect(worker, &Worker::progress, mModel, &FilesModel::updateFileStateByName);
    connect(worker, &Worker::error, this, &Controller::errorOccured);
    connect(thread, &QThread::started, worker, &Worker::process);
    connect(worker, &Worker::finished, thread, &QThread::quit);
    connect(worker, &Worker::finished, worker, &Worker::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::finished, this, &Controller::updateModel);
    thread->start();
}

void Controller::updateModel()
{
    mModel->setFolderPath(mPath);
}
