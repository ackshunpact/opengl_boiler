#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <vector>
#include <iostream>
#include <algorithm>
namespace engine::primitives {

	// TriangleGrid.h
	struct TerrainParams {
		float maskFreq = 0.006f;
		float maskLow = 0.45f, maskHigh = 0.65f;
		float plainsHeight = 2.0f;
		float mountainHeight = 60.0f;
		float peakExp = 2.0f;
		int   ridgedOctaves = 5;
		float detailLow = 0.2f, detailHigh = 4.0f;
	};

	inline float fbm(glm::vec2 p, int octaves = 3, float lacunarity = 2.0f, float gain = 0.5f) {
		float sum = 0.0f, amp = 1.0f, freq = 1.f, norm = 0.0f;
		for (int o = 0; o < octaves; ++o) {
			sum += amp * glm::perlin(p * freq);
			norm += amp;
			amp *= gain;
			freq *= lacunarity;
		}
		return sum / norm;
	}
	inline float ridged(glm:: vec2 p, int octaves = 2) {
		float sum = 0, amp = 0.5f, freq = 1, prev = 1;
		for (int o = 0; o < octaves; ++o) {
			float n = 1.0f - std::abs(glm::perlin(p * freq));
			n *= n;
			sum += n * amp * prev;
			prev = n;
			amp *= 0.5f; freq *= 2.0f;
		}
		return sum;
	}

	inline float terrainHeight(float x, float z, const TerrainParams& tp) {
		glm::vec2 p(x, z);
		float region = glm::smoothstep(tp.maskLow, tp.maskHigh, glm::perlin(p * tp.maskFreq) * 0.5f + 0.5f);
		region *= region;
		float plains = tp.plainsHeight * fbm(p * 0.01f, 3, 2.0f, 0.4f);
		float m = ridged(p * tp.maskFreq, tp.ridgedOctaves);
		m = std::pow(m, tp.peakExp);
		float mountains = tp.mountainHeight * m;
		return glm::mix(plains, mountains, region);
	}


	
	inline MeshData triangleGrid(int gridSize, const TerrainParams& tp) {
		const float frequency = 0.3f;
		const float amplitude = 28.0f;
		MeshData md;

		auto height = [&](int i, int j) {
			i = std::clamp(i, 0, gridSize - 1);
			j = std::clamp(j, 0, gridSize - 1);
			return md.vertices[j * gridSize + i].position.y;
			};


		for (int j = 0; j < gridSize; ++j) {
			for (int i = 0; i < gridSize; ++i) {
				float h = terrainHeight(float(i), float(j), tp);
				md.vertices.push_back(Vertex(glm::vec3(float(i), h, float(j)), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(float(i), float(j))));
			}
		}

		for (int j = 0; j < gridSize - 1; ++j) {
			for (int i = 0; i < gridSize - 1; ++i) {
				uint32_t tl = j * gridSize + i;
				uint32_t tr = tl + 1;
				uint32_t bl = tl + gridSize;
				uint32_t br = bl + 1;

				md.indices.push_back(tl);
				md.indices.push_back(bl);
				md.indices.push_back(tr);

				md.indices.push_back(tr);
				md.indices.push_back(bl);
				md.indices.push_back(br);
			}
		}

		const float space = 1.0f;
		for (int j = 0; j < gridSize; ++j) {
			for (int i = 0; i < gridSize; ++i) {
				float hxm = terrainHeight(i - 1, j, tp), hxp = terrainHeight(i + 1, j, tp);
				float hzm = terrainHeight(i, j - 1, tp), hzp = terrainHeight(i, j + 1, tp);
	/*			float hxm = height(i - 1, j), hxp = height(i + 1, j);
				float hzm = height(i, j - 1), hzp = height(i, j + 1);*/
				glm::vec3 n(hxm - hxp, 2.0f * space, hzm - hzp);
				md.vertices[j * gridSize + i].normal = glm::normalize(n);
			}
		}

		return md;
	}


}