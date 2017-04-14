#include "clang/Lex/PragmaAsCheck.h"
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
namespace asCheck{
    File_offsetptr_map locations;
    std::list<clang::FunctionDecl*> caredFunctions;
    bool isThisDeclAsCheckFunction(clang::Decl* D)
    {
        //TO DO: implement find function is asCheck or not
        return false;
    }
    File_offsetptr_map* getLocations()
    {
        return &locations;
    }
    void insertCaredFunctions(clang::FunctionDecl* FD)
    {
        caredFunctions.push_back(FD);
    }
}//end of namespace asCheck
