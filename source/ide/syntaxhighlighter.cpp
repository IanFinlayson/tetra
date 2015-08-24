#include "syntaxhighlighter.h"

// the colors used for highlighting
const unsigned int COLOR_TYPE = 0xBF9407;
const unsigned int COLOR_KEYWORD = 0x0315B6;
const unsigned int COLOR_PARALLEL = 0x0315B6;
const unsigned int COLOR_VALUE = 0xBB002B;
const unsigned int COLOR_STRING = 0xBB002B;
const unsigned int COLOR_COMMENT = 0x007231; 

// build a QColor from a 24-bit hexadecimal color value
QColor colorFromHex(unsigned int color) {
  int r = (color & 0xFF0000) >> 16;
  int g = (color & 0x00FF00) >> 8;
  int b = color & 0x0000FF;
  return QColor(r, g, b, 255);
}

// set up the highlighting
Highlighter::Highlighter(QTextDocument *parent): QSyntaxHighlighter(parent){
    HighlightingRule rule;

    // types
    typeFormat.setForeground(colorFromHex(COLOR_TYPE));
    QStringList typePatterns;
    typePatterns << "\\bint\\b" << "\\breal\\b" << "\\bstring\\b" << "\\bbool\\b";
    foreach (const QString &pattern, typePatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = typeFormat;
        highlightingRules.append(rule);
    }

    // keywords
    keywordFormat.setForeground(colorFromHex(COLOR_KEYWORD));
    QStringList keywordPatterns;
    keywordPatterns << "\\bif\\b" << "\\belif\\b" << "\\belse\\b" << "\\bfor\\b" << "\\bin\\b"  <<
                       "\\bwhile\\b" << "\\bcontinue\\b" << "\\bbreak\\b" << "\\bdef\\b" << "\\bor\\b" <<
                       "\\band\\b" << "\\bnot\\b" << "\\bpass\\b" << "\\breturn\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }


    // parallel stuff
    parallelFormat.setForeground(colorFromHex(COLOR_PARALLEL));
    QStringList parallelPatterns;
    parallelPatterns << "\\bparallel\\b"<< "\\bbackground\\b" << "\\block\\b";
    foreach (const QString &pattern, parallelPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = parallelFormat;
        highlightingRules.append(rule);
    }


    // values
    valueFormat.setForeground(colorFromHex(COLOR_VALUE));
    QStringList valuePatterns;
    valuePatterns << "\\btrue\\b" << "\\bfalse\\b" << "[0-9]\\d{0,3}" << "\\d.\\d";
    foreach (const QString &pattern, valuePatterns){
        rule.pattern = QRegExp(pattern);
        rule.format = valueFormat;
        highlightingRules.append(rule);
    }

    // strings
    quotationFormat.setForeground(colorFromHex(COLOR_STRING));
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // comments
    singleLineCommentFormat.setForeground(colorFromHex(COLOR_COMMENT));
    rule.pattern = QRegExp("#[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

}

void Highlighter::highlightBlock(const QString &text){
    foreach (const HighlightingRule &rule, highlightingRules){
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}

