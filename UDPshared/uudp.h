#pragma once
#include <time.h>

typedef struct {
    long long  sequence;
    long long timestamp;
} DataPacket;

// The reason this is in a seperate file is because I want to use this
// on the server size as well#pragma once
