#include <signal.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <execinfo.h>
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>

//typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;
using traced = boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace>;
namespace bs = boost::stacktrace;

using namespace std;
//using namespace boost::stacktrace;

template <class E>
void throw_with_trace(const E &e)
{
    throw boost::enable_error_info(e)
        << traced(bs::stacktrace());
}

void bar(int i)
{
    //std::terminate();

    int *foo = (int *)-1;
    *foo = 0;

    if (i >= 4)
        throw_with_trace(std::out_of_range("'i' must be less than 4 in oops()"));
}

void foo(int i)
{
    bar(i);
}

void my_signal_handler(int sig)
{
    
    //stacktrace::safe_dump_to("./backtrace.dump");
    // ::raise(SIGABRT);

    auto st = bs::stacktrace();
    std::cerr << "signal : " << sig << "\n"
              << "stacktrace: \n" << st << endl;
    
    ::signal(sig, SIG_DFL);

    // exit(1);
}

void print_dump()
{
    if (filesystem::exists("./backtrace.dump"))
    {
        // there is a backtrace
        std::ifstream ifs("./backtrace.dump");

        bs::stacktrace st = bs::stacktrace::from_dump(ifs);
        std::cout << "Previous run crashed:\n"
                  << st << std::endl;
    }
}

void handler(int sig)
{
    void *array[32];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, sizeof(array));

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    //exit(1);
    ::signal(sig, SIG_DFL);
}

int main(int argc, char *argv[])
{
    try
    {
        print_dump();

        ::signal(SIGSEGV, my_signal_handler);
        ::signal(SIGABRT, my_signal_handler);

        foo(5); // testing assert handler

        //::raise(SIGABRT);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        const boost::stacktrace::stacktrace *st = boost::get_error_info<traced>(e);
        if (st)
        {
            std::cerr << *st << '\n';
        }
    }

    return 0;
}
