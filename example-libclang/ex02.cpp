extern "C"
{
#include "clang-c/Index.h"
}

#include <iostream>
#include <string>


int main(int argc, char** argv)
{
    std::cout << "Hello";
    if (argc != 2)
    {
        return -1;
    }
    std::cout << " LLVM\n";

    std::string fileName(argv[1]);
    std::cout << "We've got file: " << fileName << '\n';
    //cl::ParseCommandLineOptions(argc, argv, "Diagnostics example");
    CXIndex index = clang_createIndex(0, 0);
    const char *args[] = {"-I/usr/include", "-I."};
    CXTranslationUnit translationUnit = clang_parseTranslationUnit(index, fileName.c_str(), args, 2, nullptr, 0,
                                                                   CXTranslationUnit_None);

    CXFile file = clang_getFile(translationUnit, fileName.c_str());
    CXSourceLocation loc_start = clang_getLocationForOffset(translationUnit, file, 0);
    CXSourceLocation loc_end = clang_getLocationForOffset(translationUnit, file, 60);
    CXSourceRange range = clang_getRange(loc_start, loc_end);
    unsigned numTokens = 0;
    CXToken *tokens = nullptr;
    clang_tokenize(translationUnit, range, &tokens, &numTokens);

    for (unsigned i = 0; i < numTokens; ++i)
    {
        enum CXTokenKind kind = clang_getTokenKind(tokens[i]);
        CXString name = clang_getTokenSpelling(translationUnit, tokens[i]);
        switch (kind)
        {
        case CXToken_Punctuation:
        {
            std::cout << "PUNCTUATION(" << clang_getCString(name) << ") ";
            break;
        }
        case CXToken_Keyword:
        {
            std::cout << "KEYWORD(" << clang_getCString(name) << ") ";
            break;
        }
        case CXToken_Identifier:
        {
            std::cout << "IDENTIFIER(" << clang_getCString(name) << ") ";
            break;
        }
        case CXToken_Literal:
        {
            std::cout << "COMMENT(" << clang_getCString(name) << ") ";
            break;
        }

        default:
        {
            std::cout << "UNKNOWN(" << clang_getCString(name) << ") ";
            break;
        }
        }

        clang_disposeString(name);
    }

    std::cout << '\n';

    clang_disposeTranslationUnit(translationUnit);
    clang_disposeIndex(index);
    return 0;
}
