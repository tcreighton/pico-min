
#include "json-report.hpp"

namespace CScmdctl {

    void JsonReport::openDocument() {// reinitialize in case this is called on a JsonReport object that has been used.
        indentLevel_ = 0;
        isFirstInList_ = true;
        firstInListStack_ = std::stack <bool>();
        doc_.clear();
        doc_ << "{";
        startNewList();
    }

    void JsonReport::closeDocument() {
        doc_ << "\n}\n";
        endList();
    }

    void JsonReport::openObject(const std::string &objectName) {
        if (isFirstInList_) {
            isFirstInList_ = false;
            //            addIndent();
        } else {
            addComma();
        }
        addNewLine();
        indent();
        addObjectName(objectName);
        addOpenBrace();
        startNewList();
    }

    void JsonReport::openObject() {
        if (isFirstInList_) {
            isFirstInList_ = false;
        } else {
            addComma();
        }
        addNewLine();
        indent();
        addOpenBrace();
        startNewList();
    }

    void JsonReport::closeObject() {
        endList();
        addNewLine();
        indent();
        addCloseBrace();
    }

    // Note that array is not fully implemented or tested.
    // Each element can be added via addQuotedString or addNumber.
    void JsonReport::openArray(const std::string &arrayName) {
        if (isFirstInList_) {
            isFirstInList_ = false;
//            addIndent();
        } else {
            addComma();
        }
        addNewLine();
        indent();
        addObjectName(arrayName);   // named only if it's not at the root.
        addOpenBracket();
        startNewList();
    }

    void JsonReport::closeArray() {
        endList();
        addNewLine();
        indent();
        addCloseBracket();
    }

    void JsonReport::addElementBody(const std::string &name) {
        addNewLine();
        indent();
        addElementName(name);
    }

    void JsonReport::addElement(const std::string &name, const std::string &value) {
        if (!isFirstInList_) {
            addComma();
        } else {
            isFirstInList_ = false;
        }
        addNewLine();
        indent();
        addQuotedString(name);
        addColon();
        addQuotedString(value);
    }

    void JsonReport::addElement(const std::string &name, const std::string_view value) {
        addElement(name, std::string(value));
    }

    void JsonReport::addElement(const std::string &name, const int8_t value) {
        if (!isFirstInList_) {
            addComma();
        } else {
            isFirstInList_ = false;
        }
        addElementBody(name);
        addElementValue(value);
    }

    void JsonReport::addElement(const std::string &name, const uint8_t value) {
        if (!isFirstInList_) {
            addComma();
        } else {
            isFirstInList_ = false;
        }
        addElementBody(name);
        addElementValue(value);
    }

    void JsonReport::addElement(const std::string &name, const int16_t value) {
        if (!isFirstInList_) {
            addComma();
        } else {
            isFirstInList_ = false;
        }
        addElementBody(name);
        addElementValue(value);
    }

    void JsonReport::addElement(const std::string &name, const uint16_t value) {
        if (!isFirstInList_) {
            addComma();
        } else {
            isFirstInList_ = false;
        }
        addElementBody(name);
        addElementValue(value);
    }

    void JsonReport::addElement(const std::string &name, const int32_t value) {
        if (!isFirstInList_) {
            addComma();
        } else {
            isFirstInList_ = false;
        }
        addElementBody(name);
        addElementValue(value);
    }

    void JsonReport::addElement(const std::string &name, const uint32_t value) {
        if (!isFirstInList_) {
            addComma();
        } else {
            isFirstInList_ = false;
        }
        addElementBody(name);
        addElementValue(value);
    }

    void JsonReport::addElement(const std::string &name, const int64_t value) {
        if (!isFirstInList_) {
            addComma();
        } else {
            isFirstInList_ = false;
        }
        addElementBody(name);
        addElementValue(value);
    }

    void JsonReport::addElement(const std::string &name, const uint64_t value) {
        if (!isFirstInList_) {
            addComma();
        } else {
            isFirstInList_ = false;
        }
        addElementBody(name);
        addElementValue(value);
    }

    void JsonReport::addElement(const std::string &name, const float value) {
        if (!isFirstInList_) {
            addComma();
        } else {
            isFirstInList_ = false;
        }
        addElementBody(name);
        addElementValue(value);
    }

    void JsonReport::addElement(const std::string &name, const double value) {
        if (!isFirstInList_) {
            addComma();
        } else {
            isFirstInList_ = false;
        }
        addElementBody(name);
        addElementValue(value);
    }

    void JsonReport::addElement(const std::string &name, const bool value) {
        if (!isFirstInList_) {
            addComma();
        } else {
            isFirstInList_ = false;
        }
        addElementBody(name);
        addElementValue(value);
    }

    void JsonReport::startNewList() {
        firstInListStack_.push(isFirstInList_);
        isFirstInList_ = true;
        addIndent();
    }

    // NOLINTNEXTLINE(clang-analyzer-core.CallAndMessage
    void JsonReport::endList() {
        if (!firstInListStack_.empty()) {
            isFirstInList_ = firstInListStack_.top();
            firstInListStack_.pop();
        }
        subtractIndent();
    }

}
