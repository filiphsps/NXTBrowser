#CPP_VERSION := -std=c++17
#CPP_COMPILER_FLAGS := -Wno-missing-field-initializers -Wno-implicit-fallthroug

all:
	make -f Makefile.switch all
	make -f Makefile.macos
#	make -f Makefile.win32

clean:
	make -f Makefile.switch clean
	make -f Makefile.macos clean
#	make -f Makefile.win32 clean

macos:
	make -f Makefile.macos

switch:
	make -f Makefile.switch

win32:
	make -f Makefile.win32