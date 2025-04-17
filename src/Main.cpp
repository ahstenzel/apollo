#include "MainWindow.hpp"
#include "AssetInfo.hpp"

MainWindow* g_mainWindow = nullptr;

int main(int argc, char* argv[]) {
	srand(time(NULL));
	QApplication app(argc, argv);
	populateInfoMap();
	MainWindow window;
	g_mainWindow = &window;
	window.show();
	return app.exec();
}