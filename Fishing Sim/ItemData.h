#pragma once
#include <vector>
#include <string>

namespace ItemData {
	struct Rod {
		int head; // max kgs it can get
		int reel; // how many seconds it takes to reel a fish
		int grip; // how many times it will reroll
	};
	struct Bait {
		int type;	// how many seconds it takes to reel a fish		/2
					// how many times it will reroll				/2
	};
	struct Fish {
		std::string name;
		int basePrice;	// the price wich the fish will start from
		int chance;		// the chance of obtainging the fish
	};

	inline std::vector<Fish> FishTypes{
		{ "Common Carp", 10, 500 },
		{ "Bass", 20, 250 },
		{ "Trout", 30, 166 },
		{ "Salmon", 40, 125 },
		{ "Catfish", 50, 100 }
	};
	Fish LoadItemData(std::string name);
	
}