CXX=${CXX:-"g++ -std=c++11"}
$CXX -g3 -Wall -Wextra dmsb.cc  xapian-search.cc helpers/base64.cc helpers/xapian-helpers.cc helpers/base32.cc -lxapian -o dms-cmdl
