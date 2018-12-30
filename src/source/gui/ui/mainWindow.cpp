#include "gui/ui/mainWindow.h"

MainWindow::MainWindow() {
	window = uiNewWindow("NXTBrowser", 640, 480, 1);

    address_bar_entry = uiNewEntry();
    uiEntrySetText(address_bar_entry, "http://lite.cnn.io/en");

    address_bar_group = uiNewForm();
    uiFormSetPadded(address_bar_group, 0);
    uiFormAppend(address_bar_group,
		NULL,
		uiControl(address_bar_entry),
        0);
    uiWindowSetChild(window, uiControl(address_bar_group));

    uiControlShow(uiControl(window));
}

MainWindow::~MainWindow() {
	
}