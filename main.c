#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#define  M_C  10

const double FPS = 60;
const double FTM = (double)1 / 60;

typedef struct {
    double x;
    double y;
} vector2;

typedef struct {
    vector2 position;
    vector2 velocity;
    double  intensity;
} photon;

vector2 v2(double x, double y) { return (vector2) { .x = x, .y = y }; }

double distance(vector2 a, vector2 b) {
    double dist = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    return dist;
}

photon* linearPhotonStream(int amt, vector2 position, vector2 direction, int freq) {
    photon* stream = (photon*)malloc(sizeof(photon) * amt);
    
    double gradient = direction.y / direction.x;
    double horizontalFreq = (direction.x / amt);

    vector2 speed = v2(cos(direction.x / M_C), sin(direction.y / M_C));

    for (int i = 0; i < amt; i++) {
        double x = horizontalFreq * i;
        double p = gradient * x + position.y;

        stream[i] = (photon) {
            .position = v2(x, p),
            .velocity = v2(speed.x, speed.y),
            .intensity = 1,
        };

        // printf("(%f, %f)\n", x, p);              // UNCOMMENT FOR POSITION DEBUG
        // printf("(%f, %f)\n", speed.x, speed.y);  // UNCOMMENT FOR VELOCITY DEBUG
    }

    return stream;
}

void update(photon* stream, int amt) {
    double last = time(0);
    int    framecount = 0;

    while (true) {
        double current = time(0);
        double delta = current - last;


        stream->position.y += 1;
        printf("%f\n", delta);


        last = current;

        double elapsedFrameTime = time(0) - current;
        double remainingFrameTime = FTM - elapsedFrameTime;

        if (remainingFrameTime > 0)
            usleep((useconds_t)(remainingFrameTime * 1000000.0));
    }
}

int main() {
    int amt = 10;
    double freq = 1;
    
    vector2 origin = v2(0, 0);
    vector2 direction = v2(0.1, 0.23);

    photon* stream = linearPhotonStream(amt, origin, direction, freq);

    update(stream, amt);

    free(stream);
    return 0;
}