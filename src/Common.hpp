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

// ============================================================================ Includes

// Standard libraries
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
#include <QWidget>
#include <QMainWindow>
#include <QGroupBox>
#include <QScrollArea>
#include <QTreeView>
#include <QItemDelegate>
#include <QTabWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QStackedWidget>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSplitter>
#include <QEvent>
#include <QMouseEvent>
#include <QStyledItemDelegate>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QDomDocument>
#include <QXMLStreamWriter>
#include <QXMLStreamReader>
#include <QVariant>
#include <QVariantList>
#include <QVector>
#include <QStack>
#include <QList>
#include <QException>
#include <QMimeData>
#include <QApplication>
#include <QObjectCleanupHandler>
#include <QPainter>
#include <QListView>
#include <QStringListModel>
#include <QImageReader>

// External libraries
#define LZ4_HEAPMODE 1
#include "lz4.h"
#define CBC 1
#define ECB 0
#define CTR 0
#include "aes.h"

// ============================================================================ Helper functions

/// @brief Check if the two version strings match. If 'exact' is false, only match the major & minior revisions.
/// @param v1 Version string 1
/// @param v2 Version string 2
/// @param exact Require patch revision to match as well
/// @return True if correct revisions match
bool versionStringMatch(QString v1, QString v2, bool exact = false);

/// @brief Generate a random printable character.
char randChar();

/// @brief Round the number up to the next multiple.
uint64_t roundUp(std::uint64_t num, std::uint64_t multiple);

/// @brief Calcualte the CRC32 value for the given data.
/// @param data Data buffer
/// @param length Buffer length
/// @param previousCRC Previous CRC value (only if data is being appended to an existing CRC value)
/// @return CRC value
std::uint32_t crc32Calculate(const void* data, std::size_t length, std::uint32_t previousCRC = 0);

extern const std::uint32_t crc32Lookup[256];

bool versionStringMatch(QString v1, QString v2, bool exact = false);

QString imageFormatString(QImage::Format format);

QString textureGroupNameBase(const QString& name);

QString textureGroupNameIncrement(const QString& name);

void byteArrayPushInt8(QByteArray* array, std::uint8_t num);
void byteArrayPushInt16(QByteArray* array, std::uint16_t num);
void byteArrayPushInt32(QByteArray* array, std::uint32_t num);
void byteArrayPushInt64(QByteArray* array, std::uint64_t num);

void byteArraySetInt8(QByteArray* array, qsizetype idx, std::uint8_t num);
void byteArraySetInt16(QByteArray* array, qsizetype idx, std::uint16_t num);
void byteArraySetInt32(QByteArray* array, qsizetype idx, std::uint32_t num);
void byteArraySetInt64(QByteArray* array, qsizetype idx, std::uint64_t num);

void byteArrayPushStr(QByteArray* array, QString str, qsizetype maxLen);
void byteArrayPushStr(QByteArray* array, const char* str, qsizetype strLen, qsizetype maxLen);
void byteArraySetStr(QByteArray* array, qsizetype idx, QString str, qsizetype maxLen);
void byteArraySetStr(QByteArray* array, qsizetype idx, const char* str, qsizetype strLen, qsizetype maxLen);

void byteArrayPad(QByteArray* array, qsizetype alignTo);