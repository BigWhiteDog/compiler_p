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

using namespace clang;

namespace {


class asCheckPrintFunctionsConsumer : public ASTConsumer {
public:
	CompilerInstance *CI;
	asCheckPrintFunctionsConsumer(CompilerInstance &CI)
	{
		this->CI=&CI;
	}
  	virtual bool HandleTopLevelDecl(DeclGroupRef DG) {

  		SourceManager& sm=CI->getSourceManager();
  		DiagnosticsEngine &D = CI->getDiagnostics();
  		for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; ++i) {
			Decl *D = *i;
                        
		  	//if (const NamedDecl *ND = dyn_cast<NamedDecl>(D))
		  	//{
		  		SourceLocation fsl = D->getLocation();
		  		std::pair<FileID,unsigned> p = sm.getDecomposedLoc(fsl);
	  			asCheck::locations[p.first].push_back(asCheck::offset_ptr(p.second,D));
		  	//}
		}
		for (asCheck::File_offsetptr_map::iterator i = asCheck::locations.begin(); i != asCheck::locations.end(); ++i)
		{
			i->second.sort();
			int nest_count=0;
			int last_end_loc=-1;
			for(std::list<asCheck::offset_ptr>::iterator j=i->second.begin(),je=i->second.end();j!= je;j++)
			{
				if(j->ptr==NULL)//one ascheck
				{
					if (nest_count)//last asCheck doesn't match a func. need to raise an error
					{
						//raise an error: repeat asCheck
						unsigned DiagID = D.getCustomDiagID(
						DiagnosticsEngine::Error, "repeat asCheck");
						D.Report(DiagID);
						return false;
					}		
					if((int)j->FilePos<last_end_loc)//if ascheck is in the block of last decl
					{
						//raise an error: asCheck in block
						unsigned DiagID = D.getCustomDiagID(
						DiagnosticsEngine::Error, "asCheck in block");
						D.Report(DiagID);
						return false;
					}
					nest_count++;

				}
				else
				{
					SourceLocation ts=j->ptr->getLocEnd();
					last_end_loc=(int)sm.getDecomposedLoc(ts).second;
					if (FunctionDecl *ND = dyn_cast<FunctionDecl>(j->ptr))
					{
						if (ND->isThisDeclarationADefinition())//check if definition
						{
							if (nest_count)//is asChecked
							{
								llvm::errs()<<ND->getNameAsString()<<":1\n";
								asCheck::caredFunctions.push_back(ND);
								nest_count--;
							}
							else
							{
								llvm::errs()<<ND->getNameAsString()<<":0\n";
							}
							
						}
						else if (nest_count)
						{
							//raise an error: not a definition
							unsigned DiagID = D.getCustomDiagID(
							DiagnosticsEngine::Error, "asCheck followed by a function decl which isn't a definition");
							D.Report(DiagID);
							return false;
						}
						
					}
					else
					{
						//raise an error: asCheck followed by a non-function decl
						unsigned DiagID = D.getCustomDiagID(
						DiagnosticsEngine::Error, "asCheck followed by a non-function decl");
						D.Report(DiagID);
						return false;
					}
				}
			}
			if (nest_count)//raise an error: some ascheck doesn't match a function definition
			{
				unsigned DiagID = D.getCustomDiagID(
				DiagnosticsEngine::Error, "asCheck dosen't match any function decl");
				D.Report(DiagID);
				return false;
			}
		}
		return true;
  	}
};

class PrintasCheckAction : public PluginASTAction {
protected:
  	ASTConsumer *CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) {
		return new asCheckPrintFunctionsConsumer(CI);
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
		ros << "Help for Print asCheck plugin goes here\n";
	}

};

}

static FrontendPluginRegistry::Add<PrintasCheckAction>
X("print-asCheck", "print asCheck function");
