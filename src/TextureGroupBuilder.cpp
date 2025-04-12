#include "TextureGroupBuilder.hpp"

inline bool TextureSort::operator()(AssetDescriptorTexture* p1, AssetDescriptorTexture* p2) {
	return ((p1->getWidth() * p1->getHeight()) < (p2->getWidth() * p2->getHeight()));
}

bool TextureGroupEntry::isNull() const {
	return (!m_texture);
}

TextureGroupBuilder::TextureGroupBuilder(int pageSize, QString groupName, QImage::Format format) :
	m_pageSize(pageSize), m_groupName(groupName), m_format(format) {}

TextureGroupBuilder::~TextureGroupBuilder() = default;

bool TextureGroupBuilder::push(AssetDescriptorTexture* asset) {
	if (asset->assetType() == AssetType::Texture) {
		if (asset->getWidth() >= 0) {
			m_assets.push_back(asset);
			return true;
		}
	}
	return false;
}

AssetDescriptorTexture* TextureGroupBuilder::top() const {
	return m_assets.size() > 0
		? m_assets.back()
		: nullptr;
}

void TextureGroupBuilder::pop() {
	m_assets.pop_back();
}

int TextureGroupBuilder::size() const {
	return m_assets.size();
}

QString TextureGroupBuilder::groupName() const {
	return m_groupName;
}

QString TextureGroupBuilder::getErrorMessage() const {
	return m_errorMessage;
}

void TextureGroupBuilder::assign(std::vector<AssetDescriptorTexture*>&& assets) {
	m_assets = std::move(assets);
}

TextureGroupEntry TextureGroupBuilder::entry(const QString& name) const {
	auto pair = m_entries.find(name);
	return pair == m_entries.end()
		? TextureGroupEntry()
		: pair->second;
}

std::vector<AssetDescriptorTexture*> TextureGroupBuilder::buildGroup() {
	// Sort assets by total area
	std::sort(m_assets.begin(), m_assets.end(), TextureSort());

	// Fill group left-to-right, top-to-bottom
	int currRowHeight = 0;
	int currRowOffset = 0;
	int currColOffset = 0;
	std::vector<AssetDescriptorTexture*> leftoverAssets;
	for(int i = 0; i < m_assets.size(); ++i) {
		AssetDescriptorTexture* asset = m_assets[i];
		if (currColOffset + asset->getWidth() <= m_pageSize && currRowOffset + asset->getHeight() <= m_pageSize) {
			// Construct texture group entry
			TextureGroupEntry entry;
			entry.m_texture = asset;
			entry.m_xpos = currColOffset;
			entry.m_ypos = currRowOffset;
			m_entries.insert(std::make_pair(asset->name(), entry));
			currColOffset += asset->getWidth();
			currRowHeight = std::max(currRowHeight, asset->getHeight());
		}
		else if (currColOffset + asset->getWidth() > m_pageSize && currRowOffset + asset->getHeight() < m_pageSize) {
			// Check if row is full
			currColOffset = 0;
			currRowOffset += currRowHeight;
			currRowHeight = 0;

			// Re-check the current texture
			i--;
		}
		else {
			// Asset cant fit in this group, remove it and set it aside
			leftoverAssets.push_back(m_assets[i]);
			m_assets.erase(m_assets.begin() + i);
			i--;
		}
	}

	return leftoverAssets;
}

QImage TextureGroupBuilder::generateImage() {
	m_errorMessage.clear();
	try {
		QImage finalTexture(m_pageSize, m_pageSize, m_format);
		finalTexture.fill(QColor(0, 0, 0, 0));
		QPainter painter(&finalTexture);

		// Read all entries & copy data into final texture
		for (auto& entryPair : m_entries) {
			TextureGroupEntry entry = entryPair.second;
			QImageReader reader(entry.m_texture->filename());
			QImage sourceTexture = reader.read();
			if (sourceTexture.isNull()) {
				QString msg = QString("Failed to open texture file '%1'\n%2").arg(entry.m_texture->filename()).arg(reader.errorString());
				throw std::exception(msg.toStdString().c_str());
			}
			painter.drawImage(QPoint(entry.m_xpos, entry.m_ypos), sourceTexture);
		}
		painter.end();
		return finalTexture;
	}
	catch (std::exception& e) {
		m_errorMessage = QString(e.what());
		return QImage();
	}
}

std::unordered_map<QString, TextureGroupEntry>::iterator TextureGroupBuilder::begin() {
	return m_entries.begin();
}

std::unordered_map<QString, TextureGroupEntry>::iterator TextureGroupBuilder::end() {
	return m_entries.end();
}

std::unordered_map<QString, TextureGroupEntry>::const_iterator TextureGroupBuilder::cbegin() const {
	return m_entries.cbegin();
}

std::unordered_map<QString, TextureGroupEntry>::const_iterator TextureGroupBuilder::cend() const {
	return m_entries.cend();
}