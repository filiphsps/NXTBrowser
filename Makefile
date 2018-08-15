all:
	make -f Makefile.switch all
	make -f Makefile.macos
#	make -f Makefile.windows

clean:
	make -f Makefile.switch clean
	make -f Makefile.macos clean
#	make -f Makefile.windows clean

windows:
	make -f Makefile.windows

macos:
	make -f Makefile.macos

switch:
	make -f Makefile.switch