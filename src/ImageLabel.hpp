#pragma once
/**
 * ImageLabel.hpp
 *
 * Simple label that displays a pixmap.
 */
#include "Common.hpp"

class ImageLabel : public QLabel {
	Q_OBJECT
public:
	explicit ImageLabel(const QPixmap& pixmap, QWidget* parent = nullptr);
	explicit ImageLabel(QWidget* parent = nullptr);
	~ImageLabel();

	virtual int heightForWidth(int width) const;
	virtual QSize sizeHint() const;
	QPixmap scaledPixmap() const;

public slots:
	void setPixmap(const QPixmap& pixmap);
	void resizeEvent(QResizeEvent* ev);

private:
	QPixmap m_pixmap;
};

/*
class ImageLabel : public QLabel {
	Q_OBJECT;
public:
	explicit ImageLabel(QWidget* parent = nullptr);
	explicit ImageLabel(const QPixmap& pixmap, QWidget* parent = nullptr);
	~ImageLabel();

public slots:
	void setPixmap(const QPixmap& pixmap);

private slots:
	void resizeImage();

protected:
	void resizeEvent(QResizeEvent* ev);
};
*/
