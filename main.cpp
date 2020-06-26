#include <iostream>
#include <vector>

#include <sys/utsname.h>

#include "zsystem.h"
#include "ztools.h"
#include "zbase64.h"
#include "zpool.h"

#include "zinterface.h"

int main (int argc, char* argv[]) {
    // struct utsname uts;
    // if (uname(&uts) == -1) exit(EXIT_FAILURE);
    // printf("Node name: %s \n", uts.nodename);
    // printf("System name: %s \n", uts.sysname);
    // printf("Release: %s \n", uts.release);
    // printf("Version: %s \n", uts.version);
    // printf("Machine: %s \n", uts.machine);
    // #ifdef _GNU_SOURCE
    //     printf("Domain name: %s \n", uts.domainname);
    // #endif
    printf("time: %s \n", ztool::ztime::now().c_str());

    // std::string s = "你好，这里是中文";

    // std::string encoded = zlib::base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
    // std::string decoded = zlib::base64_decode(encoded);
    
    // printf("%s\n", encoded.c_str());

    // printf("%s\n", decoded.c_str());

    printf("pid: %d \n", ztool::zsystem::get_current_process_id());
    struct Person {
        char name[24];
        int age;
    };
    zlib::zpool<Person> _pool;
    std::vector<Person *> vec;

    for (int i = 0; i < 200; ++i) {
        Person *p = create_from_pool(_pool);
        vec.push_back(p);
    }

    for (int i = 199; i >= 0; --i) {
        Person *p = vec[i];
        recover_to_pool(_pool, p);
    }

    exit(EXIT_SUCCESS);
}