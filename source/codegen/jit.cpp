#include "codegen/jit.h"

#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Passes/PassBuilder.h>

#include "llvm/Support/raw_os_ostream.h"
#include "spdlog/spdlog.h"

namespace ZRegex {

  void optimizeModule(llvm::Module& module, llvm::PassBuilder::OptimizationLevel lvl) {
    if (module.getName() != "rgx_module") return;
    // Create the analysis managers.
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    llvm::CGSCCAnalysisManager CGAM;
    llvm::ModuleAnalysisManager MAM;

    // Create the new pass manager builder.
    // Take a look at the PassBuilder constructor parameters for more
    // customization, e.g. specifying a TargetMachine or various debugging
    // options.

    llvm::PassBuilder PB;
    // Make sure to use the default alias analysis pipeline, otherwise we'll end
    // up only using a subset of the available analyses.
    FAM.registerPass([&] { return PB.buildDefaultAAPipeline(); });

    // Register all the basic analyses with the managers.
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

    // Create the pass manager.
    // This one corresponds to a typical -O2 optimization pipeline.
    spdlog::debug("{}", lvl == llvm::PassBuilder::OptimizationLevel::O0);
    llvm::ModulePassManager MPM = PB.buildPerModuleDefaultPipeline(lvl);

    PB.registerPipelineStartEPCallback(
        [&](llvm::ModulePassManager& MPM, llvm::PassBuilder::OptimizationLevel Level) {
          MPM.addPass(PB.buildInlinerPipeline(lvl, llvm::ThinOrFullLTOPhase::None));
        });

    if (spdlog::get_level() == SPDLOG_LEVEL_DEBUG) {
      spdlog::info("################ BEFORE OPTIMIZATION PASS ################");
      module.print(llvm::errs(), nullptr);
    }
    // Optimize the IR!
    MPM.run(module, MAM);
    if (spdlog::get_level() == SPDLOG_LEVEL_DEBUG) {
      spdlog::info("################ AFTER OPTIMIZATION PASS ################");
      module.print(llvm::errs(), nullptr);
    }
  }

  JIT::JIT(llvm::orc::ThreadSafeContext& ctx, const CodegenOpts& opts)
      : target_machine(llvm::EngineBuilder().selectTarget()),
        data_layout(target_machine->createDataLayout()),
        execution_session(std::make_unique<llvm::orc::UnsupportedExecutorProcessControl>()),
        context(ctx),
        object_layer(execution_session,
                     []() { return std::make_unique<llvm::SectionMemoryManager>(); }),
        compile_layer(execution_session, object_layer,
                      std::make_unique<llvm::orc::SimpleCompiler>(*target_machine)),
        optimize_layer(execution_session, compile_layer,
                       [this, opts](llvm::orc::ThreadSafeModule m,
                                    const llvm::orc::MaterializationResponsibility&) {
                         optimizeModule(*m.getModuleUnlocked(), opts.getOptimizationLevelforLLVM());
                         return m;
                       }),
        mainDylib(cantFail(execution_session.createJITDylib("<main>"), "createJITDylib failed")) {
    // Lookup symbols in host process
    auto generator = llvm::cantFail(llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(
        data_layout.getGlobalPrefix(), [](auto&) { return true; }));
    mainDylib.addGenerator(move(generator));
  }

  llvm::Error JIT::addModule(std::unique_ptr<llvm::Module> module) {
    return optimize_layer.add(mainDylib, llvm::orc::ThreadSafeModule{move(module), context});
  }

  void* JIT::getPointerToFunction(const std::string& name) {
    auto error_or_symbol = execution_session.lookup(&mainDylib, name);
    if (!error_or_symbol) {
      spdlog::error(llvm::toString(error_or_symbol.takeError()));
      return nullptr;
    }
    return reinterpret_cast<void*>(static_cast<uintptr_t>(error_or_symbol->getAddress()));
  }
};  // namespace ZRegex
