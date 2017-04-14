#ifndef LLVM_CLANG_LEX_PRAGMA_ASCHECK_H
#define LLVM_CLANG_LEX_PRAGMA_ASCHECK_H
#include "clang/AST/AST.h"
//#include "clang/AST/DeclBase.h"
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
	~offset_ptr()
	{
		return;
	}
    };
    typedef std::map<clang::FileID, std::list<offset_ptr> > File_offsetptr_map;
    bool isThisDeclAsCheckFunction(clang::Decl*);
    File_offsetptr_map* getLocations();
    void insertCaredFunctions(clang::FunctionDecl*);
}//end of namespace asCheck

#endif
