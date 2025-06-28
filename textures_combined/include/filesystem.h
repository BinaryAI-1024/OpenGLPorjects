#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#pragma warning(disable: 4996)
#include <windows.h>
#include <string>
#include <cstdlib>


class FileSystem
{
private:
    typedef std::string(*Builder) (const std::string& path);

public:
    static std::string getPath(const std::string& path)
    {
        static std::string(*pathBuilder)(std::string const&) = getPathBuilder();
        return (*pathBuilder)(path);
    }

private:
    static std::string const& getRoot()
    {
        static std::string root;
        if (root.empty())
        {
            // 获取当前项目路径
            char buffer[MAX_PATH];
            GetCurrentDirectoryA(MAX_PATH, buffer);
            root = buffer;

        }
        return root;
    }

    //static std::string(*foo (std::string const &)) getPathBuilder()
    static Builder getPathBuilder()
    {
        if (getRoot() != "")
            return &FileSystem::getPathRelativeRoot;
        else
            return &FileSystem::getPathRelativeBinary;
    }

    static std::string getPathRelativeRoot(const std::string& path)
    {
        return getRoot() + std::string("/") + path;
    }

    static std::string getPathRelativeBinary(const std::string& path)
    {
        return "../../../" + path;
    }


};

// FILESYSTEM_H
#endif