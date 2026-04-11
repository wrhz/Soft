#ifndef __VARS_H__
#define __VARS_H__

#include "utils.h"
#include <filesystem>

static ULONG_PTR gdiplusToken;
static fs::path exe_dir = utils::getExeParentDirString();

#endif