#pragma once

#define APOLLO_VERSION_MAJOR 1
#define APOLLO_VERSION_MINOR 0
#define APOLLO_VERSION_PATCH 0

#define STRINGIFY_(X) #X
#define STRINGIFY(X) STRINGIFY_(X)
#define MAKE_VERSION_STR(major, minor, patch) (STRINGIFY(major) "." STRINGIFY(minor) "." STRINGIFY(patch))
#define APOLLO_VERSION_STR MAKE_VERSION_STR(APOLLO_VERSION_MAJOR, APOLLO_VERSION_MINOR, APOLLO_VERSION_PATCH)

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

// External libraries
#define LZ4_HEAPMODE 1
#include "lz4.h"
#define CBC 1
#define ECB 0
#define CTR 0
#include "aes.h"