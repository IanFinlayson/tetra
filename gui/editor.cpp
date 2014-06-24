#include "editor.h"
#include "mainwindow.h"
#include <QPlainTextEdit>
Editor::Editor(QWidget *parent):QPlainTextEdit(parent){
}

void Editor::keyPressEvent(QKeyEvent *e){
    QTextCursor textCursor = this->textCursor();
    if(e->key() == Qt::Key_Tab){
        textCursor.insertText("    ");
    }
    else if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return){
        int leadingSpaces = getLeadingSpaces(textCursor.block().text());
        textCursor.insertText("\n");
        for(int i = 1; i <= leadingSpaces; i++){
            textCursor.insertText(" ");
        }
    }
    else{
        QPlainTextEdit::keyPressEvent(e);
    }
}

int Editor::getLeadingSpaces(QString line){
    int spaces = 0;
    while(line[spaces].isSpace()){
        spaces++;
    }
    return spaces;
}
