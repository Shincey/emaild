#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <sys/utsname.h>
#include <iostream>
#include "public/zsystem.h"
#include "public/ztools.h"
#include "public/zbase64.h"




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

    std::string s = "你好哦噢好";


    // std::string encoded = zlib::base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
    // std::string decoded = zlib::base64_decode(decoded);
    
    // printf("%s\n", encoded.c_str());

    // printf("%s\n", decoded.c_str());

    ztool::zsystem::get_current_process_id();

    exit(EXIT_SUCCESS);
}