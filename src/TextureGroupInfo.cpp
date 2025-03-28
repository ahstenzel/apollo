#include "TextureGroupInfo.hpp"
#include "MainWindow.hpp"

QString g_defaultTextureGroupName = "(default)";
QStringList g_textureGroups = { g_defaultTextureGroupName };
QStringList g_textureGroupSizes = {
	"512",
	"1024",
	"2048",
	"4096"
};

TextureGroupContents::TextureGroupContents(QWidget* parent) :
	QGroupBox(parent) {
	m_textureGroupIndex = 0;
	m_layout = new QVBoxLayout();
	setLayout(m_layout);
	
	m_listView_Textures = new QListView();
	m_layout->addWidget(m_listView_Textures);
	m_listView_Textures->setStyleSheet("QListView { border:0; }");
}

TextureGroupContents::~TextureGroupContents() {
	delete m_layout;
}

void TextureGroupContents::showEvent(QShowEvent* ev) {
	setTextureGroupIndex(m_textureGroupIndex);
}

int TextureGroupContents::textureGroupIndex() const {
	return m_textureGroupIndex;
}

void TextureGroupContents::setTextureGroupIndex(int index) {
	QStringListModel* model = new QStringListModel();
	m_textureGroupIndex = index;
	if (g_mainWindow && m_textureGroupIndex >= 0 && m_textureGroupIndex < g_textureGroups.size()) {
		// Iterate through assets, finding all textures belonging to this group
		QStringList textureFiles;
		auto assetModel = g_mainWindow->getProjectWidget()->getAssetModel();
		QModelIndexList indexes = assetModel->getFilesWithProperty({ AssetType::Texture }, 3, { QString::number(m_textureGroupIndex) });
		for (auto& index : indexes) {
			AssetTreeItem* item = static_cast<AssetTreeItem*>(index.internalPointer());
			textureFiles << item->assetDescriptor()->name();
		}
		model->setStringList(textureFiles);
	}
	delete m_listView_Textures->model();
	m_listView_Textures->setModel(model);
}

TextureGroupNameDialog::TextureGroupNameDialog(QWidget* parent) :
	QDialog(parent) {
	m_layout = new QFormLayout();
	setLayout(m_layout);
	setWindowTitle("Add Texture Group");

	m_lineEdit_TextureGroupName = new QLineEdit();
	QRegularExpression regex_title("^[a-zA-Z][\\w\\-_]+");
	m_validator = new QRegularExpressionValidator(regex_title);
	m_lineEdit_TextureGroupName->setValidator(m_validator);
	connect(m_lineEdit_TextureGroupName, &QLineEdit::textChanged, this, &TextureGroupNameDialog::onChanged_ValidateAssets);
	m_layout->addRow("Texture Group Name", m_lineEdit_TextureGroupName);

	m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	m_buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
	connect(m_buttonBox, &QDialogButtonBox::accepted, this, &AssetDialog::accept);
	connect(m_buttonBox, &QDialogButtonBox::rejected, this, &AssetDialog::reject);
	m_layout->addWidget(m_buttonBox);
}

TextureGroupNameDialog::~TextureGroupNameDialog() {
	delete m_layout;
}

QString TextureGroupNameDialog::getTextureGroupName() {
	QString name = "";
	TextureGroupNameDialog dialog;
	if (dialog.exec() == QDialog::DialogCode::Accepted) {
		name = dialog.value();
	}
	return name;
}

QString TextureGroupNameDialog::value() {
	return m_lineEdit_TextureGroupName->text();
}

void TextureGroupNameDialog::setValue(const QString& value) {
	m_lineEdit_TextureGroupName->setText(value);
}

void TextureGroupNameDialog::onChanged_ValidateAssets() {
	m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
		!m_lineEdit_TextureGroupName->text().isEmpty()
	);
}