#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Host.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"

#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/Parser.h"
#include "clang/Parse/ParseAST.h"

#include <iostream>

static llvm::cl::opt<std::string> FileName(llvm::cl::Positional,
                                           llvm::cl::desc("Input file"),
                                           llvm::cl::Required);

int main(int argc, char** argv)
{
    llvm::cl::ParseCommandLineOptions(argc, argv, "My simple frontend\n");
    clang::CompilerInstance CI;
    clang::DiagnosticOptions diagnosticOptions;
    CI.createDiagnostics();

    return 0;
}
