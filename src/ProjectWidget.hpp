#pragma once
/**
 * ProjectWidget.hpp
 *
 * Top level container for project file.
 */
#include "Common.hpp"
#include "AssetInfo.hpp"
#include "AssetDialog.hpp"
#include "TextureGroupInfo.hpp"

/// <summary>
/// Top level widget for loading and maintaining a project file.
/// </summary>
class ProjectWidget : public QWidget {
	Q_OBJECT
public:
	ProjectWidget(QWidget* parent = nullptr);
	~ProjectWidget();

	void init();
	bool load(const QString& filename);
	bool save(const QString& filename = "");
	bool generate(const QString& filename = "");

	AssetTreeView* getAssetTree() const;
	AssetTreeModel* getAssetModel() const;
	QString projectFilename();
	QString getErrorMessage() const;
	bool dirty() const;
	void setDirty(bool dirtyFlag);

private:
	void setProjectFilename(const QString& filename);
	QDomElement addElement(QDomDocument& doc, QDomNode& node, const QString& tag, const QString& value = "");
	void setAssetModel(AssetTreeModel* model);

	QString m_projectFilename;
	QString m_errorMessage;
	bool m_dirtyFlag;

	QVBoxLayout* m_layout_Main;
	QGroupBox* m_groupBox_Info;
	QFormLayout* m_layout_Info;
	QLabel* m_label_Info_Name;
	QCheckBox* m_checkBox_Info_UseEncryption;
	QLineEdit* m_lineEdit_Info_EncryptionPassword;
	QTabWidget* m_tabWidget_Main;
	QWidget* m_widget_AssetTab;
	QVBoxLayout* m_layout_AssetTab;
	QSplitter* m_splitter_AssetTab;
	AssetTreeView* m_treeView_AssetTab_Assets;
	AssetEditor* m_scrollArea_AssetTab_AssetEditor;
	QGroupBox* m_groupBox_AssetTab_AssetButtons;
	QHBoxLayout* m_layout_AssetTab_AssetButtons;
	QPushButton* m_pushButton_AssetTab_AddGroup;
	QPushButton* m_pushButton_AssetTab_AddFile;
	AssetDialog* m_assetDialog;
	QWidget* m_widget_TextureGroupTab;
	QVBoxLayout* m_layout_TextureGroupTab;
	QGroupBox* m_groupBox_TextureGroupTab_Info;
	QGridLayout* m_layout_TextureGroupTab_Info;
	QComboBox* m_comboBox_TextureGroupTab_Info_TextureGroup;
	QComboBox* m_comboBox_TextureGroupTab_Info_Size;
	QComboBox* m_comboBox_TextureGroupTab_Info_Format;
	QPushButton* m_pushButton_TextureGroupTab_AddGroup;
	QPushButton* m_pushButton_TextureGroupTab_DeleteGroup;
	TextureGroupContents* m_groupBox_TextureGroupTab_ViewGroup;
	QWidget* m_widget_TileSetTab;

public slots:
	void onCheckStateChanged_CheckBox_UseEncryption(Qt::CheckState state);
	void onClicked_PushButton_AssetTab_AddGroup();
	void onClicked_PushButton_AssetTab_EditGroup();
	void onClicked_PushButton_AssetTab_DeleteGroup();
	void onClicked_PushButton_AssetTab_AddFile();
	void onClicked_PushButton_AssetTab_EditFile();
	void onClicked_PushButton_AssetTab_DeleteFile();
	void onFinished_AssetDialog_AddFile(int status);
	void onFinished_AssetDialog_EditFile(int status);
	void contextMenu_AssetTree(const QPoint& pos);
	void onDataChanged();
	void onChanged_ComboBox_TextureGroupTab_TextureGroup(int index);
	void onClicked_PushButton_TextureGroupTab_AddGroup();
	void onClicked_PushButton_TextureGroupTab_DeleteGroup();
};
