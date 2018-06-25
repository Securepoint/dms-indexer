#include "magic_helpers.h"
#include "xapian-modes.h"
#include "xapian-exports.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <xapian.h>
#include <fstream>
#include <string>


Xapian::WritableDatabase * xdb;
Xapian::Document xdoc;

void usage(){
    const char *msg = "we need 2 parameters:\n   -d PATHTOXAPIANDB -f FILETOINDEX\n";

    write(2, msg, strlen(msg));
    exit(1);
}


int index_file(const char *filename){

  Xapian::TermGenerator indexer;
  time_t now;
  struct tm *tm;
  char date[128];

  std::ifstream infile;
  std::string filecontent;

  infile.open(filename);

  if (infile.is_open()) {
    filecontent.assign( (std::istreambuf_iterator<char>(infile) ), (std::istreambuf_iterator<char>()    ) );


    Xapian::Stem st_de("german");

    indexer.set_stemmer(st_de);
    indexer.set_document(xdoc);

    now = time(NULL);
    tm = localtime(&now);
    strftime(date, sizeof(date), "%Y%m%d", tm);
    xdoc.add_value(SLOT_ID_DATE, date);

    indexer.set_database(*xdb);
    xdoc.set_data(std::string(filecontent).substr(0, 2000));
    indexer.index_text(filecontent);
    xdb->add_document(xdoc);
    xdb->commit();
    return (0);
  }
  return (1);
}
  
int main (int argc, char **argv){

  char *dbpath = NULL;
  char *filetoindex = NULL;
  char *mimetype = NULL;
  const char *extractedfile;
  int c;


  while ((c = getopt(argc, argv, "d:f:")) != -1) {
    switch (c) {
     case 'd':
         dbpath = optarg;
         break;
     case 'f':
         filetoindex = optarg;
         break;
     default:
         usage();
    }
  }
  if (!dbpath || !filetoindex)
      usage();

  mimetype = get_mimetype_from_file(filetoindex);

  xdb = new Xapian::WritableDatabase(dbpath, Xapian::DB_CREATE_OR_OPEN);

  extractedfile = extract_document(filetoindex, mimetype);

  xdoc.add_value(SLOT_ID_MIMETYPE, mimetype);
  xdoc.add_value(SLOT_ID_DOCNAME, filetoindex);

  return (index_file(extractedfile));
}
