#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Support/FileSystem.h>

#include <memory>

std::unique_ptr<llvm::Module> makeLLVMModule()
{
    auto module = std::make_unique<llvm::Module>("./sum-gen.ll", llvm::getGlobalContext());
    llvm::SmallVector<llvm::Type*, 2> funcTyArgs;
    funcTyArgs.push_back(llvm::IntegerType::get(module->getContext(), 32));
    funcTyArgs.push_back(llvm::IntegerType::get(module->getContext(), 32));
    llvm::FunctionType* funcTy = llvm::FunctionType::get(
        llvm::IntegerType::get(module->getContext(), 32),
        funcTyArgs,
        false);

    llvm::Function* funcSum = llvm::Function::Create(
        /*Type=*/ funcTy,
        /*Linkage=*/ llvm::GlobalValue::ExternalLinkage,
        /*Name=*/ "sum",
        module.get());
    funcSum->setCallingConv(llvm::CallingConv::C);

    auto args = funcSum->arg_begin();
    llvm::Value& int32_a_ref = *args++;
    auto int32_a = &int32_a_ref;
    int32_a->setName("a");
    llvm::Value& int32_b_ref = *args++;
    auto int32_b = &int32_b_ref;
    int32_b->setName("b");

    llvm::BasicBlock* labelEntry = llvm::BasicBlock::Create(
        module->getContext(),
        "entry",
        funcSum,
        0);

    auto ptrA = new llvm::AllocaInst(llvm::IntegerType::get(module->getContext(), 32),
                                     "a.addr",
                                     labelEntry);
    ptrA->setAlignment(4);

    auto ptrB = new llvm::AllocaInst(llvm::IntegerType::get(module->getContext(), 32),
                                     "b.addr",
                                     labelEntry);
    ptrB->setAlignment(4);

    auto st0 = new llvm::StoreInst(int32_a, ptrA, false, labelEntry);
    auto st1 = new llvm::StoreInst(int32_b, ptrB, false, labelEntry);
    st0->setAlignment(4);
    st1->setAlignment(4);

    auto ld0 = new llvm::LoadInst(ptrA, "", false, labelEntry);
    auto ld1 = new llvm::LoadInst(ptrB, "", false, labelEntry);
    ld0->setAlignment(4);
    ld1->setAlignment(4);

    auto addRes = llvm::BinaryOperator::Create(llvm::Instruction::Add,
                                               ld0,
                                               ld1,
                                               "add",
                                               labelEntry);
    llvm::ReturnInst::Create(module->getContext(), addRes, labelEntry);
    return module;
}

int main()
{
    auto mod = makeLLVMModule();
    llvm::verifyModule(*mod);
    std::error_code ErrorInfo;
    auto out = std::make_unique<llvm::tool_output_file>("./sum-gen.bc",
                                                        ErrorInfo,
                                                        llvm::sys::fs::F_None);

    if (ErrorInfo)
    {
        llvm::errs() << ErrorInfo.message() << '\n';
        return -1;
    }

    WriteBitcodeToFile(mod.get(), out->os());
    out->keep();
    return 0;
}
