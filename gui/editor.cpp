#include "editor.h"
#include <QPlainTextEdit>
Editor::Editor(QWidget *parent):QPlainTextEdit(parent){
    connect(this, SIGNAL(cursorPositionChanged()),
         this, SLOT(updateCursorCoordinates()));
}

void Editor::keyPressEvent(QKeyEvent *e){
    cursor = this->textCursor();
    if(e->key() == Qt::Key_Tab){
        int pos = cursor.positionInBlock();
        if((pos+4)%4 == 0 || getLeadingSpaces() < pos){
            cursor.insertText("    ");
        }
        else{
            while(pos > 4){
                pos-=4;
            }
            for(int i = 0; i < pos; i++){
                cursor.deletePreviousChar();
            }
            cursor.insertText("    ");
        }
    }
    else if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return){
        int leadingSpaces = getLeadingSpaces();
        if(cursor.block().text().endsWith(":")){
            leadingSpaces+=4;
        }
        cursor.insertText("\n");
        for(int i = leadingSpaces; i >= 4; i-=4){
            cursor.insertText("    ");
        }
    }
    else if(e->key() == Qt::Key_Backspace || e->key() == Qt::Key_Delete){
        if(isTab("left")){
            int pos = cursor.positionInBlock();
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
            int pos = cursor.positionInBlock();
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
    cursor = this->textCursor();
    QString line = cursor.block().text();
    int spaces = 0;
    while(line[spaces].isSpace()){
        spaces++;
    }
    return spaces;
}

bool Editor::isTab(QString direction){
    cursor = this->textCursor();
    int pos = cursor.positionInBlock();
    QString line = cursor.block().text();
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

void Editor::updateCursorCoordinates(){
    cursor = this->textCursor();
    QString x = QString::number(cursor.blockNumber()+1);
    QString y = QString::number(cursor.positionInBlock());
    coordinates = x + ", " + y;
}

QString Editor::getCoordinates(){
    return coordinates;
}


