/*
 *①logが保存しているディレクトリ->notionに記録する
 *date : 2024/05/26
 *auther : Shunsuke.I
 */
#include "log2notion.hpp"

int main(int aArgc, char *aArgv[])
{
    if (!setOption(aArgc, aArgv))
        return EXIT_FAILURE;
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;


    try
    {
        setSigInt();

        std::vector<std::string> directories_name = list_files(path);

        for (const auto &directory : directories_name)
        {
            //printf("Main: Directory: %s\n", directory.c_str());
            /*            std::string head = directory.substr(0, 4);
                        std::string middle = directory.substr(5, 4);
                        std::cout << "先頭の4文字: " << head;
                        std::cout << "真ん中の4文字: " << middle << std::endl;
            */
            sendHTTPRequest(directory);
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
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    // データを送信する関数の定義
    size_t totalSize = size * nmemb;
    memcpy((char *)userp + strlen((char *)userp), (const char *)contents, totalSize);
    ((char *)userp)[totalSize + strlen((char *)userp)] = '\0'; // Null-terminate the string
    return totalSize;
}
void sendHTTPRequest(const std::string &directory)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    char response[2048] = {0}; // Initialize the response buffer
    std::string head = directory.substr(0, 4);
    std::string middle1 = directory.substr(5, 2);
    std::string middle2 = directory.substr(7, 2);
    std::string date_num = head + "-" + middle1 + "-" + middle2;
    // CURLライブラリの初期化
    curl_global_init(CURL_GLOBAL_DEFAULT);
    // 単一のHTTPリクエスト用のCURLハンドルを初期化
    curl = curl_easy_init(); // 戻り値はCURL*型ポインタ　正常に初期化＝有効なCURLハンドルを指す。　エラーが発生した場合＝NULLポインタが実行
    if (curl)
    {
        // Set the URL for creating a new page in the database
        char url[] = "https://api.notion.com/v1/pages";

        // Replace this with the actual property name from your Notion database
        const char *numberProperty = "numberProperty";

        // Create the JSON payload for the new page
        char data[1024];
        snprintf(data, sizeof(data),
                 "{\"parent\": {\"database_id\": \"%s\"},"
                 "\"properties\": {"
                 "\"Name\": {\"title\": [{\"text\": {\"content\": \"%s\"}}]},"
                 "\"date\": {\"date\": {\"start\": \"%s\",\"end\": null}},"
                 "\"log_or_memo\": {\"select\": {\"name\": \"log\", \"color\": \"blue\"}}"
                 "},"
                 "\"children\": ["
                 "{\"object\": \"block\","
                 "\"type\": \"heading_1\","
                 "\"heading_1\": {\"rich_text\": [{\"type\": \"text\", \"text\": {\"content\": \"RTK-GNSS\", \"link\": null}}],\"is_toggleable\": true}},"
                 "{\"object\": \"block\","
                 "\"type\": \"heading_1\","
                 "\"heading_1\": {\"rich_text\": [{\"type\": \"text\", \"text\": {\"content\": \"IMU\", \"link\": null}}],\"is_toggleable\": true}},"
                 "{\"object\": \"block\","
                 "\"type\": \"heading_1\","
                 "\"heading_1\": {\"rich_text\": [{\"type\": \"text\", \"text\": {\"content\": \"LOCALIZER\", \"link\": null}}],\"is_toggleable\": true}}"
                 "]"
                 "}",
                 DATABASE_ID, directory.c_str(), date_num.c_str());
        // char *dataに後方の文字列を生成する。サイズはsizeof(data)で決定する。ヌル終端文字が必ずバッファの末尾に追加される。

        // Set the headers
        // HTTPリクエストのヘッダーにheadersに後方の文字列を追加している。
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Notion-Version: 2022-06-28");
        char authHeader[128];
        snprintf(authHeader, sizeof(authHeader), "Authorization: Bearer %s", NOTION_API_TOKEN);
        headers = curl_slist_append(headers, authHeader);

        // Set curl options
        curl_easy_setopt(curl, CURLOPT_URL, url);                     // HTTPリクエストを送信するURL
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);          // HTTPリクエストのヘッダーを設定
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);             // HTTP POSTリクエストのデータを設定
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); // サーバーからのレスポンスを処理するために使用されるコールバック関数を設定
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);          // 指定したコールバック関数が取得するユーザーデータへのポインタを設定する

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else
        {
            printf("%s --> OK\n",directory.c_str());
            printf("Response from server: %s\n", response);
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    curl_global_cleanup();
}
