#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    (void)argc;
    char app_dir[PATH_MAX];
    char executable[PATH_MAX];
    char library_path[PATH_MAX * 2];
    const char *slash = strrchr(argv[0], '/');
    size_t dir_length = slash ? (size_t)(slash - argv[0]) : 1;
    const char *base = slash ? argv[0] : ".";
    if (dir_length >= sizeof(app_dir)) return 1;
    memcpy(app_dir, base, dir_length);
    app_dir[dir_length] = '\0';

    if (snprintf(executable, sizeof(executable), "%s/supertux2", app_dir) >= (int)sizeof(executable) ||
        snprintf(library_path, sizeof(library_path), "%s:%s", app_dir,
                 getenv("LD_LIBRARY_PATH") ? getenv("LD_LIBRARY_PATH") : "") >= (int)sizeof(library_path) ||
        setenv("LD_LIBRARY_PATH", library_path, 1) != 0 ||
        setenv("SUPERTUX_WEBOS_REMOTE", "1", 1) != 0) {
        return 1;
    }

    char log_path[PATH_MAX];
    if (snprintf(log_path, sizeof(log_path), "%s/supertux.log", app_dir) < (int)sizeof(log_path)) {
        int log_fd = open(log_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (log_fd >= 0) {
            dup2(log_fd, STDOUT_FILENO);
            dup2(log_fd, STDERR_FILENO);
            close(log_fd);
        }
    }

    int output_arg = 1;
    for (int input_arg = 1; argv[input_arg]; ++input_arg) {
        if (argv[input_arg][0] == '{' &&
            strstr(argv[input_arg], "\"@system_native_app\"")) continue;
        argv[output_arg++] = argv[input_arg];
    }
    argv[output_arg] = NULL;
    argv[0] = executable;
    execv(executable, argv);
    perror("execv supertux2");
    return 1;
}
