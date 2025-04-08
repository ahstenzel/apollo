#include "ResourceGenerator.hpp"

ResourceSectionTextureGroup::ResourceSectionTextureGroup() {}

ResourceSectionTextureGroup::ResourceSectionTextureGroup(std::size_t capacity) {
	m_imageData.reserve(capacity);
}

ResourceSectionTextureGroup::ResourceSectionTextureGroup(ResourceSectionTextureGroup&& other) noexcept {
	m_stride = std::move(other.m_stride);
	m_dirty = std::move(other.m_dirty);
	m_byteArray = std::move(m_byteArray);
	m_imageData = std::move(other.m_imageData);
}

ResourceSectionTextureGroup::ResourceSectionTextureGroup(const ResourceSectionTextureGroup& other) {
	m_stride = other.m_stride;
	m_dirty = other.m_dirty;
	m_byteArray = m_byteArray;
	m_imageData = other.m_imageData;
}

ResourceSectionTextureGroup::~ResourceSectionTextureGroup() = default;

void swap(ResourceSectionTextureGroup& lhs, ResourceSectionTextureGroup& rhs) {
	std::swap(lhs.m_stride, rhs.m_stride);
	std::swap(lhs.m_dirty, rhs.m_dirty);
	std::swap(lhs.m_byteArray, rhs.m_byteArray);
	std::swap(lhs.m_imageData, rhs.m_imageData);
}

ResourceSectionTextureGroup& ResourceSectionTextureGroup::operator=(ResourceSectionTextureGroup& other) {
	swap(*this, other);
	return *this;
}

ResourceSectionTextureGroup& ResourceSectionTextureGroup::operator=(ResourceSectionTextureGroup&& other) noexcept {
	swap(*this, other);
	return *this;
}

void ResourceSectionTextureGroup::insert(const QImage& image) {
	QByteArray imageData((char*)(image.bits(), image.sizeInBytes()));
	m_imageData.push_back(imageData);
	m_stride = std::max(m_stride, roundUp(imageData.size(), APOLLO_ARC_ALIGN));
	m_dirty = true;
}

std::size_t ResourceSectionTextureGroup::size() const {
	return m_imageData.size();
}

std::size_t ResourceSectionTextureGroup::sizeInBytes() const {
	return roundUp(16 + (m_stride * m_imageData.size()), APOLLO_ARC_ALIGN);
}

bool ResourceSectionTextureGroup::isEmpty() const {
	return m_imageData.empty();
}

QByteArray ResourceSectionTextureGroup::toBytes() {
	if (m_dirty) {
		m_dirty = false;
		m_byteArray.clear();

		// Build header
		byteArrayPushStr(&m_byteArray, "ATXG", 4);
		byteArrayPushInt32(&m_byteArray, std::uint32_t(m_imageData.size()));
		byteArrayPushInt64(&m_byteArray, m_stride);

		// Build images
		for (auto& image : m_imageData) {
			m_byteArray.append(image);
			byteArrayPad(&m_byteArray, APOLLO_ARC_ALIGN);
		}
	}
	return m_byteArray;
}

ResourceSectionAssetTable::ResourceSectionAssetTable() {}

ResourceSectionAssetTable::ResourceSectionAssetTable(std::size_t capacity) {
	resize(capacity);
}

ResourceSectionAssetTable::ResourceSectionAssetTable(ResourceSectionAssetTable&& other) noexcept {
	m_capacity = std::move(other.m_capacity);
	m_length = std::move(other.m_length);
	m_dirty = std::move(other.m_dirty);
	m_byteArray = std::move(other.m_byteArray);
	m_ctrl = new std::uint8_t[m_capacity];
	m_nodes = new AssetNode[m_capacity];
	for (std::size_t i = 0; i < m_capacity; ++i) {
		m_ctrl[i] = std::move(other.m_ctrl[i]);
		m_nodes[i] = std::move(other.m_nodes[i]);
	}
}

ResourceSectionAssetTable::ResourceSectionAssetTable(const ResourceSectionAssetTable& other) {
	m_capacity = other.m_capacity;
	m_length = other.m_length;
	m_dirty = other.m_dirty;
	m_byteArray = other.m_byteArray;
	m_ctrl = new std::uint8_t[m_capacity];
	m_nodes = new AssetNode[m_capacity];
	for (std::size_t i = 0; i < m_capacity; ++i) {
		m_ctrl[i] = other.m_ctrl[i];
		m_nodes[i] = other.m_nodes[i];
	}
}

ResourceSectionAssetTable::~ResourceSectionAssetTable() {
	delete[] m_ctrl;
	delete[] m_nodes;
}

void swap(ResourceSectionAssetTable& lhs, ResourceSectionAssetTable& rhs) {
	std::swap(lhs.m_capacity, rhs.m_capacity);
	std::swap(lhs.m_length, rhs.m_length);
	std::swap(lhs.m_dirty, rhs.m_dirty);
	std::swap(lhs.m_byteArray, rhs.m_byteArray);
	std::swap(lhs.m_ctrl, rhs.m_ctrl);
	std::swap(lhs.m_nodes, rhs.m_nodes);
}

ResourceSectionAssetTable& ResourceSectionAssetTable::operator=(ResourceSectionAssetTable& other) {
	swap(*this, other);
	return *this;
}

ResourceSectionAssetTable& ResourceSectionAssetTable::operator=(ResourceSectionAssetTable&& other) noexcept {
	swap(*this, other);
	return *this;
}

void ResourceSectionAssetTable::insert(char* name, std::size_t len, AssetElementType data) {
	// Error check
	if (!name) { return; }

	// Resize if needed
	if (m_capacity == 0) {
		resize(m_defaultCapacity);
	}
	else if (m_length / (float)m_capacity >= 0.875f) {
		resize(m_capacity * 2);
	}

	// Hash the key
	AssetHashType h = hash(name, len);
	std::size_t pos = h1(h) & (m_capacity - 1);

	// Linear probe to find an empty bucket
	while (1) {
		std::uint8_t* ctrl = &m_ctrl[pos];
		if (ctrlIsEmpty(*ctrl)) {
			// Deep copy hash key & data
			AssetNode* node = &m_nodes[pos];
			strncpy_s(node->name, name, len);
			node->data = data;

			// Save lower bits of hash to the control block
			AssetHashType low = h2(h);
			memcpy_s(ctrl, 1, &low, 1);
			break;
		}
		else {
			pos = (pos + 1) & (m_capacity - 1);
		}
	}
	m_length++;
	m_dirty = true;
}

std::size_t ResourceSectionAssetTable::size() const {
	return m_length;
}

std::size_t ResourceSectionAssetTable::sizeInBytes() const {
	return roundUp(12 + (41 * m_capacity), APOLLO_ARC_ALIGN);
}

bool ResourceSectionAssetTable::isEmpty() const {
	return m_length == 0;
}

QByteArray ResourceSectionAssetTable::toBytes() {
	if (m_dirty) {
		m_dirty = false;
		m_byteArray.clear();

		// Write header
		byteArrayPushStr(&m_byteArray, "ARFT", 4);
		byteArrayPushInt32(&m_byteArray, std::uint32_t(m_length));
		byteArrayPushInt32(&m_byteArray, std::uint32_t(m_capacity));

		// Write control block
		for (std::size_t i = 0; i < m_capacity; ++i) {
			byteArrayPushInt8(&m_byteArray, m_ctrl[i]);
		}

		// Write nodes
		for (std::size_t i = 0; i < m_capacity; ++i) {
			AssetNode node = m_nodes[i];
			byteArrayPushStr(&m_byteArray, node.name, 32);
			byteArrayPushInt64(&m_byteArray, node.data);
		}

		// Padding
		byteArrayPad(&m_byteArray, APOLLO_ARC_ALIGN);
	}
	return m_byteArray;
}

void ResourceSectionAssetTable::resize(std::size_t newCapacity) {
	// Create new buffers
	std::uint8_t* newCtrl = new std::uint8_t[newCapacity];
	AssetNode* newNodes = new AssetNode[newCapacity];
	if (!newCtrl || !newNodes) { 
		return; 
	}

	// Zero out new buffers
	memset(newCtrl, 0x80, sizeof(*newCtrl) * newCapacity);
	memset(newNodes, 0, sizeof(*newNodes) * newCapacity);

	// Overwrite old buffers
	std::uint8_t* oldCtrl = m_ctrl;
	AssetNode* oldNodes = m_nodes;
	std::size_t oldCapacity = m_capacity;
	m_ctrl = newCtrl;
	m_nodes = newNodes;
	m_capacity = newCapacity;
	m_length = 0;

	// Move over old contents
	for (std::size_t i = 0; i < oldCapacity; ++i) {
		std::uint8_t ctrl = oldCtrl[i];
		if (!ctrlIsEmpty(ctrl)) {
			AssetNode node = oldNodes[i];
			insert(&node.name[0], sizeof(node.name), node.data);
		}
	}
	delete[] oldCtrl;
	delete[] oldNodes;
}

ResourceSectionAssetTable::AssetHashType ResourceSectionAssetTable::hash(char* name, std::size_t len) {
	std::uint8_t mask_size = sizeof(AssetHashType) - 4;
	AssetHashType hash = m_hashFNVPrime;
	AssetHashType mask = 0xF << mask_size;
	for (std::size_t i = 0; i < len; ++i) {
		hash = (hash << 4) + (AssetHashType)name[i];
		AssetHashType g = hash & mask;
		if (g != 0) { 
			hash ^= g >> mask_size; 
		}
		hash &= ~g;
	}
	return hash;
}

ResourceSectionAssetTable::iterator ResourceSectionAssetTable::begin() {
	iterator it(this, std::numeric_limits<std::size_t>::max());
	return ++it;
}

ResourceSectionAssetTable::iterator ResourceSectionAssetTable::end() {
	return iterator(this, m_capacity);
}

ResourceSectionAssetTable::iterator::iterator(ResourceSectionAssetTable* table, std::size_t idx) :
	m_table(table), m_idx(idx) {}

ResourceSectionAssetTable::iterator& ResourceSectionAssetTable::iterator::operator++() {
	// Find next valid block
	if (m_idx == std::numeric_limits<std::size_t>::max()) {
		m_idx = 0; 
	}
	else { 
		++m_idx; 
	}
	while (m_idx < m_table->m_capacity) {
		std::uint8_t ctrl = m_table->m_ctrl[m_idx];
		if (!ResourceSectionAssetTable::ctrlIsEmpty(ctrl)) { 
			break; 
		}
		++m_idx;
	}
	return *this;
}

ResourceSectionAssetTable::iterator ResourceSectionAssetTable::iterator::operator++(int) {
	iterator tmp = *this;
	++(*this);
	return tmp;
}

ResourceSectionAssetTable::iterator::reference ResourceSectionAssetTable::iterator::operator*() {
	return m_table->m_nodes[m_idx];
}

ResourceSectionAssetTable::iterator::pointer ResourceSectionAssetTable::iterator::operator->() {
	return &m_table->m_nodes[m_idx];
}

ResourceSectionAssetData::ResourceSectionAssetData(std::vector<TextureGroupBuilder>* textureGroups) :
	m_textureGroups(textureGroups) {}

ResourceSectionAssetData::ResourceSectionAssetData(std::size_t capacity, std::vector<TextureGroupBuilder>* textureGroups) :
	ResourceSectionAssetData(textureGroups) {
	m_assets.reserve(capacity);
}

ResourceSectionAssetData::ResourceSectionAssetData(ResourceSectionAssetData&& other) noexcept {
	m_assets = std::move(other.m_assets);
	m_dirty = std::move(other.m_dirty);
	m_byteArray = std::move(other.m_byteArray);
}

ResourceSectionAssetData::ResourceSectionAssetData(const ResourceSectionAssetData& other) {
	m_assets = other.m_assets;
	m_dirty = other.m_dirty;
	m_byteArray = other.m_byteArray;
}

ResourceSectionAssetData::~ResourceSectionAssetData() = default;

void swap(ResourceSectionAssetData& lhs, ResourceSectionAssetData& rhs) {
	std::swap(lhs.m_assets, rhs.m_assets);
	std::swap(lhs.m_dirty, rhs.m_dirty);
	std::swap(lhs.m_byteArray, rhs.m_byteArray);
}

ResourceSectionAssetData& ResourceSectionAssetData::operator=(ResourceSectionAssetData& other) {
	swap(*this, other);
	return *this;
}

ResourceSectionAssetData& ResourceSectionAssetData::operator=(ResourceSectionAssetData&& other) noexcept {
	swap(*this, other);
	return *this;
}

void ResourceSectionAssetData::insert(const AssetDescriptorPtr& asset) {
	m_assets.push_back(asset);
	m_dirty = true;
}

std::size_t ResourceSectionAssetData::size() const {
	return m_assets.size();
}

bool ResourceSectionAssetData::isEmpty() const {
	return m_assets.empty();
}

QByteArray ResourceSectionAssetData::toBytes() {
	if (m_dirty) {
		m_dirty = false;
		m_byteArray.clear();
		m_assetOffsetMap.clear();

		// Add textures referenced by the texture groups
		for(std::size_t i = 0; i < m_textureGroups->size(); ++i) {
			TextureGroupBuilder textureGroup = m_textureGroups->at(i);
			for (auto& entryPair : textureGroup) {
				// Construct simple asset block that references texture group
				TextureGroupEntry entry = entryPair.second;

				// Write header
				QByteArray assetData;
				byteArrayPushStr(&assetData, g_assetInfoMap[AssetType::Texture].m_signature, 4);
				byteArrayPushInt32(&assetData, 0u);
				byteArrayPushInt64(&assetData, 0ul);
				byteArrayPushInt64(&assetData, 0ul);
				byteArrayPushInt64(&assetData, 0ul);
				byteArrayPushInt64(&assetData, 0ul);
				byteArrayPushStr(&assetData, entry.m_texture->name(), 32);
				byteArrayPushInt64(&assetData, 0ul);

				// Write data
				byteArrayPushInt32(&assetData, uint32_t(i));
				byteArrayPushInt32(&assetData, uint32_t(entry.m_xpos));
				byteArrayPushInt32(&assetData, uint32_t(entry.m_ypos));
				byteArrayPushInt32(&assetData, uint32_t(entry.m_texture->getWidth()));
				byteArrayPushInt32(&assetData, uint32_t(entry.m_texture->getHeight()));

				byteArrayPad(&assetData, APOLLO_ARC_ALIGN);
				m_assetOffsetMap.insert(std::make_pair(entry.m_texture->name(), m_byteArray.size()));
				m_byteArray.append(assetData);
			}
		}

		// Write assets
		for (auto& asset : m_assets) {
			// Skip textures unless no texture groups have been defined
			if (asset->assetType() != AssetType::Texture || !m_textureGroups) {
				// Write header
				QByteArray assetData;
				byteArrayPushStr(&assetData, g_assetInfoMap[AssetType::Texture].m_signature, 4);
				byteArrayPushInt32(&assetData, 0u);
				byteArrayPushInt64(&assetData, 0ul);
				byteArrayPushInt64(&assetData, 0ul);
				byteArrayPushInt64(&assetData, 0ul);
				byteArrayPushInt64(&assetData, 0ul);
				byteArrayPushStr(&assetData, asset->name(), 32);
				byteArrayPushInt64(&assetData, 0ul);

				// Write data
				assetData.append(asset->toBytes());

				byteArrayPad(&assetData, APOLLO_ARC_ALIGN);
				m_assetOffsetMap.insert(std::make_pair(asset->name(), m_byteArray.size()));
				m_byteArray.append(assetData);
			}
		}
	}
	return m_byteArray;
}

uint64_t ResourceSectionAssetData::getAssetOffset(const QString& name) {
	// Generate offset map
	toBytes();

	// Find offset for asset
	auto pair = m_assetOffsetMap.find(name);
	return pair == m_assetOffsetMap.end()
		? std::numeric_limits<std::uint64_t>::max()
		: pair->second;
}
