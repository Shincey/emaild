#include <iostream>
#include <vector>

#include <sys/utsname.h>

#include "zsystem.h"
#include "ztools.h"
#include "zbase64.h"
#include "zpool.h"

#include "zinterface.h"

#include "common.h"

#include "logger.h"
#include "zthread.h"


int main (int argc, const char **argv, const char **env) {
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
    printf("time: %s \n", tool::time::now().c_str());

    // std::string s = "你好，这里是中文";

    // std::string encoded = lib::base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
    // std::string decoded = lib::base64_decode(encoded);
    
    // printf("%s\n", encoded.c_str());

    // printf("%s\n", decoded.c_str());

    // printf("pid: %d \n", tool::sys::get_current_process_id());
    // struct Person {
    //     char name[24];
    //     int age;
    // };
    // lib::zpool<Person> _pool;
    // std::vector<Person *> vec;

    // for (int i = 0; i < 200; ++i) {
    //     Person *p = create_from_pool(_pool);
    //     vec.push_back(p);
    // }

    // for (int i = 199; i >= 0; --i) {
    //     Person *p = vec[i];
    //     recover_to_pool(_pool, p);
    // }

    class TCPSession : public zif:: iTCPSession {
    public:
        virtual int on_recv(zif::iCore *__core, const void *__content, const int __size) {
            char tmp[1024];
            snprintf(tmp, __size, "%s", (const char *)__content);
            printf("on_recv: %s\n", tmp);
            return __size;
        }
        virtual void on_connect(zif::iCore *__core) {
            printf("on_connect hello world\n");
        }
        virtual void on_disconnect(zif::iCore *__core) {
            printf("on_disconnect\n");
        }
        virtual void on_connect_failed(zif::iCore *__core) {
            printf("on_connect_failed\n");
        }
    };

    class TCPServer : public zif::iTCPServer {
    public:
        virtual zif::iTCPSession * on_malloc_connection(zif::iCore *__core, const char *__remote_ip, const s32 __remote_port) {
            return new TCPSession;
        }
        virtual void on_error(zif::iCore *__core, zif::iTCPSession *__session) {
            printf("on_error\n");
        }
        virtual void on_release(zif::iCore *__core) {
            printf("on_release\n");
        }
    };
    
    core::Core::instance()->parse_args(argc, argv);
    if (core::Core::instance()->get_args("pause")) {
        getchar();
    }

    const char *name = core::Core::instance()->get_args("name");
    if (name) { core::Core::instance()->set_core_name(name); }
    else { core::Core::instance()->set_core_name("test"); }

    core::Core::instance()->launch();

    zif::iTCPServer *tcpsvr = new TCPServer;
    core::Core::instance()->launch_tcp_server(tcpsvr, "127.0.0.1", 3200, 1024, 1024);

    while (true) {
        core::Core::instance()->loop();
    }
    
    exit(EXIT_SUCCESS);
}