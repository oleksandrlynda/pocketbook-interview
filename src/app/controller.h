#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QAbstractItemModel>

class QSortFilterProxyModel;
class FilesModel;

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString imagesPath READ imagesPath CONSTANT)
    Q_PROPERTY(QAbstractItemModel* model READ model CONSTANT)
public:
    explicit Controller(QObject *parent = nullptr);

    void init(const QString& path);
    QString imagesPath() const;
    QAbstractItemModel* model() const;

    Q_INVOKABLE void filter(const QString& suffix);
    Q_INVOKABLE void processFile(int index);

private:
    void updateModel();

signals:
    void errorOccured(const QString& error);

private:
    QString mPath;
    FilesModel* mModel = nullptr;
    QSortFilterProxyModel* mFilterModel = nullptr;
};

#endif // CONTROLLER_H
