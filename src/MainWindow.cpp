#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent) {

	// Set window properties
	setWindowTitle("Apollo - <New Project>");
	setWindowIcon(QIcon(":/images/apollo.ico"));

	// Main container
	m_widget_Main = new QWidget();
	m_layout_Main = new QVBoxLayout();
	m_widget_Main->setLayout(m_layout_Main);
	setCentralWidget(m_widget_Main);

	// Menu bar
	m_menu_File = menuBar()->addMenu("&File");
	QAction* Action_File_NewProject = new QAction("&New Project");
	QAction* Action_File_OpenProject = new QAction("&Open Project");
	QAction* Action_File_SaveProject = new QAction("&Save Project");
	QAction* Action_File_SaveAsProject = new QAction("Save Project As");
	QAction* Action_File_ExportProject = new QAction("&Export Resource File");
	Action_File_NewProject->setShortcut(Qt::Key_N | Qt::CTRL);
	Action_File_OpenProject->setShortcut(Qt::Key_O | Qt::CTRL);
	Action_File_SaveProject->setShortcut(Qt::Key_S | Qt::CTRL);
	Action_File_SaveAsProject->setShortcut(Qt::Key_S | Qt::SHIFT | Qt::CTRL);
	Action_File_ExportProject->setShortcut(Qt::Key_E | Qt::CTRL);
	connect(Action_File_NewProject, &QAction::triggered, this, &MainWindow::onMenu_File_NewProject);
	connect(Action_File_OpenProject, &QAction::triggered, this, &MainWindow::onMenu_File_OpenProject);
	connect(Action_File_SaveProject, &QAction::triggered, this, &MainWindow::onMenu_File_SaveProject);
	connect(Action_File_SaveAsProject, &QAction::triggered, this, &MainWindow::onMenu_File_SaveAsProject);
	connect(Action_File_ExportProject, &QAction::triggered, this, &MainWindow::onMenu_File_ExportProject);
	m_menu_File->addAction(Action_File_NewProject);
	m_menu_File->addAction(Action_File_OpenProject);
	m_menu_File->addAction(Action_File_SaveProject);
	m_menu_File->addAction(Action_File_SaveAsProject);
	m_menu_File->addSeparator();
	m_menu_File->addAction(Action_File_ExportProject);
	m_menu_Help = menuBar()->addMenu("&Help");
	QAction* Action_Help_About = new QAction("About");
	connect(Action_Help_About, &QAction::triggered, this, &MainWindow::onMenu_Help_About);
	m_menu_Help->addAction(Action_Help_About);

	// Project widget
	m_projectWidget_Main = new ProjectWidget(this);
	m_layout_Main->addWidget(m_projectWidget_Main);

	// Info bar
	m_label_Info = new QLabel();
	m_layout_Main->addWidget(m_label_Info);
}

MainWindow::~MainWindow() {
	delete m_layout_Main;
}

const ProjectWidget* MainWindow::getProjectWidget() const {
	return m_projectWidget_Main;
}

void MainWindow::onMenu_File_NewProject() {
	// Check if a project is loaded
	QString filename = m_projectWidget_Main->projectFilename();
	if (!filename.isEmpty()) {
		// Ask for confirmation
		auto reply = QMessageBox::question(
			this,
			"New Project",
			"Create a new project?",
			QMessageBox::Yes | QMessageBox::No
		);
		if (reply == QMessageBox::Yes) {
			// Check if the user wants to save the current project
			auto reply = confirmSave();
			if (reply == QMessageBox::Cancel) {
				return;
			}
			m_projectWidget_Main->init();
		}
	}
}

void MainWindow::onMenu_File_OpenProject() {
	// Check if the user wants to save the current project
	auto reply = confirmSave();
	if (reply == QMessageBox::Cancel) {
		return;
	}

	// Open a new project
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Project File"), ".", "*.arp");
	if (filename.isEmpty()) {
		return;
	}
	if (!m_projectWidget_Main->load(filename)) {
		QMessageBox::critical(this, "Error", QString("Failed to load project file!\n%1").arg(m_projectWidget_Main->getErrorMessage()));
	}
}

void MainWindow::onMenu_File_SaveProject() {
	m_saveFlag = false;
	QString filename = m_projectWidget_Main->projectFilename();
	if (filename.isEmpty()) {
		filename = QFileDialog::getSaveFileName(this, tr("Save Project File"), ".", "*.arp");
		if (filename.isEmpty()) {
			return;
		}
	}
	if (!m_projectWidget_Main->save(filename)) {
		QMessageBox::critical(this, "Error", QString("Failed to save project file!\n%1").arg(m_projectWidget_Main->getErrorMessage()));
	}
	m_saveFlag = true;
}

void MainWindow::onMenu_File_SaveAsProject() {
	m_saveFlag = false;
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Project File As"), ".", "*.arp");
	if (filename.isEmpty()) {
		return;
	}
	if (!m_projectWidget_Main->save(filename)) {
		QMessageBox::critical(this, "Error", QString("Failed to save project file!\n%1").arg(m_projectWidget_Main->getErrorMessage()));
	}
	m_saveFlag = true;
}

void MainWindow::onMenu_File_ExportProject() {
	
}

void MainWindow::onMenu_Help_About() {
	QMessageBox::information(this, "About", QString("APOLLO\nVersion (%1)").arg(APOLLO_VERSION_STR));
}

QMessageBox::StandardButton MainWindow::confirmSave() {
	QMessageBox::StandardButton reply = QMessageBox::Yes;
	if (m_projectWidget_Main->dirty()) {
		reply = QMessageBox::question(
			this,
			"Save Project File",
			"Do you want to save the current project?",
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
		);
		if (reply == QMessageBox::Yes) {
			onMenu_File_SaveProject();
			if (!m_saveFlag) {
				// User was given save dialog but cancelled out
				reply = QMessageBox::Cancel;
			}
		}
	}
	return reply;
}