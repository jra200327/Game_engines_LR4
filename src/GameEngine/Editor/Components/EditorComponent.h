#ifndef EDITORCOMPONENT_H
#define EDITORCOMPONENT_H

#include <string>

// Компонент-маркер, что объект создан в редакторе
struct EditorComponent
{
    std::string AssetName;  // Имя ассета (текстуры), из которого создан объект
};

#endif // EDITORCOMPONENT_H