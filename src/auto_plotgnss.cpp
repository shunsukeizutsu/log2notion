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
//#include "ssm-log.hpp"
#include "gnss.hpp"
#include "creatfig.hpp"

#define STRLEN 256

static void ctrlC(int aStatus);
static void setSigInt();
static void Terminate(void);
static bool setOption(int aArgc, char *aArgv[]);
static int printShortHelp(const char *programName);
std::vector<std::string> list_files(const char *path);

static char program_path[ STRLEN/5 ] = "/home/shun330/script_log2notion";
static char path[STRLEN/4] = "/home/shun330/readdire_test";
static char log_name[STRLEN / 4] = "rtk_gnss.log";
static char save_filename[STRLEN / 2] = "rtk_gnss";


static char xlabel0[STRLEN / 4] = "X [m]";
static char ylabel0[STRLEN / 4] = "Y [m]";
static char zlabel0[STRLEN / 4] = "Z [m]";

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
            sprintf(fname,"/home/shun330/log2notion/script/plot_gnss.sh %s/%s",path,directory.c_str());
            int ret = system(fname);
            //printf("Main: Directory: %s\n", fname);
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
std::vector<std::string> list_files(const char *path)
{
    std::vector<std::string> directories_name;
    // 指定されたディレクトリ内のファイル名とディレクトリ名を取得
    struct dirent *entry;
    struct stat statbuf;
    DIR *dp = opendir(path); // 指定されたディレクトリを開く

    if (dp == NULL)
    {
        perror("opendir");
        return directories_name;
    }

    while ((entry = readdir(dp)) != NULL)
    { // 指定されたディレクトリ内のエントリを順番に読み取る
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        if (stat(fullpath, &statbuf) == -1)
        {
            // ファイルまたはディレクトリであるかを判別する
            perror("stat");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode))
        {
            // Ignore "." and ".." directories（隠しファイルは無視する）
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            directories_name.push_back(entry->d_name);
            // printf("Directory: %s\n", fullpath);
            //  Recursively list files in the sub-directory
            //  list_files(fullpath);
        }
        else
        {
            // printf("File: %s\n", fullpath);
        }
    }
    closedir(dp);
    return directories_name;
}