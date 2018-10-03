#ifndef INTEGERTEXT_HPP
#define INTEGERTEXT_HPP

#include <QObject>
#include <QPainter>
#include <QQuickPaintedItem>

class IntegerText : public QQuickPaintedItem
{
	Q_OBJECT
	Q_PROPERTY(quint32 value READ value WRITE setValue NOTIFY valueChanged)
	Q_PROPERTY(DisplayMode displayMode READ displayMode WRITE setDisplayMode NOTIFY displayModeChanged)
	Q_PROPERTY(int contentWidth MEMBER _content_width CONSTANT)
	Q_PROPERTY(int contentHeight MEMBER _content_height CONSTANT)
	Q_PROPERTY(QString asHexString READ asHexString NOTIFY valueChanged)
	Q_PROPERTY(QString asSignedDecimalString READ asSignedDecimalString NOTIFY valueChanged)
	Q_PROPERTY(QString asUnsignedDecimalString READ asUnsignedDecimalString NOTIFY valueChanged)

public:
	// TODO: cache values for case "update once, read many times"
	IntegerText(QQuickItem* parent = nullptr)
		: QQuickPaintedItem(parent)
		, _value(0)
		, _font("Hack")
		, _pen(QColor("#d3d3d3"))
		, _display_mode(DisplayMode::Hexadecimal)
		, _content_width(0)
		, _content_height(0)
	{
		_font.setPixelSize(18);
		QFontMetrics metrics(_font);
		_content_width = metrics.width("-9999999999");
		_content_height = metrics.height();
	}

	enum DisplayMode {
		Hexadecimal,
		SignedDecimal,
		UnsignedDecimal
	};
	Q_ENUM(DisplayMode)


	quint32 value() const {
		return _value;
	}

	void setValue(quint32 value) {
		if(_value != value) {
			_value = value;
			emit valueChanged();
			update();
		}
	}

	DisplayMode displayMode() const {
		return _display_mode;
	}

	void setDisplayMode(DisplayMode value) {
		if(value != displayMode()) {
			_display_mode = value;
			emit displayModeChanged();
			update();
		}
	}

	QString asHexString() const {
		return QString("%1").arg(_value, 8, 16, QChar('0'));
	}

	QString asSignedDecimalString() const {
		return QString::number(asSignedInteger());
	}

	QString asUnsignedDecimalString() const {
		return QString::number(_value);
	}

	void paint(QPainter* painter) override {
		QString txt = asString();
		int flags = Qt::AlignVCenter | Qt::AlignRight;
		painter->setPen(_pen);
		painter->setFont(_font);
		painter->drawText(this->boundingRect(), flags, txt);
	}

signals:
	void valueChanged();
	void displayModeChanged();

public slots:

private:

	QString asString() const {
		switch(_display_mode) {
			case DisplayMode::SignedDecimal: return asSignedDecimalString();
			case DisplayMode::UnsignedDecimal: return asUnsignedDecimalString();
			default:
			case DisplayMode::Hexadecimal: return asHexString();
		}
	}

	int32_t asSignedInteger() const {
		return static_cast<int32_t>(_value);
	}

	uint32_t _value;
	QFont _font;
	const QPen _pen;
	DisplayMode _display_mode;
	int _content_width;
	int _content_height;
};

#endif // INTEGERTEXT_HPP
