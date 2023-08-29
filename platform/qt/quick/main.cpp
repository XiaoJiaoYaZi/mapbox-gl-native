#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickStyle>


int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    QQuickStyle::setStyle("Basic");

    QQmlApplicationEngine engine;
    const QUrl url("qrc:/mbglquick/platform/qt/quick/Main.qml");
    engine.rootContext()->setContextProperty("$App", app.applicationDirPath());
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
