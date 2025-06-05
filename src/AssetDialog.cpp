#include "AssetDialog.hpp"

AssetDialog::AssetDialog(bool edit, QWidget* parent) :
	QDialog(parent) {
	m_layout = new QFormLayout();
	setLayout(m_layout);
	if (edit) setWindowTitle("Edit Asset");
	else setWindowTitle("Add Asset");

	m_lineEdit_Filename = new BrowseLineEdit();
	connect(m_lineEdit_Filename->lineEdit(), &QLineEdit::textChanged, this, &AssetDialog::onChanged_LineEdit_Filename);
	connect(m_lineEdit_Filename->pushButton(), &QPushButton::clicked, this, &AssetDialog::onClicked_Filename_Browse);
	m_layout->addRow("File", m_lineEdit_Filename);

	m_lineEdit_Title = new QLineEdit();
	QRegularExpression regex_title("^[a-zA-Z][\\w\\-_]+");
	m_validator = new QRegularExpressionValidator(regex_title);
	m_lineEdit_Title->setValidator(m_validator);
	connect(m_lineEdit_Title, &QLineEdit::textChanged, this, &AssetDialog::onChanged_ValidateAssets);
	m_layout->addRow("Title", m_lineEdit_Title);

	m_label_AssetType = new QLabel("");
	m_layout->addRow("Type", m_label_AssetType);

	m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	m_buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
	connect(m_buttonBox, &QDialogButtonBox::accepted, this, &AssetDialog::accept);
	connect(m_buttonBox, &QDialogButtonBox::rejected, this, &AssetDialog::reject);
	m_layout->addWidget(m_buttonBox);
}

AssetDialog::~AssetDialog() {
	delete m_validator;
	delete m_layout;
}

AssetDescriptorPtr AssetDialog::getAssetDescriptor() {
	AssetDescriptorPtr ptr = nullptr;
	AssetDialog dialog;
	if (dialog.exec() == QDialog::DialogCode::Accepted) {
		ptr = dialog.value();
	}
	return ptr;
}

AssetDescriptorPtr AssetDialog::value() {
	QString infoString = buildAssetString({ "%1", "%2", "%3" }, 
		m_label_AssetType->text(), 
		m_lineEdit_Title->text(), 
		m_lineEdit_Filename->text()
	);
	return AssetDescriptorFactory(infoString);
}

void AssetDialog::setValue(const AssetDescriptorPtr& value) {
	QString infoString = value->toString();
	QStringList list = infoString.split(g_assetDelim);
	m_label_AssetType->setText(list[0]);
	m_lineEdit_Title->setText((list.size() > 1) ? list[1] : "");
	m_lineEdit_Filename->setText((list.size() > 2) ? list[2] : "");
}

void AssetDialog::onClicked_Filename_Browse() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), g_dialogPathCache, "");
	if (!filename.isEmpty()) {
		QFileInfo fileInfo(filename);
		g_dialogPathCache = fileInfo.absolutePath();
		m_lineEdit_Filename->setText(filename);
	}
}

void AssetDialog::onChanged_LineEdit_Filename() {
	// Get filename extension
	QString filename = m_lineEdit_Filename->text();
	QFileInfo fileInfo(filename);
	QString fileExtension = fileInfo.completeSuffix();
	if (!filename.isEmpty() && m_lineEdit_Title->text().isEmpty()) {
		auto match = m_validator->regularExpression().match(fileInfo.baseName());
		if (match.hasMatch()) {
			m_lineEdit_Title->setText(fileInfo.baseName());
		}
	}

	// Get asset type
	AssetType type = AssetType::Unknown;
	if (!fileExtension.isEmpty()) {
		for (auto& pair : g_assetInfoMap) {
			if (pair.second.m_extensions.contains(fileExtension)) {
				type = pair.first;
				break;
			}
		}
		if (type == AssetType::Unknown) {
			type = AssetType::Binary;
		}
	}

	// Populate widgets
	m_label_AssetType->setText(g_assetInfoMap[type].m_name);
	onChanged_ValidateAssets();
}

void AssetDialog::onChanged_ValidateAssets() {
	m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
		!m_lineEdit_Title->text().isEmpty() &&
		!m_lineEdit_Filename->text().isEmpty() &&
		!m_label_AssetType->text().isEmpty()
	);
}