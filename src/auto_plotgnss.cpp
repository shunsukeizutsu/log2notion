#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include "gnss.hpp"
#include "creatfig.hpp"

#define STRLEN 256

using namespace std;
static void ctrlC(int aStatus);
static void setSigInt();
static void Terminate(void);
static bool setOption(int aArgc, char *aArgv[]);
static int printShortHelp(const char *programName);
std::vector<std::string> list_files(const char *path);

static char program_path[ STRLEN/5 ] = "/home/haselab17/script_log2notion";
static char path[STRLEN/4] = "/home/haselab17/readdire_test";
static char log_name[STRLEN / 4] = "rtk_gnss.log";
static char save_filename[STRLEN / 2] = "rtk_gnss";


static int gShutOff = 0;

int main(int aArgc, char *aArgv[])
{
    if (!setOption(aArgc, aArgv))
        return EXIT_FAILURE;

    try
    {
        setSigInt();

        std::vector<std::string> directories_name = list_files(path);

        for (const auto &directory : directories_name)
        {
            char fname[STRLEN];
            sprintf(fname,"/home/izutsu330/log2notion/script/plot_gnss.sh %s/%s",path,directory.c_str());
            int ret = system(fname);
        }
    }
    catch (std::runtime_error const &error)
    {
        std::cout << error.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "An unknown fatal eeror has occured. Aborting." << std::endl;
    }

    Terminate();
    return EXIT_SUCCESS;
}
static void ctrlC(int aStatus)
{
    signal(SIGINT, NULL);
    gShutOff = 1;
}
static void setSigInt()
{
    struct sigaction sig;
    memset(&sig, 0, sizeof(sig));
    sig.sa_handler = ctrlC;
    sigaction(SIGINT, &sig, NULL);
}
static bool setOption(int aArgc, char *aArgv[])
{
    int opt, optIndex = 0;
    struct option longOpt[] = {
        {"log_path", 1, 0, 'p'},
        {"help", 0, 0, 'h'},
        {0, 0, 0, 0}};

    while ((opt = getopt_long(aArgc, aArgv, "p:h", longOpt, &optIndex)) != -1)
    {
        switch (opt)
        {
        case 'p':
            sprintf(path, "%s", optarg);
            break;
        case 'h':
            printShortHelp(aArgv[0]);
            return false;
            break;
        default:
            fprintf(stderr, "help : %s -h\n", aArgv[0]);
            return false;
            break;
        }
    }
    return true;
}
static int printShortHelp(const char *programName)
{
    fputs("HELP\n", stderr);
    fprintf(stderr, "\t$ %s -p %s\n", programName, path);
    fputs("OPTION\n", stderr);
    printf("\t-p | --path     PATH     : Set path for log file (default=%s)\n", path);
    return EXIT_SUCCESS;
}
static void Terminate(void)
{
    printf("\nend\n");
}
vector<string> list_files(const char *path)
{
    vector<string> directories_name;
    struct dirent *entry;

    DIR *dp = opendir(path);

    if (dp == NULL)
    {
        fprintf(stderr, "Error! Cannot open %s\n", path);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("open path\n");
    }
    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            directories_name.push_back(entry->d_name);
        }
    }
    closedir(dp);
    return directories_name;
}