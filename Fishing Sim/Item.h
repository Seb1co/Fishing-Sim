#pragma once
#include <vector>
#include <variant>
#include <SFML/Graphics.hpp>
//id:
//0 - fishing rod
//1 - bait
//2 - fish

class Rod{
private:
	int Id;
	int Head, Reel, Grip; //Head -> how many kgs a fish can have,		(0,1...)
						  //Reel -> how fast you can reel it,			(0,1...)
						  //Grip -> the luck that you have				(0,1...)
public:
	Rod(int grip,int reel,int head);
	Rod() = default;
	int getHead();
	int getReel();
	int getGrip();
	int getId();

	void setHead(int head);
	void setReel(int reel);
	void setGrip(int grip);
};
class Bait{
private:
	int Id;
	int Type;
public:
	Bait(int type);
	Bait() = default;
	int getType();
	int getId();

	void setType(int type);
};
class Fish{
private:
	int Id;
	int Species;
	float Price;
	float Weight;
public:
	Fish(int species,float weight);
	Fish() = default;
	int getSpecies();
	double getWeight();
	int getId();
	float getPrice();

	void setSpecies(int species);
	void setWeight(float weight);
};

namespace Data {
    class Fish {
    public:
        Fish() = default;

        struct FishData {
            const char* name;
            float chance;
            int basePrice;
        };

        inline static const std::vector<FishData> fishdata{
            {"Carp",    0.5f,  5},
            {"Bass",    0.2f,  10},
            {"Trout",   0.1f,  18},
            {"Salmon",  0.05f, 29},
            {"Catfish", 0.01f, 40}
        };

        inline static sf::Texture fishTextures[5] = {
            sf::Texture("assets/images/fish/Carp.png"),
            sf::Texture("assets/images/fish/Bass.png"),
            sf::Texture("assets/images/fish/Trout.png"),
            sf::Texture("assets/images/fish/Salmon.png"),
            sf::Texture("assets/images/fish/Catfish.png")
        };

        static FishData LoadFishData(int species);
    };

    class Bait {
    public:
        Bait() = default;

        struct BaitData {
            const char* name;
            float       wait_time; // seconds
            int         chance;    // rerolls
			float       price;
        };

        inline static std::vector<BaitData> baitdata{
            {"Meat",    0.25f, 0, 0},
            {"Worm",    0.5f,  0, 50},
            {"Fruit",   0.75f, 1, 200},
            {"Garlic",  1.25f, 1, 550},
            {"Seafood", 1.5f,  2, 900}
        };

        inline static sf::Texture baitTextures[5] = {
            sf::Texture("assets/images/bait/meat.png"),
            sf::Texture("assets/images/bait/worm.png"),
            sf::Texture("assets/images/bait/fruit.png"),
            sf::Texture("assets/images/bait/garlic.png"),
            sf::Texture("assets/images/bait/seafood.png")
        };

        static BaitData LoadBaitData(int type);
    };

    class Rod {
    public:
        enum Materials {
            Wooden,
            Bamboo,
            Stone,
            Modern,
            Carbon,
        };

        struct HeadData {
            const char* material;
            int         maxkg;
            float       price;
        };

        struct ReelData {
            const char* material;
			int         time;     // time to reel in seconds
			float       price;
        };

        struct GripData {
            const char* material;
            int         chance;   // rerolls
			float       price;
        };

        inline static const std::vector<HeadData> headData{
            {"Wood",   5,  0},
            {"Bamboo", 8,  150},
            {"Stone",  15, 400},
            {"Modern", 28, 760},
            {"Carbon", 50, 1200}
        };

        inline static const std::vector<ReelData> reelData{
            {"Wood",   8, 0},
            {"Bamboo", 6, 100},
            {"Stone",  4, 300},
            {"Modern", 3, 800},
            {"Carbon", 2, 1050}
        };

        inline static const std::vector<GripData> gripData{
            {"Wood",   1, 0},
            {"Bamboo", 2, 225},
            {"Stone",  3, 450},
            {"Modern", 4, 900},
            {"Carbon", 5, 1100}
        };

        // Texturi separate
        inline static sf::Texture headTextures[5] = {
            sf::Texture("assets/images/rod/head/wooden.png"),
            sf::Texture("assets/images/rod/head/bamboo.png"),
            sf::Texture("assets/images/rod/head/stone.png"),
            sf::Texture("assets/images/rod/head/modern.png"),
            sf::Texture("assets/images/rod/head/carbon.png")
        };

        inline static sf::Texture reelTextures[5] = {
            sf::Texture("assets/images/rod/reel/wooden.png"),
            sf::Texture("assets/images/rod/reel/bamboo.png"),
            sf::Texture("assets/images/rod/reel/stone.png"),
            sf::Texture("assets/images/rod/reel/modern.png"),
            sf::Texture("assets/images/rod/reel/carbon.png")
        };

        inline static sf::Texture gripTextures[5] = {
            sf::Texture("assets/images/rod/grip/wooden.png"),
            sf::Texture("assets/images/rod/grip/bamboo.png"),
            sf::Texture("assets/images/rod/grip/stone.png"),
            sf::Texture("assets/images/rod/grip/modern.png"),
            sf::Texture("assets/images/rod/grip/carbon.png")
        };

        static HeadData LoadHeadData(int material);

        static ReelData LoadReelData(int material);

        static GripData LoadGripData(int material);
    };
}

