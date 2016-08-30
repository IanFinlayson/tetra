/* searchbox.cpp
 * custom line edit which only exists to make escape close the box */

#include "searchbox.h"

SearchBox::SearchBox(QWidget*) {
}

/* the whole point of this object is to close search on escape key */
void SearchBox::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Escape) {
        emit closeSearch();
    } else {
        QLineEdit::keyPressEvent(e);
    }
}
