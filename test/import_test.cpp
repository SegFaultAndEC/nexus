#include <windows.h>

#include "test.hpp"
using namespace nx;

void importTest() {
    auto line = "std->func()";
    auto tokens = lexer(line);
    std::vector<ASTNode *> pool;
    auto guarder = PoolCollectGuarder([&] { collectPool(pool); });
    auto parser = Parser(tokens, pool);
    State state;
    state.loadBuildInFunction();
    Evaluator evaluator(state);
    HINSTANCE hDLL = nullptr;
    try {
        hDLL = LoadLibrary(TEXT("nxstd.dll"));
        if (!hDLL) {
            throw std::runtime_error("无法加载 DLL");
        }
        auto func = (RegisterFunc)GetProcAddress(hDLL, NX_REGISTER_FUNC_NAME);
        if (!func) {
            throw std::runtime_error("无法获取函数地址");
        }
        func(state);
    } catch (const std::exception &e) {
        std::cerr << "错误: " << e.what() << std::endl;
        if (hDLL) {
            FreeLibrary(hDLL);
        }
        return;
    }

    try {
        auto node = parser.parseExpression();
        evaluator.eval(node);
    } catch (const NxError &e) {
        e.show();
    }
    FreeLibrary(hDLL);
};