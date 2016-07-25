extern "C"
{
#include "clang-c/Index.h"
}

// #include "llvm/Support/CommandLine.h"
#include <iostream>
#include <string>
//using namespace llvm;

//static cl::opt<std::string> FileName(cl::Positional, cl::desc("Input file"), cl::Required);

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        return -1;
    }

    std::string FileName(argv[1]);
    //cl::ParseCommandLineOptions(argc, argv, "Diagnostics example");
    CXIndex index = clang_createIndex(0, 0);
    const char *args[] = {"-I/usr/include", "-I."};
    CXTranslationUnit translationUnit =
        clang_parseTranslationUnit(index, FileName.c_str(), args, 2, NULL, 0,
                                   CXTranslationUnit_None);
    unsigned diagnosticCount = clang_getNumDiagnostics(translationUnit);

    for (unsigned i = 0; i < diagnosticCount; ++i)
    {
        CXDiagnostic diagnostic = clang_getDiagnostic(translationUnit, i);
        CXString category = clang_getDiagnosticCategoryText(diagnostic);
        CXString message = clang_getDiagnosticSpelling(diagnostic);
        unsigned severity = clang_getDiagnosticSeverity(diagnostic);
        CXSourceLocation loc = clang_getDiagnosticLocation(diagnostic);
        CXString fName;
        unsigned line = 0;
        unsigned col = 0;
        clang_getPresumedLocation(loc, &fName, &line, &col);
        std::cout << "Severity: " << severity << " File: "
                  << clang_getCString(fName) << " Line: "
                  << line << " Col: " << col << " Category: \""
                  << clang_getCString(category) << "\" Message: "
                  << clang_getCString(message) << std::endl;

        clang_disposeString(fName);
        clang_disposeString(message);
        clang_disposeString(category);
        clang_disposeDiagnostic(diagnostic);
    }

    clang_disposeTranslationUnit(translationUnit);
    clang_disposeIndex(index);
    return 0;
}
