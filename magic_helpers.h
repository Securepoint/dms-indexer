#ifndef __SP_MAGIC_H
#define __SP_MAGIC_H 1

#ifdef __cplusplus
extern "C" {
#endif
/*
 * checks content with libmagic and returns an allocated mime string, which has to be freed by the caller
 * ex.:  "text/html"
 */
char *get_mimetype_from_buffer(const char *buf, long buflen);
char *get_mimetype_from_file(const char *filename);

#ifdef __cplusplus
}
#endif

#endif
