#include <cmath>
#include <cstdio>
#include <cstring>
#include <unistd.h>

const int width = 160, height = 44;
char buffer[width * height];
float zBuffer[width * height];
const int backgroundASCIICode = ' ';
const float K1 = 60;
const int distanceFromCam = 100;
const float sphereRadius = 20;
float lightDir[3] = {0.0f, 0.0f, -1.0f}; // Light direction
float A = 0.0f, B = 0.0f; // Rotation angles

// Calculate dot product of two vectors
float dotProduct(float vec1[3], float vec2[3]) {
    return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

// Normalize a vector
void normalize(float vec[3]) {
    float length = sqrt(dotProduct(vec, vec));
    vec[0] /= length;
    vec[1] /= length;
    vec[2] /= length;
}

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

// Calculate and render a point on the sphere
void renderPoint(float sphereX, float sphereY, float sphereZ, char ch) {
    // Rotate the point in 3D space
    rotateX(sphereY, sphereZ, A);
    rotateY(sphereX, sphereZ, B);

    // Project the point to 2D
    int xp, yp;
    float ooz;
    projectPoint(sphereX, sphereY, sphereZ, xp, yp, ooz);

    // Calculate shading based on the angle to the light source
    float normal[3] = {sphereX, sphereY, sphereZ};
    normalize(normal);
    float lightIntensity = dotProduct(normal, lightDir);

    // Map light intensity to a character
    char shade = ' ';
    if (lightIntensity > 0.7f) shade = '@';
    else if (lightIntensity > 0.4f) shade = '#';
    else if (lightIntensity > 0.2f) shade = '*';
    else if (lightIntensity > 0.1f) shade = '.';

    int idx = xp + yp * width;
    if (xp >= 0 && xp < width && yp >= 0 && yp < height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = shade;
        }
    }
}

int main() {
    printf("\x1b[2J");
    normalize(lightDir);

    while (true) {
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * sizeof(float));

        // Rotate the sphere
        A += 0.04f;
        B += 0.03f;

        // Render the sphere surface
        for (float theta = 0; theta < 2 * M_PI; theta += 0.02f) {
            for (float phi = 0; phi < M_PI; phi += 0.02f) {
                float x = sphereRadius * sin(phi) * cos(theta);
                float y = sphereRadius * sin(phi) * sin(theta);
                float z = sphereRadius * cos(phi);
                renderPoint(x, y, z, '@');
            }
        }

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
