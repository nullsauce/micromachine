#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "cpu.hpp"
#include "QCpu.hpp"
#include "QMemRegion.hpp"
#include "MemView.hpp"
#include "Disassembler.hpp"
#include "AddressTracker.hpp"
#include "AddressTrackerList.hpp"

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<QCpu>("Fla", 1, 0, "Cpu");
    qmlRegisterUncreatableType<Register>("Fla", 1, 0, "Register", "cannot create");
    qmlRegisterUncreatableType<Instruction>("Fla", 1, 0, "Instruction", "cannot create");
    qmlRegisterType<MemView>("Fla", 1, 0, "MemView");
    qmlRegisterType<Disassembler>("Fla", 1, 0, "Disassembler");
    qmlRegisterUncreatableType<QMemRegion>("Fla", 1, 0, "MemoryRegion", "cannot create");
    qmlRegisterType<AddressTrackerList>("Fla", 1, 0, "AddressTrackerList");
    qmlRegisterUncreatableType<AddressTracker>("Fla", 1, 0, "AddressTracker", "cannot create");
    QCpu processor;
	if(argc < 1) {
		qDebug() << "Elf file argument is missing";
		return -1;
	}
	processor.loadElf(argv[1]);
    engine.rootContext()->setContextProperty("CPU", &processor);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
