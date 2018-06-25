#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int call_external_cmd(const char **argv, const char *stdout2file)
{
  int pid, status = 1, rc, killed = 0;
  unsigned int timeout = 10 * 60 * 1000 * 1000, timer = 0;

  if (0 > (pid = fork()))
    return pid;

  if (0 == pid) {
    /* born to serve */
    int outfd, outerr;
    if (0 < (outerr = open("/dev/null", O_WRONLY | O_NOCTTY))) {
      dup2(outerr, STDERR_FILENO);
    }
    if (stdout2file) {
      if (0 < (outfd = open(stdout2file, O_CREAT | O_WRONLY, 0666))) {
        dup2(outfd, STDOUT_FILENO);
      }
    }

    execv(argv[0], (char *const *) argv);
    _exit(-1);
  } else if (0 < pid) {
    do {
      rc = waitpid(pid, &status, WNOHANG);
      if (!WIFEXITED(status))
        usleep(2000);
      timer += 2000;
      if (timeout == timer) {
        killed = 99;
        rc = kill(pid, SIGTERM);
      } else if ((timeout + 1000000) < timer) {
        killed = 99;
        rc = kill(pid, SIGKILL);
      }
      if (rc && (WIFEXITED(status) || WIFSIGNALED(status)))
        break;
    } while (0 <= rc);
    if (killed) {
      return killed;
    } else if (WIFEXITED(status)) {
      return (WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
      return (WTERMSIG(status));
    }
  }
  return status;
}
