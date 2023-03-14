#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "controller.h"

QString parseImagesDir(const QGuiApplication& app)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("PocketBook interview");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption option(QStringList() << "i" << "images-directory", "images directory", "images directory");
    option.setDefaultValue("./images");
    parser.addOption(option);
    parser.process(app);

    return parser.value(option);
}

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    auto controller = new Controller(&app);
    controller->init(parseImagesDir(app));
    engine.rootContext()->setContextProperty("controller", controller);
    engine.load(url);

    return app.exec();
}
