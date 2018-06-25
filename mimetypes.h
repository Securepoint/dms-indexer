#ifndef __MIMETYPES_H
#define __MIMETYPES_H
#include <map>
#include <string>

typedef std::map < std::string, std::string > MimeMap;

const MimeMap::value_type rawData[] = {
  MimeMap::value_type("msword", "doc"),
  MimeMap::value_type
    ("vnd.openxmlformats-officedocument.wordprocessingml.document",
    "docx"),
  MimeMap::value_type
    ("vnd.openxmlformats-officedocument.wordprocessingml.template",
    "docx"),
  MimeMap::value_type("vnd.ms-excel", "xls"),
  MimeMap::value_type("vnd.openxmlformats-officedocument.spreadsheetml.sheet",
    "xlsx"),
  MimeMap::value_type
    ("vnd.openxmlformats-officedocument.spreadsheetml.template",
    "xlsx"),
  MimeMap::value_type("vnd.ms-excel.sheet.macroEnabled.12", "xlsx"),
  MimeMap::value_type("vnd.ms-excel.template.macroEnabled.12", "xlsx"),
  MimeMap::value_type("vnd.ms-powerpoint", "ppt"),
  MimeMap::value_type
    ("vnd.openxmlformats-officedocument.presentationml.presentation", "pptx"),
  MimeMap::value_type
    ("vnd.openxmlformats-officedocument.presentationml.template",
    "pptx"),
  MimeMap::value_type
    ("vnd.openxmlformats-officedocument.presentationml.slideshow",
    "pptx"),
  MimeMap::value_type("x-ms-bmp", "bmp"),
  MimeMap::value_type("vnd.sun.xml", "sxw"),
  MimeMap::value_type("vnd.sun.xml.writer", "sxw"),
  MimeMap::value_type("vnd.oasis.opendocument", "sxw"),
  MimeMap::value_type("vnd.oasis.opendocument.text", "odt"),
  MimeMap::value_type("vnd.oasis.opendocument.spreadsheet", "ods"),
  MimeMap::value_type("vnd.oasis.opendocument.presentation", "odp"),
  MimeMap::value_type("vnd.oasis.opendocument.graphics", "odg"),
  MimeMap::value_type("x-abiword-compressed", "abic"),
  MimeMap::value_type("x-abiword", "abi"),
  MimeMap::value_type("postscript", "ps"),
  /* apple special
   * numbers = excel
   * keynote = powerpoint
   * pages = word */
  MimeMap::value_type("vnd.apple.keynote", "key"),
  MimeMap::value_type("vnd.apple.numbers", "numbers"),
  MimeMap::value_type("vnd.apple.pages", "pages"),
};

const int numElems = sizeof rawData / sizeof rawData[0];
static MimeMap mimemap(rawData, rawData + numElems);

#endif
