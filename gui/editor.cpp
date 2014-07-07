#include "editor.h"
#include "mainwindow.h"
#include <QPlainTextEdit>
Editor::Editor(QWidget *parent):QPlainTextEdit(parent){
}

void Editor::keyPressEvent(QKeyEvent *e){
    QTextCursor textCursor = this->textCursor();
    if(e->key() == Qt::Key_Tab){
        int pos = textCursor.positionInBlock();
        if((pos+4)%4 == 0 || getLeadingSpaces() < pos){
            textCursor.insertText("    ");
        }
        else{
            while(pos > 4){
                pos-=4;
            }
            for(int i = 0; i < pos; i++){
                textCursor.deletePreviousChar();
            }
            textCursor.insertText("    ");
        }
    }
    else if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return){
        int leadingSpaces = getLeadingSpaces();
        if(textCursor.block().text().endsWith(":")){
            leadingSpaces+=4;
        }
        textCursor.insertText("\n");
        for(int i = leadingSpaces; i >= 4; i-=4){
            textCursor.insertText("    ");
        }
    }
    else if(e->key() == Qt::Key_Backspace || e->key() == Qt::Key_Delete){
        if(isTab("left")){
            int pos = textCursor.positionInBlock();
            while(pos > 4){
                pos-=4;
            }
            for(int i=0; i < pos; i++){
                QPlainTextEdit::keyPressEvent(e);
            }
        }
        else{
            QPlainTextEdit::keyPressEvent(e);
        }
    }
    else if(e->key() == Qt::Key_Left){
        if(isTab("left")){
            int pos = textCursor.positionInBlock();
            while(pos > 4){
                pos-=4;
            }
            for(int i=0; i < pos; i++){
                QPlainTextEdit::keyPressEvent(e);
            }
        }
        else{
            QPlainTextEdit::keyPressEvent(e);
        }
    }
    else if(e->key() == Qt::Key_Right){
        if(isTab("right")){
            for(int i=0; i < 4; i++){
                QPlainTextEdit::keyPressEvent(e);
            }
        }
        else{
            QPlainTextEdit::keyPressEvent(e);
        }
    }
    else{
        QPlainTextEdit::keyPressEvent(e);
    }
}

int Editor::getLeadingSpaces(){
    QTextCursor textCursor = this->textCursor();
    QString line = textCursor.block().text();
    int spaces = 0;
    while(line[spaces].isSpace()){
        spaces++;
    }
    return spaces;
}

bool Editor::isTab(QString direction){
    QTextCursor textCursor = this->textCursor();
    int pos = textCursor.positionInBlock();
    QString line = textCursor.block().text();
    QString substring;
    bool isTab = false;
    if(direction == "left"){
        substring = line.mid(pos-4, 4);
    }
    else{
        substring = line.mid(pos, 4);
    }
    if(substring == "    "){
        isTab = true;
    }
    return isTab;
}
