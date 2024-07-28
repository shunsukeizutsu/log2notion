#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <getopt.h>

using namespace std;
static void ctrlC(int aStatus);
static void setSigInt();
static void Terminate(void);
static bool setOption(int aArgc, char *aArgv[]);
static void printShortHelp(const char *programName);
vector<string> list_files(const char *path);

static char path[256] = "/home/haselab17/log2notion/log";

static int gShutOff = 0;

int main(int aArgc, char *aArgv[])
{
    if (!setOption(aArgc, aArgv))
        return EXIT_FAILURE;

    try
    {
        setSigInt();
        vector<string> directories_name = list_files(path);

        for(int i=0;i<directories_name.size();i++)
        {
            printf("%s\n",directories_name[i].c_str());
        }
    }
    catch (std::runtime_error const &error)
    {
        cerr << error.what() << endl;
    }
    catch (...)
    {
        cerr << "An unknown fatal error has occurred. Aborting" << endl;
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
static void printShortHelp(const char *programName)
{
    fputs("HELP\n", stderr);
    fprintf(stderr, "\t$ %s -p %s\n", programName, path);
    fputs("OPTION\n", stderr);
    printf("\t-p | --path     PATH     : Set path for log file (default=%s)\n", path);
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
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") ==0)
                continue;
            directories_name.push_back(entry->d_name);
        }
    }
    closedir(dp);
    return directories_name;
}