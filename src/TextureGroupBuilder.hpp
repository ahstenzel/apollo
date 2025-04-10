#pragma once
/**
 * TextureGroupInfo.hpp
 *
 * Collection of classes and functions for editing texture groups.
 */
#include "Common.hpp"
#include "AssetInfo.hpp"
#include "TextureGroupInfo.hpp"

/// <summary>
/// Single entry for a texture that points to its place in a group.
/// </summary>
struct TextureGroupEntry {
	AssetDescriptorTexture* m_texture = nullptr;
	int m_xpos = 0;
	int m_ypos = 0;

	bool isNull() const;
};

/// <summary>
/// Generates a collective images for a texture group. Textures should be added with push(), then the group should be built
/// with buildGroup(), and any leftover textures that dont fit will be returned. Once built, the final image can be rendered
/// using generateImage().
/// </summary>
class TextureGroupBuilder {
public:
	TextureGroupBuilder(int pageSize = -1, QString groupName = "", QImage::Format format = QImage::Format_ARGB32);
	~TextureGroupBuilder();

	bool push(AssetDescriptorTexture* asset);
	AssetDescriptorTexture* top() const;
	void pop();
	int size() const;
	QString groupName() const;
	QString error() const;
	void assign(std::vector<AssetDescriptorTexture*>&& assets);
	TextureGroupEntry entry(const QString& name) const;

	/// <summary>
	/// Pack as many of the supplied textures into this group as possible, calculating their final positions. Any leftovers 
	/// that cant fit will be removed from this builders internal buffer and returned from this function.
	/// </summary>
	/// <returns>Leftover textures</returns>
	std::vector<AssetDescriptorTexture*> buildGroup();

	/// <summary>
	/// Render the final image once the group has been built with buildGroup().
	/// </summary>
	/// <returns></returns>
	QImage generateImage();

	std::unordered_map<QString, TextureGroupEntry>::iterator begin();
	std::unordered_map<QString, TextureGroupEntry>::iterator end();
	std::unordered_map<QString, TextureGroupEntry>::const_iterator cbegin() const;
	std::unordered_map<QString, TextureGroupEntry>::const_iterator cend() const;

private:
	int m_pageSize;
	QString m_groupName;
	QImage::Format m_format;
	QString m_errorString = "";
	std::vector<AssetDescriptorTexture*> m_assets;
	std::unordered_map<QString, TextureGroupEntry> m_entries;
};

/// <summary>
/// Helper struct for sorting texture assets by area.
/// </summary>
struct TextureSort {
	inline bool operator() (AssetDescriptorTexture* p1, AssetDescriptorTexture* p2);
};