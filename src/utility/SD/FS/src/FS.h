/*
 FS.h - file system wrapper
 Copyright (c) 2015 Ivan Grokhotkov. All rights reserved.
 This file is part of the esp8266 core for Arduino environment.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef FS_H
#define FS_H

#include <memory>
#include <Arduino.h>

namespace fs
{

// #define "r"       "r"
// #define FILE_WRITE      "w"
// #define FILE_APPEND     "a"

class SDAudioFile;

class FileImpl;
typedef std::shared_ptr<FileImpl> FileImplPtr;
class FSImpl;
typedef std::shared_ptr<FSImpl> FSImplPtr;

enum SeekMode {
    SeekSet = 0,
    SeekCur = 1,
    SeekEnd = 2
};

class SDAudioFile : public Stream
{
public:
    SDAudioFile(FileImplPtr p = FileImplPtr()) : _p(p) {
        _timeout = 0;
    }

    size_t write(uint8_t) override;
    size_t write(const uint8_t *buf, size_t size) override;
    int available() override;
    int read() override;
    int peek() override;
    void flush() override;
    size_t read(uint8_t* buf, size_t size);
    size_t readBytes(char *buffer, size_t length)
    {
        return read((uint8_t*)buffer, length);
    }

    bool seek(uint32_t pos, SeekMode mode);
    bool seek(uint32_t pos)
    {
        return seek(pos, SeekSet);
    }
    size_t position() const;
    size_t size() const;
    void close();
    operator bool() const;
    time_t getLastWrite();
    const char* name() const;

    boolean isDirectory(void);
    SDAudioFile openNextFile(const char* mode = "r");
    void rewindDirectory(void);

protected:
    FileImplPtr _p;
};

class FS
{
public:
    FS(FSImplPtr impl) : _impl(impl) { }

    SDAudioFile open(const char* path, const char* mode = "r");
    SDAudioFile open(const String& path, const char* mode = "r");

    bool exists(const char* path);
    bool exists(const String& path);

    bool remove(const char* path);
    bool remove(const String& path);

    bool rename(const char* pathFrom, const char* pathTo);
    bool rename(const String& pathFrom, const String& pathTo);

    bool mkdir(const char *path);
    bool mkdir(const String &path);

    bool rmdir(const char *path);
    bool rmdir(const String &path);


protected:
    FSImplPtr _impl;
};

} // namespace fs
#ifndef FS_NO_GLOBALS
using fs::FS;
using fs::SDAudioFile;
using fs::SeekMode;
using fs::SeekSet;
using fs::SeekCur;
using fs::SeekEnd;
#endif //FS_NO_GLOBALS

#endif //FS_H
