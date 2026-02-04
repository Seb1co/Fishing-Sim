#include "ItemData.h"
#include <vector>

namespace ItemData {
    Fish LoadFishData(std::string name) {
        for (auto fish : FishTypes)
			if (fish.name == name)
                return fish;
	}
}