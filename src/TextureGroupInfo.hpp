#pragma once
/**
 * TextureGroupInfo.hpp
 *
 * Collection of classes and functions for editing texture groups.
 */
#include "Common.hpp"

extern QString g_defaultTextureGroupName;
extern QStringList g_textureGroups;
extern QStringList g_textureGroupSizes;

/// <summary>
/// Widget for displaying all textures in a group.
/// </summary>
class TextureGroupContents : public QGroupBox {
	Q_OBJECT
public:
	explicit TextureGroupContents(QWidget* parent = nullptr);
	~TextureGroupContents();

	virtual void showEvent(QShowEvent* ev);
	int textureGroupIndex() const;

public slots:
	void setTextureGroupIndex(int index);

private:
	int m_textureGroupIndex;

	QVBoxLayout* m_layout;
	QListView* m_listView_Textures;
};

/// <summary>
/// Dialog for naming a texture group.
/// </summary>
class TextureGroupNameDialog : public QDialog {
	Q_OBJECT
public:
	explicit TextureGroupNameDialog(QWidget* parent = nullptr);
	~TextureGroupNameDialog();

	static QString getTextureGroupName();
	QString value();
	void setValue(const QString& value);

private:
	QFormLayout* m_layout;
	QLineEdit* m_lineEdit_TextureGroupName;
	QDialogButtonBox* m_buttonBox;
	QRegularExpressionValidator* m_validator;

public slots:
	void onChanged_ValidateAssets();
};