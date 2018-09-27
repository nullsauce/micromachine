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
#include "InstructionDetails.hpp"

int main(int argc, char *argv[])
{
	//QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;
	const std::vector<QString> fonts = {
		"FiraCode-Regular.otf",
		"FiraCode-Bold.otf",
		"FiraCode-Medium.otf",
		"FiraCode-Light.otf",
		"FiraCode-Retina.otf",
		"SourceCodeVariable-Italic.otf",
		"Hack-Regular.ttf",
		"Hack-Italic.ttf",
		"Hack-BoldItalic.ttf",
		"Hack-Bold.ttf",
	};
	for(const auto& font : fonts) {
		if(-1 == QFontDatabase::addApplicationFont(QString(":/fonts/%1").arg(font))) {
			qWarning() << "failed to load application font" << font;
		}
	}
	QFontDatabase database;
	qDebug() << database.families();

	qmlRegisterType<QCpu>("Fla", 1, 0, "Cpu");
	qmlRegisterUncreatableType<Register>("Fla", 1, 0, "Register", "cannot create");
	qmlRegisterUncreatableType<Instruction>("Fla", 1, 0, "Instruction", "cannot create");
	qmlRegisterType<MemView>("Fla", 1, 0, "MemView");
	qmlRegisterType<Disassembler>("Fla", 1, 0, "Disassembler");
	qmlRegisterUncreatableType<QMemRegion>("Fla", 1, 0, "MemoryRegion", "cannot create");
	qmlRegisterType<AddressTrackerList>("Fla", 1, 0, "AddressTrackerList");
	qmlRegisterUncreatableType<AddressTracker>("Fla", 1, 0, "AddressTracker", "cannot create");
	QCpu processor;
	qmlRegisterType<InstructionDetails>("Fla", 1, 0, "InstructionDetails");
	qmlRegisterType<InstructionComponent>("Fla", 1, 0, "InstructionComponent");
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
