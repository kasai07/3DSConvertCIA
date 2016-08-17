#include "fs.h"
#include "draw.h"
#include "menu.h"
#include "hid.h"
#include "keys.h"
#include "tga.h"
#include "pathmenu.h"

#include "game.h"
#include "i2c.h"
#include "fatfs/ff.h"
#include "fatfs/sdmmc.h"


static bool fs_ok = false;
static FATFS fs;
static FIL file;
static DIR dir;

/* Volume - Partition resolution table (default table) */
u32 InitKeys()
{
	SetupSector0x96Key0x11(); // Sector0x96 key - no effect on error level
	SetupTwlKey0x03();   // TWL KeyX / KeyY
	
	LoadKeyFromFile(0x25, 'X', NULL); 	// NCCH 7x KeyX
    LoadKeyFromFile(0x18, 'X', NULL);	// NCCH Secure3 KeyX
    LoadKeyFromFile(0x1B, 'X', NULL); 	// NCCH Secure4 KeyX
	LoadKeyFromFile(0x24, 'Y', NULL);	// AGBSAVE CMAC KeyY
}

u32 Wait()
{
	DebugColor(WHITE, TRANSPARENT, TOP_SCREEN1,"Press Any Key");
	InputWait();

}

bool InitFS()
{
    bool ret = fs_ok = (f_mount(&fs, "0:", 1) == FR_OK);
    if (ret)
        f_chdir(GetWorkDir());

    return ret;
}

void DeinitFS()
{
    fs_ok = false;
    f_mount(NULL, "0:", 1);
}

bool CheckSD()
{
    return fs_ok;
}

const char* GetWorkDir()
{
    const char* root = "/";
    const char* work_dirs[] = { WORK_DIRS };
    u32 n_dirs = sizeof(work_dirs) / sizeof(char*);
    
    if (!fs_ok)
        return NULL;
    
    u32 i;
    for (i = 0; i < n_dirs; i++) {
        FILINFO fno = { .lfname = NULL };
        if ((f_stat(work_dirs[i], &fno) == FR_OK) && (fno.fattrib & AM_DIR))
            break;
    }
    
    return ((i >= n_dirs) ? root : work_dirs[i]);
}
const char* GetGameDir()
{
    const char* game_dirs[] = { GAME_DIRS };
    u32 n_dirs = sizeof(game_dirs) / sizeof(char*);
    
    u32 i;
    for (i = 0; i < n_dirs; i++) {
        FILINFO fno;
        if ((f_stat(game_dirs[i], &fno) == FR_OK) && (fno.fattrib & AM_DIR))
            break;
    }
    
    return ((i >= n_dirs) ? NULL : game_dirs[i]);
}
bool DebugCheckFreeSpace(size_t required)
{
	if (!fs_ok)
        return false;
    if (required > RemainingStorageSpace()) {
        DebugColor(RED, TRANSPARENT, TOP_SCREEN1,"Not enough space left on SD card");
        return false;
    }
    
    return true;
}

bool FileOpen(const char* path)
{
    if (!fs_ok)
        return false;
    unsigned flags = FA_READ | FA_WRITE | FA_OPEN_EXISTING;
    unsigned flags_ro = FA_READ | FA_OPEN_EXISTING;
    bool ret = (f_open(&file, path, flags) == FR_OK) ||
        (f_open(&file, path, flags_ro) == FR_OK);
    f_lseek(&file, 0);
    f_sync(&file);
    return ret;
}

bool DebugFileOpen(const char* path)
{
    DebugColor(WHITE, TRANSPARENT, TOP_SCREEN1,"Opening %s ...", path);
    if (!FileOpen(path)) {
        DebugColor(RED, TRANSPARENT, TOP_SCREEN1,"Could not open %s!", path);
        return false;
    }
    
    return true;
}

bool FileCreate(const char* path, bool truncate)
{
   
    unsigned flags = FA_READ | FA_WRITE;
    flags |= truncate ? FA_CREATE_ALWAYS : FA_OPEN_ALWAYS;
    bool ret = (f_open(&file, path, flags) == FR_OK);
    f_lseek(&file, 0);
    f_sync(&file);
    return ret;
}

bool DebugFileCreate(const char* path, bool truncate) {
    DebugColor(WHITE, TRANSPARENT, TOP_SCREEN1,"Creating %s ...", path);
    if (!FileCreate(path, truncate)) {
        Wait();
		DebugColor(RED, TRANSPARENT, TOP_SCREEN1,"Could not create %s!", path);
        
		return false;
    }

    return true;
}

size_t FileRead(void* buf, size_t size, size_t foffset)
{
    UINT bytes_read = 0;
    f_lseek(&file, foffset);
    f_read(&file, buf, size, &bytes_read);
    return bytes_read;
}

bool DebugFileRead(void* buf, size_t size, size_t foffset) {
    size_t bytesRead = FileRead(buf, size, foffset);
    if(bytesRead != size) {
        DebugColor(RED, TRANSPARENT, TOP_SCREEN1,"ERROR, file is too small!");
        Wait();
		return false;
    }
    
    return true;
}

size_t FileWrite(void* buf, size_t size, size_t foffset)
{
    UINT bytes_written = 0;
    f_lseek(&file, foffset);
    f_write(&file, buf, size, &bytes_written);
    f_sync(&file);
    return bytes_written;
}

bool DebugFileWrite(void* buf, size_t size, size_t foffset)
{
    size_t bytesWritten = FileWrite(buf, size, foffset);
    if(bytesWritten != size) {
        DebugColor(RED, TRANSPARENT, TOP_SCREEN1,"SD failure or SD full");
        Wait();
		return false;
    }
    
    return true;
}

size_t FileGetSize()
{
    return f_size(&file);
}

void FileClose()
{
    f_close(&file);
}

bool DirMake(const char* path)
{
    if (!fs_ok)
        return false;
    FRESULT res = f_mkdir(path);
    bool ret = (res == FR_OK) || (res == FR_EXIST);
    return ret;
}

bool DebugDirMake(const char* path)
{
    DebugColor(WHITE, TRANSPARENT, TOP_SCREEN1,"Creating dir %s ...", path);
    if (!DirMake(path)) {
        DebugColor(RED, TRANSPARENT, TOP_SCREEN1,"Could not create %s!", path);
        return false;
    }
    
    return true;
}

bool DirOpen(const char* path)
{
    
    return (f_opendir(&dir, path) == FR_OK);
}

bool DebugDirOpen(const char* path)
{
    DebugColor(WHITE, TRANSPARENT, TOP_SCREEN1,"Opening %s ...", path);
    if (!DirOpen(path)) {
        DebugColor(RED, TRANSPARENT, TOP_SCREEN1,"Could not open %s!", path);
        return false;
    }
    
    return true;
}

bool DirRead(char* fname, int fsize)
{
    FILINFO fno;
    fno.lfname = fname;
    fno.lfsize = fsize;
    bool ret = false;
    
	while (f_readdir(&dir, &fno) == FR_OK) {
        
		if (fno.fname[0] == 0) break;
        
		if ((fno.fname[0] != '.') && !(fno.fattrib & AM_DIR)) 
		{
           
			if (fname[0] == 0)
			{
				
				strcpy(fname, fno.fname);
				
			}
            
			ret = true;
            ;
			break;
        }
    }
    return ret;
}

void DirClose()
{
    f_closedir(&dir);
}
bool GetFileListWorker(char** list, int* lsize, char* fpath, int fsize, bool recursive, bool inc_files, bool inc_dirs)
{
    DIR pdir;
    FILINFO fno;
    char* fname = fpath + strnlen(fpath, fsize - 1);
    bool ret = false;
    
    if (f_opendir(&pdir, fpath) != FR_OK)
        return false;
    (fname++)[0] = '/';
    
    while (f_readdir(&pdir, &fno) == FR_OK) {
        if ((strncmp(fno.fname, ".", 2) == 0) || (strncmp(fno.fname, "..", 3) == 0))
            continue; // filter out virtual entries
        strncpy(fname, fno.fname, (fsize - 1) - (fname - fpath));
        if (fno.fname[0] == 0) {
            ret = true;
            break;
        } else if ((inc_files && !(fno.fattrib & AM_DIR)) || (inc_dirs && (fno.fattrib & AM_DIR))) {
            snprintf(*list, *lsize, "%s\n", fpath);
            for(;(*list)[0] != '\0' && (*lsize) > 1; (*list)++, (*lsize)--); 
            if ((*lsize) <= 1) break;
        }
        if (recursive && (fno.fattrib & AM_DIR)) {
            if (!GetFileListWorker(list, lsize, fpath, fsize, recursive, inc_files, inc_dirs))
                break;
        }
    }
    f_closedir(&pdir);
    
    return ret;
}

bool GetFileList(const char* path, char* list, int lsize, bool recursive, bool inc_files, bool inc_dirs)
{
    char fpath[256]; // 256 is the maximum length of a full path
    strncpy(fpath, path, 256);
    return GetFileListWorker(&list, &lsize, fpath, 256, recursive, inc_files, inc_dirs);
}

bool GetFileListWorkerp(char** list, int* lsize, char* fpath, int fsize, bool recursive)
{
    DIR pdir;
    FILINFO fno;
    char* fname = fpath + strnlen(fpath, fsize - 1);
    bool ret = false;
    
    if (f_opendir(&pdir, fpath) != FR_OK) return false;
    (fname++)[0] = '/';
    fno.lfname = fname;
    fno.lfsize = fsize - (fname - fpath);
    
    while (f_readdir(&pdir, &fno) == FR_OK) {
        if (fno.fname[0] == '.') continue;
        if (fname[0] == 0)
            strcpy(fname, fno.fname);
        if (fno.fname[0] == 0) {
            ret = true;
            break;
        } else if (fno.fattrib & AM_DIR) {
            if (recursive && !GetFileListWorkerp(list, lsize, fpath, fsize, recursive))
                break;
        } else {
            snprintf(*list, *lsize, "%s\n", fpath);
            for(;(*list)[0] != '\0' && (*lsize) > 1; (*list)++, (*lsize)--); 
            if ((*lsize) <= 1) break;
        }
    }
    f_closedir(&pdir);
    
    return ret;
}

bool GetFileListp(const char* path, char* list, int lsize, bool recursive)
{
    if (!fs_ok)
        return false;
    char fpath[256];
    strncpy(fpath, path, 256);
    return GetFileListWorkerp(&list, &lsize, fpath, 256, recursive);
}

size_t FileGetData(const char* path, void* buf, size_t size, size_t foffset)
{
    unsigned flags = FA_READ | FA_OPEN_EXISTING;
    FIL tmp_file;
    if (!fs_ok)
        return 0;
    if (f_open(&tmp_file, path, flags) == FR_OK) {
        UINT bytes_read = 0;
        bool res = false;
        f_lseek(&tmp_file, foffset);
        f_sync(&tmp_file);
        res = (f_read(&tmp_file, buf, size, &bytes_read) == FR_OK);
        f_close(&tmp_file);
        return (res) ? bytes_read : 0;
    }
    
    return 0;
}

size_t FileDumpData(const char* path, void* buf, size_t size)
{
    unsigned flags = FA_WRITE | FA_CREATE_ALWAYS;
    FIL tmp_file;
    UINT bytes_written = 0;;
    bool res = false;
    if (!fs_ok)
        return 0;
    if (f_open(&tmp_file, path, flags) != FR_OK)
        return 0;
    f_lseek(&tmp_file, 0);
    f_sync(&tmp_file);
    res = (f_write(&tmp_file, buf, size, &bytes_written) == FR_OK);
    f_close(&tmp_file);
    
    return (res) ? bytes_written : 0;
}

bool PartitionFormat(const char* label)
{
    UINT p_size_gb = ((uint64_t) getMMCDevice(1)->total_size * 512) / (1000 * 1000 * 1000);
    UINT c_size = (p_size_gb < 4) ? 0 : (p_size_gb < 15) ? 32768 : 65536;
    bool ret = (f_mkfs("0:", 0, c_size) == FR_OK);
    if (ret && label) {
        char label0[16];
        snprintf(label0, 16, "0:%-11.11s", label);
        f_setlabel(label0);
    }
    return ret;
}

static uint64_t ClustersToBytes(FATFS* fs, DWORD clusters)
{
    uint64_t sectors = clusters * fs->csize;
    #if _MAX_SS != _MIN_SS
    return sectors * fs->ssize;
    #else
    return sectors * _MAX_SS;
    #endif
}

uint64_t RemainingStorageSpace()
{
    if (!fs_ok)
        return -1;
    DWORD free_clusters;
    FATFS *fs2;
    FRESULT res = f_getfree("0:", &free_clusters, &fs2);
    if (res != FR_OK)
        return -1;

    return ClustersToBytes(&fs, free_clusters);
}

uint64_t TotalStorageSpace()
{
    if (!fs_ok)
        return -1;
    return ClustersToBytes(&fs, fs.n_fatent - 2);
}
void Reboot()
{
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 2);
    while(true);
}


void PowerOff()
{
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 0);
    while (true);
}


size_t FileInjectTo(const char* dest, u32 offset_in, u32 offset_out, u32 size, bool overwrite, void* buf, size_t bufsize)
{
    unsigned flags = FA_WRITE | (overwrite ? FA_CREATE_ALWAYS : FA_OPEN_ALWAYS);
    size_t fsize = f_size(&file);
    size_t result = size;
    FIL dfile;
    // make sure the containing folder exists
    char tmp[256] = { 0 };
    strncpy(tmp, dest, sizeof(tmp) - 1);
    for (char* p = tmp + 1; *p; p++) {
        if (*p == '/') {
            char s = *p;
            *p = 0;
            f_mkdir(tmp);
            *p = s;
        }
    }
    // fix size var if zero
    if (size == 0)
        result = size = fsize - offset_in;
    // do the actual injecting
    if (f_open(&dfile, dest, flags) != FR_OK)
        return 0;
    f_lseek(&dfile, offset_out);
    f_sync(&dfile);
    f_lseek(&file, offset_in);
    f_sync(&file);
    for (size_t pos = 0; pos < size; pos += bufsize) {
        UINT bytes_read = 0;
        UINT bytes_written = 0;
        ShowProgress(pos, size);
        if (pos + bufsize > size)
            bufsize = size - pos;
        if ((f_read(&file, buf, bufsize, &bytes_read) != FR_OK) ||
            (f_write(&dfile, buf, bytes_read, &bytes_written) != FR_OK) ||
            (bytes_read != bytes_written)) {
            result = 0;
            break;
        }
    }
    ShowProgress(0, 0);
    f_close(&dfile);
    return result;
}

size_t FileCopyTo(const char* dest, void* buf, size_t bufsize)
{
    return FileInjectTo(dest, 0, 0, 0, true, buf, bufsize);
}

u32 DeleteFileGame(u32 index, u32 extension)
{
	
	
	char path[60];
    
	snprintf(path, 60, "Game3ds/menu/topbg.tga");
	loadtga(true,false,path,0,0);
	snprintf(path, 60, "Game3ds/menu/ciaBox.tga");
	loadtga(true,false,path,75,45);
	
	DrawStringFColor(WHITE, TRANSPARENT, 95, 80, true, "Delete the game !");
	DrawStringFColor(WHITE, TRANSPARENT, 95, 90, true, "%s",c[index]);
	DrawStringFColor(WHITE, TRANSPARENT, 95, 100, true, " ");
	DrawStringFColor(WHITE, TRANSPARENT, 95, 110, true, "A : Delete");
	DrawStringFColor(WHITE, TRANSPARENT, 95, 120, true, "B : Return");
	if(extension == 0)
	{
		snprintf(path, 60, "Game3ds/3ds/%s.3ds",c[index]);
	}
	if(extension == 1)
	{
		snprintf(path, 60, "Game3ds/cia/%s.cia",c[index]);
	}
	
	while (true) 
	{
    
    u32 pad_state = InputWait();
    
	if (pad_state & BUTTON_A) 
	{
        
		f_unlink(path);
		return 0;
		
	} else if (pad_state & BUTTON_B) {
      
		return 0;
		
	} 
	}

}

#if !defined(ALT_PROGRESS)
void ShowProgress(u64 current, u64 total)
{
    const u32 progX = SCREEN_WIDTH_TOP - 40;
    const u32 progY = SCREEN_HEIGHT - 20;
    
    if (total > 0) {
        char progStr[8];
        snprintf(progStr, 8, "%3llu%%", (current * 100) / total);
        DrawString(TOP_SCREEN0, progStr, progX, progY, WHITE, BLACK);
        DrawString(TOP_SCREEN1, progStr, progX, progY, WHITE, BLACK);
    } else {
        DrawString(TOP_SCREEN0, "    ", progX, progY, WHITE, BLACK);
        DrawString(TOP_SCREEN1, "    ", progX, progY, WHITE, BLACK);
    }
}
#endif