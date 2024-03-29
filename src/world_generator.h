#pragma once

#include <glm/glm.hpp>
#include <noise_generator.h>

#include <vector>

class worldGenerator {
public:
	int instance_count = 512;

	std::vector<glm::vec3> positions;

	void init_positions();
	void generate_world();

private:
};