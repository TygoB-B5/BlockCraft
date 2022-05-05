#pragma once

#include "glm/glm.hpp"
#include "core.h"

namespace glr
{

	class perlinnoise1d
	{
	public:

		perlinnoise1d(float seed, uint32_t size = 256, uint32_t octaves = 1, float bias = 2.0f)
		{
			set(seed, size, octaves, bias);
		}

		~perlinnoise1d()
		{
			delete(_seeds);
			delete(_outputs);
		}

		void set(float seed, uint32_t size, uint32_t octaves, float bias)
		{
			_size = size;
			_seeds = new float[size];
			_outputs = new float[size];


			srand(seed);

			for (size_t i = 0; i < size; i++)
			{
				_seeds[i] = (float)rand() / (float)RAND_MAX;
			}


			for (size_t x = 0; x < size; x++)
			{
				float noise = 0.0f;
				float scale = 1.0f;
				float scaleAcc = 0.0f;

				for (size_t o = 0; o < octaves; o++)
				{
					int pitch = size >> o;
					int sample = (x / pitch) * pitch;
					int sample2 = (sample + pitch) % size;

					float blend = (float)(x - sample) / (float)pitch;
					float fSample = (1.0f - blend) * _seeds[sample] + blend * _seeds[sample2];
					noise += fSample * scale;
					scaleAcc += scale;

					bias < 0.02f ? bias = 0.02f : bias;
					scale = scale / bias;
				}

				_outputs[x] = noise / scaleAcc;
			}
		}

		float getHeightAtPosition(uint32_t x)
		{
			x %= _size;
			return _outputs[x];
		}

	private:

		float* _seeds;
		float* _outputs;
		uint32_t _size;
	};

	class perlinnoise2d
	{
	public:

		perlinnoise2d(float seed = NULL, uint32_t size = 256, uint32_t octaves = 3, float bias = 2.0f)
		{
			set(seed, size, octaves, bias);
		}

		~perlinnoise2d()
		{
			delete(_seeds);
			delete(_outputs);
		}

		void set(float seed, uint32_t size, uint32_t octaves, float bias)
		{
			_size = size;
			_seeds = new float[size * size];
			_outputs = new float[size * size];

			srand(seed);

			for (size_t i = 0; i < size * size; i++)
			{
				_seeds[i] = (float)rand() / (float)RAND_MAX;
			}


			for (size_t x = 0; x < size; x++)
			{
				for (size_t y = 0; y < size; y++)
				{
					float noise = 0.0f;
					float scale = 1.0f;
					float scaleAcc = 0.0f;

					for (size_t o = 0; o < octaves; o++)
					{
						int pitch = size >> o;

						int samplex = (x / pitch) * pitch;
						int sampley = (y / pitch) * pitch;

						int sample2x = (samplex + pitch) % size;
						int sample2y = (sampley + pitch) % size;


						float blendx = (float)(x - samplex) / (float)pitch;
						float blendy = (float)(y - sampley) / (float)pitch;


						float fSample1 = (1.0f - blendx) * _seeds[sampley * size + samplex] + blendx  * _seeds[sampley * size + sample2x];
						float fSample2 = (1.0f - blendx) * _seeds[sample2y * size + samplex] + blendx * _seeds[sample2y * size + sample2x];


						noise += (blendy * (fSample2 - fSample1) + fSample1) * scale;
						scaleAcc += scale;

						bias < 0.2f ? bias = 0.2f : bias;
						scale = scale / bias;
					}

					_outputs[y * size + x] = noise / scaleAcc;
				}
			}
		}

		float getHeightAtPosition(uint32_t x, uint32_t y)
		{
			x %= _size;
			y %= _size;
			return _outputs[y * _size + x];
		}

		float* _seeds;
		float* _outputs;
		uint32_t _size;
	};
}