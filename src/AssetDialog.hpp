#pragma once
/**
 * AssetDialog.hpp
 *
 * Dialog for adding a new asset to the project.
 */
#include "Common.hpp"
#include "BrowseLineEdit.hpp"
#include "AssetInfo.hpp"

/// <summary>
/// Popup dialog for adding a new asset to the project.
/// </summary>
class AssetDialog : public QDialog {
	Q_OBJECT;
public:
	AssetDialog(QWidget* parent = nullptr);
	~AssetDialog();

	/// <summary>
	/// Wrapper for spawning a dialog and returning the asset descriptor from it.
	/// </summary>
	/// <returns>Asset descriptor pointer</returns>
	static AssetDescriptorPtr getAssetDescriptor();

	/// <summary>
	/// Get the descriptor for the asset represented by the dialog.
	/// </summary>
	/// <returns>Asset descriptor pointer</returns>
	AssetDescriptorPtr value();

	/// <summary>
	/// Set the asset represented by the dialog to the given descriptor.
	/// </summary>
	/// <param name="value"></param>
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