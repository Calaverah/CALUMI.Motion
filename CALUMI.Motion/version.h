//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#ifndef VERSION_HEADER_FILE_H
#define VERSION_HEADER_FILE_H

#define MAJORVAL 0

#define MINORVAL 1

#define PATCHVAL 1

#define REVISIONVAL 1

#define STR(x) #x
// Main macro to combine arguments into a string
#define MAKE_STRING(...) STR(__VA_ARGS__)

#define VER_FILEVERSION MAJORVAL,MINORVAL,PATCHVAL,REVISIONVAL
#define VER_PRODUCTVERSION MAJORVAL,MINORVAL,PATCHVAL,REVISIONVAL

#define VER_FILEVERSION_STR MAKE_STRING(MAJORVAL.MINORVAL.PATCHVAL.REVISIONVAL)
#define VER_PRODUCTVERSION_STR MAKE_STRING(MAJORVAL.MINORVAL.PATCHVAL)

#endif
