#pragma once
/**
 * ResourceGenerator.hpp
 *
 * Collection of classes and functions for generating the final packed binary file.
 */
#include "Common.hpp"
#include "AssetInfo.hpp"
#include "TextureGroupInfo.hpp"
#include "TextureGroupBuilder.hpp"

class ResourceSectionTextureGroup {
public:
	ResourceSectionTextureGroup();
	ResourceSectionTextureGroup(std::size_t capacity);
	ResourceSectionTextureGroup(ResourceSectionTextureGroup&& other) noexcept;
	ResourceSectionTextureGroup(const ResourceSectionTextureGroup& other);
	~ResourceSectionTextureGroup();

	friend void swap(ResourceSectionTextureGroup& lhs, ResourceSectionTextureGroup& rhs);
	ResourceSectionTextureGroup& operator=(ResourceSectionTextureGroup& other);
	ResourceSectionTextureGroup& operator=(ResourceSectionTextureGroup&& other) noexcept;

	void insert(const QImage& image);
	std::size_t size() const;
	std::size_t sizeInBytes() const;
	bool isEmpty() const;
	QByteArray toBytes();

private:
	std::uint64_t m_stride = 0u;
	bool m_dirty = true;
	QByteArray m_byteArray;
	std::vector<QByteArray> m_imageData;
};

class ResourceSectionAssetTable {
private:
	typedef std::uint32_t AssetHashType;
	typedef std::uint64_t AssetElementType;
	const static AssetHashType m_hashFNVPrime = 16777619U;
	const static std::size_t m_defaultCapacity = 4;

public:
	ResourceSectionAssetTable();
	ResourceSectionAssetTable(std::size_t capacity);
	ResourceSectionAssetTable(ResourceSectionAssetTable&& other) noexcept;
	ResourceSectionAssetTable(const ResourceSectionAssetTable& other);
	~ResourceSectionAssetTable();

	friend void swap(ResourceSectionAssetTable& lhs, ResourceSectionAssetTable& rhs);
	ResourceSectionAssetTable& operator=(ResourceSectionAssetTable& other);
	ResourceSectionAssetTable& operator=(ResourceSectionAssetTable&& other) noexcept;

	void insert(char* name, std::size_t len, AssetElementType data);
	std::size_t size() const;
	std::size_t sizeInBytes() const;
	bool isEmpty() const;
	QByteArray toBytes();

private:
	void resize(std::size_t newCapacity);
	static AssetHashType hash(char* name, std::size_t len);
	static inline AssetHashType h1(AssetHashType h) {
		return h >> 7;
	}
	static inline AssetHashType h2(AssetHashType h) {
		return h & 0x7F;
	}
	static inline bool ctrlIsEmpty(std::uint8_t h) {
		return h & 0x80;
	}

	/// @brief Struct representing a single file in a file table.
	struct AssetNode {
		char name[32] = "\0";
		AssetElementType data = 0;
	};

	std::uint8_t* m_ctrl = nullptr;
	AssetNode* m_nodes = nullptr;
	std::size_t m_capacity = 0;
	std::size_t m_length = 0;
	bool m_dirty = true;
	QByteArray m_byteArray;
public:
	/// @brief Iterator for elements in a file table.
	class iterator {
	private:
		ResourceSectionAssetTable* m_table;
		std::size_t m_idx;
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = AssetNode;
		using pointer = AssetNode*;
		using reference = AssetNode&;
		using iterator_category = std::forward_iterator_tag;

		iterator(ResourceSectionAssetTable* table, std::size_t idx);
		iterator& operator++();
		iterator operator++(int);
		reference operator*();
		pointer operator->();

		friend bool operator==(const iterator& lhs, const iterator& rhs) {
			return lhs.m_table == rhs.m_table && lhs.m_idx == rhs.m_idx;
		}

		friend bool operator!=(const iterator& lhs, const iterator& rhs) {
			return !(lhs == rhs);
		}
	};

	iterator begin();
	iterator end();
};

class ResourceSectionAssetData {
public:
	ResourceSectionAssetData(std::vector<TextureGroupBuilder>* textureGroups = nullptr);
	ResourceSectionAssetData(std::size_t capacity, std::vector<TextureGroupBuilder>* textureGroups = nullptr);
	ResourceSectionAssetData(ResourceSectionAssetData&& other) noexcept;
	ResourceSectionAssetData(const ResourceSectionAssetData& other);
	~ResourceSectionAssetData();

	friend void swap(ResourceSectionAssetData& lhs, ResourceSectionAssetData& rhs);
	ResourceSectionAssetData& operator=(ResourceSectionAssetData& other);
	ResourceSectionAssetData& operator=(ResourceSectionAssetData&& other) noexcept;

	void insert(const AssetDescriptorPtr& asset);
	std::size_t size() const;
	bool isEmpty() const;
	QByteArray toBytes();
	uint64_t getAssetOffset(const QString& name);

private:
	bool m_dirty = true;
	std::vector<AssetDescriptorPtr> m_assets;
	std::vector<TextureGroupBuilder>* m_textureGroups;
	std::unordered_map<QString, std::uint64_t> m_assetOffsetMap;
	QByteArray m_byteArray;
};