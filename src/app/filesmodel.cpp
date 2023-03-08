#include "filesmodel.h"

#include <QDir>

FilesModel::FilesModel(QObject *parent)
{

}

int FilesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return mList.count();
}

QVariant FilesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount())
    {
        return {};
    }

    const auto fileInfo = mList.at(index.row());

    switch (role) {
    case Name:
        return fileInfo.fileName();
    case Size:
        return fileInfo.size();
    case Suffix:
        return fileInfo.suffix();
    default:
        return {};
    }
}

QHash<int, QByteArray> FilesModel::roleNames() const
{
    return {
        {Name, "name"},
        {Size, "size"},
        {Suffix, "suffix"}
    };
}

void FilesModel::setFolderPath(const QString &path)
{
    beginResetModel();
    QDir dir(path);
    mList = dir.entryInfoList({"*.bmp", "*.png", "*.barch"});
    endResetModel();
}
