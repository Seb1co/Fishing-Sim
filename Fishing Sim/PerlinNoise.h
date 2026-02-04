#pragma once

#include <vector>

class PerlinNoise {
private:
    int Seed;
    // ----------------------------
    // Structura Vector2
    // ----------------------------
    class Vector2 {
    public:
        double x, y;
        Vector2(double x, double y);
        double dot(const Vector2& other) const;
    };
    // ----------------------------
    // Functii interne Perlin Noise
    // ----------------------------
    void Shuffle(std::vector<int>& arrayToShuffle);
	std::vector<int> MakePermutation(int seed); // seed variant

    std::vector<int> Permutation; // tabelul de permutari

    Vector2 GetConstantVector(int v);
    double Fade(double t);
    double Lerp(double t, double a1, double a2);

    double Noise2D(double x, double y);

    int FractalBrownianMotion(int x, int y, int numOctaves);

public:
    PerlinNoise() = default; // constructor
	PerlinNoise(int seed); // seed variant constructor

    std::vector<std::vector<int>> generateMatrix(
        int width,
        int height,
        int octaves
    );
};
