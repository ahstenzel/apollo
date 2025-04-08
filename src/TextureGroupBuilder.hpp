#pragma once
/**
 * TextureGroupInfo.hpp
 *
 * Collection of classes and functions for editing texture groups.
 */
#include "Common.hpp"
#include "AssetInfo.hpp"
#include "TextureGroupInfo.hpp"

struct TextureGroupEntry {
	AssetDescriptorTexture* m_texture = nullptr;
	int m_xpos = 0;
	int m_ypos = 0;

	bool isNull() const;
};

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

	std::vector<AssetDescriptorTexture*> buildGroup();
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

struct TextureSort {
	inline bool operator() (AssetDescriptorTexture* p1, AssetDescriptorTexture* p2);
};