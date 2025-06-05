#include "AssetInfo.hpp"
#include "ImageLabel.hpp"
#include "MainWindow.hpp"
#include "TextureGroupInfo.hpp"

// Asset descriptors ==========================================================

const QChar g_assetDelim = '#';
const QString g_rootAssetName = "(root)";

std::unordered_map<AssetType, AssetInfo> g_assetInfoMap;

void populateInfoMap() {
	g_assetInfoMap = {
	{ AssetType::Unknown, {&AssetDescriptorBase::factory,    "<ERROR>", "AERR", QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical), {}}},
	{ AssetType::Group,   {&AssetDescriptorGroup::factory,   "Group",   "AGRP", QApplication::style()->standardIcon(QStyle::SP_DirIcon),  {} }},
	{ AssetType::Texture, {&AssetDescriptorTexture::factory, "Texture", "AIMG", QApplication::style()->standardIcon(QStyle::SP_FileIcon), { "bmp", "png", "tiff", "svg" }}},
	{ AssetType::Sound,   {&AssetDescriptorSound::factory,   "Sound",   "AWAV", QApplication::style()->standardIcon(QStyle::SP_FileIcon), { "mp3", "mp4", "ogg", "wav" }}},
	{ AssetType::Mesh,    {&AssetDescriptorMesh::factory,    "Mesh",    "AMSH", QApplication::style()->standardIcon(QStyle::SP_FileIcon), { "obj", "stl" }}},
	{ AssetType::Text,    {&AssetDescriptorText::factory,    "Text",    "ATXT", QApplication::style()->standardIcon(QStyle::SP_FileIcon), { "txt", "ini", "xml", "json" }}},
	{ AssetType::Font,    {&AssetDescriptorFont::factory,    "Font",    "AFNT", QApplication::style()->standardIcon(QStyle::SP_FileIcon), { "ttf", "otf" }}},
	{ AssetType::Binary,  {&AssetDescriptorBinary::factory,  "Binary",  "ABIN", QApplication::style()->standardIcon(QStyle::SP_FileIcon), {} }},
	};
}

AssetDescriptorBase::AssetDescriptorBase() = default;

AssetDescriptorBase::~AssetDescriptorBase() = default;

AssetType AssetDescriptorBase::assetType() const {
	return m_assetType;
}

QString AssetDescriptorBase::toString() const {
	return g_assetInfoMap[m_assetType].m_name;
}

void AssetDescriptorBase::fromString(const QString& string) {
	Q_UNUSED(string);
}

QByteArray AssetDescriptorBase::toBytes() const {
	return QByteArray();
}

QString AssetDescriptorBase::name() {
	return m_name;
}

void AssetDescriptorBase::setName(const QString& name) {
	m_name = name;
}

AssetDescriptorPtr AssetDescriptorBase::factory() {
	return std::make_shared<AssetDescriptorBase>();
}

AssetDescriptorGroup::AssetDescriptorGroup() {
	m_assetType = AssetType::Group;
}

AssetDescriptorGroup::~AssetDescriptorGroup() = default;

QString AssetDescriptorGroup::toString() const {
	return buildAssetString({ "%1", "%2" }, 
		g_assetInfoMap[m_assetType].m_name,
		m_name
	);
}

void AssetDescriptorGroup::fromString(const QString& string) {
	QStringList list = string.split(g_assetDelim);
	if (list.isEmpty() || list[0] != g_assetInfoMap[m_assetType].m_name) {
		return;
	}
	m_name = (list.size() > 1) ? list[1] : "";
}

AssetDescriptorPtr AssetDescriptorGroup::factory() {
	return std::make_shared<AssetDescriptorGroup>();
}

AssetDescriptorTexture::AssetDescriptorTexture() {
	m_assetType = AssetType::Texture;
}

AssetDescriptorTexture::~AssetDescriptorTexture() = default;

QString AssetDescriptorTexture::toString() const {
	return buildAssetString({ "%0", "%1", "%2", "%3", "%4", "%5", "%6", "%7", "%8", "%9", "%10", "%11", "%12" },
		g_assetInfoMap[m_assetType].m_name,
		m_name,
		m_filename,
		QString::number(m_textureGroupIndex),
		m_animationData.m_frameCount,
		m_animationData.m_framesPerRow,
		m_animationData.m_offsetX,
		m_animationData.m_offsetY,
		m_animationData.m_rows,
		m_animationData.m_spacingX,
		m_animationData.m_spacingY,
		m_originX,
		m_originY
	);
}

void AssetDescriptorTexture::fromString(const QString& string) {
	QStringList list = string.split(g_assetDelim);
	if (list.isEmpty() || list[0] != g_assetInfoMap[m_assetType].m_name) {
		return;
	}
	m_name = (list.size() > 1) ? list[1] : "";
	m_filename = (list.size() > 2) ? list[2] : "";
	m_textureGroupIndex = (list.size() > 3) ? list[3].toInt() : 0;
	m_animationData.m_frameCount =   (list.size() > 4) ? list[4].toInt() : 1;
	m_animationData.m_framesPerRow = (list.size() > 5) ? list[5].toInt() : 1;
	m_animationData.m_offsetX =      (list.size() > 6) ? list[6].toInt() : 0;
	m_animationData.m_offsetY =      (list.size() > 7) ? list[7].toInt() : 0;
	m_animationData.m_rows =         (list.size() > 8) ? list[8].toInt() : 1;
	m_animationData.m_spacingX =     (list.size() > 9) ? list[9].toInt() : 0;
	m_animationData.m_spacingY =     (list.size() > 10) ? list[10].toInt() : 0;
	m_originX =                      (list.size() > 11) ? list[11].toInt() : 0;
	m_originY =                      (list.size() > 12) ? list[12].toInt() : 0;
	updateSize();
}

QByteArray AssetDescriptorTexture::toBytes() const {
	return QByteArray();
}

QString AssetDescriptorTexture::filename() const {
	return m_filename;
}

void AssetDescriptorTexture::setFilename(const QString& filename) {
	m_filename = filename;
}

int AssetDescriptorTexture::textureGroupIndex() const {
	return m_textureGroupIndex;
}

void AssetDescriptorTexture::setTextureGroupIndex(int textureGroupIndex) {
	m_textureGroupIndex = textureGroupIndex;
}

int AssetDescriptorTexture::getWidth() const {
	return m_width;
}

int AssetDescriptorTexture::getHeight() const {
	return m_height;
}

void AssetDescriptorTexture::updateSize() {
	QImageReader reader(m_filename);
	QImage image = reader.read();
	if (!image.isNull()) {
		m_width = image.width();
		m_height = image.height();
	}
	else {
		m_width = -1;
		m_height = -1;
	}
}

AssetDescriptorTexture::AnimationData AssetDescriptorTexture::getAnimationData() const {
	return m_animationData;
}

void AssetDescriptorTexture::setAnimationData(const AnimationData& data) {
	m_animationData = data;
}

int AssetDescriptorTexture::getOriginX() const {
	return m_originX;
}

void AssetDescriptorTexture::setOriginX(int originX) {
	m_originX = originX;
}

int AssetDescriptorTexture::getOriginY() const {
	return m_originY;
}

void AssetDescriptorTexture::setOriginY(int originY) {
	m_originY = originY;
}

AssetDescriptorPtr AssetDescriptorTexture::factory() {
	return std::make_shared<AssetDescriptorTexture>();
}

AssetDescriptorSound::AssetDescriptorSound() {
	m_assetType = AssetType::Sound;
}

AssetDescriptorSound::~AssetDescriptorSound() = default;

QString AssetDescriptorSound::toString() const {
	return buildAssetString({ "%1", "%2", "%3" },
		g_assetInfoMap[m_assetType].m_name,
		m_name,
		m_filename
	);
}

void AssetDescriptorSound::fromString(const QString& string) {
	QStringList list = string.split(g_assetDelim);
	if (list.isEmpty() || list[0] != g_assetInfoMap[m_assetType].m_name) {
		return;
	}
	m_name = (list.size() > 1) ? list[1] : "";
	m_filename = (list.size() > 2) ? list[2] : "";
}

QByteArray AssetDescriptorSound::toBytes() const {
	QFile file(m_filename);
	if (!file.open(QIODevice::ReadOnly)) {
		return QByteArray();
	}
	QByteArray bytes = file.readAll();
	file.close();
	return bytes;
}

AssetDescriptorPtr AssetDescriptorSound::factory() {
	return std::make_shared<AssetDescriptorSound>();
}

AssetDescriptorMesh::AssetDescriptorMesh() {
	m_assetType = AssetType::Mesh;
}

AssetDescriptorMesh::~AssetDescriptorMesh() = default;

QString AssetDescriptorMesh::toString() const {
	return buildAssetString({ "%1", "%2", "%3" },
		g_assetInfoMap[m_assetType].m_name,
		m_name,
		m_filename
	);
}

void AssetDescriptorMesh::fromString(const QString& string) {
	QStringList list = string.split(g_assetDelim);
	if (list.isEmpty() || list[0] != g_assetInfoMap[m_assetType].m_name) {
		return;
	}
	m_name = (list.size() > 1) ? list[1] : "";
	m_filename = (list.size() > 2) ? list[2] : "";
}

QByteArray AssetDescriptorMesh::toBytes() const {
	QFile file(m_filename);
	if (!file.open(QIODevice::ReadOnly)) {
		return QByteArray();
	}
	QByteArray bytes = file.readAll();
	file.close();
	return bytes;
}

AssetDescriptorPtr AssetDescriptorMesh::factory() {
	return std::make_shared<AssetDescriptorMesh>();
}

AssetDescriptorText::AssetDescriptorText() {
	m_assetType = AssetType::Text;
}

AssetDescriptorText::~AssetDescriptorText() = default;

QString AssetDescriptorText::toString() const {
	return buildAssetString({ "%1", "%2", "%3" },
		g_assetInfoMap[m_assetType].m_name,
		m_name,
		m_filename
	);
}

void AssetDescriptorText::fromString(const QString& string) {
	QStringList list = string.split(g_assetDelim);
	if (list.isEmpty() || list[0] != g_assetInfoMap[m_assetType].m_name) {
		return;
	}
	m_name = (list.size() > 1) ? list[1] : "";
	m_filename = (list.size() > 2) ? list[2] : "";
}

QByteArray AssetDescriptorText::toBytes() const {
	QFile file(m_filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return QByteArray();
	}
	QByteArray bytes = file.readAll();
	file.close();
	return bytes;
}

AssetDescriptorPtr AssetDescriptorText::factory() {
	return std::make_shared<AssetDescriptorText>();
}

AssetDescriptorFont::AssetDescriptorFont() {
	m_assetType = AssetType::Font;
}

AssetDescriptorFont::~AssetDescriptorFont() = default;

QString AssetDescriptorFont::toString() const {
	return buildAssetString({ "%1", "%2", "%3" },
		g_assetInfoMap[m_assetType].m_name,
		m_name,
		m_filename
	);
}

void AssetDescriptorFont::fromString(const QString& string) {
	QStringList list = string.split(g_assetDelim);
	if (list.isEmpty() || list[0] != g_assetInfoMap[m_assetType].m_name) {
		return;
	}
	m_name = (list.size() > 1) ? list[1] : "";
	m_filename = (list.size() > 2) ? list[2] : "";
}

QByteArray AssetDescriptorFont::toBytes() const {
	QFile file(m_filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return QByteArray();
	}
	QByteArray bytes = file.readAll();
	file.close();
	return bytes;
}

AssetDescriptorPtr AssetDescriptorFont::factory() {
	return std::make_shared<AssetDescriptorFont>();
}

AssetDescriptorBinary::AssetDescriptorBinary() {
	m_assetType = AssetType::Binary;
}

AssetDescriptorBinary::~AssetDescriptorBinary() = default;

QString AssetDescriptorBinary::toString() const {
	return buildAssetString({ "%1", "%2", "%3" },
		g_assetInfoMap[m_assetType].m_name,
		m_name,
		m_filename
	);
}

void AssetDescriptorBinary::fromString(const QString& string) {
	QStringList list = string.split(g_assetDelim);
	if (list.isEmpty() || list[0] != g_assetInfoMap[m_assetType].m_name) {
		return;
	}
	m_name = (list.size() > 1) ? list[1] : "";
	m_filename = (list.size() > 2) ? list[2] : "";
}

QByteArray AssetDescriptorBinary::toBytes() const {
	QFile file(m_filename);
	if (!file.open(QIODevice::ReadOnly)) {
		return QByteArray();
	}
	QByteArray bytes = file.readAll();
	file.close();
	return bytes;
}

AssetDescriptorPtr AssetDescriptorBinary::factory() {
	return std::make_shared<AssetDescriptorBinary>();
}

AssetDescriptorPtr AssetDescriptorFactory(const QString& string) {
	// Determine asset type
	QStringList list = string.split(g_assetDelim);
	if (list.empty()) {
		return nullptr;
	}
	QString typeName = list[0];
	AssetType type = AssetType::Unknown;
	for (auto& pair : g_assetInfoMap) {
		if (pair.second.m_name == typeName) {
			type = pair.first;
			break;
		}
	}
	if (type == AssetType::Unknown) {
		return nullptr;
	}

	// Create asset descriptor & populate data
	AssetDescriptorPtr ptr = g_assetInfoMap[type].m_factoryFunction();
	ptr->fromString(string);
	return ptr;
}

AssetDescriptorPtr AssetDescriptorFactory(const AssetType& type) {
	AssetDescriptorPtr ptr = g_assetInfoMap[type].m_factoryFunction();
	return ptr;
}


// Asset tree items ===========================================================

AssetTreeItem::AssetTreeItem(AssetTreeItem* parentItem) :
	m_parentItem(parentItem), m_assetDescriptor(nullptr) {}

AssetTreeItem::~AssetTreeItem() {}

void AssetTreeItem::appendChild(std::shared_ptr<AssetTreeItem>&& child) {
	m_childItems.push_back(std::move(child));
}

bool AssetTreeItem::insertChild(std::size_t index, std::shared_ptr<AssetTreeItem>&& child) {
	if (index > m_childItems.size()) {
		return false;
	}
	auto it = m_childItems.begin() + index;
	m_childItems.insert(it, std::move(child));
	return true;
}

bool AssetTreeItem::removeChild(std::size_t index) {
	if (index >= m_childItems.size() || m_childItems.empty()) {
		return false;
	}
	auto it = m_childItems.begin() + index;
	m_childItems.erase(it);
	return true;
}

std::shared_ptr<AssetTreeItem> AssetTreeItem::child(int row) const {
	return row >= 0 && row < childCount() ? m_childItems.at(row) : nullptr;
}

int AssetTreeItem::childCount() const {
	return int(m_childItems.size());
}

int AssetTreeItem::columnCount() const {
	return 1;
}

QVariant AssetTreeItem::data(int column) const {
	return column == 0
		? m_assetDescriptor->name()
		: m_assetDescriptor->toString();
}

void AssetTreeItem::setData(int column, const QVariant& data) {
	if (!m_assetDescriptor) {
		m_assetDescriptor = AssetDescriptorFactory(data.toString());
	}
	else {
		if (column == 0) {
			m_assetDescriptor->setName(data.toString());
		}
		else {
			m_assetDescriptor->fromString(data.toString());
		}
	}
}

void AssetTreeItem::setData(AssetDescriptorPtr&& assetDescriptor) {
	m_assetDescriptor = std::move(assetDescriptor);
}

int AssetTreeItem::row() const {
	if (m_parentItem == nullptr) {
		return 0;
	}
	const auto it = std::find_if(m_parentItem->m_childItems.cbegin(), m_parentItem->m_childItems.cend(),
		[this](const std::shared_ptr<AssetTreeItem>& treeItem) {
			return treeItem.get() == this;
		}
	);

	if (it != m_parentItem->m_childItems.cend()) {
		return std::distance(m_parentItem->m_childItems.cbegin(), it);
	}
	Q_ASSERT(false);
	return -1;
}

AssetTreeItem* AssetTreeItem::parentItem() {
	return m_parentItem;
}

void AssetTreeItem::setParentItem(AssetTreeItem* parent) {
	m_parentItem = parent;
}

bool AssetTreeItem::insertColumns(int column, int count) {
	Q_UNUSED(column);
	Q_UNUSED(count);
	return false;
}

bool AssetTreeItem::removeColumns(int column, int count) {
	Q_UNUSED(column);
	Q_UNUSED(count);
	return false;
}

AssetDescriptorPtr AssetTreeItem::assetDescriptor() const {
	return m_assetDescriptor;
}

AssetType AssetTreeItem::assetType() const {
	if (m_assetDescriptor) {
		return m_assetDescriptor->assetType();
	}
	return AssetType::Unknown;
}


// Asset tree model ===========================================================

const QString AssetTreeModel::m_assetTreeItemMimeType = "application/x-qabstractitemmodeldatalist";

AssetTreeModel::AssetTreeModel(QObject* parent) :
	QAbstractItemModel(parent) {
	AssetDescriptorPtr ptr = AssetDescriptorFactory(AssetType::Group);
	ptr->setName(g_rootAssetName);
	m_rootItem = std::make_unique<AssetTreeItem>();
	m_rootItem->setData(std::move(ptr));
}

AssetTreeModel::~AssetTreeModel() = default;

QVariant AssetTreeModel::data(const QModelIndex& index, int role) const {
	if (!index.isValid()) {
		return {};
	}

	const auto* item = itemFromIndex(index);
	if (role == Qt::DecorationRole) {
		return g_assetInfoMap[item->assetType()].m_icon;
	}
	else if (role == Qt::DisplayRole) {
		return item->data(0);
	}
	else if (role == Qt::UserRole) {
		return item->data(1);
	}
	else {
		return {};
	}
}

bool AssetTreeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	if (index.isValid()) {
		AssetTreeItem* item = itemFromIndex(index);
		if (role == Qt::EditRole) {
			item->setData(0, value);
			emit dataChanged(index, index);
			return true;
		}
		else if (role == Qt::UserRole) {
			item->setData(1, value);
			emit dataChanged(index, index);
			return true;
		}
	}
	return false;
}

AssetType AssetTreeModel::assetType(const QModelIndex& index) const {
	if (!index.isValid()) {
		return AssetType::Unknown;
	}
	AssetTreeItem* item = itemFromIndex(index);
	return item->assetType();
}

Qt::ItemFlags AssetTreeModel::flags(const QModelIndex& index) const {
	if (!index.isValid()) {
		return Qt::ItemFlags(Qt::ItemIsDropEnabled);
	}
	Qt::ItemFlags flags = Qt::ItemIsEditable | Qt::ItemIsDragEnabled | QAbstractItemModel::flags(index);
	if (assetType(index) == AssetType::Group) {
		flags |= Qt::ItemIsDropEnabled;
	}
	return flags;
}

QVariant AssetTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
	Q_UNUSED(section);
	Q_UNUSED(orientation);
	Q_UNUSED(role);
	return QVariant{};
}

bool AssetTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role) {
	Q_UNUSED(value);
	Q_UNUSED(role);
	emit headerDataChanged(orientation, section, section);
	return true;
}

QModelIndex AssetTreeModel::index(int row, int column, const QModelIndex& parent) const {
	if (!hasIndex(row, column, parent))
		return {};

	AssetTreeItem* parentItem = itemFromIndex(parent);
	if (auto childItem = parentItem->child(row)) {
		return createIndex(row, column, childItem.get());
	}
	return {};
}

QModelIndex AssetTreeModel::rootIndex() const {
	return createIndex(0, 0, m_rootItem.get());
}

QModelIndex AssetTreeModel::parent(const QModelIndex& index) const {
	if (!index.isValid()) {
		return {};
	}

	AssetTreeItem* childItem = itemFromIndex(index);
	AssetTreeItem* parentItem = childItem->parentItem();

	return parentItem != m_rootItem.get()
		? createIndex(parentItem->row(), 0, parentItem) 
		: QModelIndex{};
}

int AssetTreeModel::rowCount(const QModelIndex& parent) const {
	if (parent.column() > 0) {
		return 0;
	}
	const AssetTreeItem* parentItem = itemFromIndex(parent);
	return parentItem->childCount();
}

int AssetTreeModel::columnCount(const QModelIndex& parent) const {
	Q_UNUSED(parent);
	return m_rootItem->columnCount();
}

bool AssetTreeModel::insertRows(int row, int count, const QModelIndex& parent) {
	bool success = true;
	AssetTreeItem* parentItem = itemFromIndex(parent);
	beginInsertRows(parent, row, row + count - 1);
	if (parentItem->assetType() == AssetType::Group) {
		int rows = rowCount();
		if (row >= rows) {
			for (int i = 0; i < count; ++i) {
				parentItem->appendChild(std::make_shared<AssetTreeItem>(parentItem));
			}
		}
		else {
			for (int i = 0; i < count; ++i) {
				if (!parentItem->insertChild(std::size_t(row + i), std::make_shared<AssetTreeItem>(parentItem))) {
					success = false;
					break;
				}
			}
		}
	}
	else {
		success = false;
	}
	endInsertRows();
	return success;
}

bool AssetTreeModel::removeRows(int row, int count, const QModelIndex& parent) {
	bool success = true;
	AssetTreeItem* parentItem = itemFromIndex(parent);
	beginRemoveRows(parent, row, row + count - 1);
	for (int i = 0; i < count; ++i) {
		if (!parentItem->removeChild(row)) {
			success = false;
			break;
		}
	}
	endRemoveRows();
	return success;
}

bool AssetTreeModel::insertColumns(int column, int count, const QModelIndex& parent) {
	Q_UNUSED(column);
	Q_UNUSED(count);
	return false;
}

bool AssetTreeModel::removeColumns(int column, int count, const QModelIndex& parent) {
	Q_UNUSED(column);
	Q_UNUSED(count);
	Q_UNUSED(parent);
	return false;
}

Qt::DropActions AssetTreeModel::supportedDragActions() const {
	return Qt::MoveAction;
}

Qt::DropActions AssetTreeModel::supportedDropActions() const {
	return Qt::MoveAction;
}

QStringList AssetTreeModel::mimeTypes() const {
	QStringList types;
	types << m_assetTreeItemMimeType;
	return types;
}

QMimeData* AssetTreeModel::mimeData(const QModelIndexList& indexes) const {
	QMimeData* mimeData = new QMimeData();
	QList<AssetTreeItem*> items;
	QByteArray encodedData;
	QDataStream stream(&encodedData, QIODevice::WriteOnly);
	foreach(const QModelIndex& idx, indexes) {
		AssetTreeItem* item = itemFromIndex(idx);
		if (!items.contains(item)) {
			items << item;
		}
	}
	stream << QCoreApplication::applicationPid();
	qint64 size = items.size();
	stream << qint64(items.size());
	foreach(AssetTreeItem* item, items) {
		stream << reinterpret_cast<qint64>(item);
	}
	mimeData->setData(m_assetTreeItemMimeType, encodedData);
	return mimeData;
}

bool AssetTreeModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) {
	Q_UNUSED(action);
	Q_UNUSED(row);
	Q_UNUSED(parent);
	if (!data->hasFormat(m_assetTreeItemMimeType)) {
		return false;
	}
	if (column > 0) {
		return false;
	}
	return true;
}

bool AssetTreeModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) {
	if (!canDropMimeData(data, action, row, column, parent)) {
		return false;
	}
	if (action == Qt::IgnoreAction) {
		return true;
	}

	// Verify data stream
	QByteArray encodedData = data->data(m_assetTreeItemMimeType);
	QDataStream stream(&encodedData, QIODevice::ReadOnly);
	qint64 senderPid;
	stream >> senderPid;
	if (senderPid != QCoreApplication::applicationPid()) {
		return false;
	}
	AssetTreeItem* parentItem = itemFromIndex(parent);
	qint64 itemCount;
	stream >> itemCount;
	if (row == -1 || column == -1) {
		if (parent.isValid()) {
			row = 0;
		}
		else {
			row = rowCount(parent);
		}
	}

	// Extract data from stream
	bool success = true;
	for (qint64 i = 0; i < itemCount; ++i) {
		// Get source item
		qint64 data;
		stream >> data;
		AssetTreeItem* currItem = reinterpret_cast<AssetTreeItem*>(data);

		// Insert new row & move data over
		if (!insertRows(row, 1, parent)) {
			success = false;
			break;
		}
		QModelIndex inserted = index(row, 0, parent);
		AssetTreeItem* insertedItem = itemFromIndex(inserted);
		insertedItem->m_assetDescriptor.swap(currItem->m_assetDescriptor);
		insertedItem->m_childItems.swap(currItem->m_childItems);
		for (auto& child : insertedItem->m_childItems) {
			child->m_parentItem = insertedItem;
		}
	}
	return success;
}

QModelIndex AssetTreeModel::getFile(const QString& name, AssetType type) const {
	AssetTreeItem* rootItem = m_rootItem.get();
	std::stack<AssetTreeItem*> itemStack;
	itemStack.push(rootItem);
	while (!itemStack.empty()) {
		AssetTreeItem* currentItem = itemStack.top();
		itemStack.pop();
		if (currentItem->assetType() == AssetType::Group) {
			for (auto& child : currentItem->m_childItems) {
				itemStack.push(child.get());
			}
		}
		else {
			if (currentItem->data(0) == name && (type == AssetType::Unknown || currentItem->assetType() == type)) {
				return createIndex(currentItem->row(), 0, currentItem);
			}
		}
	}
	return {};
}

QModelIndexList AssetTreeModel::getFilesWithProperty(QList<AssetType> types, int propertyIndex, QStringList acceptableValues) const {
	QModelIndexList results = {};
	AssetTreeItem* rootItem = m_rootItem.get();
	std::stack<AssetTreeItem*> itemStack;
	itemStack.push(rootItem);
	while (!itemStack.empty()) {
		AssetTreeItem* currentItem = itemStack.top();
		itemStack.pop();
		if (currentItem->assetType() == AssetType::Group) {
			for (auto& child : currentItem->m_childItems) {
				itemStack.push(child.get());
			}
		}
		else if (types.contains(currentItem->assetType())) {
			QStringList properties = currentItem->data(1).toString().split(g_assetDelim);
			if (propertyIndex >= 0 && propertyIndex < properties.size()) {
				if (acceptableValues.contains(properties[propertyIndex])) {
					results << createIndex(currentItem->row(), 0, currentItem);
				}
			}
		}
	}
	return results;
}

AssetTreeItem* AssetTreeModel::itemFromIndex(const QModelIndex& index) const {
	return index.isValid()
		? static_cast<AssetTreeItem*>(index.internalPointer())
		: m_rootItem.get();
}

// Asset item delegate ========================================================

AssetTreeItemDelegate::AssetTreeItemDelegate() = default;

AssetTreeItemDelegate::~AssetTreeItemDelegate() = default;

void AssetTreeItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
	if (!lineEdit->isModified()) {
		return;
	}
	QString text = lineEdit->text();
	if (text.isEmpty()) {
		return;
	}
	QStyledItemDelegate::setModelData(editor, model, index);
	
}


// Asset tree view ============================================================

AssetTreeView::AssetTreeView(QWidget* parent) :
	QTreeView(parent) {
	setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
	setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	setDragEnabled(true);
	setAcceptDrops(true);
	setDragDropMode(QAbstractItemView::DragDropMode::InternalMove);
	setDropIndicatorShown(true);
	setSortingEnabled(false);
	setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
	setStyleSheet("AssetTreeView::item { padding: 3px; }");
}

AssetTreeView::~AssetTreeView() = default;

void AssetTreeView::mousePressEvent(QMouseEvent* event) {
	QModelIndex item = indexAt(event->pos());
	bool selected = selectionModel()->isSelected(item);
	QTreeView::mousePressEvent(event);
	if ((item.row() == -1 && item.column() == -1)) {
		setCurrentIndex({});
	}
}

void AssetTreeView::setModel(QAbstractItemModel* model) {
	QTreeView::setModel(model);
	connect(model, &QAbstractItemModel::dataChanged, this, &AssetTreeView::onDataChanged_Model);
	connect(model, &QAbstractItemModel::rowsInserted, this, &AssetTreeView::onRowsInserted_Model);
	connect(model, &QAbstractItemModel::rowsMoved, this, &AssetTreeView::onRowsMoved_Model);
	connect(model, &QAbstractItemModel::rowsRemoved, this, &AssetTreeView::onRowsRemoved_Model);
}

void AssetTreeView::onDataChanged_Model(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles) {
	Q_UNUSED(topLeft);
	Q_UNUSED(bottomRight);
	Q_UNUSED(roles);
	emit assetsChanged();
}

void AssetTreeView::onRowsInserted_Model(const QModelIndex& parent, int first, int last) {
	Q_UNUSED(parent);
	Q_UNUSED(first);
	Q_UNUSED(last);
	emit assetsChanged();
}

void AssetTreeView::onRowsMoved_Model(const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationRow) {
	Q_UNUSED(sourceParent);
	Q_UNUSED(sourceStart);
	Q_UNUSED(sourceEnd);
	Q_UNUSED(destinationParent);
	Q_UNUSED(destinationRow);
	emit assetsChanged();
}

void AssetTreeView::onRowsRemoved_Model(const QModelIndex& parent, int first, int last) {
	Q_UNUSED(parent);
	Q_UNUSED(first);
	Q_UNUSED(last);
	emit assetsChanged();
}


// Asset editor ===============================================================

AssetEditor::AssetEditor(QWidget* parent) :
	QScrollArea(parent) {
	setStyleSheet("AssetEditor { border: 0px; }");
	setWidgetResizable(true);
	setMinimumSize(QSize(16, 16));

	m_groupBox_Editor = nullptr;
	m_layout_Contents = new QVBoxLayout();
	m_groupBox_Contents = new QGroupBox();
	setWidget(m_groupBox_Contents);
	m_groupBox_Contents->setLayout(m_layout_Contents);
	setIndex({});
}

AssetEditor::~AssetEditor() {
	if (m_layout_Contents) {
		delete m_layout_Contents;
	}
}

QModelIndex AssetEditor::index() const {
	return m_index;
}

void AssetEditor::setIndex(const QModelIndex& index) {
	// Construct editor from index
	QGroupBox* editorNew = nullptr;
	m_index = index;
	if (index.isValid()) {
		// Set widgets based on asset type
		AssetTreeItem* item = static_cast<AssetTreeItem*>(index.internalPointer());
		AssetDescriptorPtr descriptor = item->assetDescriptor();
		if (descriptor->assetType() == AssetType::Texture) {
			AssetDescriptorTexture* texture = static_cast<AssetDescriptorTexture*>(descriptor.get());
			AssetEditorTexture* textureEditorNew = new AssetEditorTexture(texture);
			connect(textureEditorNew, &AssetEditorTexture::assetsChanged, this, &AssetEditor::onChanged_Asset);
			editorNew = qobject_cast<QGroupBox*>(textureEditorNew);
		}
		else if (descriptor->assetType() == AssetType::Sound) {

		}
		else if (descriptor->assetType() == AssetType::Mesh) {

		}
		else if (descriptor->assetType() == AssetType::Text) {

		}
		else if (descriptor->assetType() == AssetType::Font) {

		}
		else if (descriptor->assetType() == AssetType::Binary) {

		}
	}

	// Replace existing editor
	if (m_groupBox_Editor) {
		int index = m_layout_Contents->indexOf(m_groupBox_Editor);
		QLayoutItem* item = m_layout_Contents->takeAt(index);
		delete item->widget();
		delete item;
		m_groupBox_Editor = nullptr;
	}
	if (editorNew) {
		m_layout_Contents->addWidget(editorNew);
		m_groupBox_Editor = editorNew;
	}
}

QSize AssetEditor::sizeHint() const {
	return QSize(512, 512);
}

void AssetEditor::onChanged_Index(const QModelIndex& current, const QModelIndex& previous) {
	Q_UNUSED(previous);
	if (!m_ignoreWidgetChanges) {
		setIndex(current);
	}
}

void AssetEditor::onChanged_Asset() {
	emit assetsChanged();
}

AssetEditorTexture::AssetEditorTexture(AssetDescriptorTexture* texture, QWidget* parent) :
	QGroupBox(parent) {
	setStyleSheet("AssetEditorTexture { border:0; }");
	setFlat(true);

	m_texture = texture;
	m_layout = new QGridLayout();
	setLayout(m_layout);

	QFileInfo textureFile(m_texture->filename());
	if (!textureFile.exists()) {
		QIcon errorIcon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical);
		QPixmap errorPixmap = errorIcon.pixmap(QSize(16, 16));
		m_layout->addWidget(new ImageLabel(errorPixmap), 0, 0, 1, 1, Qt::AlignCenter);
		m_layout->addWidget(new QLabel(QString("Failed to find file (%1)!").arg(textureFile.fileName())), 2, 0, Qt::AlignCenter);
		m_comboBox_TextureGroups = nullptr;
		m_comboBox_TextureGroups = nullptr;
		m_spinBox_Ani_FrameCount = nullptr;
		m_spinBox_Ani_FramesPerRow = nullptr;
		m_spinBox_Ani_OffsetX = nullptr;
		m_spinBox_Ani_OffsetY = nullptr;
		m_spinBox_Ani_Rows = nullptr;
		m_spinBox_Ani_SpacingX = nullptr;
		m_spinBox_Ani_SpacingY = nullptr;
		m_spinBox_OriginX = nullptr;
		m_spinBox_OriginY = nullptr;
	}
	else {
		QImage textureImage(m_texture->filename());
		m_layout->addWidget(new ImageLabel(QPixmap(m_texture->filename())), 0, 0, 2, 2, Qt::AlignCenter);
		m_layout->addWidget(new QLabel("File: "), 2, 0, Qt::AlignRight);
		m_layout->addWidget(new QLabel(textureFile.fileName()), 2, 1, Qt::AlignLeft);
		m_layout->addWidget(new QLabel("Properties: "), 3, 0, Qt::AlignRight);
		m_layout->addWidget(new QLabel("Size: "), 4, 0, Qt::AlignRight);
		m_layout->addWidget(new QLabel(QString("%1 x %2").arg(textureImage.width()).arg(textureImage.height())), 4, 1, Qt::AlignLeft);
		m_layout->addWidget(new QLabel("Format: "), 5, 0, Qt::AlignRight);
		m_layout->addWidget(new QLabel(QString("%1").arg(imageFormatString(textureImage.format()))), 5, 1, Qt::AlignLeft);
		
		m_layout->addWidget(new QLabel("Texture Group: "), 6, 0, Qt::AlignRight);
		m_comboBox_TextureGroups = new QComboBox();
		m_comboBox_TextureGroups->addItems(g_textureGroups);
		m_comboBox_TextureGroups->setCurrentIndex(m_texture->textureGroupIndex());
		connect(m_comboBox_TextureGroups, &QComboBox::currentIndexChanged, this, &AssetEditorTexture::onChanged_ComboBox_TextureGroups);
		m_layout->addWidget(m_comboBox_TextureGroups, 6, 1, Qt::AlignLeft);

		m_layout->addWidget(new QLabel("Origin X:"), 7, 0, Qt::AlignRight);
		m_spinBox_OriginX = new QSpinBox();
		m_spinBox_OriginX->setValue(m_texture->getOriginX());
		connect(m_spinBox_OriginX, &QSpinBox::valueChanged, this, &AssetEditorTexture::onChanged_Data);
		m_layout->addWidget(m_spinBox_OriginX, 7, 1);

		m_layout->addWidget(new QLabel("Origin Y:"), 8, 0, Qt::AlignRight);
		m_spinBox_OriginY = new QSpinBox();
		m_spinBox_OriginY->setValue(m_texture->getOriginY());
		connect(m_spinBox_OriginY, &QSpinBox::valueChanged, this, &AssetEditorTexture::onChanged_Data);
		m_layout->addWidget(m_spinBox_OriginY, 8, 1);

		auto animationData = texture->getAnimationData();
		m_layout->addWidget(new QLabel("Animation: "), 9, 0, Qt::AlignRight);

		m_layout->addWidget(new QLabel("Number of Frames: "), 10, 0, Qt::AlignRight);
		m_spinBox_Ani_FrameCount = new QSpinBox();
		m_spinBox_Ani_FrameCount->setMinimum(1);
		m_spinBox_Ani_FrameCount->setValue(animationData.m_frameCount);
		connect(m_spinBox_Ani_FrameCount, &QSpinBox::valueChanged, this, &AssetEditorTexture::onChanged_Data);
		m_layout->addWidget(m_spinBox_Ani_FrameCount, 10, 1);

		m_layout->addWidget(new QLabel("Frames Per Row: "), 11, 0, Qt::AlignRight);
		m_spinBox_Ani_FramesPerRow = new QSpinBox();
		m_spinBox_Ani_FramesPerRow->setMinimum(1);
		m_spinBox_Ani_FramesPerRow->setValue(animationData.m_framesPerRow);
		connect(m_spinBox_Ani_FramesPerRow, &QSpinBox::valueChanged, this, &AssetEditorTexture::onChanged_Data);
		m_layout->addWidget(m_spinBox_Ani_FramesPerRow, 11, 1);

		m_layout->addWidget(new QLabel("Number of Rows:"), 12, 0, Qt::AlignRight);
		m_spinBox_Ani_Rows = new QSpinBox();
		m_spinBox_Ani_Rows->setMinimum(1);
		m_spinBox_Ani_Rows->setValue(animationData.m_rows);
		connect(m_spinBox_Ani_Rows, &QSpinBox::valueChanged, this, &AssetEditorTexture::onChanged_Data);
		m_layout->addWidget(m_spinBox_Ani_Rows, 12, 1);

		m_layout->addWidget(new QLabel("Offset X:"), 13, 0, Qt::AlignRight);
		m_spinBox_Ani_OffsetX = new QSpinBox();
		m_spinBox_Ani_OffsetX->setMinimum(0);
		m_spinBox_Ani_OffsetX->setValue(animationData.m_offsetX);
		connect(m_spinBox_Ani_OffsetX, &QSpinBox::valueChanged, this, &AssetEditorTexture::onChanged_Data);
		m_layout->addWidget(m_spinBox_Ani_OffsetX, 13, 1);

		m_layout->addWidget(new QLabel("Offset Y:"), 14, 0, Qt::AlignRight);
		m_spinBox_Ani_OffsetY = new QSpinBox();
		m_spinBox_Ani_OffsetY->setMinimum(0);
		m_spinBox_Ani_OffsetY->setValue(animationData.m_offsetY);
		connect(m_spinBox_Ani_OffsetY, &QSpinBox::valueChanged, this, &AssetEditorTexture::onChanged_Data);
		m_layout->addWidget(m_spinBox_Ani_OffsetY, 14, 1);

		m_layout->addWidget(new QLabel("Spacing X:"), 15, 0, Qt::AlignRight);
		m_spinBox_Ani_SpacingX = new QSpinBox();
		m_spinBox_Ani_SpacingX->setMinimum(0);
		m_spinBox_Ani_SpacingX->setValue(animationData.m_spacingX);
		connect(m_spinBox_Ani_SpacingX, &QSpinBox::valueChanged, this, &AssetEditorTexture::onChanged_Data);
		m_layout->addWidget(m_spinBox_Ani_SpacingX, 15, 1);

		m_layout->addWidget(new QLabel("Spacing Y:"), 16, 0, Qt::AlignRight);
		m_spinBox_Ani_SpacingY = new QSpinBox();
		m_spinBox_Ani_SpacingY->setMinimum(0);
		m_spinBox_Ani_SpacingY->setValue(animationData.m_spacingY);
		connect(m_spinBox_Ani_SpacingY, &QSpinBox::valueChanged, this, &AssetEditorTexture::onChanged_Data);
		m_layout->addWidget(m_spinBox_Ani_SpacingY, 16, 1);
	}
	m_layout->setRowStretch(m_layout->rowCount(), 1);
}

AssetEditorTexture::~AssetEditorTexture() {
	delete m_layout;
}

void AssetEditorTexture::showEvent(QShowEvent* ev) {
	// Update item previews
	m_ignoreWidgetChanges = true;
	if (m_comboBox_TextureGroups) {
		m_comboBox_TextureGroups->clear();
		m_comboBox_TextureGroups->addItems(g_textureGroups);
		m_comboBox_TextureGroups->setCurrentIndex(m_texture->textureGroupIndex());
	}
	m_ignoreWidgetChanges = false;
}

void AssetEditorTexture::onChanged_ComboBox_TextureGroups(int index) {
	if (!m_ignoreWidgetChanges) {
		if (index != m_texture->textureGroupIndex()) {
			m_texture->setTextureGroupIndex(index);
			emit assetsChanged();
		}
	}
}

void AssetEditorTexture::onChanged_Data(int value) {
	AssetDescriptorTexture::AnimationData animationData;
	if (m_spinBox_Ani_FrameCount) animationData.m_frameCount = m_spinBox_Ani_FrameCount->value();
	if (m_spinBox_Ani_FramesPerRow) animationData.m_framesPerRow = m_spinBox_Ani_FramesPerRow->value();
	if (m_spinBox_Ani_OffsetX) animationData.m_offsetX = m_spinBox_Ani_OffsetX->value();
	if (m_spinBox_Ani_OffsetY) animationData.m_offsetY = m_spinBox_Ani_OffsetY->value();
	if (m_spinBox_Ani_Rows) animationData.m_rows = m_spinBox_Ani_Rows->value();
	if (m_spinBox_Ani_SpacingX) animationData.m_spacingX = m_spinBox_Ani_SpacingX->value();
	if (m_spinBox_Ani_SpacingY) animationData.m_spacingY = m_spinBox_Ani_SpacingY->value();
	m_texture->setAnimationData(animationData);
	if (m_spinBox_OriginX) m_texture->setOriginX(m_spinBox_OriginX->value());
	if (m_spinBox_OriginY) m_texture->setOriginY(m_spinBox_OriginY->value());
	emit assetsChanged();
}
