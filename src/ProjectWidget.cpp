#include "ProjectWidget.hpp"
#include "MainWindow.hpp"

bool versionStringMatch(QString v1, QString v2, bool exact) {
	QStringList rev1 = v1.split(".");
	QStringList rev2 = v2.split(".");
	if (rev1.length() < 3 || rev2.length() < 3) return false;
	if (rev1[0] != rev2[0]) return false;
	if (rev1[1] != rev1[1]) return false;
	if (exact && rev1[2] != rev2[2]) return false;
	return true;
}

ProjectWidget::ProjectWidget(QWidget* parent) :
	QWidget(parent) {
	m_assetDialog = nullptr;
	m_projectFilename = "";
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

	// Main container
	m_layout_Main = new QVBoxLayout();
	setLayout(m_layout_Main);

	// Info box
	m_groupBox_Info = new QGroupBox("Project Info");
	m_layout_Main->addWidget(m_groupBox_Info);
	m_layout_Info = new QFormLayout();
	m_groupBox_Info->setLayout(m_layout_Info);
	m_label_Info_Name = new QLabel();
	m_layout_Info->addRow("Project Name", m_label_Info_Name);

	// Create tabs
	m_tabWidget_Main = new QTabWidget();
	m_layout_Main->addWidget(m_tabWidget_Main);

	// Asset tab
	m_widget_AssetTab = new QWidget();
	m_tabWidget_Main->addTab(m_widget_AssetTab, "Assets");
	m_layout_AssetTab = new QVBoxLayout();
	m_widget_AssetTab->setLayout(m_layout_AssetTab);
	//   Splitter
	m_splitter_AssetTab = new QSplitter();
	m_splitter_AssetTab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	m_layout_AssetTab->addWidget(m_splitter_AssetTab);
	//     Asset tree
	m_treeView_AssetTab_Assets = new AssetTreeView();
	m_treeView_AssetTab_Assets->setItemDelegate(new AssetTreeItemDelegate());
	m_treeView_AssetTab_Assets->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	m_treeView_AssetTab_Assets->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_treeView_AssetTab_Assets, &QTreeView::customContextMenuRequested, this, &ProjectWidget::contextMenu_AssetTree);
	connect(m_treeView_AssetTab_Assets, &AssetTreeView::assetsChanged, this, &ProjectWidget::onDataChanged);
	m_splitter_AssetTab->addWidget(m_treeView_AssetTab_Assets);
	//     Asset editor
	m_scrollArea_AssetTab_AssetEditor = new AssetEditor();
	m_scrollArea_AssetTab_AssetEditor->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	connect(m_scrollArea_AssetTab_AssetEditor, &AssetEditor::assetsChanged, this, &ProjectWidget::onDataChanged);
	m_splitter_AssetTab->addWidget(m_scrollArea_AssetTab_AssetEditor);
	//   Button group
	m_groupBox_AssetTab_AssetButtons = new QGroupBox();
	m_groupBox_AssetTab_AssetButtons->setFlat(true);
	m_groupBox_AssetTab_AssetButtons->setStyleSheet("QGroupBox { border:0; }");
	m_groupBox_AssetTab_AssetButtons->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	m_layout_AssetTab_AssetButtons = new QHBoxLayout();
	m_groupBox_AssetTab_AssetButtons->setLayout(m_layout_AssetTab_AssetButtons);
	m_layout_AssetTab->addWidget(m_groupBox_AssetTab_AssetButtons);
	m_pushButton_AssetTab_AddGroup = new QPushButton("Add Group");
	connect(m_pushButton_AssetTab_AddGroup, &QPushButton::clicked, this, &ProjectWidget::onClicked_PushButton_AssetTab_AddGroup);
	m_layout_AssetTab_AssetButtons->addWidget(m_pushButton_AssetTab_AddGroup);
	m_pushButton_AssetTab_AddFile = new QPushButton("Add File");
	connect(m_pushButton_AssetTab_AddFile, &QPushButton::clicked, this, &ProjectWidget::onClicked_PushButton_AssetTab_AddFile);
	m_layout_AssetTab_AssetButtons->addWidget(m_pushButton_AssetTab_AddFile);
	m_layout_AssetTab_AssetButtons->addStretch();

	// Texture page tab
	m_widget_TextureGroupTab = new QWidget();
	m_tabWidget_Main->addTab(m_widget_TextureGroupTab, "Texture Groups");
	m_layout_TextureGroupTab = new QVBoxLayout();
	m_widget_TextureGroupTab->setLayout(m_layout_TextureGroupTab);
	//   Info Box
	m_groupBox_TextureGroupTab_Info = new QGroupBox();
	m_layout_TextureGroupTab->addWidget(m_groupBox_TextureGroupTab_Info);
	m_layout_TextureGroupTab_Info = new QGridLayout();
	m_groupBox_TextureGroupTab_Info->setLayout(m_layout_TextureGroupTab_Info);
	m_comboBox_TextureGroupTab_Info_TextureGroup = new QComboBox();
	m_comboBox_TextureGroupTab_Info_TextureGroup->addItems(g_textureGroups);
	m_comboBox_TextureGroupTab_Info_TextureGroup->setCurrentIndex(-1);
	m_comboBox_TextureGroupTab_Info_Size = new QComboBox();
	m_comboBox_TextureGroupTab_Info_Size->addItems(g_textureGroupSizes);
	m_comboBox_TextureGroupTab_Info_Size->setCurrentIndex(0);
	m_pushButton_TextureGroupTab_AddGroup = new QPushButton("Add Group");
	connect(m_pushButton_TextureGroupTab_AddGroup, &QPushButton::clicked, this, &ProjectWidget::onClicked_PushButton_TextureGroupTab_AddGroup);
	m_pushButton_TextureGroupTab_DeleteGroup = new QPushButton("Delete Group");
	connect(m_pushButton_TextureGroupTab_DeleteGroup, &QPushButton::clicked, this, &ProjectWidget::onClicked_PushButton_TextureGroupTab_DeleteGroup);
	m_pushButton_TextureGroupTab_DeleteGroup->setEnabled(false);

	m_layout_TextureGroupTab_Info->addWidget(new QLabel("Page Size"), 0, 0, Qt::AlignRight);
	m_layout_TextureGroupTab_Info->addWidget(m_comboBox_TextureGroupTab_Info_Size, 0, 1, Qt::AlignLeft);
	m_layout_TextureGroupTab_Info->addWidget(new QLabel("Texture Group"), 1, 0, Qt::AlignRight);
	m_layout_TextureGroupTab_Info->addWidget(m_comboBox_TextureGroupTab_Info_TextureGroup, 1, 1, Qt::AlignLeft);
	m_layout_TextureGroupTab_Info->setColumnStretch(2, 1);
	m_layout_TextureGroupTab_Info->addWidget(m_pushButton_TextureGroupTab_AddGroup, 1, 3, Qt::AlignCenter);
	m_layout_TextureGroupTab_Info->addWidget(m_pushButton_TextureGroupTab_DeleteGroup, 1, 4, Qt::AlignCenter);
	//   Viewer
	m_groupBox_TextureGroupTab_ViewGroup = new TextureGroupContents();
	connect(m_comboBox_TextureGroupTab_Info_TextureGroup, &QComboBox::currentIndexChanged, m_groupBox_TextureGroupTab_ViewGroup, &TextureGroupContents::setTextureGroupIndex);
	connect(m_comboBox_TextureGroupTab_Info_TextureGroup, &QComboBox::currentIndexChanged, this, &ProjectWidget::onChanged_ComboBox_TextureGroupTab_TextureGroup);
	m_layout_TextureGroupTab->addWidget(m_groupBox_TextureGroupTab_ViewGroup);

	// Tile set tab
	m_widget_TileSetTab = new QWidget();
	m_tabWidget_Main->addTab(m_widget_TileSetTab, "Tile Sets");

	// Initialize to blank page
	init();
}

ProjectWidget::~ProjectWidget() {
	delete m_layout_Main;
}

void ProjectWidget::init() {
	setProjectFilename("");
	AssetTreeModel* oldModel = getAssetModel();
	AssetTreeModel* newModel = new AssetTreeModel();
	setAssetModel(newModel);
	delete oldModel;
	
	m_comboBox_TextureGroupTab_Info_TextureGroup->setCurrentIndex(-1);
	m_groupBox_TextureGroupTab_ViewGroup->setTextureGroupIndex(-1);

	setDirty(false);
}

bool ProjectWidget::load(const QString& filename) {
	// Get project file
	try {
		QFile file(filename);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			throw std::exception("Could not open file for reading");
		}
		QDomDocument doc("arp");
		if (!doc.setContent(&file)) {
			throw std::exception("Could not get content from file");
		}
		file.close();

		// Verify contents
		QDomElement elemRoot = doc.documentElement();
		if (!versionStringMatch(elemRoot.attribute("version"), APOLLO_VERSION_STR)) {
			throw std::exception("Version string mismatch");
		}
		if (elemRoot.tagName() != "arp" || doc.doctype().name() != "arp") {
			throw std::exception("Document is not a valid arp filetype ");
		}

		// Get assets
		QStringList textureGroups;
		AssetTreeModel* model = new AssetTreeModel();
		QDomElement elemTop = elemRoot.firstChildElement();
		for (; !elemTop.isNull(); elemTop = elemTop.nextSiblingElement()) {
			QString tagName = elemTop.tagName();
			if (elemTop.tagName() == "compression") {

			}
			else if (elemTop.tagName() == "cipherMethod") {

			}
			else if (elemTop.tagName() == "cipherPassword") {

			}
			else if (elemTop.tagName() == "asset") {
				// Iterate through file tree
				typedef std::pair<QModelIndex, QDomElement> IndexElementPair;
				std::list<IndexElementPair> assetElementList;
				assetElementList.push_back(IndexElementPair::pair({}, elemTop));
				while (!assetElementList.empty()) {
					// Extract descriptor, index, and element nodes
					IndexElementPair pair = assetElementList.front();
					assetElementList.pop_front();
					QModelIndex indexCurrent = pair.first;
					QDomElement elemCurrent = pair.second;

					// Add asset to tree
					QModelIndex indexChild = {};
					if (elemCurrent.attribute("root", "false") == "false") {
						QString descriptorCurrent = elemCurrent.attribute("descriptor");
						int row = model->rowCount(indexCurrent);
						if (model->insertRow(row, indexCurrent)) {
							indexChild = model->index(row, 0, indexCurrent);
							model->setData(indexChild, descriptorCurrent, Qt::UserRole);
						}
						else {
							throw std::exception("Failed to add row to asset tree");
						}
					}

					// Iterate through children
					if (elemCurrent.attribute("group", "false") == "true") {
						QDomElement elemChild = elemCurrent.firstChildElement();
						for (; !elemChild.isNull(); elemChild = elemChild.nextSiblingElement()) {
							assetElementList.push_back(IndexElementPair::pair(indexChild, elemChild));
						}
					}
				}
			}
			else if (elemTop.tagName() == "textureGroups") {
				int count = elemTop.attribute("count", "0").toInt();
				textureGroups = elemTop.attribute("groups", "").split(g_assetDelim);
				if (count != textureGroups.size()) {
					throw std::exception("Invalid number of texture groups!");
				}
			}
			else {
				QString msg = QString("Invalid token '%1'").arg(elemTop.tagName());
				throw std::exception(msg.toStdString().c_str());
			}
		}

		// Assign retrieved values
		setProjectFilename(filename);
		AssetTreeModel* oldModel = getAssetModel();
		setAssetModel(model);
		delete oldModel;
		
		g_textureGroups.clear();
		g_textureGroups << g_defaultTextureGroupName;
		g_textureGroups << textureGroups;
		m_comboBox_TextureGroupTab_Info_TextureGroup->clear();
		m_comboBox_TextureGroupTab_Info_TextureGroup->addItems(g_textureGroups);
		m_comboBox_TextureGroupTab_Info_TextureGroup->setCurrentIndex(-1);
		m_groupBox_TextureGroupTab_ViewGroup->setTextureGroupIndex(-1);

		setDirty(false);

		return true;
	}
	catch (std::exception& e) {
		m_errorMessage = QString(e.what());
		return false;
	}
}

bool ProjectWidget::save(const QString& filename) {
	// Update name
	QString oldFilename = m_projectFilename;
	if (!filename.isEmpty() && filename != m_projectFilename) {
		setProjectFilename(filename);
	}

	// Create project file
	try {
		QFile file(m_projectFilename);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
			throw std::exception("Could not open file for writing!");
		}
		QDomDocument doc("arp");
		doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""));
		QDomElement elemRoot = addElement(doc, doc, "arp");
		elemRoot.setAttribute("version", APOLLO_VERSION_STR);
		addElement(doc, elemRoot, "compression", "false");
		addElement(doc, elemRoot, "cipherMethod", "none");
		addElement(doc, elemRoot, "cipherPassword", "");

		// Iterate through asset tree
		AssetTreeModel* model = getAssetModel();
		QModelIndex indexRoot = model->rootIndex();
		if (!indexRoot.isValid()) {
			throw std::exception("Invalid asset tree root!");
		}
		AssetTreeItem* assetRoot = static_cast<AssetTreeItem*>(indexRoot.internalPointer());
		typedef std::pair<AssetTreeItem*, QDomElement> AssetElementPair;
		std::list<AssetElementPair> assetElementList;
		assetElementList.push_back(AssetElementPair::pair(assetRoot, elemRoot));
		while (!assetElementList.empty()) {
			// Extract asset & element nodes
			AssetElementPair pair = assetElementList.front();
			assetElementList.pop_front();
			AssetTreeItem* assetCurrent = pair.first;
			QDomElement elemCurrent = pair.second;
			QString assetDescriptor = assetCurrent->assetDescriptor()->toString();

			// Add asset to document
			QDomElement elemChild = addElement(doc, elemCurrent, "asset");
			elemChild.setAttribute("descriptor", assetDescriptor);
			if (assetCurrent->assetDescriptor()->name() == g_rootAssetName) {
				elemChild.setAttribute("root", "true");
			}
			if (assetCurrent->assetType() == AssetType::Group) {
				elemChild.setAttribute("group", "true");
				for (int i = 0; i < assetCurrent->childCount(); ++i) {
					AssetTreeItem* assetChild = assetCurrent->child(i).get();
					assetElementList.push_back(AssetElementPair::pair(assetChild, elemChild));
				}
			}
		}

		// Save texture groups
		QStringList reducedTextureGroups = g_textureGroups;
		reducedTextureGroups.removeFirst();
		QString textureGroupString = reducedTextureGroups.join(g_assetDelim);
		QDomElement elemTextureGroups = addElement(doc, elemRoot, "textureGroups");
		elemTextureGroups.setAttribute("count", QString::number(reducedTextureGroups.size()));
		elemTextureGroups.setAttribute("groups", textureGroupString);

		// Close file
		file.write(doc.toByteArray());
		file.close();
		setDirty(false);
		return true;
	}
	catch (std::exception& e) {
		m_errorMessage = QString(e.what());
		setProjectFilename(oldFilename);
		return false;
	}
}

const AssetTreeView* ProjectWidget::getAssetTree() const {
	return m_treeView_AssetTab_Assets;
}

const AssetTreeModel* ProjectWidget::getAssetModel() const {
	return static_cast<AssetTreeModel*>(m_treeView_AssetTab_Assets->model());
}

QString ProjectWidget::projectFilename() {
	return m_projectFilename;
}

void ProjectWidget::setProjectFilename(const QString& filename) {
	m_projectFilename = filename;
	QFileInfo file(filename);
	QString label = (filename.isEmpty()) ? "<New Project>" : file.baseName();
	if (m_dirtyFlag) {
		label += "*";
	}
	m_label_Info_Name->setText(label);
	if (g_mainWindow) {
		g_mainWindow->setWindowTitle(QString("Apollo - %1").arg(label));
	}
}

QString ProjectWidget::getErrorMessage() const {
	return m_errorMessage;
}

bool ProjectWidget::dirty() const {
	return m_dirtyFlag;
}

void ProjectWidget::setDirty(bool dirtyFlag) {
	m_dirtyFlag = dirtyFlag;
	setProjectFilename(m_projectFilename);
}

QDomElement ProjectWidget::addElement(QDomDocument& doc, QDomNode& node, const QString& tag, const QString& value) {
	QDomElement elem = doc.createElement(tag);
	node.appendChild(elem);
	if (!value.isEmpty()) {
		QDomText txt = doc.createTextNode(value);
		elem.appendChild(txt);
	}
	return elem;
}

void ProjectWidget::setAssetModel(AssetTreeModel* model) {
	m_treeView_AssetTab_Assets->setModel(model);
	m_treeView_AssetTab_Assets->selectionModel()->setModel(model);
	connect(m_treeView_AssetTab_Assets->selectionModel(), &QItemSelectionModel::currentChanged, m_scrollArea_AssetTab_AssetEditor, &AssetEditor::onChanged_Index);
	m_scrollArea_AssetTab_AssetEditor->setIndex({});
}

void ProjectWidget::onClicked_PushButton_AssetTab_AddGroup() {
	// Get current group
	QModelIndex parent = m_treeView_AssetTab_Assets->selectionModel()->currentIndex();
	if (parent.isValid()) {
		// If the currently selected item isnt a group, get the group containing it
		AssetTreeItem* parentItem = static_cast<AssetTreeItem*>(parent.internalPointer());
		if (parentItem->assetType() != AssetType::Group) {
			parent = parent.parent();
		}
	}

	// Add child to that group
	AssetTreeModel* model = getAssetModel();
	int row = model->rowCount(parent);
	if (model->insertRow(row, parent)) {
		QModelIndex index = model->index(row, 0, parent);
		QString descriptor = buildAssetString({ "Group", "New Group" });
		model->setData(index, descriptor, Qt::UserRole);
		m_treeView_AssetTab_Assets->scrollTo(index);
		m_treeView_AssetTab_Assets->edit(index);
		m_treeView_AssetTab_Assets->setCurrentIndex(index);
	}
}

void ProjectWidget::onClicked_PushButton_AssetTab_EditGroup() {
	QModelIndex index = m_treeView_AssetTab_Assets->selectionModel()->currentIndex();
	if (index.isValid()) {
		m_treeView_AssetTab_Assets->scrollTo(index);
		m_treeView_AssetTab_Assets->edit(index);
	}
}

void ProjectWidget::onClicked_PushButton_AssetTab_DeleteGroup() {
	QModelIndex index = m_treeView_AssetTab_Assets->selectionModel()->currentIndex();
	if (index.isValid()) {
		// Ask for confirmation
		AssetTreeItem* item = static_cast<AssetTreeItem*>(index.internalPointer());
		if (item->assetType() != AssetType::Group) {
			return;
		}
		auto reply = QMessageBox::question(
			this, 
			"Delete Group", 
			QString("Delete group '%1'?").arg(item->data(0).toString()), 
			QMessageBox::Yes | QMessageBox::No
		);
		if (reply == QMessageBox::Yes) {
			AssetTreeModel* model = getAssetModel();
			model->removeRow(index.row(), index.parent());
		}
	}
}

void ProjectWidget::onClicked_PushButton_AssetTab_AddFile() {
	m_assetDialog = new AssetDialog(this);
	m_assetDialog->open();
	connect(m_assetDialog, &AssetDialog::finished, this, &ProjectWidget::onFinished_AssetDialog_AddFile);
}

void ProjectWidget::onClicked_PushButton_AssetTab_EditFile() {
	QModelIndex index = m_treeView_AssetTab_Assets->selectionModel()->currentIndex();
	if (index.isValid()) {
		AssetTreeItem* item = static_cast<AssetTreeItem*>(index.internalPointer());
		m_assetDialog = new AssetDialog(this);
		m_assetDialog->setValue(item->assetDescriptor());
		m_assetDialog->open();
		connect(m_assetDialog, &AssetDialog::finished, this, &ProjectWidget::onFinished_AssetDialog_EditFile);
	}
}

void ProjectWidget::onFinished_AssetDialog_AddFile(int status) {
	if (status == QDialog::Accepted && m_assetDialog) {
		// Check if file already exists
		AssetTreeModel* model = getAssetModel();
		AssetDescriptorPtr asset = m_assetDialog->value();
		if (model->getFile(asset->name()).isValid()) {
			QMessageBox::warning(this, "Warning", QString("An asset named '%1' already exists!").arg(asset->name()));
			m_assetDialog->open();
			return;
		}

		// Get current group
		QModelIndex parent = m_treeView_AssetTab_Assets->selectionModel()->currentIndex();
		if (parent.isValid()) {
			// If the currently selected item isnt a group, get the group containing it
			AssetTreeItem* parentItem = static_cast<AssetTreeItem*>(parent.internalPointer());
			if (parentItem->assetType() != AssetType::Group) {
				parent = parent.parent();
			}
		}

		// Add child to that group
		int row = model->rowCount(parent);
		if (model->insertRow(row, parent)) {
			QModelIndex index = model->index(row, 0, parent);
			model->setData(index, asset->toString(), Qt::UserRole);
			m_treeView_AssetTab_Assets->scrollTo(index);
			m_treeView_AssetTab_Assets->setCurrentIndex(index);
		}
	}
	delete m_assetDialog;
	m_assetDialog = nullptr;
}

void ProjectWidget::onFinished_AssetDialog_EditFile(int status) {
	if (status == QDialog::Accepted && m_assetDialog) {
		// Check if file already exists
		AssetTreeModel* model = getAssetModel();
		AssetDescriptorPtr asset = m_assetDialog->value();
		if (model->getFile(asset->name()).isValid()) {
			QMessageBox::warning(this, "Warning", QString("An asset named '%1' already exists!").arg(asset->name()));
			m_assetDialog->open();
			return;
		}

		// Get current asset & modify it
		QModelIndex index = m_treeView_AssetTab_Assets->selectionModel()->currentIndex();
		if (index.isValid()) {
			AssetTreeItem* item = static_cast<AssetTreeItem*>(index.internalPointer());
			item->setData(std::move(asset));
		}
	}
	delete m_assetDialog;
	m_assetDialog = nullptr;
}

void ProjectWidget::onClicked_PushButton_AssetTab_DeleteFile() {
	QModelIndex index = m_treeView_AssetTab_Assets->selectionModel()->currentIndex();
	if (index.isValid()) {
		// Ask for confirmation
		AssetTreeItem* item = static_cast<AssetTreeItem*>(index.internalPointer());
		if (item->assetType() == AssetType::Group) {
			return;
		}
		auto reply = QMessageBox::question(
			this,
			"Delete File",
			QString("Delete file '%1'?").arg(item->data(0).toString()),
			QMessageBox::Yes | QMessageBox::No
		);
		if (reply == QMessageBox::Yes) {
			AssetTreeModel* model = getAssetModel();
			model->removeRow(index.row(), index.parent());
		}
	}
}

void ProjectWidget::contextMenu_AssetTree(const QPoint& pos) {
	QMenu contextMenu(this);
	QAction* actionAddGroup = new QAction("Add Group");
	QAction* actionAddFile = new QAction("Add File");
	connect(actionAddGroup, &QAction::triggered, this, &ProjectWidget::onClicked_PushButton_AssetTab_AddGroup);
	connect(actionAddFile, &QAction::triggered, this, &ProjectWidget::onClicked_PushButton_AssetTab_AddFile);
	contextMenu.addAction(actionAddGroup);
	contextMenu.addAction(actionAddFile);

	QModelIndex index = m_treeView_AssetTab_Assets->indexAt(pos);
	if (index.isValid()) {
		AssetTreeItem* item = static_cast<AssetTreeItem*>(index.internalPointer());
		if (item->assetType() == AssetType::Group) {
			// Right clicked group
			QAction* actionEditGroup = new QAction("Edit Group");
			QAction* actionDeleteGroup = new QAction("Delete Group");
			connect(actionEditGroup, &QAction::triggered, this, &ProjectWidget::onClicked_PushButton_AssetTab_EditGroup);
			connect(actionDeleteGroup, &QAction::triggered, this, &ProjectWidget::onClicked_PushButton_AssetTab_DeleteGroup);
			contextMenu.addAction(actionEditGroup);
			contextMenu.addAction(actionDeleteGroup);
		}
		else {
			// Right clicked file
			QAction* actionEditFile = new QAction("Edit File");
			QAction* actionDeleteFile = new QAction("Delete File");
			connect(actionEditFile, &QAction::triggered, this, &ProjectWidget::onClicked_PushButton_AssetTab_EditFile);
			connect(actionDeleteFile, &QAction::triggered, this, &ProjectWidget::onClicked_PushButton_AssetTab_DeleteFile);
			contextMenu.addAction(actionEditFile);
			contextMenu.addAction(actionDeleteFile);
		}
	}
	contextMenu.exec(m_treeView_AssetTab_Assets->mapToGlobal(pos));
}

void ProjectWidget::onDataChanged() {
	setDirty(true);
}

void ProjectWidget::onChanged_ComboBox_TextureGroupTab_TextureGroup(int index) {
	m_pushButton_TextureGroupTab_DeleteGroup->setEnabled(index > 0);
}

void ProjectWidget::onClicked_PushButton_TextureGroupTab_AddGroup() {
	QString groupName = TextureGroupNameDialog::getTextureGroupName();
	if (!groupName.isEmpty()) {
		if (g_textureGroups.contains(groupName)) {
			QMessageBox::warning(this, "Warning", QString("A texture group named '%1' already exists!").arg(groupName));
		}
		else {
			g_textureGroups << groupName;
			m_comboBox_TextureGroupTab_Info_TextureGroup->clear();
			m_comboBox_TextureGroupTab_Info_TextureGroup->addItems(g_textureGroups);
			m_comboBox_TextureGroupTab_Info_TextureGroup->setCurrentIndex(g_textureGroups.size() - 1);
			setDirty(true);
		}
	}
}

void ProjectWidget::onClicked_PushButton_TextureGroupTab_DeleteGroup() {
	int index = m_comboBox_TextureGroupTab_Info_TextureGroup->currentIndex();
	if (index > 0) {
		// Ask for confirmation
		auto reply = QMessageBox::question(
			this,
			"Delete Texture Group",
			QString("Delete texture group '%1'?").arg(g_textureGroups[index]),
			QMessageBox::Yes | QMessageBox::No
		);
		if (reply == QMessageBox::Yes) {
			// Set all assets in this group back to default
			auto assetModel = g_mainWindow->getProjectWidget()->getAssetModel();
			QModelIndexList assetIndexes = assetModel->getFilesWithProperty({ AssetType::Texture }, 3, { QString::number(index) });
			for (auto& assetIndex : assetIndexes) {
				AssetTreeItem* item = static_cast<AssetTreeItem*>(assetIndex.internalPointer());
				AssetDescriptorTexture* texture = static_cast<AssetDescriptorTexture*>(item->assetDescriptor().get());
				texture->setTextureGroupIndex(0);
			}

			// Remove from list
			g_textureGroups.removeAt(index);
			m_comboBox_TextureGroupTab_Info_TextureGroup->removeItem(index);
			setDirty(true);
		}
	}
}