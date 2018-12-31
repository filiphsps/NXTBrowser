#include "gui/ui/mainWindow.h"

MainWindow::MainWindow() {
	window = uiNewWindow("NXTBrowser", 640, 480, 1);

    uiBox *container = uiNewVerticalBox();

    // AddressBar
    address_bar_entry = uiNewEntry();
    uiEntrySetText(address_bar_entry, "http://lite.cnn.io/en");

    address_bar_group = uiNewForm();
    uiFormSetPadded(address_bar_group, 0);
    uiFormAppend(address_bar_group,
		NULL,
		uiControl(address_bar_entry),
        0);
    
    uiBoxAppend(container,
        uiControl(address_bar_group),
        0);

    // BrowserArea
    browser_area_handler->Draw = BrowserAreaOnDraw;
    browser_area = uiNewScrollingArea(browser_area_handler, 640, 1000);

    uiBoxAppend(container,
        uiControl(browser_area),
        0);

    uiWindowSetChild(window, uiControl(container));
    uiControlShow(uiControl(window));
}

MainWindow::~MainWindow() {
	
}

void MainWindow::BrowserAreaOnDraw(uiAreaHandler* handler, uiArea* area, uiAreaDrawParams* params) {
    //TODO
}