#ifndef _XAPIAN_MODES_H_
#define _XAPIAN_MODES_H_
#include <string>
const char *extract_document(char *f, char *e);
int do_ocr(std::string infile, std::string outfile, std::string language);
#endif /* _XAPIAN_MODES_H_ */
