#pragma once

#include <string>
#include <vector>

/// <summary>
/// A class that manages file operations.
/// </summary>

class File
{
public:
    static bool OpenFile(const char* path, std::string& out);
    static bool SaveFile(const char* path, const std::string& in);
    static bool FileExists(const char* path);
    static bool DirectoryExists(const char* path);
    static bool CreateDirectoryA(const char* path);
    static bool DeleteFile(const char* path);
    static bool DeleteDirectory(const char* path);
    static bool CopyFile(const char* source, const char* destination);
    static bool CopyDirectory(const char* source, const char* destination);
    static bool MoveFile(const char* source, const char* destination);
    static bool MoveDirectory(const char* source, const char* destination);
    static bool RenameFile(const char* source, const char* destination);

    static std::string GetFileName(const char* path);
    static std::string GetFileNameWithoutExtension(const char* path);
    static std::string GetFileExtension(const char* path);
    static std::string GetDirectory(const char* path);
    static std::string GetParentDirectory(const char* path);
    static std::string GetAbsolutePath(const char* path);
    static std::string GetRelativePath(const char* path);
    static std::string GetWorkingDirectory();
    static std::string GetTempDirectory();

    static std::vector<std::string> GetFilesInDirectory(const char* path);
    static std::vector<std::string> GetFilesInDirectory(const char* path, const char* extension);
    static std::vector<std::string> GetDirectoriesInDirectory(const char* path);

    static bool IsFile(const char* path);
    static bool IsDirectory(const char* path);
    static bool IsRoot(const char* path);
    static bool IsAbsolute(const char* path);
private:

};