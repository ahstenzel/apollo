#pragma once
/**
 * BrowseLineEdit.hpp
 *
 * Convenience class for having a line edit + a browse button for picking files.
 */
#include "Common.hpp"

class BrowseLineEdit : public QGroupBox {
	Q_OBJECT
public:
	BrowseLineEdit(QWidget* parent = nullptr);
	BrowseLineEdit(const QString& contents, QWidget* parent = nullptr);
	~BrowseLineEdit();

	QLineEdit* lineEdit();
	QPushButton* pushButton();
	QString text();
	void setText(const QString& text);

private:
	QHBoxLayout* m_layout;
	QLineEdit* m_lineEdit;
	QPushButton* m_pushButton;
};