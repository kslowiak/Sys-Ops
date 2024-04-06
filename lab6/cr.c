#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
creat("tst",S_IRUSR | S_IXUSR);
return 0;
}
