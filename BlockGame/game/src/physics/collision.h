#pragma once

#include "glm/glm.hpp"
#include <map>
#include "world/world.h"

namespace blockcraft
{

	struct blockCollider
	{
		blockCollider(const glm::vec3& pos1, const glm::vec3& pos2)
			: _position{ pos1, pos2 }
		{
		}

		glm::vec3 _position[2];

	};

	class collision
	{
	public:

		void update();

		void addBlockCollider(int32_t x, uint8_t y, int32_t z);

		void removeBlockCollider(int32_t x, uint8_t y, int32_t z);

		blockCollider* getBlockCollider(int32_t x, uint8_t y, int32_t z);

	private:

		std::map<uint32_t, blockCollider> _blockColliders;
	};


}