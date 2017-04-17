set(MODULE TRUE)

set( LLVM_LINK_COMPONENTS support mc)

add_clang_library(PrintasCheck PrintasCheck.cpp)

add_dependencies(PrintasCheck
  ClangAttrClasses
  ClangAttrList
  ClangCommentNodes
  ClangDeclNodes
  ClangDiagnosticCommon
  ClangStmtNodes
  )

target_link_libraries(PrintasCheck
  clangFrontend
  clangAST
  )

set_target_properties(PrintasCheck
  PROPERTIES
  LINKER_LANGUAGE CXX
  PREFIX "")
