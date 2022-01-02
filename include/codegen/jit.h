#ifndef INCLUDE_ZREGEX_CODEGEN_JIT_H
#define INCLUDE_ZREGEX_CODEGEN_JIT_H

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JITSymbol.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/IRTransformLayer.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/Orc/SymbolStringPool.h>
#include <llvm/ExecutionEngine/RTDyldMemoryManager.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/Mangler.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace ZRegex {

  class JIT {
  private:
    /// The target machine.
    std::unique_ptr<llvm::TargetMachine> target_machine;
    /// The data layout.
    const llvm::DataLayout data_layout;
    /// The execution session
    llvm::orc::ExecutionSession execution_session;
    /// The context
    llvm::orc::ThreadSafeContext& context;

    /// Optimization function using OptimizeFunction =
    /// std::function<std::unique_ptr<llvm::Module>(std::unique_ptr<llvm::Module>)>;

    /// The object layer.
    llvm::orc::RTDyldObjectLinkingLayer object_layer;
    /// The compile layer.
    llvm::orc::IRCompileLayer compile_layer;
    /// The optimize layer.
    llvm::orc::IRTransformLayer optimize_layer;
    /// The main JITDylib
    llvm::orc::JITDylib& mainDylib;

  public:
    /// The constructor.
    explicit JIT(llvm::orc::ThreadSafeContext& ctx);

    /// Get the target machine.
    auto& getTargetMachine() { return *target_machine; }
    auto& getDataLayout() const { return data_layout; }
    auto& getES() { return execution_session; }
    /// Add a module.
    llvm::Error addModule(std::unique_ptr<llvm::Module> module);

    /// Get pointer to function.
    void* getPointerToFunction(const std::string& name);
    void* getPointerToFunction(const llvm::orc::SymbolStringPtr& name) {
      std::string rr;
      llvm::raw_string_ostream stream(rr);
      execution_session.dump(stream);
      std::cout << rr << std::endl;

      auto sym = execution_session.lookup(&mainDylib, name);
      //  auto e = sym.takeError();
      std::cout << toString(sym.takeError()) << std::endl;
      return sym ? reinterpret_cast<void*>(static_cast<uintptr_t>(sym->getAddress())) : nullptr;
    }
  };

}  // namespace ZRegex

#endif
