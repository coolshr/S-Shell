#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <fcntl.h>           
#include <sys/stat.h>
#include <dirent.h>
#include <wordexp.h>
#include <pwd.h>
#include <grp.h>
#include<time.h>
#include <termios.h>
#include <ctype.h>
#include<sys/wait.h>

#include "utilities.h"
#include "cd.h"
#include "ls.h"
#include "pwd.h"
#include "echo.h"
#include "pinfo.h"
#include"history.h"