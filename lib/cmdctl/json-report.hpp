#pragma once


#include <cstdint>
#include <string>
#include <sstream>
#include <stack>


namespace CScmdctl {

    class JsonReport {

    public:

        JsonReport () = default;
        ~JsonReport () = default;

        std::string getReportDoc() const {return doc_.str();}
        void openDocument();
        void closeDocument();

        void openObject(const std::string &objectName);
        void openObject();      // anonymous object - as in array
        void closeObject();

        void openArray (const std::string &arrayName);
        void closeArray ();

        void addElement (const std::string &name, const std::string &value);
        void addElement(const std::string &name, std::string_view value);
    //    void addElement (const std::string &name, int value);
        void addElement (const std::string &name, int8_t value);
        void addElement(const std::string & name, uint8_t value);
        void addElement (const std::string &name, int16_t value);
        void addElement(const std::string & name, uint16_t value);
        void addElement (const std::string &name, int32_t value);
        void addElement(const std::string & name, uint32_t value);
        void addElement (const std::string &name, int64_t value);
        void addElement(const std::string & name, uint64_t value);
    //    void addElement (const std::string &name, size_t value);
        void addElement (const std::string &name, float value);
        void addElement (const std::string &name, double value);
        void addElement (const std::string &name, bool value);


        void startNewList ();
        void endList ();
        void addObjectName (const std::string& objectName) {
            addQuotedString(objectName);
            addColon();
        }
        void addArrayName (const std::string& arrayName) {
            addObjectName(arrayName);
        }

        void addElementName (const std::string& elementName) {
            addQuotedString(elementName);
            addColon();
        }


    private:

        void addElementBody (const std::string &name);

        void addElementValue (const std::string& elementValue) {
            addQuotedString(elementValue);
        }

        void addElementValue (const float elementValue) {
            doc_ << elementValue;
        }

        void addElementValue (const double elementValue) {
            doc_ << elementValue;
        }

        /*
        void addElementValue (const int elementValue) {
            doc_ << elementValue;
        }
        */

        void addElementValue(const int8_t elementValue) {
            doc_ << static_cast<int>(elementValue);
        }

        void addElementValue(const uint8_t elementValue) {
            doc_ << static_cast<unsigned int>(elementValue);
        }

        void addElementValue(const int16_t elementValue) {
            doc_ << elementValue;
        }

        void addElementValue(const uint16_t elementValue) {
            doc_ << elementValue;
        }

        void addElementValue(const int32_t elementValue) {
            doc_ << elementValue;
        }

        void addElementValue(const uint32_t elementValue) {
            doc_ << elementValue;
        }

        void addElementValue(const int64_t elementValue) {
            doc_ << elementValue;
        }

        void addElementValue(const uint64_t elementValue) {
            doc_ << elementValue;
        }

        // NOLINTNEXTLINE(clang-analyzer-core.CallAndMessage
        void addElementValue (const bool elementValue) {
            doc_ << (elementValue ? "true" : "false");
        }

        void addOpenBrace () {
            doc_ << "{";
        }
        void addCloseBrace () {
            doc_ << "}";
        }
        void addOpenBracket () {
            doc_ << "[";
        }
        void addCloseBracket () {
            doc_ << "]";
        }

        void addQuotedString (const std::string& s) {
            doc_ << "\"" << s << "\"";
        }
        // NOLINTNEXTLINE(clang-analyzer-core.CallAndMessage
        void addUnquotedString (const std::string& s) {
            doc_ << s;
        }
        void addUnquotedSpaces (const uint8_t spaces) {
            for (uint8_t i = 0; i < spaces; ++i) {
                doc_ << " ";
            }
        }
        void addComma () {
            doc_ << ",";
        }
        void addColon () {
            doc_ << ": ";
        }
        void addNewLine () {
            doc_ << "\n";
        }

        void indent (const int count) {
            for (int i = 0; i < count; ++i) {
                doc_ << "    ";
            }
        }
        void indent () {
            indent(indentLevel_);
        }
        void addIndent () {
            ++indentLevel_;
        }
        void subtractIndent () {indentLevel_ > 0 ? --indentLevel_ : indentLevel_ = 0;}

        bool isFirstInList_ = true;
        uint8_t indentLevel_{0};
        std::stringstream doc_{};
        std::stack <bool> firstInListStack_{};
    };

}

