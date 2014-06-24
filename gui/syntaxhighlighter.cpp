#include "syntaxhighlighter.h"

Highlighter::Highlighter(QTextDocument *parent): QSyntaxHighlighter(parent){
    HighlightingRule rule;

    typeFormat.setForeground(QColor(68, 165, 0, 255));
    typeFormat.setFontWeight(QFont::Bold);
    QStringList typePatterns;
    typePatterns << "\\bint\\b" << "\\breal\\b" << "\\bstring\\b" << "\\bbool\\b";

    foreach (const QString &pattern, typePatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = typeFormat;
        highlightingRules.append(rule);
    }

    keywordFormat.setForeground(QColor(197, 113, 0, 255));
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bif\\b" << "\\belif\\b" << "\\belse\\b" << "\\bfor\\b" << "\\bin\\b" << "\\bparallel\\b" <<
                       "\\bwhile\\b" << "\\bcontinue\\b" << "\\bbreak\\b" << "\\bdef\\b" << "\\bor\\b" <<
                       "\\band\\b" << "\\bnot\\b" << "\\bpass\\b" << "\\breturn\\b" << "\\bbackground\\b" << "\\block\\b";

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    valueFormat.setForeground(QColor(180, 37, 18, 255));
    QStringList valuePatterns;
    valuePatterns << "\\btrue\\b" << "\\bfalse\\b" << "[0-9]\\d{0,3}" << "\\d.\\d" << "\".*\"";
    foreach (const QString &pattern, valuePatterns){
        rule.pattern = QRegExp(pattern);
        rule.format = valueFormat;
        highlightingRules.append(rule);
    }

    singleLineCommentFormat.setForeground(QColor(67, 36, 212, 255));
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

