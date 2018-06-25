#include <stdlib.h>
#include <string.h>
#include <magic.h>

/* does what the name promises */
char *get_mimetype_from_buffer_or_file(const char *filename, const char *buf,
                                       size_t buflen)
{
  magic_t m = NULL;
  char *result = NULL;
  const char *tmp = NULL;

  if (!(m = magic_open(MAGIC_MIME_TYPE))) {
    return NULL;
  } else if (magic_load(m, NULL)) {
    /* */
  } else if (!filename && (!buf || (1 > buflen) || !(tmp = magic_buffer(m, buf, buflen)))) {
    /* */
  } else if (filename && !(tmp = magic_file(m, filename))) {
    /* */
  } else if (tmp) {
    result = strdup(tmp);
  }

  magic_close(m);

  return result;
}

char *get_mimetype_from_buffer(const char *buf, size_t buflen)
{
  return (get_mimetype_from_buffer_or_file(NULL, buf, buflen));
}

char *get_mimetype_from_file(const char *filename)
{
  return (get_mimetype_from_buffer_or_file(filename, NULL, 0));
}
