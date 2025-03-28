#include "ImageLabel.hpp"

ImageLabel::ImageLabel(const QPixmap& pixmap, QWidget* parent) :
	ImageLabel(parent) {
	setPixmap(pixmap);
	setAlignment(Qt::AlignCenter);
}

ImageLabel::ImageLabel(QWidget* parent) :
	QLabel(parent) {
	setMinimumSize(16, 16);
	setMaximumSize(512, 512);
	setScaledContents(false);
	setFrameShape(QFrame::Panel);
}

ImageLabel::~ImageLabel() = default;

int ImageLabel::heightForWidth(int width) const {
	return m_pixmap.isNull()
		? height()
		: int(((qreal)m_pixmap.height() * width) / m_pixmap.width());
}

QSize ImageLabel::sizeHint() const {
	int w = width();
	return QSize(w, heightForWidth(w));
}

QPixmap ImageLabel::scaledPixmap() const {
	return m_pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::FastTransformation);
}

void ImageLabel::setPixmap(const QPixmap& pixmap) {
	m_pixmap = pixmap;
	QLabel::setPixmap(scaledPixmap());
}

void ImageLabel::resizeEvent(QResizeEvent* ev) {
	if (!m_pixmap.isNull()) {
		QLabel::setPixmap(scaledPixmap());
	}
}