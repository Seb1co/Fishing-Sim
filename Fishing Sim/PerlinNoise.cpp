#include "PerlinNoise.h"

#include <iostream>
#include <cmath>
#include <random>
using namespace std;

// -----------------------------------------
// Constructor PerlinNoise
// Creates a permutation table for noise generation
// -----------------------------------------
PerlinNoise::PerlinNoise(int seed) :  Seed(seed){
    Permutation = MakePermutation(seed);
}

// -----------------------------------------
// Vector2 implementation
// Stores a 2D vector and provides dot product
// -----------------------------------------
PerlinNoise::Vector2::Vector2(double x, double y) : x(x), y(y) {}

double PerlinNoise::Vector2::dot(const Vector2& other) const {
    return x * other.x + y * other.y;  // dot product between 2 vectors
}

// -----------------------------------------
// Shuffle function
// Randomly shuffles a vector for permutation table randomness
// -----------------------------------------
void PerlinNoise::Shuffle(vector<int>& arrayToShuffle) {
    for (int e = arrayToShuffle.size() - 1; e > 0; --e) {

        // Generate a random index between 0 and e-1
        int index = round((double)rand() / RAND_MAX * (e - 1));

        // Swap values for randomization
        swap(arrayToShuffle[e], arrayToShuffle[index]);
    }
}

// -----------------------------------------
// MakePermutation
// Creates a 512-entry permutation table (256 repeated twice)
// Used to simulate infinite wrapping for noise
// -----------------------------------------
vector<int> PerlinNoise::MakePermutation(int seed) {
    vector<int> permutation;

    // Fill with numbers 0..255
    for (int i = 0; i < this->Seed ; i++)
        permutation.push_back(i);

    // Shuffle the first 256 entries
    Shuffle(permutation);

    // Duplicate them to get 512 entries
    for (int i = 0; i < this->Seed ; i++)
        permutation.push_back(permutation[i]);

    return permutation;
}

// -----------------------------------------
// GetConstantVector
// Returns one of 4 possible gradient vectors based on input hash
// -----------------------------------------
PerlinNoise::Vector2 PerlinNoise::GetConstantVector(int v) {
    int h = v & 3; // mask last 2 bits -> 0..3

    if (h == 0) return Vector2(1.0, 1.0);
    else if (h == 1) return Vector2(-1.0, 1.0);
    else if (h == 2) return Vector2(-1.0, -1.0);
    else return Vector2(1.0, -1.0);
}

// -----------------------------------------
// Fade function
// Smooths transitions using a quintic polynomial
// Important for smooth Perlin noise gradients
// -----------------------------------------
double PerlinNoise::Fade(double t) {
    return ((6 * t - 15) * t + 10) * t * t * t;
}

// -----------------------------------------
// Linear interpolation (lerp)
// Blends between a1 and a2 using weight t
// -----------------------------------------
double PerlinNoise::Lerp(double t, double a1, double a2) {
    return a1 + t * (a2 - a1);
}

// -----------------------------------------
// Noise2D
// Generates classic Perlin Noise at (x,y)
// Steps:
// 1. Identify the cell the point is in
// 2. Compute gradient vectors at corners
// 3. Compute dot products between gradients and distance vectors
// 4. Interpolate results for smooth output
// -----------------------------------------
double PerlinNoise::Noise2D(double x, double y) {

    // Determine which cell (grid square) the point is inside
    int X = ((int)floor(x)) & 255;
    int Y = ((int)floor(y)) & 255;

    // Fractional part inside the cell
    double xf = x - floor(x);
    double yf = y - floor(y);

    // Vectors to the 4 corners of the cell
    Vector2 topRight(xf - 1.0, yf - 1.0);
    Vector2 topLeft(xf, yf - 1.0);
    Vector2 bottomRight(xf - 1.0, yf);
    Vector2 bottomLeft(xf, yf);

    // Look up gradient vector hashes using permutation table
    int valueTopRight = Permutation[Permutation[X + 1] + Y + 1];
    int valueTopLeft = Permutation[Permutation[X] + Y + 1];
    int valueBottomRight = Permutation[Permutation[X + 1] + Y];
    int valueBottomLeft = Permutation[Permutation[X] + Y];

    // Compute dot products between corner gradients and distance vectors
    double dotTopRight = topRight.dot(GetConstantVector(valueTopRight));
    double dotTopLeft = topLeft.dot(GetConstantVector(valueTopLeft));
    double dotBottomRight = bottomRight.dot(GetConstantVector(valueBottomRight));
    double dotBottomLeft = bottomLeft.dot(GetConstantVector(valueBottomLeft));

    // Smooth interpolation weights
    double u = Fade(xf);
    double v = Fade(yf);

    // Bilinear interpolation using fade curves
    return Lerp(u,
        Lerp(v, dotBottomLeft, dotTopLeft),
        Lerp(v, dotBottomRight, dotTopRight)
    );
}

// -----------------------------------------
// Fractal Brownian Motion (fBm)
// Combines multiple octaves of Perlin Noise
// - amplitude decreases each octave
// - frequency increases
// Creates more natural terrain
// -----------------------------------------
int PerlinNoise::FractalBrownianMotion(int x, int y, int numOctaves)
{
    double result = 0.0;
    // Amplitude controls how strong each noise layer (octave) is.
    // Higher amplitude = stronger contribution from that octave → larger hills and depth.
    double amplitude = 1.0;
    // Frequency controls the size of noise patterns.
    // Lower frequency = large smooth areas; higher frequency = fine details.
    double frequency = 0.1;

    for (int octave = 0; octave < numOctaves; ++octave)
    {
        // Generate noise at given frequency and scale it by amplitude
        double n = amplitude * Noise2D(x * frequency, y * frequency);
        result += n;

        // Reduce amplitude and increase frequency for next octave
        amplitude *= 0.5;
        frequency *= 2.0;
    }

    // Multiply by 100 and convert to int for simpler terrain values
    return int(result * 100);
}

// -----------------------------------------
// generateMatrix
// Creates a 2D matrix of width x height noise values
// Used for generating a heightmap or terrain map
// -----------------------------------------
std::vector<std::vector<int>> PerlinNoise::generateMatrix(int width, int height, int octaves)
{
    // Create empty matrix (height rows, width columns)
    std::vector<std::vector<int>> matrix(
        height,
        std::vector<int>(width, 0.0)
    );
    if (width % 2 == 0 || height % 2 == 0) {
        // Fill matrix with fBm noise values
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {

                // Generate noise value for each cell
                matrix[y][x] = (x == width / 2 - 1 || x == width / 2) and (y == width / 2 - 1 || y == width / 2) ? 200 : FractalBrownianMotion(x, y, octaves);
            }
        }
    }
    else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Generate noise value for each cell
                matrix[y][x] = (x == int(width / 2) + 1) and (y == int(width / 2) + 1) ? 200 : FractalBrownianMotion(x, y, octaves);
            }
        }
    }
    return matrix;
}
