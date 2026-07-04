#ifndef CONFIG_H
#define CONFIG_H

#include "range.h"

const RangeV2     simPosition  = {{-200, -200}, {200,  200}};
const RangeV2     simVelocity  = {0};
const RangeDouble simMass      = {0.1, 10};
const double      simTheta     = 1.2;
const double      simStep      = 1.0 / 60.0;
const int         simIterCount = (1.0 / simStep) * 60;
const int         simBodyCount = 1000;


// int simThreadCountPerProcess[] = {1, 2, 3, 2, 1, 2, 3, 2, 1, 2};
int simThreadCountPerProcess[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

#endif /* CONFIG_H */
