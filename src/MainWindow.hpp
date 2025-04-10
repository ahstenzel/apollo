#pragma once
/**
 * mainwindow.hpp
 *
 * Main window class.
 */
#include "Common.hpp"
#include "ProjectWidget.hpp"

/// <summary>
/// Widget for application window.
/// </summary>
class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	const ProjectWidget* getProjectWidget() const;

public slots:
	void onMenu_File_NewProject();
	void onMenu_File_OpenProject();
	void onMenu_File_SaveProject();
	void onMenu_File_SaveAsProject();
	void onMenu_File_ExportProject();
	void onMenu_Help_About();

private:
	bool m_saveFlag = false;
	QMessageBox::StandardButton confirmSave();

	QMenu* m_menu_File;
	QMenu* m_menu_Help;
	QWidget* m_widget_Main;
	QVBoxLayout* m_layout_Main;

	ProjectWidget* m_projectWidget_Main;
	QLabel* m_label_Info;
};

extern MainWindow* g_mainWindow;
