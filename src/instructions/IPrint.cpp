#include "IPrint.h"

IPrint::IPrint(std::string msg, SymbolTable& symbolTable, Logger& logger) 
    : msg(std::move(msg)), symbolTable(symbolTable), logger(logger) {}

void IPrint::execute() {
    std::string log;

    if (symbolTable.contains(msg)) {
        log = "Value from " + msg + " = " + std::to_string(symbolTable[msg]);
        // std::cout << "Pointer: " << &symbolTable[msg] << std::endl;
    } else {
        log = msg;
    }

    logger.emplace_back(getTimestamp(), log);
}