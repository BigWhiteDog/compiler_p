//===- PrintasCheck.cpp ---------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Example clang plugin which simply prints the names of all the top-level decls
// in the input file.
//
//===----------------------------------------------------------------------===//

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"

#include "clang/Lex/PragmaAsCheck.h"

#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"

#include <map>

using namespace clang;

namespace {

class offset_ptr 
{
protected:
	unsigned FilePos;
	Decl * ptr;
	bool operator < (offset_ptr t)
	{
		return FilePos<t.FilePos;
	}
	offset_ptr(unsigned FilePos,Decl *ptr)
	{
		this.FilePos=FilePos;
		this.ptr=ptr;
	}
};

typedef std::map<FileID, std::list<offset_ptr>> File_offsetptr_map;

class PrintFunctionsConsumer : public ASTConsumer {
public:
	CompilerInstance &CI;
	PrintFunctionsConsumer(CompilerInstance &CI)
	{
		this.CI=CI;
	}
  	virtual bool HandleTopLevelDecl(DeclGroupRef DG) {

  		SourceManager& sm=CI.getSourceManager();
  		File_offsetptr_map m;


  		for (asCheck::LocationManager i = asCheck::locations.begin(),e= asCheck::locations.end(); i != e; ++i)
  		{
  			const SourceLocation L=*i;
  			std::pair<FileID,unsigned> p = sm.getDecomposedLoc(L);
  			m[p.first].push_back(offset_ptr(p.second,NULL));

  		}
  		for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; ++i) {
			const Decl *D = *i;
		  	if (const NamedDecl *ND = dyn_cast<NamedDecl>(D))
		  	{
		  		SourceLocation fsl= ND->getLocation();
		  		std::pair<FileID,unsigned> p = sm.getDecomposedLoc(fsl);
	  			m[p.first].push_back(offset_ptr(p.second,D));
		  	}
		}
		for (File_offsetptr_map::iterator i = m.begin(); i != m.end(); ++i)
		{
			i->second.sort();
			int nest_count=0;
			int last_end_loc=-1;
			for(std::list<offset_ptr>::iterator j=i->second.begin(),je=i->second.end();j!= je;j++)
			{
				if(j->ptr==NULL)//one ascheck
				{
					nest_count++;
					if((int)j->FilePos<last_end_loc)

				}
				else
				{
					SourceLocation ts=j->ptr->getLocEnd();
					last_end_loc=(int)sm.getDecomposedLoc(ts).second;
					if (const FunctionDecl *ND = dyn_cast<FunctionDecl>(D))
					{
						/* code */
					}
				}
			}
			if (nest_count)//some ascheck doesn't match a function Decl
			{
				
			}
		}

		for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; ++i) {
			const Decl *D = *i;
		  	if (const FunctionDecl *ND = dyn_cast<FunctionDecl>(D))
		  	{
		  		SourceLocation fsl= ND->getLocation();

				llvm::errs() << ND->getNameAsString() << "\"\n";
		  	}
		}



		
		return true;
  	}
};

class PrintasCheckAction : public PluginASTAction {
protected:
  	ASTConsumer *CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) {
		return new PrintFunctionsConsumer(CI);
  	}

  	bool ParseArgs(const CompilerInstance &CI,
				 const std::vector<std::string>& args) {
		for (unsigned i = 0, e = args.size(); i != e; ++i) {
	  		llvm::errs() << "PrintFunctionNames arg = " << args[i] << "\n";

	 	 	// Example error handling.
	  		if (args[i] == "-an-error") {
				DiagnosticsEngine &D = CI.getDiagnostics();
				unsigned DiagID = D.getCustomDiagID(
				DiagnosticsEngine::Error, "invalid argument '" + args[i] + "'");
				D.Report(DiagID);
				return false;
	  		}
		}
		if (args.size() && args[0] == "help")
		  	PrintHelp(llvm::errs());
		return true;
  	}
	void PrintHelp(llvm::raw_ostream& ros) {
		ros << "Help for PrintFunctionNames plugin goes here\n";
	}

};

}

static FrontendPluginRegistry::Add<PrintasCheckAction>
X("print-asCheck", "print asCheck function");
