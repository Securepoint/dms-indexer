#include "xapian-modes.h"
#include "mimetypes.h"
#include "helpers/call_external_cmd.h"

#include <dirent.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

#include <fstream>
#include <sstream>


using namespace std;
/*
 * Extracts the file if possible and returns the extracted filename
 */

const char *extract_document(char *fname_c, char *ext_c)
{

  const char *result = NULL;
  string extension = ext_c;
  string filename = fname_c;
  string temp;
  char *md5hash, *tmp = NULL;
  size_t lastpoint;

  int outfd, mimetmp = 0, forkreturn = 0;

    if (string::npos == (lastpoint = extension.find_last_of("/"))) {
      return (NULL);
    } else {
      extension = extension.substr(lastpoint + 1);
    }
  temp = mimemap[extension];
  if (temp.length())
      extension = temp;
  /* try to guess type by filename extension */
  if ("octet-stream" == extension || "zip" == extension) {
    if (string::npos == (lastpoint = filename.find_last_of("."))) {
      return (NULL);
    } else {
      extension = filename.substr(lastpoint + 1);
    }
  }

  /* check extension and try to convert */
  if ("pdf" == extension) {
    const char *argv[] = { (char *) "/usr/bin/pdftotext", "-enc", "UTF-8",
      "-nopgbrk", filename.c_str(), "pdfout", NULL
    };
    call_external_cmd(argv, NULL);
    struct stat filestat;
    /* if output file is nearly empty assume its an Image PDF-Document (maybe ocr content) */
    if (!(stat("pdfout", &filestat)) && (5 > filestat.st_size)) {
      unlink("pdfout");
      const char *transform[] =
        { "/usr/bin/pdftoppm", "-mono", "-r", "100", filename.c_str(),
        "pbmfile", NULL
      };
      if (0 == (forkreturn = call_external_cmd(transform, NULL))) {
        struct stat multistat;
        string multiname = "pbmfile-";
        int multiindex = 1;
        ostringstream multifilename;
        ofstream mergedoutput("pdfout.txt");

        multifilename << multiname << multiindex << ".pbm";
        while (!stat(multifilename.str().c_str(), &multistat)) {
          if (0 == do_ocr(multifilename.str(), "pdftmp", "deu")) {
            ifstream infile("pdftmp.txt");
            mergedoutput << infile.rdbuf() << endl;
            infile.close();
            result = "pdfout.txt";
          }
          multifilename.str("");
          multifilename << multiname << ++multiindex << ".pbm";
        }
        mergedoutput.close();
      }

    } else {
      result = "pdfout";
    }
  } else if ("txt" == extension || extension == "plain") {
    result = filename.c_str();
  } else if ("ps" == extension) {
    const char *argv[] =
      { (char *) "/usr/bin/ps2pdf", filename.c_str(), "tmp.pdf", NULL };
    if (0 == (forkreturn = call_external_cmd(argv, NULL))) {
      const char *argv2[] =
        { (char *) "/usr/bin/pdftotext", "-enc", "UTF-8", "-nopgbrk",
        "tmp.pdf", "psout", NULL
      };
      if (0 == (forkreturn = call_external_cmd(argv2, NULL))) {
        result = "psout";
      }
      unlink("tmp.pdf");
    }
  } else if ("docx" == extension) {
    const char *argv[] =
      { "/usr/bin/unzip", "-p", filename.c_str(), "word/document.xml",
      NULL
    };
    if (0 == (forkreturn = call_external_cmd(argv, "docxdocument.xml"))) {
        result = "docxdocument.xml";
    }
  } else if ("sxw" == extension || "odt" == extension || "ods" == extension
    || "odp" == extension) {
    /* staroffice /openoffice */
    const char *argv[] =
      { "/usr/bin/unzip", "-p", filename.c_str(), "content.xml", NULL };
    if (0 == (forkreturn = call_external_cmd(argv, "sxwdocument.xml"))) {
        result = "sxwdocument.xml";
      unlink("sxwdocument.xml");
    }
  } else if ("xlsx" == extension) {
    const char *argv[] =
      { "/usr/bin/unzip", "-p", filename.c_str(), "xl/sharedStrings.xml",
      NULL
    };
    if (0 == (forkreturn = call_external_cmd(argv, "xlsout.xml"))) {
        result = "xlsout.xml";
      unlink("xlsout.xml");
    }
  } else if ("pptx" == extension) {
    const char *argv[] = { "/usr/bin/unzip", "-p", filename.c_str(),
      "ppt/notesSlides/notesSlide*.xml", NULL
    };
    if (0 == (forkreturn = call_external_cmd(argv, "pptout.xml"))) {
        result = "pptout.xml";
      unlink("pptout.xml");
    }
  } else if ("abic" == extension) {
    const char *argv[] = { "/bin/gzip", "-dc", filename.c_str(), NULL };
    if (0 == (forkreturn = call_external_cmd(argv, "abiout.xml"))) {
        result = "abiout.xml";
      unlink("abiout.xml");
    }
  } else if ("rtf" == extension) {
    const char *argv[] =
      { "/usr/bin/unrtf", "-P", "/usr/lib/unrtf/", "--nopict", "--text",
      filename.c_str(), NULL
    };
    if (0 == (forkreturn = call_external_cmd(argv, "rtfout"))) {
      result = "rtfout";
    }
  } else if ("doc" == extension) {
    const char *argv[] =
      { "/usr/bin/antiword", "-mUTF-8.txt", filename.c_str(), NULL };
    if (0 == (forkreturn = call_external_cmd(argv, "dumpeddoc"))) {
      result = "dumpeddoc";
    }
  } else if ("xls" == extension) {
    const char *argv[] =
      { "/usr/bin/binaryextract.sh", filename.c_str(), "xlsout", NULL };
    if (0 == (forkreturn = call_external_cmd(argv, NULL))) {
      result = "xlsout";
    }
  } else if ("key" == extension || "numbers" == extension
    || "pages" == extension) {
    const char *indexfile;
    if ("key" == extension) {
      indexfile = "index.apxl";
    } else
      indexfile = "index.xml";

    const char *argv[] =
      { "/usr/bin/unzip", "-p", filename.c_str(), indexfile, NULL };
    if (0 == (forkreturn = call_external_cmd(argv, "appledocument.xml"))) {
        result = "appledocument.xml";
      unlink("appledocument.xml");
    }

  } else if ("tif" == extension || "tiff" == extension) {
    if (0 == do_ocr(filename, "ocrout", "deu")) {
      result = "ocrout.txt";
    }
  } else {
    /* dont care about that extension */
    return NULL;
  }
  return (result);
}

