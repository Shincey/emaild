#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <sys/utsname.h>
#include <iostream>
#include "public/zsystem.h"
#include "public/ztools.h"




int main (int argc, char* argv[]) {
    struct utsname uts;
    if (uname(&uts) == -1) exit(EXIT_FAILURE);
    printf("Node name: %s \n", uts.nodename);
    printf("System name: %s \n", uts.sysname);
    printf("Release: %s \n", uts.release);
    printf("Version: %s \n", uts.version);
    printf("Machine: %s \n", uts.machine);
    #ifdef _GNU_SOURCE
        printf("Domain name: %s \n", uts.domainname);
    #endif
    printf("%s \n", "hello world");

    printf("%s \n", ztool::ztime::now().c_str());

    std::vector<std::string> res;
    ztool::split("hello,what,word", ",", res);
    for (auto r : res) std::cout << r << std::endl;

    exit(EXIT_SUCCESS);
}