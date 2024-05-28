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
#include <curl/curl.h> //HTTPリクエストを送信すsるためのCURLライブラリ
#include "ssm-log.hpp"
#include "gnss.hpp"

#define NOTION_API_TOKEN "???????????????????" // NotionAPIトークン
#define DATABASE_ID "???????????????"                        // データを追加する対象のデータベースID
#define NEWPAGE "test_page1"

static void ctrlC(int aStatus);
static void setSigInt();
static void Terminate(void);
static bool setOption(int aArgc, char *aArgv[]);
static int printShortHelp(const char *programName);
void sendHTTPRequest(const std::string &directory);
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
std::vector<std::string> list_files(const char *path);

static char path[128] = "/home/shun330/readdire_test";

static int gShutOff = 0;