#ifndef MOVEINPUTEVENT_H
#define MOVEINPUTEVENT_H

#include <SFML/System/InputStream.hpp>

enum class MoveDirection
{
    Left,
    Right,
    None
};

struct MoveInputEvent
{
    MoveDirection Direction;
};

#endif //MOVEINPUTEVENT_H
