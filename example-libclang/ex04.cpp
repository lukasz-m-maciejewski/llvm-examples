#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Host.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"

#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Parse/Parser.h"

#include <iostream>
#include <memory>

static llvm::cl::opt<std::string> FileName(llvm::cl::Positional,
                                           llvm::cl::desc("Input file"),
                                           llvm::cl::Required);

int main(int argc, char** argv)
{
    llvm::cl::ParseCommandLineOptions(argc, argv, "My simple frontend\n");
    clang::CompilerInstance CI;
    clang::DiagnosticOptions diagnosticOptions;
    CI.createDiagnostics();

    auto PTO = std::make_shared<clang::TargetOptions>();
    PTO->Triple = llvm::sys::getDefaultTargetTriple();
    clang::TargetInfo* PTI =
        clang::TargetInfo::CreateTargetInfo(CI.getDiagnostics(), PTO);
    CI.setTarget(PTI);
    CI.createFileManager();
    CI.createSourceManager(CI.getFileManager());
    CI.createPreprocessor(clang::TU_Complete);
    CI.getPreprocessorOpts().UsePredefines = false;
    std::unique_ptr<clang::ASTConsumer> astConsumer =
        clang::CreateASTPrinter(nullptr, "");
    CI.setASTConsumer(std::move(astConsumer));

    CI.createASTContext();
    CI.createSema(clang::TU_Complete, nullptr);
    const clang::FileEntry* pFile = CI.getFileManager().getFile(FileName);
    if (not pFile)
    {
        std::cerr << "File not found: " << FileName << '\n';
        return 1;
    }
    auto& sourceManager = CI.getSourceManager();

    sourceManager.setMainFileID(sourceManager.createFileID(
        pFile, clang::SourceLocation(), clang::SrcMgr::C_User));
    CI.getDiagnosticClient().BeginSourceFile(CI.getLangOpts(), 0);
    clang::ParseAST(CI.getSema());

    CI.getASTContext().PrintStats();
    CI.getASTContext().Idents.PrintStats();

    return 0;
}
