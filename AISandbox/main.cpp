
#define _CRT_SECURE_NO_WARNINGS
#define _DEBUG_OUT
#include "BTNodes.h"
#include "AIEngine.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <vector>
#include <random>
#include <list>
#include <cstdlib>
#include <cmath>

#include "PerlinNoise.h"

static int seed = 0;
static double perlin_xy_scalar = 50.0;
static double perlin_octave_scalar = 1.0;
static double perlin_redistribution_scalar = 1.0;
static double perlin_polar = 1.0;
static double perlin_equator = 180.0;


PerlinNoise* g_Noise = nullptr;

/*
* What defines a Nation?
* 
* "...große, meist geschlossen siedelnde Gemeinschaft von Menschen
* mit gleicher Abstammung, Geschichte, Sprache, Kultur,
* die ein politisches Staatswesen bilden"
*/

/*
* How to store/represent HISTORY of a nation?
* 
* Well, what is history? --> a sequence of some events.
* Thus, we could have distinct event types defined and the history would be e.g.
* a linked list of those events.
* 
* Where previos events can lead to special other ones or exclude others etc.
* 
* What kind of events could those be? E.g. defined in an enum and something like
* 
* "War", "Revolution", "Major Breakthrough", "Minor Breakthrough", "Great Preacher", "Racial Tensions" etc. etc.
*/

/*
* What is Culture?
* 
* Culture is an umbrella term
* which encompasses the social behavior and norms
* found in human societies, as well as the knowledge,
* beliefs, arts, laws, customs, capabilities, and habits of the individuals in these groups.
* 
* 
* Thus "Culture" is defined as a sum of norms, beliefs and habits of a nation.
*/

struct Nation // Or Tribe...
{

	enum HistoryEvents
	{
		HIST_WAR,
		HIST_COMMUNICATION_EST,
		HIST_TERRITORY_DISPUTE,
		HIST_TRADE_EST,
		HIST_FRIENDSHIP,
		HIST_BELIEF_EST,
		HIST_RELIGION_FOUND,
		HIST_TECH_BREAKTHROUG_MINOR,
		HIST_TECH_BREAKTHROUG_MAJOR
	};


	enum GovernmentForm
	{
		GOV_MONARCHY,
		GOV_AUTOCRACY,
		GOV_DEMOCRACY,
		GOV_OLIGARCHY,
		GOV_ARISTOCRACY
	};


	enum Race
	{
		RACE_HUMAN,
		RACE_DARKELF,
		RACE_HIGHELF,
		RACE_GNOME,
		RACE_TROLL,
		RACE_DWARF,
		RACE_GOBLIN,
		RACE_ORC
	};


	// Define culture as sum of following
	enum Norms
	{
		NORM_PATRIARCHAT,
		NORM_MATRIARCHAT,
		NORM_NEUTRAL_FAMILY_STRUCTURE,
		NORM_HAND_SHAKING,
		NORM_FEMALE_SPEAK_SCARCE,
		NORM_HARD_WORKING,
		NORM_MALE_SPEAK_SCARCE,
		NORM_ENJOY_LIFE
	};

	enum Beliefs
	{
		BELIEF_KARMA,
		BELIEF_AFTERLIFE,
		BELIEF_ONE_LIFE,
		BELIEF_MANY_DEITIES,
		BELIEF_ONE_DEITY,
		BELIEF_NON_CORPOREAL_BEINGS,
		BELIEF_SUPREMACY_OF_OWN_NATION
	};

	enum Habits
	{
		HABIT_MONOGAMY, // One partner
		HABIT_POLYGAMY, // Several partners
		HABIT_NON_VIOLENT_PROBLEM_SOLVING,
		HABIT_VIOLENT_PROBLEM_SOLVING,
		HABIT_ANTI_FOREIGNERS,
		HABIT_PRO_FOREIGNERS,
		HABIT_BARTERING,
	};




	std::vector<Habits> m_Habits;
	std::vector<Beliefs> m_Beliefs;
	std::vector<Norms> m_Norms;
	std::list<HistoryEvents> m_History;

	Race m_Race;
	GovernmentForm m_GovForm;
	std::string m_Language;
};













struct Biome
{
	Biome(const std::string& name, double height, double moisture) : moisture(moisture), name(name), height(height){}

	std::string name;
	double moisture;
	double height;
};


struct Tree
{
	Tree(double x, double y, const std::string& biome) : x(x), y(y), biome(biome) {}

	double x;
	double y;
	std::string biome;
};


std::vector<std::vector<double>> g_HeightMap;
std::vector<std::vector<Biome*>> g_BiomeMap;
std::vector<std::vector<Tree*>> g_TreeMap;


static const int g_VecSize = 200;



class HeightGenerator : public BTAction
{
public:
	HeightGenerator(std::string name) : BTAction(name)
	{
		// Initialize empty forests...
		g_HeightMap.resize(g_VecSize);
		for (auto& v : g_HeightMap)
		{
			v.resize(g_VecSize);
		}

		g_BiomeMap.resize(g_VecSize);
		for (auto& v : g_BiomeMap)
		{
			v.resize(g_VecSize);
		}

		g_TreeMap.resize(g_VecSize);
		for (auto& v : g_TreeMap)
		{
			v.resize(g_VecSize);
		}
	}


	BTNodeResult command() override final
	{
		using namespace std;

		// Create a new forest matrix.
		std::vector<std::vector<double>> new_forests;
		new_forests.resize(g_VecSize);
		for (auto& v : new_forests)
		{
			v.resize(g_VecSize);
		}
		
		for (int i = 0; i < g_HeightMap.size(); i++)
		{
			for (int j = 0; j < g_HeightMap[i].size(); j++)
			{
				// Ignore the 0 and last indices.
				if (i == 0 || j == 0 || i == g_HeightMap.size() - 1 || j == g_HeightMap[i].size() - 1) continue;


				// Get neighbors.
				double neighbors = _neighbors(i, j);

				double dist = 2 * std::max(std::abs(i), std::abs(j));
				neighbors = (neighbors - neighbors / dist) / 2;

				neighbors = std::lerp(neighbors, g_HeightMap[i][j], neighbors);

				double PI = 3.14145;
				neighbors = 10 * neighbors * neighbors + 0.1 + (0.2 - 0.1) * std::sin(PI * (g_VecSize / g_VecSize));

				if (neighbors < 0.1)
				{
					new_forests[i][j] = 0.1;
				}
				else if (neighbors < 0.12)
				{
					new_forests[i][j] = 0.12;
				}
				if (neighbors >= 0.12 && neighbors <= 0.3)
				{
					new_forests[i][j] = 0.2;
				}
				else if (neighbors > 0.3 && neighbors <= 0.6)
				{
					new_forests[i][j] = 0.5;
				}
				else if (neighbors > 0.6 && neighbors <= 0.7)
				{
					new_forests[i][j] = 0.6;
				}
				else if (neighbors > 0.7 && neighbors <= 1.0)
				{
					new_forests[i][j] = 1.0;
				}
				else // Greater than 1.0
				{
					new_forests[i][j] = 0;
				}
			}
		}


		// Replace the old one with the new one.
		g_HeightMap = new_forests;

#ifdef _DEBUG_OUT
		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;
#endif

		return BTNodeResult::SUCCESS;
	}


private:


	double _neighbors(int x, int y)
	{

		// We have to check 8 neighbors.
		double neighbors = g_HeightMap[x][y];
		
		double a = g_HeightMap[x - 1][y - 1];
		double b = g_HeightMap[x][y - 1];
		double c = g_HeightMap[x + 1][y - 1];
		double d = g_HeightMap[x - 1][y];
		double e = g_HeightMap[x + 1][y];
		double f = g_HeightMap[x - 1][y + 1];
		double g = g_HeightMap[x][y + 1];
		double h = g_HeightMap[x + 1][y + 1];


		neighbors = (a + b + c + d + e + f + g + h) / 8.0;

		/*
		if (g_HeightMap[x - 1][y - 1] != 0) neighbors++;

		if (g_HeightMap[x][y - 1] != 0) neighbors++;

		if (g_HeightMap[x + 1][y - 1] != 0) neighbors++;

		if (g_HeightMap[x - 1][y] != 0) neighbors++;

		if (g_HeightMap[x + 1][y] != 0) neighbors++;

		if (g_HeightMap[x - 1][y + 1] != 0) neighbors++;

		if (g_HeightMap[x][y + 1] != 0) neighbors++;

		if (g_HeightMap[x + 1][y + 1] != 0) neighbors++;
		*/

		return neighbors;
	}
};


#include <fstream>
#include <cstdio>

class AISandbox : public olc::PixelGameEngine
{
public:
	AISandbox()
	{

		sAppName = "AISandbox";
	}

public:
	bool OnUserCreate() override
	{
		m_AIEngine = new AIEngine();


		BehaviorTree* tree = new BehaviorTree("Conway Life Tree");

		BTSequence* seq = new BTSequence("Starting Sequence");
		BTTimer* timer = new BTTimer("Timer", BTTimer::Granularity::Seconds, BTTimer::Policy::Greater, 0.0);
		HeightGenerator* forests = new HeightGenerator("HeightGenerator");



		seq->addChild(timer);
		seq->addChild(forests);

		tree->setRoot(seq);


		m_AIEngine->addBT(tree);


		/*
		* Define starting forest set for the game of life, else nothing will happen.
		*/
		for (int i = 0; i < g_VecSize; i++)
		{
			for (int j = 0; j < g_VecSize; j++)
			{
				g_HeightMap[i][j] = 0.1 * (rand() % 10);
				g_BiomeMap[i][j] = nullptr;
				g_TreeMap[i][j] = nullptr;
			}

		}

		g_Noise = new PerlinNoise(2313);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		using namespace std;

		Clear(olc::Pixel(1, 1, 1, 1));
		
		if (GetKey(olc::Key::SPACE).bPressed)
		{
			m_AIEngine->update();

			perlin_xy_scalar = 25;
			perlin_redistribution_scalar = 4.75;
		}


		if (GetKey(olc::Key::ENTER).bPressed)
		{

			std::vector<std::vector<Tree*>> new_forest_map;
			new_forest_map.resize(g_VecSize);
			for (auto& v : new_forest_map)
			{
				v.resize(g_VecSize);
			}
		

			for (int i = 1; i < g_VecSize - 1; i++)
			{
				for (int j = 1; j < g_VecSize - 1; j++)
				{

					double d = g_BiomeMap[i][j]->height;
					double m = g_BiomeMap[i][j]->moisture;


					// Draw created map with added trees.
					if (d < 0.2) // Ocean
					{
					}
					else if (d >= 0.2 && d < 0.25) // Sea
					{
					}
					else if (d >= 0.25 && d < 0.3) // Sand
					{
					}
					else if (d >= 0.3 && d < 0.4) // Savannah
					{
						if (m < 0.5)
						{
							if (rand() % 20 == 0)
							{
								new_forest_map[i][j] = new Tree(i, j, "Tundra");
							}

						}
						else if (m >= 0.5)
						{
							if (rand() % 30 == 0)
							{
								new_forest_map[i][j] = new Tree(i, j, "Savannah");
							}

						}
					}
					else if (d >= 0.4 && d < 0.9) // Land / Jungle
					{
						if (m >= 0.3)
						{
							if (rand() % 4 == 0)
							{
								new_forest_map[i][j] = new Tree(i, j, "Jungle");
							}

						}
						else if (m < 0.3)
						{
							if (rand() % 8 == 0)
							{
								new_forest_map[i][j] = new Tree(i, j, "Temperate");
							}

						}
					}
					else // Mountains
					{
					}

				}

			}

			g_TreeMap = new_forest_map;
		}


		/*
		for (int i = 0; i < g_HeightMap.size(); i++)
		{
			for (int j = 0; j < g_HeightMap[i].size(); j++)
			{
				double d = g_HeightMap[i][j];

				int r = d * 255;
				int g = d * 255;
				int b = d * 255;


				// Draw terrain types based on heightmap.
				if (d < 0.1) // Ocean
				{
					Draw(i, j, olc::Pixel(0, 0, 139));
				}
				else if (d >= 0.1 && d < 0.4) // Sea
				{
					Draw(i, j, olc::Pixel(0, 178, 238));
				}
				else if (d >= 0.4 && d < 0.45) // Sand
				{
					Draw(i, j, olc::Pixel(244, 164, 0));
				}
				else if (d >= 0.6 && d < 0.7)
				{
					Draw(i, j, olc::Pixel(139, 69, 19));
				}
				else // Land
				{
					Draw(i, j, olc::Pixel(0, 139, 0));
				}


				//Draw(i, j, olc::Pixel(r, g, b));
			}
		}
		*/

		if (GetKey(olc::Key::R).bPressed)
		{
			for (int i = 0; i < g_VecSize; i++)
			{
				for (int j = 0; j < g_VecSize; j++)
				{
					g_HeightMap[i][j] = 0.1 * (rand() % 2);
				}

			}
		}
		

		
		if (GetKey(olc::Key::R).bPressed)
		{
			delete g_Noise;
			seed = rand();
			g_Noise = new PerlinNoise(seed);
		}


		
		if (GetKey(olc::Key::UP).bHeld)
		{
			perlin_xy_scalar += 1.1;
		}
		if (GetKey(olc::Key::DOWN).bHeld)
		{
			perlin_xy_scalar -= 1.1;
		}
		if (GetKey(olc::Key::RIGHT).bHeld)
		{
			perlin_octave_scalar += 0.1;
		}
		if (GetKey(olc::Key::LEFT).bHeld)
		{
			perlin_octave_scalar -= 0.1;
		}
		if (GetKey(olc::Key::W).bHeld)
		{
			perlin_redistribution_scalar += 0.01;
		}
		if (GetKey(olc::Key::S).bHeld)
		{
			perlin_redistribution_scalar -= 0.01;
		}
		if (GetKey(olc::Key::Q).bHeld)
		{
			perlin_polar += 0.1;
		}
		if (GetKey(olc::Key::E).bHeld)
		{
			perlin_polar -= 0.1;
		}
		if (GetKey(olc::Key::O).bHeld)
		{
			perlin_equator += 0.1;
		}
		if (GetKey(olc::Key::P).bHeld)
		{
			perlin_equator -= 0.1;
		}


		for (int i = 0; i < g_VecSize; i++)
		{
			for (int j = 0; j < g_VecSize; j++)
			{
				double x = (double)j / ((double)g_VecSize) - 0.5;
				double y = (double)i / ((double)g_VecSize) - 0.5;

				double d = 0.0;
				double m = 0.0;

				// Generate the perlin height map.
				/*
				* Good values seem to be:
				* perlin_xy_scalar = 25
				* perlin_redistribution_scalar = 4.75
				* 
				* rest does not need to be set.
				*/
				double wavelength = g_VecSize / perlin_xy_scalar;

				d = 1.0 * g_Noise->noise(wavelength * x, wavelength * y, 1.0) +
					0.5 * g_Noise->noise(2 * wavelength * x, 2 * wavelength * y, 1.0) +
					0.25 * g_Noise->noise(4 * wavelength * x, 4 * wavelength * y, 1.0);

				d = std::pow(d, perlin_redistribution_scalar);
				d = std::clamp(d, 0.0, 1.0);



				// Generate perlin moisture map.
				m = g_Noise->noise(wavelength * x, wavelength * y, wavelength * x * y);

				// Manhatten.
				double dist = std::abs(x) + std::abs(y);
				m = perlin_polar * (0.5 + m - dist) / 2;

				m = std::clamp(m, 0.0, 1.0);


				// Draw terrain types based on heightmap.
				if (d < 0.2) // Ocean
				{
					Draw(i, j, olc::Pixel(0, 0, 139));
				}
				else if (d >= 0.2 && d < 0.25) // Sea
				{
					Draw(i, j, olc::Pixel(0, 178, 238));
				}
				else if (d >= 0.25 && d < 0.3) // Sand
				{
					Draw(i, j, olc::Pixel(245, 222, 179));
				}
				else if (d >= 0.3 && d < 0.4) // Savannah
				{
					Draw(i, j, olc::Pixel(244, 164, 96));
				}
				else if (d >= 0.4 && d < 0.9) // Land / Jungle
				{
					Draw(i, j, olc::Pixel(34, 139, 34));
				}
				else // Mountains
				{
					Draw(i, j, olc::Pixel(139, 69, 19));
				}


				// Draw moisture map.
				if (m < 0.1)
				{
					Draw(g_VecSize + i, j, olc::Pixel(0, 0, 139));
				}
				else if (m >= 0.1 && m < 0.12)
				{
					Draw(g_VecSize + i, j, olc::Pixel(65, 105, 225));
				}
				else if (m >= 0.12 && m < 0.3)
				{
					Draw(g_VecSize + i, j, olc::Pixel(0, 191, 255));
				}
				else
				{
					Draw(g_VecSize + i, j, olc::Pixel(135, 206, 250));
				}



				// Draw created map from height and moisture.
				if (d < 0.2) // Ocean
				{
					Draw(i, g_VecSize + j, olc::Pixel(0, 0, 139));
					
					g_BiomeMap[i][j] = new Biome("Ocean", d, m);
				}
				else if (d >= 0.2 && d < 0.25) // Sea
				{
					Draw(i, g_VecSize + j, olc::Pixel(0, 178, 238));

					g_BiomeMap[i][j] = new Biome("Sea", d, m);

				}
				else if (d >= 0.25 && d < 0.3) // Sand
				{
					Draw(i, g_VecSize + j, olc::Pixel(245, 222, 179));

					g_BiomeMap[i][j] = new Biome("Sand", d, m);
				}
				else if (d >= 0.3 && d < 0.4) // Savannah
				{
					if (m < 0.5)
					{
						Draw(i, g_VecSize + j, olc::Pixel(0, 100, 0)); // Tundra Tile.

						g_BiomeMap[i][j] = new Biome("Tundra", d, m);

					}
					else if (m >= 0.5)
					{
						Draw(i, g_VecSize + j, olc::Pixel(244, 164, 96)); // Savannah

						g_BiomeMap[i][j] = new Biome("Savannah", d, m);

					}
				}
				else if (d >= 0.4 && d < 0.9) // Land / Jungle
				{
					if (m >= 0.3)
					{
						Draw(i, g_VecSize + j, olc::Pixel(75, 0, 130)); // Jungle Tile.

						g_BiomeMap[i][j] = new Biome("Jungle", d, m);

					}
					else if (m < 0.3)
					{
						Draw(i, g_VecSize + j, olc::Pixel(34, 139, 34)); // Normal Temperate.

						g_BiomeMap[i][j] = new Biome("Temperate", d, m);

					}
				}
				else // Mountains
				{
					Draw(i, g_VecSize + j, olc::Pixel(139, 69, 19));

					g_BiomeMap[i][j] = new Biome("Mountain", d, m);

				}



				// Draw created map with added trees.
				if (d < 0.2) // Ocean
				{
					Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(0, 0, 139));
					if (g_TreeMap[i][j] != nullptr)
					{
						if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
					}
				}
				else if (d >= 0.2 && d < 0.25) // Sea
				{
					Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(0, 178, 238));
					if (g_TreeMap[i][j] != nullptr)
					{
						if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
					}
				}
				else if (d >= 0.25 && d < 0.3) // Sand
				{
					Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(245, 222, 179));
					if (g_TreeMap[i][j] != nullptr)
					{
						if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
					}
				}
				else if (d >= 0.3 && d < 0.4) // Savannah
				{
					if (m < 0.5)
					{
						Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(0, 100, 0)); // Tundra Tile.
						if (g_TreeMap[i][j] != nullptr)
						{
							if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
						}
					}
					else if (m >= 0.5)
					{
						Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(244, 164, 96)); // Savannah
						if (g_TreeMap[i][j] != nullptr)
						{
							if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
						}
					}
				}
				else if (d >= 0.4 && d < 0.9) // Land / Jungle
				{
					if (m >= 0.3)
					{
						Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(75, 0, 130)); // Jungle Tile.
						if (g_TreeMap[i][j] != nullptr)
						{
							if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
						}
					}
					else if (m < 0.3)
					{
						Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(34, 139, 34)); // Normal Temperate.
						if (g_TreeMap[i][j] != nullptr)
						{
							if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
							{
								FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
							}
						}
					}
				}
				else // Mountains
				{
					Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(139, 69, 19));
					if (g_TreeMap[i][j] != nullptr)
					{
						if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
						{
							FillCircle(g_VecSize + i, g_VecSize + j, 1, olc::BACK);
						}
					}
				}
			}
		}
		
		
		DrawString(0, 0, "Frequency:" + std::to_string(perlin_xy_scalar));
		DrawString(0, 10, "Octave:" + std::to_string(perlin_octave_scalar));
		DrawString(0, 20, "Redist:" + std::to_string(perlin_redistribution_scalar));
		DrawString(0, 30, "Polar:" + std::to_string(perlin_polar));
		DrawString(0, 40, "Equator:" + std::to_string(perlin_equator));
		
		return true;
	}



private:


	AIEngine* m_AIEngine = nullptr;
};



int main()
{
	AISandbox demo;
	if (demo.Construct(648, 480, 2, 2))
		demo.Start();
	return 0;
}