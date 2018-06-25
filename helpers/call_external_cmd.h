#ifndef __CALL_EXTERNAL_CMD_H
#define __CALL_EXTERNAL_CMD_H
/* calls command specified in argv
 *
 *  if stdout2file is given,
 *  stdout is suprisingly written to that file
 *
 *
 */

#ifdef  __cplusplus
extern "C" {
#endif

  int call_external_cmd(const char **argv, const char *stdout2file);

#ifdef  __cplusplus
}
#endif
#endif
