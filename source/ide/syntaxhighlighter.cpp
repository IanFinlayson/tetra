#include "syntaxhighlighter.h"

// the colors used for highlighting
const unsigned int COLOR_TYPE = 0xb28c10;
const unsigned int COLOR_KEYWORD = 0x004088;
const unsigned int COLOR_BUILTIN = 0x73378c;
const unsigned int COLOR_VALUE = 0xaa0000;
const unsigned int COLOR_STRING = 0xaa0000;
const unsigned int COLOR_COMMENT = 0x1A5D11;

// build a QColor from a 24-bit hexadecimal color value
QColor colorFromHex(unsigned int color) {
  int r = (color & 0xFF0000) >> 16;
  int g = (color & 0x00FF00) >> 8;
  int b = color & 0x0000FF;
  return QColor(r, g, b, 255);
}

// set up the highlighting
Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
  HighlightingRule rule;

  // types
  typeFormat.setForeground(colorFromHex(COLOR_TYPE));
  QStringList typePatterns;
  typePatterns << "\\bint\\b"
               << "\\breal\\b"
               << "\\bstring\\b"
               << "\\bbool\\b"
               << "\\btask\\b"
               << "\\bmutex\\b";
  foreach (const QString &pattern, typePatterns) {
    rule.pattern = QRegExp(pattern);
    rule.format = typeFormat;
    highlightingRules.append(rule);
  }

  // keywords
  keywordFormat.setForeground(colorFromHex(COLOR_KEYWORD));
  QStringList keywordPatterns;
  keywordPatterns << "\\bif\\b"
                  << "\\bclass\\b"
                  << "\\bdef\\b"
                  << "\\bif\\b"
                  << "\\belif\\b"
                  << "\\belse\\b"
                  << "\\bwhile\\b"
                  << "\\bfor\\b"
                  << "\\bin\\b"
                  << "\\bparallel\\b"
                  << "\\breturn\\b"
                  << "\\bopen\\b"
                  << "\\bimport\\b"
                  << "\\blambda\\b"
                  << "\\bbackground\\b"
                  << "\\bwait\\b"
                  << "\\block\\b"
                  << "\\binit\\b"
                  << "\\bglobal\\b"
                  << "\\binit\\b"
                  << "\\bself\\b"
                  << "\\bconstant\\b"
                  << "\\bnone\\b"
                  << "\\bas\\b";

  foreach (const QString &pattern, keywordPatterns) {
    rule.pattern = QRegExp(pattern);
    rule.format = keywordFormat;
    highlightingRules.append(rule);
  }

  // builtin stuff
  builtinFormat.setForeground(colorFromHex(COLOR_BUILTIN));
  QStringList builtinPatterns;
  builtinPatterns << "\\bprint\\b"
                  << "\\blen\\b"
                  << "\\bread_int\\b"
                  << "\\bread_string\\b"
                  << "\\bread_bool\\b"
                  << "\\bread_real\\b"
                  << "\\barray\\b"
                  << "\\bkeys\\b";
  foreach (const QString &pattern, builtinPatterns) {
    rule.pattern = QRegExp(pattern);
    rule.format = builtinFormat;
    highlightingRules.append(rule);
  }

  // values
  valueFormat.setForeground(colorFromHex(COLOR_VALUE));
  QStringList valuePatterns;
  valuePatterns << "\\btrue\\b"
                << "\\bfalse\\b"
                << "[0-9]\\d{0,3}"
                << "\\d.\\d";
  foreach (const QString &pattern, valuePatterns) {
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

void Highlighter::highlightBlock(const QString &text) {
  foreach (const HighlightingRule &rule, highlightingRules) {
    QRegExp expression(rule.pattern);
    int index = expression.indexIn(text);
    while (index >= 0) {
      int length = expression.matchedLength();
      setFormat(index, length, rule.format);
      index = expression.indexIn(text, index + length);
    }
  }
}
