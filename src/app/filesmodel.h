#ifndef FILESMODEL_H
#define FILESMODEL_H

#include <QAbstractListModel>
#include <QFileInfoList>

class FilesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        Name = Qt::UserRole,
        Size,
        Suffix,
        State
    };

    FilesModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setFolderPath(const QString& path);
    void updateFileStateByName(const QString& name, const QString& state);

private:
    QFileInfoList mList;
    QVector<QString> mStates;
};

#endif // FILESMODEL_H
