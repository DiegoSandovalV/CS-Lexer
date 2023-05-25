#ifndef LEXER_H
#define LEXER_H

#include <regex>
#include <string>

class Lexer {
public:

    //Regex for operators
    std::string processOperators(const std::string& content)const {
    std::regex opRegex(R"((\{|\}|\[|\]|\(|\)|\.|,|:|;|\+|-|\*|/|%|&|\||\^|!|~|=|<|>|\?|==|!=|<=|>=|\+=|-=|\*=|/=|%=|&=|\|=|\^=|<<|=>)+)");
    std::string result = std::regex_replace(content, opRegex, "<span class=\'operator\'>$&</span>");
    return result;
}


    //Regex for keywords
 std::string processKeywords(const std::string& content) const{
    std::regex accessTypeRegex(getRegexWithRestriction(R"(\b(public|private|protected)\b)"));
    std::regex dataModifierRegex(getRegexWithRestriction(R"(\b(abstract|const|explicit|extern|implicit|override|readonly|sealed|static|unsafe|virtual|volatile)\b)"));
    std::regex classStructRegex(getRegexWithRestriction(R"(\b(class|struct)\b)"));
    std::regex booleanRegex(getRegexWithRestriction(R"(\b(bool|true|false)\b)"));
    std::regex dataTypeRegex(getRegexWithRestriction(R"(\b(int|double|float|char|void)\b)"));
    
    std::regex keywordRegex(getRegexWithRestriction(R"(\b(as|base|break|byte|case|catch|checked|continue|decimal|default|delegate|do|else|enum|event|finally|fixed|for|foreach|goto|if|in|interface|internal|is|lock|long|namespace|new|null|object|operator|out|params|ref|return|sbyte|short|sizeof|stackalloc|string|switch|this|throw|try|typeof|uint|ulong|unchecked|unsafe|ushort|using|while)\b)"));

    std::string result = std::regex_replace(content, accessTypeRegex, "<span class=\'access-type\'>$&</span>");
    result = std::regex_replace(result, dataModifierRegex, "<span class=\'data-modifier\'>$&</span>");
    result = std::regex_replace(result, classStructRegex, "<span class=\'class-struct\'>$&</span>");
    result = std::regex_replace(result, booleanRegex, "<span class=\'boolean\'>$&</span>");
    result = std::regex_replace(result, dataTypeRegex, "<span class=\'data-type\'>$&</span>");
    result = std::regex_replace(result, keywordRegex, "<span class=\'keyword\'>$&</span>");

    return result;
}

    //Regex for comments
std::string processComments(const std::string& content) const {
    std::string singleLineComment = getRegexRestrictionComments("//") + "[^\n]*\\\n";
    std::string delimitedComment = getRegexRestrictionComments("/\\*") + "(.*?)" + getRegexRestrictionComments("\\*/");
    std::string regexString = "(" + singleLineComment + "|" + delimitedComment + ")";
    std::regex commentRegex(regexString,std::regex::extended);

    std::string result = std::regex_replace(content, commentRegex, "<span class=\'comment\'>$&</span>");
    return result;
}

    //Regex for strings
    std::string processStrings(const std::string& content) const {
    std::regex stringRegex(R"("(?:[^"\\]|\\.)*")");

    std::string result = std::regex_replace(content, stringRegex, "<span class=\'string\'>$&</span>");
    return result;
}

    //Regex for literals
    std::string processLiterals(const std::string& content)const  {
    std::regex binaryRegex(R"(0[bB][01]+)");
    std::regex hexRegex(R"(0[xX][0-9a-fA-F]+)");

    std::string result = content;

    // Process binary literals
    result = std::regex_replace(result, binaryRegex, "<span class=\"binary-literal\">$&</span>");

    // Process hexadecimal literals
    result = std::regex_replace(result, hexRegex, "<span class=\"hex-literal\">$&</span>");

    return result;
}



    //Lex function
  std::string lex(const std::string& content) const {
    std::string result = processOperators(content);
    result = processKeywords(result);
    result = processComments(result);
    result = processStrings(result);
    result = processLiterals(result);
    return result;
}


private:
    //Restriction for span tags
    std::string getRegexWithRestriction(const std::string& regexContent)const {
        return "(?![^<]*>)" + regexContent;
    }

    //Restriction for comments
    std::string getRegexRestrictionComments(const std::string& search)const {
        return "(<[^/>]*>" + search + "</[^>]*>)";
    }
};

#endif // LEXER_H
