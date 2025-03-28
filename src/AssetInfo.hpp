#pragma once
/**
 * AssetInfo.hpp
 *
 * Collection of classes and functions for viewing & storing assets.
 */
#include "Common.hpp"

enum class AssetType {
	Unknown = 0,
	Group,
	Texture,
	Sound,
	Mesh,
	Text,
	Binary,
};

extern const QChar g_assetDelim;
extern const QString g_rootAssetName;

QString ImageFormatString(QImage::Format format);

class AssetDescriptorBase;
typedef std::shared_ptr<AssetDescriptorBase> AssetDescriptorPtr;

template<typename... Args>
inline QString buildAssetString(QStringList tokens, Args&&... args) {
	return tokens.join(g_assetDelim).arg(std::forward<Args>(args)...);
}
inline QString buildAssetString(QStringList tokens) {
	return tokens.join(g_assetDelim);
}

class AssetDescriptorBase {
public:
	AssetDescriptorBase();
	~AssetDescriptorBase();
	AssetType assetType() const;
	virtual QString toString() const;
	virtual void fromString(const QString& string);

	virtual QString name();
	virtual void setName(const QString& name);

	static AssetDescriptorPtr factory();
protected:
	AssetType m_assetType = AssetType::Unknown;
	QString m_name = "";
};

class AssetDescriptorGroup : public AssetDescriptorBase {
public:
	AssetDescriptorGroup();
	~AssetDescriptorGroup();
	QString toString() const override;
	void fromString(const QString& string) override;

	static AssetDescriptorPtr factory();
};

class AssetDescriptorTexture : public AssetDescriptorBase {
public:
	AssetDescriptorTexture();
	~AssetDescriptorTexture();
	QString toString() const override;
	void fromString(const QString& string) override;

	QString filename() const;
	void setFilename(const QString& filename);
	int textureGroupIndex() const;
	void setTextureGroupIndex(int textureGroupIndex);

	static AssetDescriptorPtr factory();
private:
	QString m_filename = "";
	int m_textureGroupIndex = 0;
};

class AssetDescriptorSound : public AssetDescriptorBase {
public:
	AssetDescriptorSound();
	~AssetDescriptorSound();
	QString toString() const override;
	void fromString(const QString& string) override;

	static AssetDescriptorPtr factory();
private:
	QString m_filename = "";
};

class AssetDescriptorMesh : public AssetDescriptorBase {
public:
	AssetDescriptorMesh();
	~AssetDescriptorMesh();
	QString toString() const override;
	void fromString(const QString& string) override;

	static AssetDescriptorPtr factory();
private:
	QString m_filename = "";
};

class AssetDescriptorText : public AssetDescriptorBase {
public:
	AssetDescriptorText();
	~AssetDescriptorText();
	QString toString() const override;
	void fromString(const QString& string) override;

	static AssetDescriptorPtr factory();
private:
	QString m_filename = "";
};

class AssetDescriptorBinary : public AssetDescriptorBase {
public:
	AssetDescriptorBinary();
	~AssetDescriptorBinary();
	QString toString() const override;
	void fromString(const QString& string) override;

	static AssetDescriptorPtr factory();
private:
	QString m_filename = "";
};

typedef AssetDescriptorPtr(*AssetDescriptorFactoryFptr)();

struct AssetInfo {
	AssetDescriptorFactoryFptr m_factoryFunction = nullptr;
	QString m_name;
	QIcon m_icon;
	QStringList m_extensions;
};

extern std::unordered_map<AssetType, AssetInfo> g_assetInfoMap;

void populateInfoMap();

AssetDescriptorPtr AssetDescriptorFactory(const QString& string);

AssetDescriptorPtr AssetDescriptorFactory(const AssetType& type);

class AssetTreeItem;

class AssetTreeModel : public QAbstractItemModel {
	Q_OBJECT
public:
	Q_DISABLE_COPY_MOVE(AssetTreeModel)

	explicit AssetTreeModel(QObject * parent = nullptr);
	~AssetTreeModel() override;

	QVariant data(const QModelIndex& index, int role) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	AssetType assetType(const QModelIndex& index) const;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	bool setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role = Qt::EditRole) override;
	QModelIndex index(int row, int column, const QModelIndex& parent = {}) const override;
	QModelIndex rootIndex() const;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent = {}) const override;
	int columnCount(const QModelIndex& parent = {}) const override;
	bool insertRows(int row, int count, const QModelIndex& parent = {}) override;
	bool removeRows(int row, int count, const QModelIndex& parent = {}) override;
	bool insertColumns(int column, int count, const QModelIndex& parent = {}) override;
	bool removeColumns(int column, int count, const QModelIndex& parent = {}) override;
	Qt::DropActions supportedDragActions() const override;
	Qt::DropActions supportedDropActions() const override;
	QStringList mimeTypes() const;
	QMimeData* mimeData(const QModelIndexList& indexes) const override;
	bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent);
	bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;

	QModelIndex getFile(const QString& name, AssetType type = AssetType::Unknown) const;
	QModelIndexList getFilesWithProperty(QList<AssetType> types, int propertyIndex, QStringList acceptableValues) const;

private:
	AssetTreeItem* itemFromIndex(const QModelIndex& index) const;

	std::unique_ptr<AssetTreeItem> m_rootItem;
	static const QString m_assetTreeItemMimeType;
};

class AssetTreeItem {
public:
	explicit AssetTreeItem(AssetTreeItem* parentItem = nullptr);
	~AssetTreeItem();

	void appendChild(std::shared_ptr<AssetTreeItem>&& child);
	bool insertChild(std::size_t index, std::shared_ptr<AssetTreeItem>&& child);
	bool removeChild(std::size_t index);
	std::shared_ptr<AssetTreeItem> child(int row) const;
	int childCount() const;

	int columnCount() const;
	QVariant data(int column) const;
	void setData(int column, const QVariant& data);
	void setData(AssetDescriptorPtr&& assetDescriptor);
	int row() const;
	AssetTreeItem* parentItem();
	void setParentItem(AssetTreeItem* parent);
	bool insertColumns(int column, int count);
	bool removeColumns(int column, int count);

	AssetDescriptorPtr assetDescriptor() const;
	AssetType assetType() const;

protected:
	friend class AssetTreeModel;
	std::vector<std::shared_ptr<AssetTreeItem>> m_childItems;
	AssetTreeItem* m_parentItem;
	AssetDescriptorPtr m_assetDescriptor;
};

class AssetTreeView : public QTreeView {
	Q_OBJECT
public:
	AssetTreeView(QWidget* parent = nullptr);
	~AssetTreeView();

	virtual void mousePressEvent(QMouseEvent* event);
	void setModel(QAbstractItemModel* model);

public slots:
	void onDataChanged_Model(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles);
	void onRowsInserted_Model(const QModelIndex& parent, int first, int last);
	void onRowsMoved_Model(const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationRow);
	void onRowsRemoved_Model(const QModelIndex& parent, int first, int last);

signals:
	void assetsChanged();
};

class AssetTreeItemDelegate: public QStyledItemDelegate {
	Q_OBJECT
public:
	AssetTreeItemDelegate();
	~AssetTreeItemDelegate();

	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
};

class AssetEditor : public QScrollArea {
	Q_OBJECT
public:
	AssetEditor(QWidget* parent = nullptr);
	~AssetEditor();

	virtual void showEvent(QShowEvent* ev);
	QModelIndex index() const;
	void setIndex(const QModelIndex& index);
	virtual QSize sizeHint() const;

public slots:
	void onChanged_Index(const QModelIndex& current, const QModelIndex& previous);
	void onChanged_ComboBox_TextureGroups(int index);

signals:
	void assetsChanged();

private:
	QGroupBox* m_groupBox_Contents;
	QGridLayout* m_layout_Contents;
	QModelIndex m_index;

	bool m_ignoreWidgetChanges;

	QComboBox* m_comboBox_textureGroups;
};
