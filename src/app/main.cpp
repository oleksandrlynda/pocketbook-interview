#include "bmpimage.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    BmpImage image("Z:\\Pocketbook\\pocketbook-interview\\images\\test-image-2-gs.bmp");
    if (!image.load())
    {
        qDebug("Not loaded %s", image.errorString().data());
    }
    else
    {
        qDebug("Loaded");
    }

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
