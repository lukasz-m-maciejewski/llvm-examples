#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_os_ostream.h"
//#include "llvm/Support/system_error.h"
#include <iostream>
#include <memory>

using namespace llvm;

static cl::opt<std::string> FileName(cl::Positional, cl::desc("Bitcode file"), cl::Required);

int main(int argc, char** argv)
{
    cl::ParseCommandLineOptions(argc, argv, "LLVM hello world\n");
    LLVMContext context;
    llvm::ErrorOr<std::unique_ptr<MemoryBuffer>> maybeMb{MemoryBuffer::getFile(FileName)};
    if (not maybeMb)
    {
        std::cout << "Error";
        return -1;
    }
    auto& mb = *maybeMb;

    llvm::ErrorOr<std::unique_ptr<Module>> maybeM = parseBitcodeFile(*mb, context);
    if (not maybeM)
    {
        std::cerr << "Error reading bitcode\n"; // << std::end;
        return -1;
    }
    auto& m = *maybeM;

    raw_os_ostream O(std::cout);
    auto & functionList = m->getFunctionList();

    std::for_each(functionList.begin(), functionList.end(),
                  [&](const auto& fun)
                  {
                      if (not fun.isDeclaration())
                      {
                          O << fun.getName() << " has " << fun.size() << " basic block(s). Is it varArg? "
                            << fun.isVarArg() << "\n";
                      }
                  });

       return 0;

}
