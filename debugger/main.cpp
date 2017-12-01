#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "cpu.hpp"
#include "Cpu.hpp"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<QCpu>("Fla", 1, 0, "Cpu");
    qmlRegisterType<Register>("Fla", 1, 0, "Register");
	qmlRegisterType<Instruction>("Fla", 1, 0, "Instruction");
    QCpu processor;
    engine.rootContext()->setContextProperty("CPU", &processor);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
