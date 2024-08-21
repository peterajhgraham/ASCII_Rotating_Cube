#include <cmath>
#include <cstdio>
#include <cstring>
#include <unistd.h>

const int width = 160, height = 44;
char buffer[width * height];
float zBuffer[width * height];
const int backgroundASCIICode = ' ';
const float K1 = 40;
const int distanceFromCam = 100;
const float cubeSize = 20;
float A = 0.0f, B = 0.0f; // Rotation angles

// Rotate a 3D point around the X axis
void rotateX(float &y, float &z, float angle) {
    float newY = y * cos(angle) - z * sin(angle);
    float newZ = y * sin(angle) + z * cos(angle);
    y = newY;
    z = newZ;
}

// Rotate a 3D point around the Y axis
void rotateY(float &x, float &z, float angle) {
    float newX = x * cos(angle) + z * sin(angle);
    float newZ = -x * sin(angle) + z * cos(angle);
    x = newX;
    z = newZ;
}

// Project a 3D point onto a 2D screen
void projectPoint(float x, float y, float z, int &xp, int &yp, float &ooz) {
    ooz = 1 / (z + distanceFromCam);
    xp = (int)(width / 2 + K1 * ooz * x * 2);
    yp = (int)(height / 2 - K1 * ooz * y); // Y-axis should be inverted
}

// Render a line between two 3D points on the cube
void renderLine(float x1, float y1, float z1, float x2, float y2, float z2, char ch) {
    // Rotate the points in 3D space
    rotateX(y1, z1, A);
    rotateY(x1, z1, B);
    rotateX(y2, z2, A);
    rotateY(x2, z2, B);

    // Project the points to 2D
    int xp1, yp1, xp2, yp2;
    float ooz1, ooz2;
    projectPoint(x1, y1, z1, xp1, yp1, ooz1);
    projectPoint(x2, y2, z2, xp2, yp2, ooz2);

    // Bresenham's line algorithm for 2D line drawing
    int dx = abs(xp2 - xp1), sx = xp1 < xp2 ? 1 : -1;
    int dy = -abs(yp2 - yp1), sy = yp1 < yp2 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        int idx = xp1 + yp1 * width;
        if (xp1 >= 0 && xp1 < width && yp1 >= 0 && yp1 < height) {
            if (ooz1 > zBuffer[idx]) {
                zBuffer[idx] = ooz1;
                buffer[idx] = ch;
            }
        }
        if (xp1 == xp2 && yp1 == yp2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; xp1 += sx; }
        if (e2 <= dx) { err += dx; yp1 += sy; }
    }
}

int main() {
    printf("\x1b[2J");

    while (true) {
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * sizeof(float));

        // Rotate the cube
        A += 0.04f;
        B += 0.03f;

        // Define the cube's corners
        float cube[8][3] = {
            {-cubeSize, -cubeSize, -cubeSize},
            { cubeSize, -cubeSize, -cubeSize},
            { cubeSize,  cubeSize, -cubeSize},
            {-cubeSize,  cubeSize, -cubeSize},
            {-cubeSize, -cubeSize,  cubeSize},
            { cubeSize, -cubeSize,  cubeSize},
            { cubeSize,  cubeSize,  cubeSize},
            {-cubeSize,  cubeSize,  cubeSize},
        };

        // Draw the cube edges
        renderLine(cube[0][0], cube[0][1], cube[0][2], cube[1][0], cube[1][1], cube[1][2], '#');
        renderLine(cube[1][0], cube[1][1], cube[1][2], cube[2][0], cube[2][1], cube[2][2], '#');
        renderLine(cube[2][0], cube[2][1], cube[2][2], cube[3][0], cube[3][1], cube[3][2], '#');
        renderLine(cube[3][0], cube[3][1], cube[3][2], cube[0][0], cube[0][1], cube[0][2], '#');
        renderLine(cube[4][0], cube[4][1], cube[4][2], cube[5][0], cube[5][1], cube[5][2], '#');
        renderLine(cube[5][0], cube[5][1], cube[5][2], cube[6][0], cube[6][1], cube[6][2], '#');
        renderLine(cube[6][0], cube[6][1], cube[6][2], cube[7][0], cube[7][1], cube[7][2], '#');
        renderLine(cube[7][0], cube[7][1], cube[7][2], cube[4][0], cube[4][1], cube[4][2], '#');
        renderLine(cube[0][0], cube[0][1], cube[0][2], cube[4][0], cube[4][1], cube[4][2], '#');
        renderLine(cube[1][0], cube[1][1], cube[1][2], cube[5][0], cube[5][1], cube[5][2], '#');
        renderLine(cube[2][0], cube[2][1], cube[2][2], cube[6][0], cube[6][1], cube[6][2], '#');
        renderLine(cube[3][0], cube[3][1], cube[3][2], cube[7][0], cube[7][1], cube[7][2], '#');

        // Display the frame
        printf("\x1b[H");
        for (int i = 0; i < width * height; i++) {
            putchar(buffer[i]);
            if (i % width == width - 1) putchar('\n');
        }

        usleep(8000); // Control frame rate
    }

    return 0;
}
