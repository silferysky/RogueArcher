#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <vector>

class PerlinNoise 
{
	// The permutation vector
	std::vector<int> m_perlin;
public:
	PerlinNoise(unsigned int seed = 0)
	{
		m_perlin.resize(256);

		// Fill p with values from 0 to 255
		std::iota(m_perlin.begin(), m_perlin.end(), 0);

		// Initialize a random engine with seed
		std::default_random_engine engine(seed);

		// Suffle  using the above random engine
		std::shuffle(m_perlin.begin(), m_perlin.end(), engine);

		// Duplicate the permutation vector
		m_perlin.insert(m_perlin.end(), m_perlin.begin(), m_perlin.end());
	}

	double noise(double x, double y, double z)
	{
		// Find the unit cube that contains the point
		int X = (int)floor(x) & 255;
		int Y = (int)floor(y) & 255;
		int Z = (int)floor(z) & 255;

		// Find relative x, y, z of point in cube
		x -= floor(x);
		y -= floor(y);
		z -= floor(z);

		// Compute fade curves for each of x, y, z
		double u = fade(x);
		double v = fade(y);
		double w = fade(z);

		// Hash coordinates of the 8 cube corners
		int A = m_perlin[X] + Y;
		int AA = m_perlin[A] + Z;
		int AB = m_perlin[A + 1] + Z;
		int B = m_perlin[X + 1] + Y;
		int BA = m_perlin[B] + Z;
		int BB = m_perlin[B + 1] + Z;

		// Add blended results from 8 corners of cube
		double res = lerp(w, lerp(v, lerp(u, grad(m_perlin[AA], x, y, z), grad(m_perlin[BA], x - 1, y, z)), 
									 lerp(u, grad(m_perlin[AB], x, y - 1, z), grad(m_perlin[BB], x - 1, y - 1, z))), 
			         lerp(v, lerp(u, grad(m_perlin[AA + 1], x, y, z - 1), grad(m_perlin[BA + 1], x - 1, y, z - 1)), 
						             lerp(u, grad(m_perlin[AB + 1], x, y - 1, z - 1), grad(m_perlin[BB + 1], x - 1, y - 1, z - 1))));

		return (res + 1.0) / 2.0;
	}
private:
	double fade(double t)
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	double lerp(double t, double a, double b)
	{
		return a + t * (b - a);
	}

	double grad(int hash, double x, double y, double z)
	{
		int h = hash & 15;
		// Convert lower 4 bits of hash into 12 gradient directions
		double u = h < 8 ? x : y,
			v = h < 4 ? y : h == 12 || h == 14 ? x : z;
		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}
};
