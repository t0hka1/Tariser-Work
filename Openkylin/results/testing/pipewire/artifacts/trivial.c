#include <pipewire/pipewire.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
    pw_init(&argc, &argv);
    printf("Compiled with libpipewire %s\n"
           "Linked with libpipewire %s\n",
           pw_get_headers_version(),
           pw_get_library_version());
    return 0;
}
