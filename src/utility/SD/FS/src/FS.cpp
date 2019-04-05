/*
 FS.cpp - file system wrapper
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

#include "FS.h"
#include "FSImpl.h"

using namespace fs;

size_t SDAudioFile::write(uint8_t c)
{
    if (!_p) {
        return 0;
    }

    return _p->write(&c, 1);
}

time_t SDAudioFile::getLastWrite()
{
    if (!_p) {
        return 0;
    }

    return _p->getLastWrite();
}

size_t SDAudioFile::write(const uint8_t *buf, size_t size)
{
    if (!_p) {
        return 0;
    }

    return _p->write(buf, size);
}

int SDAudioFile::available()
{
    if (!_p) {
        return false;
    }

    return _p->size() - _p->position();
}

int SDAudioFile::read()
{
    if (!_p) {
        return -1;
    }

    uint8_t result;
    if (_p->read(&result, 1) != 1) {
        return -1;
    }

    return result;
}

size_t SDAudioFile::read(uint8_t* buf, size_t size)
{
    if (!_p) {
        return -1;
    }

    return _p->read(buf, size);
}

int SDAudioFile::peek()
{
    if (!_p) {
        return -1;
    }

    size_t curPos = _p->position();
    int result = read();
    seek(curPos, SeekSet);
    return result;
}

void SDAudioFile::flush()
{
    if (!_p) {
        return;
    }

    _p->flush();
}

bool SDAudioFile::seek(uint32_t pos, SeekMode mode)
{
    if (!_p) {
        return false;
    }

    return _p->seek(pos, mode);
}

size_t SDAudioFile::position() const
{
    if (!_p) {
        return 0;
    }

    return _p->position();
}

size_t SDAudioFile::size() const
{
    if (!_p) {
        return 0;
    }

    return _p->size();
}

void SDAudioFile::close()
{
    if (_p) {
        _p->close();
        _p = nullptr;
    }
}

SDAudioFile::operator bool() const
{
    return !!_p;
}

const char* SDAudioFile::name() const
{
    if (!_p) {
        return nullptr;
    }

    return _p->name();
}

//to implement
boolean SDAudioFile::isDirectory(void)
{
    if (!_p) {
        return false;
    }
    return _p->isDirectory();
}

SDAudioFile SDAudioFile::openNextFile(const char* mode)
{
    if (!_p) {
        return SDAudioFile();
    }
    return _p->openNextFile(mode);
}

void SDAudioFile::rewindDirectory(void)
{
    if (!_p) {
        return;
    }
    _p->rewindDirectory();
}

SDAudioFile _FS::open(const String& path, const char* mode)
{
    return open(path.c_str(), mode);
}

SDAudioFile _FS::open(const char* path, const char* mode)
{
    if (!_impl) {
        return SDAudioFile();
    }

    return SDAudioFile(_impl->open(path, mode));
}

bool _FS::exists(const char* path)
{
    if (!_impl) {
        return false;
    }
    return _impl->exists(path);
}

bool _FS::exists(const String& path)
{
    return exists(path.c_str());
}

bool _FS::remove(const char* path)
{
    if (!_impl) {
        return false;
    }
    return _impl->remove(path);
}

bool _FS::remove(const String& path)
{
    return remove(path.c_str());
}

bool _FS::rename(const char* pathFrom, const char* pathTo)
{
    if (!_impl) {
        return false;
    }
    return _impl->rename(pathFrom, pathTo);
}

bool _FS::rename(const String& pathFrom, const String& pathTo)
{
    return rename(pathFrom.c_str(), pathTo.c_str());
}


bool _FS::mkdir(const char *path)
{
    if (!_impl) {
        return false;
    }
    return _impl->mkdir(path);
}

bool _FS::mkdir(const String &path)
{
    return mkdir(path.c_str());
}

bool _FS::rmdir(const char *path)
{
    if (!_impl) {
        return false;
    }
    return _impl->rmdir(path);
}

bool _FS::rmdir(const String &path)
{
    return rmdir(path.c_str());
}


void FSImpl::mountpoint(const char * mp)
{
    _mountpoint = mp;
}

const char * FSImpl::mountpoint()
{
    return _mountpoint;
}
