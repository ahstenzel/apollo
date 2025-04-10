#pragma once
/**
 * AssetInfo.hpp
 *
 * Collection of classes and functions for viewing & storing assets.
 */
#include "Common.hpp"

/// <summary>
/// Types of assets recognized by Apollo.
/// </summary>
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

/// <summary>
/// Generate the global asset info map.
/// </summary>
void populateInfoMap();

/// <summary>
/// Join the tokens together to form a valid asset string, filling in the given arguments.
/// </summary>
/// <param name="tokens">List of tokens to join</param>
/// <param name="...args">Arguments to fill in string with</param>
/// <returns>Asset string</returns>
template<typename... Args>
inline QString buildAssetString(QStringList tokens, Args&&... args) {
	return tokens.join(g_assetDelim).arg(std::forward<Args>(args)...);
}

/// <summary>
/// Join the tokens together to form a valid asset string, filling in the given arguments.
/// </summary>
/// <param name="tokens">List of tokens to join</param>
/// <returns>Asset string</returns>
inline QString buildAssetString(QStringList tokens) {
	return tokens.join(g_assetDelim);
}

class AssetDescriptorBase;
typedef std::shared_ptr<AssetDescriptorBase> AssetDescriptorPtr;

/// <summary>
/// Base class for all asset descriptor types.
/// </summary>
class AssetDescriptorBase {
public:
	AssetDescriptorBase();
	~AssetDescriptorBase();

	/// <summary>
	/// Get the type of this asset.
	/// </summary>
	/// <returns>Asset type enum</returns>
	AssetType assetType() const;

	/// <summary>
	/// Serialize the asset to a string.
	/// </summary>
	/// <returns>Asset string</returns>
	virtual QString toString() const;

	/// <summary>
	/// Deserialzie the asset from a string.
	/// </summary>
	/// <param name="string">Asset string</param>
	virtual void fromString(const QString& string);

	/// <summary>
	/// Serialize the asset to binary form.
	/// </summary>
	/// <returns>Byte array</returns>
	virtual QByteArray toBytes() const;

	/// <summary>
	/// Get the name of the asset.
	/// </summary>
	/// <returns>Name string</returns>
	virtual QString name();

	/// <summary>
	/// Set the name of the asset.
	/// </summary>
	/// <param name="name">Name string</param>
	virtual void setName(const QString& name);

	/// <summary>
	/// Construct a new asset descriptor.
	/// </summary>
	/// <returns>Asset descriptor pointer</returns>
	static AssetDescriptorPtr factory();
protected:
	AssetType m_assetType = AssetType::Unknown;
	QString m_name = "";
};

/// <summary>
/// Asset descriptor for groups of other assets.
/// </summary>
class AssetDescriptorGroup : public AssetDescriptorBase {
public:
	AssetDescriptorGroup();
	~AssetDescriptorGroup();

	/// <summary>
	/// Serialize the group asset to a string.
	/// </summary>
	/// <returns>Asset string</returns>
	QString toString() const override;

	/// <summary>
	/// Deserialzie the group asset from a string.
	/// </summary>
	/// <param name="string"></param>
	void fromString(const QString& string) override;

	/// <summary>
	/// Construct a new group asset descriptor.
	/// </summary>
	/// <returns>Asset descriptor pointer</returns>
	static AssetDescriptorPtr factory();
};

/// <summary>
/// Asset descriptor for image files.
/// </summary>
class AssetDescriptorTexture : public AssetDescriptorBase {
public:
	AssetDescriptorTexture();
	~AssetDescriptorTexture();

	/// <summary>
	/// Serialize the texture asset to a string.
	/// </summary>
	/// <returns>Asset string</returns>
	QString toString() const override;

	/// <summary>
	/// Deserialzie the texture asset from a string.
	/// </summary>
	/// <param name="string"></param>
	void fromString(const QString& string) override;

	/// <summary>
	/// Serialize the texture asset to binary form.
	/// </summary>
	/// <returns>Byte array</returns>
	QByteArray toBytes() const override;

	/// <summary>
	/// Get the texture file for this asset.
	/// </summary>
	/// <returns>File string</returns>
	QString filename() const;

	/// <summary>
	/// Set the texture file for this asset.
	/// </summary>
	/// <param name="filename">File string</param>
	void setFilename(const QString& filename);

	/// <summary>
	/// Get the texture group this asset is assigned to.
	/// </summary>
	/// <returns>Index</returns>
	int textureGroupIndex() const;

	/// <summary>
	/// Set the texture group this asset is assigned to.
	/// </summary>
	/// <param name="textureGroupIndex">Index</param>
	void setTextureGroupIndex(int textureGroupIndex);

	/// <summary>
	/// Get the width of the texture.
	/// </summary>
	/// <returns>Width</returns>
	int getWidth() const;

	/// <summary>
	/// Get the height of the texture.
	/// </summary>
	/// <returns>Height</returns>
	int getHeight() const;

	/// <summary>
	/// Construct a new texture asset descriptor.
	/// </summary>
	/// <returns>Asset descriptor pointer</returns>
	static AssetDescriptorPtr factory();
private:
	QString m_filename = "";
	int m_textureGroupIndex = 0;
	int m_width = -1;
	int m_height = -1;

	void updateSize();
};

/// <summary>
/// Asset descriptor for sound files.
/// </summary>
class AssetDescriptorSound : public AssetDescriptorBase {
public:
	AssetDescriptorSound();
	~AssetDescriptorSound();

	/// <summary>
	/// Serialize the sound asset to a string.
	/// </summary>
	/// <returns>Asset string</returns>
	QString toString() const override;

	/// <summary>
	/// Deserialzie the sound asset from a string.
	/// </summary>
	/// <param name="string"></param>
	void fromString(const QString& string) override;

	/// <summary>
	/// Serialize the sound asset to binary form.
	/// </summary>
	/// <returns>Byte array</returns>
	QByteArray toBytes() const override;

	/// <summary>
	/// Construct a new sound asset descriptor.
	/// </summary>
	/// <returns>Asset descriptor pointer</returns>
	static AssetDescriptorPtr factory();
private:
	QString m_filename = "";
};

/// <summary>
/// Asset descriptor for mesh files.
/// </summary>
class AssetDescriptorMesh : public AssetDescriptorBase {
public:
	AssetDescriptorMesh();
	~AssetDescriptorMesh();

	/// <summary>
	/// Serialize the mesh asset to a string.
	/// </summary>
	/// <returns>Asset string</returns>
	QString toString() const override;

	/// <summary>
	/// Deserialzie the mesh asset from a string.
	/// </summary>
	/// <param name="string"></param>
	void fromString(const QString& string) override;

	/// <summary>
	/// Serialize the mesh asset to binary form.
	/// </summary>
	/// <returns>Byte array</returns>
	QByteArray toBytes() const override;

	/// <summary>
	/// Construct a new mesh asset descriptor.
	/// </summary>
	/// <returns>Asset descriptor pointer</returns>
	static AssetDescriptorPtr factory();
private:
	QString m_filename = "";
};

/// <summary>
/// Asset descriptor for text files.
/// </summary>
class AssetDescriptorText : public AssetDescriptorBase {
public:
	AssetDescriptorText();
	~AssetDescriptorText();

	/// <summary>
	/// Serialize the text asset to a string.
	/// </summary>
	/// <returns>Asset string</returns>
	QString toString() const override;

	/// <summary>
	/// Deserialzie the text asset from a string.
	/// </summary>
	/// <param name="string"></param>
	void fromString(const QString& string) override;

	/// <summary>
	/// Serialize the text asset to binary form.
	/// </summary>
	/// <returns>Byte array</returns>
	QByteArray toBytes() const override;

	/// <summary>
	/// Construct a new text asset descriptor.
	/// </summary>
	/// <returns>Asset descriptor pointer</returns>
	static AssetDescriptorPtr factory();
private:
	QString m_filename = "";
};

/// <summary>
/// Asset descriptor for binary files.
/// </summary>
class AssetDescriptorBinary : public AssetDescriptorBase {
public:
	AssetDescriptorBinary();
	~AssetDescriptorBinary();

	/// <summary>
	/// Serialize the text asset to a string.
	/// </summary>
	/// <returns>Asset string</returns>
	QString toString() const override;

	/// <summary>
	/// Deserialzie the text asset from a string.
	/// </summary>
	/// <param name="string"></param>
	void fromString(const QString& string) override;

	/// <summary>
	/// Get the contents of the binary asset.
	/// </summary>
	/// <returns>Byte array</returns>
	QByteArray toBytes() const override;

	/// <summary>
	/// Construct a new binary asset descriptor.
	/// </summary>
	/// <returns>Asset descriptor pointer</returns>
	static AssetDescriptorPtr factory();
private:
	QString m_filename = "";
};

typedef AssetDescriptorPtr(*AssetDescriptorFactoryFptr)();

/// <summary>
/// Registry info for an asset type.
/// </summary>
struct AssetInfo {
	AssetDescriptorFactoryFptr m_factoryFunction = nullptr;
	QString m_name;
	QString m_signature;
	QIcon m_icon;
	QStringList m_extensions;
};

extern std::unordered_map<AssetType, AssetInfo> g_assetInfoMap;

/// <summary>
/// Construct a new blank asset. Type is either supplied directly or inferred from an asset descriptor string.
/// </summary>
/// <param name="string">Asset descriptor string</param>
/// <returns></returns>
AssetDescriptorPtr AssetDescriptorFactory(const QString& string);

/// <summary>
/// Construct a new blank asset. Type is either supplied directly or inferred from an asset descriptor string.
/// </summary>
/// <param name="type">Asset type</param>
/// <returns></returns>
AssetDescriptorPtr AssetDescriptorFactory(const AssetType& type);

class AssetTreeItem;

/// <summary>
/// Qt item model for organizing assets.
/// </summary>
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

/// <summary>
/// Single element in an asset tree.
/// </summary>
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

/// <summary>
/// Qt movel view for displaying assets.
/// </summary>
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

/// <summary>
/// Qt item delegate for validating edited item data.
/// </summary>
class AssetTreeItemDelegate: public QStyledItemDelegate {
	Q_OBJECT
public:
	AssetTreeItemDelegate();
	~AssetTreeItemDelegate();

	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
};

/// <summary>
/// Widget for previewing an assets & its associated properties.
/// </summary>
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
