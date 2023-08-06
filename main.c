#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#define M_C 10

const double FPS = 60;
const double FTM = (double)1 / 60;

typedef struct {
    double x;
    double y;
} vector2;

typedef struct {
    vector2 pointA;
    vector2 pointB;
    double  gradient;
} line;

typedef struct {
    vector2 position;
    vector2 velocity;
    double  intensity;
} photon;

vector2 v2(double x, double y) {
    return (vector2) { x, y };
}

line segment(vector2 pointA, vector2 pointB) {
    return (line) { .pointA = pointA, .pointB = pointB,
                    .gradient = (pointB.y - pointA.y) / (pointB.x - pointA.x) };
}

double distance(vector2 a, vector2 b) {
    double dist = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    return dist;
}

photon* linearPhotonStream(int amt, vector2 position, vector2 direction, double freq) {
    photon* stream = (photon*)malloc(sizeof(photon) * amt);

    double  horizontalFreq = (direction.x / amt) / freq;
    double  gradient       = direction.y / direction.x;
    double  magnitude      = sqrt(direction.x * direction.x + direction.y * direction.y);

    vector2 speed = v2(direction.x / magnitude, direction.y / magnitude);

    for (int i = 0; i < amt; i++) {
        double x = horizontalFreq * i;
        double p = gradient * x + position.y;

        stream[i] = (photon) {
            .position  = v2(x, p),
            .velocity  = speed,
            .intensity = 1,
        };
    }

    return stream;
}

void update(photon* stream, int amt) {
    for (int framecount = 0; framecount < amt; framecount++) {
        clock_t last = clock();
        clock_t current = last;

        while ((double)(current - last) / CLOCKS_PER_SEC < FTM)
            current = clock();

        double delta = (double)(current - last) / CLOCKS_PER_SEC;

        for (int i = 0; i < amt; i++) {
            stream[i].position.x += stream[i].velocity.x * delta;
            stream[i].position.y += stream[i].velocity.y * delta;
        }
    }
}

int main() {
    line    retina    = segment(v2(10, 10), v2(10, 0));   // collision with this object is sensed
    vector2 origin    = v2(0, 0);                         // beginning of the photon stream
    vector2 direction = v2(0.1, 0.23);                    // vector2 version of the gradient of the line

    int     amt    = 5;
    double  freq   = 10;
    photon* stream = linearPhotonStream(amt, origin, direction, freq);

    update(stream, 60 * 1);

    free(stream);
    return 0;
}

