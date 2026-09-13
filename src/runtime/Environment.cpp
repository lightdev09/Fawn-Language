#include "Environment.hpp"
#include <string>

void Env::define(const Token& nameToken, Value val, const std::string& declaredType, bool isDynamic, bool isConst)
{
    std::string name = nameToken.m_lexeme;
    auto it = values.find(name);
    if (it != values.end()) {
        if (reporter) {
            std::string lineInfo = it->second.line > 0 ? " on line " + std::to_string(it->second.line) : "";
            reporter->report({ErrorKind::Reference,
                              "Redeclaration of variable '" + name + "' in the same scope",
                              nameToken.m_line,
                              "Variable '" + name + "' was already declared" + lineInfo + ". Choose a different name or assign to it without re-declaring.",
                              nameToken.m_column});
        }
        throw FatalError();
    }
    values[name] = VarEntry{val, declaredType, isDynamic, isConst, nameToken.m_line};
}

void Env::define(const std::string& name, Value val, const std::string& declaredType, bool isDynamic, bool isConst)
{
    values[name] = VarEntry{val, declaredType, isDynamic, isConst, 0};
}

bool Env::isDefinedInCurrentScope(const std::string& name) const
{
    return values.find(name) != values.end();
}

Value Env::get(const Token& nameToken)
{
    std::string name = nameToken.m_lexeme;

    auto it = values.find(name);
    if (it != values.end()) {
        return it->second.value; 
    }
    if (enclosing != nullptr) {
        return enclosing->get(nameToken);
    }

    reporter->report({ErrorKind::Reference, "Undefined variable '" + name + "'",
                      nameToken.m_line, "Declare the variable before using it.", nameToken.m_column});
    throw FatalError();
}

void Env::assign(const Token& nameToken, Value val)
{
    std::string name = nameToken.m_lexeme;
    auto it = values.find(name);
    if (it != values.end()) {
        if (it->second.isConst) {
            reporter->report({ErrorKind::Runtime, "Cannot reassign const variable '" + name + "'",
                              nameToken.m_line, "Remove 'const' or assign the value only once.", nameToken.m_column});
            throw FatalError();
        }
        if (!it->second.isDynamic) {
            if (it->second.declaredType == "float") {
                if (val.isInt()) {
                    val = Value(static_cast<float>(val.asInt()));
                } else if (val.isString()) {
                    try {
                        val = Value(std::stof(val.asString()));
                    } catch (...) {}
                }
            } else if (it->second.declaredType == "int") {
                if (val.isFloat()) {
                    val = Value(static_cast<int>(val.asFloat()));
                } else if (val.isBool()) {
                    val = Value(val.asBool() ? 1 : 0);
                } else if (val.isString()) {
                    try {
                        const std::string& str = val.asString();
                        if (str.find('.') != std::string::npos) {
                            val = Value(static_cast<int>(std::stof(str)));
                        } else {
                            val = Value(std::stoi(str));
                        }
                    } catch (...) {}
                }
            } else if (it->second.declaredType == "string") {
                if (!val.isString()) {
                    val = Value(val.stringify());
                }
            }
            std::string actual = val.getTypeAsString();
            bool matches =
                (it->second.declaredType == "int" && val.isInt()) ||
                (it->second.declaredType == "float" && val.isFloat()) ||
                (it->second.declaredType == "string" && val.isString()) ||
                (it->second.declaredType == "bool" && val.isBool());
            if (!matches) {
                reporter->report({ErrorKind::Type,
                                  "Cannot assign '" + actual + "' to statically-typed variable '" + name +
                                      "' (expected '" + it->second.declaredType + "')",
                                  nameToken.m_line,
                                  "Assign a value with type '" + it->second.declaredType + "'.", nameToken.m_column});
                throw FatalError();
            }
        }
        it->second.value = val;
        return;
    }
    if (enclosing != nullptr) { enclosing->assign(nameToken, val); return; }
    reporter->report({ErrorKind::Reference, "Cannot reassign undefined variable '" + name + "'",
                      nameToken.m_line, "Declare the variable before assigning to it.", nameToken.m_column});
    throw FatalError();
}

bool Env::isConst(const Token& nameToken)
{
    std::string name = nameToken.m_lexeme;
    auto it = values.find(name);
    if (it != values.end()) {
        return it->second.isConst;
    }
    if (enclosing != nullptr) {
        return enclosing->isConst(nameToken);
    }
    return false;
}
