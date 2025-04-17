#pragma once

// ============================================================================ Macros

#define APOLLO_VERSION_MAJOR 1
#define APOLLO_VERSION_MINOR 0
#define APOLLO_VERSION_PATCH 0

#define STRINGIFY_(X) #X
#define STRINGIFY(X) STRINGIFY_(X)
#define MAKE_VERSION_STR(major, minor, patch) (STRINGIFY(major) "." STRINGIFY(minor) "." STRINGIFY(patch))
#define APOLLO_VERSION_STR MAKE_VERSION_STR(APOLLO_VERSION_MAJOR, APOLLO_VERSION_MINOR, APOLLO_VERSION_PATCH)

#define APOLLO_ARC_ALIGN 16

enum SDL_PixelType {
    SDL_PIXELTYPE_UNKNOWN,
    SDL_PIXELTYPE_INDEX1,
    SDL_PIXELTYPE_INDEX4,
    SDL_PIXELTYPE_INDEX8,
    SDL_PIXELTYPE_PACKED8,
    SDL_PIXELTYPE_PACKED16,
    SDL_PIXELTYPE_PACKED32,
    SDL_PIXELTYPE_ARRAYU8,
    SDL_PIXELTYPE_ARRAYU16,
    SDL_PIXELTYPE_ARRAYU32,
    SDL_PIXELTYPE_ARRAYF16,
    SDL_PIXELTYPE_ARRAYF32,
    /* appended at the end for compatibility with sdl2-compat:  */
    SDL_PIXELTYPE_INDEX2
};

enum SDL_BitmapOrder {
    SDL_BITMAPORDER_NONE,
    SDL_BITMAPORDER_4321,
    SDL_BITMAPORDER_1234
};

enum SDL_PackedOrder {
    SDL_PACKEDORDER_NONE,
    SDL_PACKEDORDER_XRGB,
    SDL_PACKEDORDER_RGBX,
    SDL_PACKEDORDER_ARGB,
    SDL_PACKEDORDER_RGBA,
    SDL_PACKEDORDER_XBGR,
    SDL_PACKEDORDER_BGRX,
    SDL_PACKEDORDER_ABGR,
    SDL_PACKEDORDER_BGRA
};

enum SDL_ArrayOrder {
    SDL_ARRAYORDER_NONE,
    SDL_ARRAYORDER_RGB,
    SDL_ARRAYORDER_RGBA,
    SDL_ARRAYORDER_ARGB,
    SDL_ARRAYORDER_BGR,
    SDL_ARRAYORDER_BGRA,
    SDL_ARRAYORDER_ABGR
};

enum SDL_PackedLayout {
    SDL_PACKEDLAYOUT_NONE,
    SDL_PACKEDLAYOUT_332,
    SDL_PACKEDLAYOUT_4444,
    SDL_PACKEDLAYOUT_1555,
    SDL_PACKEDLAYOUT_5551,
    SDL_PACKEDLAYOUT_565,
    SDL_PACKEDLAYOUT_8888,
    SDL_PACKEDLAYOUT_2101010,
    SDL_PACKEDLAYOUT_1010102
};

#define SDL_DEFINE_PIXELFORMAT(type, order, layout, bits, bytes) \
    ((1 << 28) | ((type) << 24) | ((order) << 20) | \
    ((layout) << 16) | ((bits) << 8) | ((bytes) << 0))

// ============================================================================ Includes

// Standard libraries
#include <cstdlib> 
#include <ctime>
#include <iostream>
#include <memory>
#include <algorithm>
#include <exception>
#include <limits>
#include <vector>
#include <unordered_map>
#include <list>
#include <stack>

// Qt libraries
#include <QApplication>
#include <QWidget>

#include <QMainWindow>
#include <QGroupBox>
#include <QScrollArea>
#include <QTabWidget>
#include <QSplitter>
#include <QStackedWidget>

#include <QMessageBox>
#include <QFileDialog>

#include <QPushButton>
#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QSpinBox>
#include <QMenu>
#include <QMenuBar>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include <QAction>
#include <QEvent>
#include <QMouseEvent>

#include <QListView>
#include <QTreeView>
#include <QStringListModel>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QMimeData>

#include <QFile>
#include <QFileInfo>
#include <QDomDocument>
#include <QXMLStreamWriter>
#include <QXMLStreamReader>
#include <QPainter>
#include <QImageReader>

#include <QVariant>
#include <QVariantList>
#include <QVector>
#include <QStack>
#include <QList>
#include <QException>
#include <QRegularExpression>
#include <QRegularExpressionValidator>




// External libraries
#define LZ4_HEAPMODE 1
#include "lz4.h"
#define CBC 1
#define ECB 0
#define CTR 0
#include "aes.h"

// ============================================================================ Helper functions

/// <summary>
/// Check if the two version strings match. If 'exact' is false, only match the major & minor revisions.
/// </summary>
/// <param name="v1">Version string 1</param>
/// <param name="v2">Version string 2</param>
/// <param name="exact">Require patch revision to match as well</param>
/// <returns>True if correct revisions match</returns>
bool versionStringMatch(QString v1, QString v2, bool exact = false);

/// <summary>
/// Generate a random printable character.
/// </summary>
/// <returns>ASCII character</returns>
char randChar();

/// <summary>
/// Round the number up to the next multiple.
/// </summary>
/// <param name="num">Number to round</param>
/// <param name="multiple">Multiple to round to</param>
/// <returns></returns>
uint64_t roundUp(std::uint64_t num, std::uint64_t multiple);

/// <summary>
/// Calculate the CRC32 value for the given data.
/// </summary>
/// <param name="data">Data buffer</param>
/// <param name="length">Buffer length</param>
/// <param name="previousCRC">Previous CRC value (only if data is being appended to an existing CRC value)</param>
/// <returns>CRC value</returns>
std::uint32_t crc32Calculate(const void* data, std::size_t length, std::uint32_t previousCRC = 0);

extern const std::uint32_t crc32Lookup[256];

/// <summary>
/// Get a string describing the given image format.
/// </summary>
/// <param name="format">QImage format</param>
/// <returns>Description string</returns>
QString imageFormatString(QImage::Format format);

/// <summary>
/// Convert the Qt image format enum to an SDL3 format number.
/// </summary>
/// <param name="format">QImage format</param>
/// <returns>SDL format</returns>
uint32_t imageFormatSDL(QImage::Format format);

/// <summary>
/// Get the base name (no increment number) of the texture group.
/// </summary>
/// <param name="name">Group name</param>
/// <returns>Base group name</returns>
QString textureGroupNameBase(const QString& name);

/// <summary>
/// Increment the given texture group name by 1 (i.e. "TextureGroup" becomes "TextureGroup_1", then "TextureGroup_2" etc...)
/// </summary>
/// <param name="name">Group name</param>
/// <returns>Incremented group name</returns>
QString textureGroupNameIncrement(const QString& name);

/// <summary>
/// Push one byte to the end of the byte array.
/// </summary>
/// <param name="array">Byte array</param>
/// <param name="num">Value</param>
void byteArrayPushInt8(QByteArray* array, std::uint8_t num);

/// <summary>
/// Push two bytes to the end of the byte array.
/// </summary>
/// <param name="array">Byte array</param>
/// <param name="num">Value</param>
void byteArrayPushInt16(QByteArray* array, std::uint16_t num);

/// <summary>
/// Push four bytes to the end of the byte array.
/// </summary>
/// <param name="array">Byte array</param>
/// <param name="num">Value</param>
void byteArrayPushInt32(QByteArray* array, std::uint32_t num);

/// <summary>
/// Push eight bytes to the end of the byte array.
/// </summary>
/// <param name="array">Byte array</param>
/// <param name="num">Value</param>
void byteArrayPushInt64(QByteArray* array, std::uint64_t num);

/// <summary>
/// Overwrite one byte in the byte array.
/// </summary>
/// <param name="array">Byte array</param>
/// <param name="idx">Starting position</param>
/// <param name="num">Value</param>
void byteArraySetInt8(QByteArray* array, qsizetype idx, std::uint8_t num);

/// <summary>
/// Overwrite two bytes in the byte array.
/// </summary>
/// <param name="array">Byte array</param>
/// <param name="idx">Starting position</param>
/// <param name="num">Value</param>
void byteArraySetInt16(QByteArray* array, qsizetype idx, std::uint16_t num);

/// <summary>
/// Overwrite four bytes in the byte array.
/// </summary>
/// <param name="array">Byte array</param>
/// <param name="idx">Starting position</param>
/// <param name="num">Value</param>
void byteArraySetInt32(QByteArray* array, qsizetype idx, std::uint32_t num);

/// <summary>
/// Overwrite eight bytes in the byte array.
/// </summary>
/// <param name="array">Byte array</param>
/// <param name="idx">Starting position</param>
/// <param name="num">Value</param>
void byteArraySetInt64(QByteArray* array, qsizetype idx, std::uint64_t num);

/// <summary>
/// Append a string of bytes to the end of the byte array. If 'maxLen' is longer than the string, array will be padded with null characters.
/// </summary>
/// <param name="array">Byte array</param>
/// <param name="str">Value</param>
/// <param name="maxLen">Max number of characters to add (including null terminator)</param>
void byteArrayPushStr(QByteArray* array, QString str, qsizetype maxLen);

/// <summary>
/// Append a string of bytes to the end of the byte array. If 'maxLen' is longer than the string, array will be padded with null characters.
/// </summary>
/// <param name="array">Byte array</param>
/// <param name="str">Value</param>
/// <param name="strLen">Number of characters in string</param>
/// <param name="maxLen">Max number of characters to add (including null terminator)</param>
void byteArrayPushStr(QByteArray* array, const char* str, qsizetype strLen, qsizetype maxLen);

/// <summary>
/// Overwrite a portion of the byte array with a string of bytes. If 'maxLen' is longer than the string, array will be padded with null characters.
/// </summary>
/// <param name="array">Byte array</param>
/// <param name="idx">Starting position</param>
/// <param name="str">Value</param>
/// <param name="maxLen">Max number of characters to add (including null terminator)</param>
void byteArraySetStr(QByteArray* array, qsizetype idx, QString str, qsizetype maxLen);

/// <summary>
/// Overwrite a portion of the byte array with a string of bytes. If 'maxLen' is longer than the string, array will be padded with null characters.
/// </summary>
/// <param name="array">Byte array</param>
/// <param name="idx">Starting position</param>
/// <param name="str">Value</param>
/// <param name="strLen">Number of characters in string</param>
/// <param name="maxLen">Max number of characters to add (including null terminator)</param>
void byteArraySetStr(QByteArray* array, qsizetype idx, const char* str, qsizetype strLen, qsizetype maxLen);

/// <summary>
/// Pad the byte array with 0xFF until its size is a multiple of the given alignment.
/// </summary>
/// <param name="array">Byte array</param>
/// <param name="alignTo">Alignment</param>
void byteArrayAlign(QByteArray* array, qsizetype alignTo);