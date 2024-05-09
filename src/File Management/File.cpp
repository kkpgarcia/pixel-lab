#include <filesystem>
#include <fstream>
#include "File.h"

bool File::OpenFile(const char *path, std::string &out) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    out = buffer.str();

    return true;
}

bool File::IsAbsolute(const char *path) {
    return std::filesystem::path(path).is_absolute();
}

bool File::IsRoot(const char *path) {
    std::filesystem::path p(path);
    return p == p.root_path();
}

bool File::IsDirectory(const char *path) {
    return std::filesystem::is_directory(path);
}

bool File::IsFile(const char *path) {
    return std::filesystem::is_regular_file(path);
}

std::vector<std::string> File::GetDirectoriesInDirectory(const char *path) {
    std::vector<std::string> directories;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_directory()) {
            directories.push_back(entry.path().filename().string());
        }
    }
    return directories;
}

std::vector<std::string> File::GetFilesInDirectory(const char *path, const char *extension) {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().filename().string());
        }
    }
    return files;
}

std::vector<std::string> File::GetFilesInDirectory(const char *path) {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().filename().string());
        }
    }
    return files;
}

std::string File::GetTempDirectory() {
    return std::filesystem::temp_directory_path().string();
}

std::string File::GetWorkingDirectory() {
    return std::filesystem::current_path().string();
}

std::string File::GetRelativePath(const char *path) {
    return std::filesystem::relative(path).string();
}

std::string File::GetAbsolutePath(const char *path) {
    return std::filesystem::absolute(path).string();
}

std::string File::GetParentDirectory(const char *path) {
    return std::filesystem::path(path).parent_path().string();
}

std::string File::GetDirectory(const char *path) {
    return std::filesystem::path(path).parent_path().string();
}

std::string File::GetFileExtension(const char *path) {
    return std::filesystem::path(path).extension().string();
}

std::string File::GetFileNameWithoutExtension(const char *path) {
    return std::filesystem::path(path).stem().string();
}

std::string File::GetFileName(const char *path) {
    return std::filesystem::path(path).filename().string();
}

bool File::MoveDirectory(const char *source, const char *destination) {
    try {
        std::filesystem::rename(source, destination);
        return true;
    } catch (std::filesystem::filesystem_error& e) {
        return false;
    }
}

bool File::MoveFile(const char *source, const char *destination) {
    try {
        std::filesystem::rename(source, destination);
        return true;
    } catch (std::filesystem::filesystem_error& e) {
        return false;
    }
}

bool File::CopyDirectory(const char *source, const char *destination) {
    std::filesystem::copy(source, destination, std::filesystem::copy_options::recursive);
    return true;
}

bool File::CopyFile(const char *source, const char *destination) {
    return std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing);
}

bool File::DeleteDirectory(const char *path) {
    return std::filesystem::remove_all(path) > 0;
}

bool File::DeleteFile(const char *path) {
    return std::filesystem::remove(path);
}

bool File::CreateDirectoryA(const char *path) {
    return std::filesystem::create_directory(path);
}

bool File::DirectoryExists(const char *path) {
    return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

bool File::FileExists(const char *path) {
    return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
}

bool File::SaveFile(const char *path, const std::string &in) {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    file << in;
    return true;
}

bool File::RenameFile(const char *source, const char *destination) {
    try {
        std::filesystem::rename(source, destination);
        return true;
    } catch (std::filesystem::filesystem_error& e) {
        return false;
    }
}
