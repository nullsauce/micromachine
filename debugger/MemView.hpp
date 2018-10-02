#ifndef MEMVIEW_HPP
#define MEMVIEW_HPP

#include <QtMath>
#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QFontDatabase>

#include "QMemRegion.hpp"

class MemView : public QQuickPaintedItem
{
	Q_OBJECT
	Q_PROPERTY(int columns READ columns NOTIFY dimensionsChanged)
	Q_PROPERTY(int rows READ rows NOTIFY dimensionsChanged)
	Q_PROPERTY(qreal scroll READ scroll WRITE setScroll NOTIFY scrollChanged)
	Q_PROPERTY(qreal visibleRatio READ visibleRatio NOTIFY dimensionsChanged)
	Q_PROPERTY(QMemRegion* memoryRegion READ memoryRegion WRITE setMemoryRegion NOTIFY regionChanged)
public:

	MemView()
		: mFont("Hack", 12)
		, mFontColor("#bdbdb5")
		, mBackgroundColor("#1d1d1d")
		, mFontMap(generateFontMap())
		, mOffset(0)
		, mMem(nullptr)
		, mNumberOfColumns(0)
		, mNumberOfRows(0)
		, mNumberOfDisplayableItems(0)
		, mAddressAreaWidth(90) {
		connect(this, &MemView::offsetChanged, this, &MemView::refresh, Qt::DirectConnection);
		connect(this, &MemView::dimensionsChanged, this, &MemView::refresh, Qt::DirectConnection);
		connect(this, &MemView::regionChanged, this, &MemView::refresh, Qt::DirectConnection);
	}

	void setMemoryRegion(QMemRegion* region) {
		if(mMem != region) {
			if(mMem) {
				disconnect(mMem, &QMemRegion::memoryChanged, this, &MemView::refresh);
			}
			mMem = region;
			connect(mMem, &QMemRegion::memoryChanged, this, &MemView::refresh);
			emit regionChanged();
		}
	}

	QMemRegion* memoryRegion() {
		return mMem;
	}

	qreal scroll() const {
		if(nullptr == mMem) return 0;
		return (qreal)mOffset/mMem->virtualRange();
	}

	void setScroll(qreal scroll) {
		size_t totalNumberOfRowsInRange = mMem->virtualRange() / mNumberOfColumns;
		size_t atRow = (size_t)qFloor(scroll * totalNumberOfRowsInRange);
		uint32_t offset = atRow * mNumberOfColumns;
		setOffset(offset);
		emit scrollChanged();
	}

	qreal visibleRatio() const {
		if(nullptr == mMem) return 1;
		uint32_t totalNumberOfRows = (mMem->virtualRange() / mNumberOfColumns);
		return 0.9 * ((qreal)mNumberOfRows / (qreal)totalNumberOfRows);
	}

	int columns() const {
		return mNumberOfColumns;
	}

	int rows() const {
		return mNumberOfRows;
	}

	size_t ceildiv(size_t x, size_t y) {
		return(x + y - 1) / y;
	}

	int wordWidth() const {
		return 4 * mByteTextSize.width();
	}

	bool isAddressVisible(quint32 address) {
		uint32_t visibleRangeStartAddress = mMem->virtualBase() + mOffset;
		uint32_t visibleRangeSize = columns() * rows();
		uint32_t visibleRangeEndAddress = qMin(visibleRangeStartAddress + visibleRangeSize
											   , mMem->virtualBase() + mMem->virtualRange());
		return address >= visibleRangeStartAddress && address < visibleRangeEndAddress;
	}

	Q_INVOKABLE QRectF addressBounds(quint32 address) {
		if(!mMem->isValidVirtualAddress(address)) {
			return QRectF();
		}
		if(!isAddressVisible(address)) {
			return QRectF();
		}
		uint32_t visibleOffset = address - (mMem->virtualBase() + mOffset);
		int row = visibleOffset / columns();
		int col = visibleOffset % columns();
		return QRectF((col*mByteTextSize.width())+mAddressAreaWidth,
					  (row*mByteTextSize.height()),
					  mByteTextSize.width(),
					  mByteTextSize.height());
	}

	Q_INVOKABLE quint32 addressAtPos(qreal x, qreal y) {
		const QRectF hexDisplayArea = boundingRect().adjusted(mAddressAreaWidth,0,0,0);
		if(!hexDisplayArea.contains(QPointF(x,y))) {
			return 0;
		}
		x = x - hexDisplayArea.x();
		y = y - hexDisplayArea.y();
		int column = (int)qFloor(x / mByteTextSize.width());
		int row = (int)qFloor(y / mByteTextSize.height());
		uint32_t offset =  (row*columns()+column);
		return memoryRegion()->virtualBase() + offset;
	}

	void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {

		if(newGeometry.isEmpty()) return;

		const QRectF hexDisplayArea = newGeometry.adjusted(mAddressAreaWidth,0,0,0);

		int maxDisplayCols = (int)(hexDisplayArea.width() / (float)mByteTextSize.width());
		setNumberOfColumns(maxDisplayCols);

		int maxDisplayRows = (int)(hexDisplayArea.height() / (float)mByteTextSize.height());
		setNumberOfRows(maxDisplayRows);

		int maxDisplayItems = maxDisplayCols * maxDisplayRows;
		setNumberOfDisplaybleItems(maxDisplayItems);
	}

	void paint(QPainter* painter) override {

		painter->fillRect(this->boundingRect(), mBackgroundColor);
		painter->setFont(mFont);

		painter->save();

		painter->translate(mAddressAreaWidth,0);
		painter->setClipRect(0,0,mNumberOfColumns*mByteTextSize.width(), mNumberOfRows*mByteTextSize.height());

		//painter->setPen(Qt::red/*mBackgroundColor.darker(115)*/);
		const QBrush colors[2] = {QColor("#1e292e"), mBackgroundColor};
		size_t nextColor = 0;
		for(size_t y = 0; y < mNumberOfRows; y++) {

			uint32_t address = (mMem->virtualBase() + mOffset) + (y * mNumberOfColumns);
			if(!mMem->isValidVirtualAddress(address)) break;
			int offset = address % 4;
			uint32_t lastAddress = address + mNumberOfColumns;
			int lastAddressOffset = lastAddress % 4;
			uint32_t numWords = (mNumberOfColumns/4)+(mNumberOfColumns % 4 != 0);
			for(size_t x = 0; x < numWords; x++) {
				uint32_t blockStartAddress = address+(x*4);
				if(!mMem->isValidVirtualAddress(blockStartAddress)) break;
				QRectF rect(x*4*mByteTextSize.width(),y*mByteTextSize.height(),4*mByteTextSize.width(),mByteTextSize.height());
				rect.adjust(-offset*mByteTextSize.width(),0,-offset*mByteTextSize.width(),0);
				painter->fillRect(rect, colors[nextColor]);
				//painter->drawRect(rect);

				if(x != numWords-1 || lastAddressOffset == 0) {
					nextColor = (nextColor + 1) % 2;
				}
			}
		}
		painter->restore();


		painter->save();
		painter->setPen(QColor("#6c6c6c"));
		for(size_t y = 0; y < mNumberOfRows; y++) {
			uint32_t address = (mMem->virtualBase() + mOffset) + (y * mNumberOfColumns);
			painter->drawText(
				QRectF(QPointF(5, y*mByteTextSize.height()), QSize(mAddressAreaWidth, mByteTextSize.height())),
				Qt::TextSingleLine | Qt::AlignLeft | Qt::AlignVCenter,
				QString("%1").arg(address, 8, 16, QChar('0'))
			);
		}
		painter->restore();

		painter->translate(mAddressAreaWidth,0);
		painter->setPen(Qt::black);
		painter->drawLine(-3,0,-3,this->boundingRect().height());
		uint32_t offset = mOffset;
		for(size_t y = 0; y < mNumberOfRows; y++) {
			for(size_t x = 0; x < mNumberOfColumns; x++) {

				if(mMem->isValidSegmentOffset(offset)) {

					uint8_t byte = mMem->hostMemoryBase()[offset++];
					QPointF loc(
						x * mByteTextSize.width(),
						y * mByteTextSize.height()
					);

					painter->drawPixmap(
						QRectF(loc, mByteTextSize),
						mFontMap,
						QRectF(QPointF(byte*mByteTextSize.width(), 0), mByteTextSize)
					);
				}
			}
		}
	}

signals:
	void dimensionsChanged();
	void regionChanged();
	void offsetChanged();
	void scrollChanged();

public slots:
	void refresh() {
		update();
	}

private:
	bool canScrollDown() const {
		if(nullptr == mMem) return false;
		return mOffset < (mMem->virtualRange() - mNumberOfColumns);
	}

	bool canScrollUp() const {
		return mOffset >= mNumberOfColumns;
	}

	void scrollDown() {
		setOffset(mOffset + mNumberOfColumns);
	}

	void scrollUp() {
		setOffset(mOffset - mNumberOfColumns);
	}

	void tryScrollDown() {
	   if(canScrollDown()) scrollDown();
	}

	void tryScrollUp() {
		if(canScrollUp()) scrollUp();
	}

	void setOffset(size_t offset) {
		if(offset != mOffset) {
			mOffset = offset;
			emit offsetChanged();
		}
	}

	void setNumberOfColumns(size_t num) {
		if(num != mNumberOfColumns) {
			mNumberOfColumns = num;
			emit dimensionsChanged();
		}
	}

	void setNumberOfRows(size_t num) {
		if(num != mNumberOfRows) {
			mNumberOfRows = num;
			emit dimensionsChanged();
		}
	}

	void setNumberOfDisplaybleItems(size_t num) {
		if(num != mNumberOfDisplayableItems) {
			mNumberOfDisplayableItems = num;
			emit dimensionsChanged();
		}
	}


	QPixmap generateFontMap() {

		QFontMetrics metrics(mFont);
		mByteTextSize = 1.2 * metrics.size(Qt::TextSingleLine, "00");

		QPixmap image(mByteTextSize.width()*256, mByteTextSize.height());
		image.fill(Qt::transparent);
		QPainter painter(&image);
		painter.setFont(mFont);
		painter.setPen(mFontColor);

		for(int i = 0; i < 256; i++) {
			QPointF loc(
				i * mByteTextSize.width(),
				0
			);
			painter.drawText(
				QRectF(loc, mByteTextSize),
				Qt::TextSingleLine | Qt::AlignCenter,
				QString("%1").arg(i, 2, 16, QChar('0'))
			);
		}

		return image;
	}

	QFont mFont;
	QColor mFontColor;
	QColor mBackgroundColor;
	QSizeF mByteTextSize;
	const QPixmap mFontMap;

	//uint8_t* mBase;
	uint32_t mOffset;
	//uint8_t* mLimit;
	//std::vector<uint8_t> junk;

	QMemRegion* mMem;

	size_t mNumberOfColumns;
	size_t mNumberOfRows;
	size_t mNumberOfDisplayableItems;
	int mAddressAreaWidth;



};

#endif // MEMVIEW_HPP
