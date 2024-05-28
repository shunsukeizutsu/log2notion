/*
 * 指定されたディレクトリ内のディレクトリ名やファイル名をスキャンするプログラム
 * 実行方法-->./test_2 {スキャンするディレクトリ名}
 * 
*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <vector>
#include <string>

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
            //printf("Directory: %s\n", fullpath);
            // Recursively list files in the sub-directory
            // list_files(fullpath);
        }
        else
        {
            //printf("File: %s\n", fullpath);
        }
    }
    closedir(dp);
    return directories_name;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    std::vector<std::string> directories_name = list_files(argv[1]);

    for (const auto& directory : directories_name) {
        printf("Main: Directory: %s\n", directory.c_str());
    }


    return 0;
}
