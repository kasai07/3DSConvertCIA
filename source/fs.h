#pragma once

#include "common.h"


#define GAME_DIRS   "/Game3ds/3ds"
#define WORK_DIRS   "/payload_a9lh"

size_t HID_Pad();
size_t HID_Flag();
size_t BatteryLevel();
size_t Level3D();
size_t VolumeLevel();


u32 Wait();
u32 cartpresent();
u32 drawimage(bool screen, char* data, int bits, int posX, int posY,int tailleX, int tailleY);

u32 InitKeys();

bool InitFS();
void DeinitFS();
bool CheckSD();
u32 DeleteFileGame(u32 index, u32 extension);
/** Work directory handling **/
const char* GetWorkDir();

/** Checks if there is enough space free on the SD card **/
bool DebugCheckFreeSpace(size_t required);

/** Opens existing files */
bool FileOpen(const char* path);
bool DebugFileOpen(const char* path);

/** Opens new files (and creates them if they don't already exist) */
bool FileCreate(const char* path, bool truncate);
bool DebugFileCreate(const char* path, bool truncate);

/** Reads contents of the opened file */
size_t FileRead(void* buf, size_t size, size_t foffset);
bool DebugFileRead(void* buf, size_t size, size_t foffset);

/** Writes to the opened file */
size_t FileWrite(void* buf, size_t size, size_t foffset);
bool DebugFileWrite(void* buf, size_t size, size_t foffset);

/** Gets the size of the opened file */
size_t FileGetSize();

/** Creates a directory */
bool DirMake(const char* path);
bool DebugDirMake(const char* path);

/** Opens an existing directory */
bool DirOpen(const char* path);
bool DebugDirOpen(const char* path);

/** Reads next file name to fname from opened directory,
    returns false if all files in directory are processed.
    fname needs to be allocated to fsize bytes minimum. */
bool DirRead(char* fname, int fsize);

/** Get list pf files under a given path **/
bool GetFileListp(const char* path, char* list, int lsize, bool recursive);

/** Quickly opens a secondary file, gets some data, and closes it again **/
size_t FileGetData(const char* path, void* buf, size_t size, size_t foffset);

/** Quickly opens a secondary file, dumps some data, and closes it again **/
size_t FileDumpData(const char* path, void* buf, size_t size);

/** Formats the first (and only) partition using label as name **/
bool PartitionFormat(const char* label);

/** Gets remaining space on SD card in bytes */
uint64_t RemainingStorageSpace();

/** Gets total space on SD card in bytes */
uint64_t TotalStorageSpace();

void FileClose();
void DirClose();

void PowerOff();
void Reboot();


const char* GetGameDir();
bool GetFileList(const char* path, char* list, int lsize, bool recursive, bool inc_files, bool inc_dirs);

size_t FileInjectTo(const char* dest, u32 offset_in, u32 offset_out, u32 size, bool overwrite, void* buf, size_t bufsize);
size_t FileCopyTo(const char* dest, void* buf, size_t bufsize);


uint32_t NumHiddenSectors();
