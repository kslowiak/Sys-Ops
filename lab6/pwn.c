#include <stdio.h>
#include <pwd.h>

int main(int argc, char *argv[])
{
struct passwd *pwd;
pwd=getpwnam(argv[1]);
if(pwd!=NULL){
  printf("user name: %s\n",pwd->pw_name);
  printf("home directory: %s\n",pwd->pw_dir);
  printf("shell: %s\n",pwd->pw_shell);
  printf("other information: %s\n",pwd->pw_gecos);
  printf("passwd: %s\n",pwd->pw_passwd);
  return 0;
}
else
  printf("No such user.\n");
  return 1;
}
