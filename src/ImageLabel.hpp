#pragma once
/**
 * ImageLabel.hpp
 *
 * Simple label that displays a pixmap.
 */
#include "Common.hpp"

/// <summary>
/// Wrapper widget for displaying a scaled image.
/// </summary>
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