#ifndef LLVM_CLANG_LEX_PRAGMA_ASCHECK_H
#define LLVM_CLANG_LEX_PRAGMA_ASCHECK_H
#include "clang/Basic/LLVM.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include <cassert>
#include "clang/AST/AST.h"
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
#include <map>
namespace asCheck{
    class offset_ptr 
    {
      public:
	unsigned FilePos;
	clang::Decl * ptr;
	bool operator < (offset_ptr t)
	{
		return FilePos<t.FilePos;
	}
	offset_ptr(unsigned FilePos,clang::Decl *ptr)
	{
		this->FilePos=FilePos;
		this->ptr=ptr;
	}
    };
    typedef std::map<clang::FileID, std::list<offset_ptr> > File_offsetptr_map;
    File_offsetptr_map locations;
    std::list<clang::FunctionDecl*> caredFunctions;
}//end of namespace asCheck

#endif
