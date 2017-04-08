#ifndef LLVM_CLANG_LEX_PRAGMA_ASCHECK_H
#define LLVM_CLANG_LEX_PRAGMA_ASCHECK_H
#include "clang/Basic/LLVM.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include <cassert>
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/LexDiagnostic.h"
#include "clang/Lex/LiteralSupport.h"
#include "clang/Lex/MacroInfo.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PragmaAsCheck.h"
#include "llvm/Support/CrashRecoveryContext.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <list>
namespace asCheck{
    std::list<clang::SourceLocation> locations;
    typedef std::list<clang::SourceLocation>::iterator LocationManager;
}//end of namespace asCheck

#endif
