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

class line_ptr 
{
protected:
	int line_number;
	Decl * ptr;
	// SourceLocation line_ptr;
	

	virtual bool operator < (line_ptr t)
	{
		return line_number<t.line_number;
	}	
};

class PrintFunctionsConsumer : public ASTConsumer {
public:
  	virtual bool HandleTopLevelDecl(DeclGroupRef DG) {

  		for (asCheck::LocationManager i = asCheck::locations.begin(),e= asCheck::locations.end(); i != e; ++i)
  		{
  			const SourceLocation L=*i;
  			std::pair<FileID,unsigned> P= CI-> 

  		}
  		std::map<FileID, std::list<line_ptr>> m;

		for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; ++i) {
			const Decl *D = *i;
		  	if (const FunctionDecl *ND = dyn_cast<FunctionDecl>(D))
		  	{
		  		SourceLocation fsl= ND->getLocation();

				llvm::errs() << "top-level-decl: \"" << ND->getNameAsString() << "\"\n";
		  	}
		}
		return true;
  	}
};

class PrintasCheckAction : public PluginASTAction {
protected:
  	ASTConsumer *CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) {
		return new PrintFunctionsConsumer();
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
