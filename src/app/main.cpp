#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "controller.h"

// test
#include "barchimage.h"
void test()
{
    BarchData data;
    data.setData(0, DataType::Tag);
    data.setData(2, DataType::Tag);
    data.setData(3, DataType::Tag);
    data.setData(0x01);
    data.setData(0x01);
    data.setData(0x01);
    data.setData(0x01);

    QString str;
    for (const auto& bit : data.vector)
    {
        str += bit ? "1" : "0";
    }
    qDebug().noquote() << "  actual result:" << str;
    qDebug().noquote() << "expected result:" << "0101100000001000000010000000100000001";

    QString str2;
    data.addPadding();
    for (const auto& bit : data.vector)
    {
        str2 += bit ? "1" : "0";
    }
    qDebug().noquote() << "  actual result:" << str2;
    qDebug().noquote() << "expected result:" << "0101100000001000000010000000100000001000";
}
// test end

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

    test();

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
