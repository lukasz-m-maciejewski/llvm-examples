extern "C"
{
#include "clang-c/Index.h"
}

#include <iostream>

enum CXChildVisitResult visitNode(CXCursor cursor, CXCursor /*parent*/,
                                  CXClientData /*client_data*/)
{
    if (clang_getCursorKind(cursor) == CXCursor_CXXMethod or
        clang_getCursorKind(cursor) == CXCursor_FunctionDecl)
    {
        CXString name = clang_getCursorSpelling(cursor);
        CXSourceLocation loc = clang_getCursorLocation(cursor);
        CXString fName;
        unsigned line = 0;
        unsigned col = 0;
        clang_getPresumedLocation(loc, &fName, &line, &col);
        std::cout << clang_getCString(fName) << ":"
                  << line << ":" << col << " declares "
                  << clang_getCString(name) << '\n';
        return CXChildVisit_Continue;
    }
    return CXChildVisit_Recurse;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        return -1;
    }
    std::string fileName{argv[1]};
    CXIndex index = clang_createIndex(0, 0);
    const char *args[] = { "-I/usr/include", "-I." };
    CXTranslationUnit translationUnit =
        clang_parseTranslationUnit(index, fileName.c_str(), args, 2,
                                   nullptr, 0, CXTranslationUnit_None);
    CXCursor cur = clang_getTranslationUnitCursor(translationUnit);
    clang_visitChildren(cur, &visitNode, nullptr);

    clang_disposeTranslationUnit(translationUnit);
    clang_disposeIndex(index);
    return 0;
}
