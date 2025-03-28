#include "BrowseLineEdit.hpp"

BrowseLineEdit::BrowseLineEdit(QWidget* parent) :
	BrowseLineEdit("", parent) {}

BrowseLineEdit::BrowseLineEdit(const QString& contents, QWidget* parent) :
	QGroupBox(parent) {
	setFlat(true);
	setStyleSheet("BrowseLineEdit { border:0; }");
	m_layout = new QHBoxLayout();
	setLayout(m_layout);
	m_lineEdit = new QLineEdit(contents);
	m_pushButton = new QPushButton("...");
	m_layout->addWidget(m_lineEdit);
	m_layout->addWidget(m_pushButton);
	m_layout->setSpacing(0);
	m_layout->setContentsMargins(0, 0, 0, 0);
}

BrowseLineEdit::~BrowseLineEdit() {
	delete m_layout;
}

QLineEdit* BrowseLineEdit::lineEdit() {
	return m_lineEdit;
}

QPushButton* BrowseLineEdit::pushButton() {
	return m_pushButton;
}

QString BrowseLineEdit::text() {
	return m_lineEdit->text();
}

void BrowseLineEdit::setText(const QString& text) {
	m_lineEdit->setText(text);
}