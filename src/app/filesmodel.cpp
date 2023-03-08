#include "filesmodel.h"

#include <QDir>
#include <algorithm>

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
    case State:
        return mStates[index.row()];
    default:
        return {};
    }
}

QHash<int, QByteArray> FilesModel::roleNames() const
{
    return {
        {Name, "name"},
        {Size, "size"},
        {Suffix, "suffix"},
        {State, "state"}
    };
}

void FilesModel::setFolderPath(const QString &path)
{
    beginResetModel();
    QDir dir(path);
    mList = dir.entryInfoList({"*.bmp", "*.png", "*.barch"});
    mStates.clear();
    mStates.resize(mList.count());
    endResetModel();
}

void FilesModel::updateFileStateByName(const QString &name, const QString &state)
{
    auto it = std::find_if(mList.begin(), mList.end(), [&name](const QFileInfo& info){
        return name == info.fileName();
    });

    if (it != mList.end())
    {
        int fileIndex = it - mList.begin();
        mStates[fileIndex] = state;

        const auto fileModelIndex = index(fileIndex);
        emit dataChanged(fileModelIndex, fileModelIndex);
    }

}
