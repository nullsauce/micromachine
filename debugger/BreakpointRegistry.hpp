#ifndef BREAKPOINTREGISTRY_HPP
#define BREAKPOINTREGISTRY_HPP

#include <QObject>
#include <QQuickPaintedItem>
#include <unordered_map>
#include <memory>

class Register : public QObject {
	Q_OBJECT
	Q_PROPERTY(QString name READ name CONSTANT)
	Q_PROPERTY(quint32 value READ value NOTIFY valueChanged)
	Q_PROPERTY(QString hexValue READ hexValue NOTIFY valueChanged)
public:
	Register(QObject* parent = nullptr, const QString& name = "")
		: QObject(parent)
		, _val(0)
		, _name(name) {

	}

	QString hexValue() const {
		 return QString("%1").arg(_val, 8, 16, QChar('0'));
	}

	uint32_t value() const {
		return _val;
	}

	void setValue(uint32_t v) {
		if(v != _val) {
			_val = v;
			emit valueChanged();
		}
	}

	QString name() const {
		return _name;
	}

signals:
	void valueChanged();

private:

	uint32_t _val;
	const QString _name;
};

class Breakpoint : public QObject {
	Q_OBJECT
	Q_PROPERTY(quint32 address MEMBER _address CONSTANT)
	Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

public:
	void setEnabled(bool val) {
		if(val != _enabled) {
			_enabled = val;
			emit enabledChanged();
		}
	}

	bool enabled() const {
		return _enabled;
	}

	Breakpoint(uint32_t address = 0, QObject* parent = nullptr)
		: QObject(parent)
		, _enabled(true)
		, _address(address){

	}

signals:
	void enabledChanged();

private:
	bool _enabled;
	const uint32_t _address;

};

class BreakpointRegistry : public QObject {
	Q_OBJECT
public:
	using breakpoint_ptr = QSharedPointer<Breakpoint>;
	using breakpoint_list = std::unordered_map<uint32_t, breakpoint_ptr>;

public slots:

	void setBreakPoint(quint32 address, bool enabled) {
		Breakpoint& bp = findOrCreateAtAddress(address);
		bp.setEnabled(enabled);
	}

	bool toggleBreakpoint(quint32 address) {
		bool newState = !shouldBreakAt(address);
		setBreakPoint(address, newState);
		return newState;
	}

	bool shouldBreakAt(quint32 instructionAddr) {
		auto bkp = _breakpoints.find(instructionAddr);
		if(bkp != _breakpoints.end()) {
			return bkp->second->enabled();
		} else {
			return false;
		}
	}

signals:
	void breakpointsChanged();

private:

	Breakpoint& createBreakpoint(uint32_t address) {
		std::pair<breakpoint_list::iterator, bool> res = _breakpoints.emplace(
			address, breakpoint_ptr(new Breakpoint(address), &QObject::deleteLater)
		);
		Breakpoint* newBreakpoint = res.first->second.data();
		// forward changes event to breakpointsChanged
		connect(newBreakpoint, &Breakpoint::enabledChanged, this, &BreakpointRegistry::breakpointsChanged);
		emit breakpointsChanged();
		return *newBreakpoint;
	}

	Breakpoint& findOrCreateAtAddress(uint32_t address) {
		auto it = _breakpoints.find(address);
		if(_breakpoints.end() != it) {
			return *it->second.data();
		} else {
			return createBreakpoint(address);
		}
	}

	breakpoint_list _breakpoints;
};

#endif // BREAKPOINTREGISTRY_HPP
