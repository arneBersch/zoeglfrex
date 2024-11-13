#ifndef CUE_H
#define CUE_H

#include <QtWidgets>

#include "item.h"
#include "group.h"
#include "intensity.h"
#include "color.h"

class Cue : public Item {
public:
    Cue(Kernel *core);
    Cue(const Cue* item);
    ~Cue();
    float fade = 0;
    QMap<Group*, Intensity*> intensities = QMap<Group*, Intensity*>();
    QMap<Group*, Color*> colors = QMap<Group*, Color*>();
};

#endif // CUE_H
