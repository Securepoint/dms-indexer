FILES = magic_helpers.c xapian-modes.cc helpers/call_external_cmd.c helpers/xapian-index-helper.cc indexer-cmdline.cc
LIBS = -lmagic -lxapian -lstdc++ -lGraphicsMagick++
CFLAGS += "-I/usr/include/GraphicsMagick/"

all: 
	$(CC) $(FILES) $(CFLAGS) $(LIBS) -o index-file
	$(CXX) -std=c++11 dmsb.cc xapian-search.cc helpers/base64.cc helpers/xapian-helpers.cc helpers/base32.cc -lxapian -o dms-cmdl
