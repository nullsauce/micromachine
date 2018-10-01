#ifndef BREAKPOINTREGISTRY_HPP
#define BREAKPOINTREGISTRY_HPP

#include <QObject>
#include <QQuickPaintedItem>
#include <unordered_map>
#include <memory>


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
	Q_PROPERTY(QQmlListProperty<Breakpoint> breakpoints READ breakpoints NOTIFY breakpointListChanged)
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

	QQmlListProperty<Breakpoint> breakpoints() {
		return QQmlListProperty<Breakpoint>(this, _iterable_list);
	}

signals:
	void breakpointsChanged();
	void breakpointListChanged();

private:

	Breakpoint& createBreakpoint(uint32_t address) {
		std::pair<breakpoint_list::iterator, bool> res = _breakpoints.emplace(
			address, breakpoint_ptr(new Breakpoint(address), &QObject::deleteLater)
		);
		Breakpoint* newBreakpoint = res.first->second.data();
		_iterable_list.append(newBreakpoint);
		// forward changes event to breakpointsChanged
		connect(newBreakpoint, &Breakpoint::enabledChanged, this, &BreakpointRegistry::breakpointsChanged);
		emit breakpointsChanged();
		emit breakpointListChanged();
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
	QList<Breakpoint*> _iterable_list;
};

#endif // BREAKPOINTREGISTRY_HPP
