#pragma once
/**
 * AssetDialog.hpp
 *
 * Dialog for adding a new asset to the project.
 */
#include "Common.hpp"
#include "BrowseLineEdit.hpp"
#include "AssetInfo.hpp"

class AssetDialog : public QDialog {
	Q_OBJECT;
public:
	AssetDialog(QWidget* parent = nullptr);
	~AssetDialog();

	static AssetDescriptorPtr getAssetDescriptor();
	AssetDescriptorPtr value();
	void setValue(const AssetDescriptorPtr& value);

private:
	QFormLayout* m_layout;
	QLineEdit* m_lineEdit_Title;
	QLabel* m_label_AssetType;
	BrowseLineEdit* m_lineEdit_Filename;
	QDialogButtonBox* m_buttonBox;
	QRegularExpressionValidator* m_validator;

public slots:
	void onClicked_Filename_Browse();
	void onChanged_LineEdit_Filename();
	void onChanged_ValidateAssets();
};