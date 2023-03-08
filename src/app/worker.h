#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();

    void process();

    void setSuffix(const QString &newSuffix);
    void setFilePath(const QString &newFilePath);
    void setFileName(const QString &newFileName);

private:
    void convertBmpToBarch();
    void convertBarchToBmp();

signals:
    void finished();
    void progress(const QString& fileName, const QString& progress);
    void error(const QString& error);

private:
    QString mSuffix;
    QString mFileName;
    QString mFilePath;
};

#endif // WORKER_H
