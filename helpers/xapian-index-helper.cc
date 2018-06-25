#include <sys/file.h>
#include <sys/stat.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <Magick++.h>
#include <xapian.h>
#include "../xapian-exports.h"
#include "call_external_cmd.h"
#include <unistd.h>

//#include <xls.h>

using namespace std;

int do_ocr(string infile, string outfile, string language = "deu")
{
  using namespace Magick;
  // dont trust graphicksmagick its able to kill IO
  alarm(360);
  try {
    InitializeMagick(NULL);

    Image image(infile);

    // compression level
    //image.quality(0);
    image.magick("png");

    image.backgroundColor("white");
    //image.fillColor("white");
    //image.monochrome(true);
    //image.equalize();
    //image.enhance();
    //image.sharpen(0.5, 1.5);

    //image.threshold(200);
    //image.blur(2, 0.4);
    //image.filterType(TriangleFilter);
    //image.zoom(Geometry("+250%"));

    //image.matte(true);
    image.trim();

    //image.sharpen(2.0, 3.0);
    //image.strokeAntiAlias(true);

    //image.antiAlias(true);
    image.normalize();

    image.write("ocrimage");
  } catch(Exception & error_) {
    cerr << "Caught exception: " << error_.what() << endl;
    alarm(0);
    return 1;
  }
  alarm(0);

  const char *argv[] =
    { "/usr/bin/tesseract", "ocrimage", outfile.c_str(), "-l",
    language.c_str(), "sp-dms", NULL
  };
  return (call_external_cmd(argv, NULL));
  /* DEBUG */
  /*call_external_cmd(argv, "");
   * ifstream file;
   * string buf = outfile;
   * buf.append(".txt");
   * file.open(buf.c_str());
   * cout << "filename:" << buf;
   * buf.clear();
   * int ch = file.get();
   * while (file.good()) {
   * cerr << (char) ch;
   * ch = file.get();
   * }
   * 
   * file.close();
   * return 0; */
  /* DEBUG END */
}

bool is_number(const std::string & s)
{
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it))
    ++it;
  return !s.empty() && it == s.end();
}
/*
int extract_xls(const char *filename, const char *outfilename)
{

  using namespace xls;

  int ret = 1, i;
  xlsWorkBook *pWB;
  xlsWorkSheet *pWS;
  WORD cellRow, cellCol;
  ofstream outfile(outfilename);

  if (!outfile.is_open())
    return 1;

  if ((pWB = xls_open(filename, "UTF-8"))) {
    // iterate over all worksheets 
    for (i = 0; i < pWB->sheets.count; i++) {
      //* open and parse the sheet 
      if ((pWS = xls_getWorkSheet(pWB, i))) {
        xls_parseWorkSheet(pWS);
        ret = 0;
        for (cellRow = 0; cellRow <= pWS->rows.lastrow; cellRow++) {
          for (cellCol = 0; cellCol <= pWS->rows.lastcol; cellCol++) {
            xlsCell *cell = xls_cell(pWS, cellRow, cellCol);

            if ((!cell) || (cell->isHidden))
              continue;

            if (cell->str) {
              outfile << cell->str;
            }
          }
        }
        outfile << endl;
        xls_close_WS(pWS);
      }
      outfile << endl;
    }
    xls_close(pWB);
  }
  outfile.close();
  return ret;
}*/
