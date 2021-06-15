
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

static bool g_NationsGenerated = false;
static int g_NeighborOffset = 2;
static int g_NeighborStandardDistance = 10;
static bool g_ShowNeighbors = true;
static int g_NationSimulationSteps = 100;

PerlinNoise* g_Noise = nullptr;

std::mt19937 g_Generator(seed); // Seed.

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
enum class HistoricalEvent;
struct Nation // Or Tribe...
{

	Nation& operator=(const Nation& rhs)
	{
		this->m_AntiPairs = rhs.m_AntiPairs;
		this->m_GovForm = rhs.m_GovForm;
		this->m_History = rhs.m_History;
		this->m_Language = rhs.m_Language;
		this->m_Neighbors = rhs.m_Neighbors;
		this->m_Race = rhs.m_Race;
		return *this;
	}


	double operator() (const std::string& trait)
	{
		if (trait.compare("Spiritual") == 0)
		{
			return m_AntiPairs[trait];
		}
		else if (trait.compare("Materialistic") == 0)
		{
			return m_AntiPairs[trait];
		}
		else if (trait.compare("Warfaring") == 0)
		{
			return m_AntiPairs[trait];
		}
		else if (trait.compare("Pacifist") == 0)
		{
			return m_AntiPairs[trait];
		}
		else if (trait.compare("Industrial") == 0)
		{
			return m_AntiPairs[trait];
		}
		else if (trait.compare("Agricultural") == 0)
		{
			return m_AntiPairs[trait];
		}
		else if (trait.compare("Isolationist") == 0)
		{
			return m_AntiPairs[trait];
		}
		else if (trait.compare("Expansive") == 0)
		{
			return m_AntiPairs[trait];
		}
		else if (trait.compare("Technologic") == 0)
		{
			return m_AntiPairs[trait];
		}
		else if (trait.compare("Primitive") == 0)
		{
			return m_AntiPairs[trait];
		}
		else if (trait.compare("Economic") == 0)
		{
			return m_AntiPairs[trait];
		}
		else if (trait.compare("Non-Economic") == 0)
		{
			return m_AntiPairs[trait];
		}
		else
		{
			return -9999.0;
		}
	}

	enum GovernmentForm
	{
		GOV_NONE,
		GOV_MONARCHY,
		GOV_AUTOCRACY,
		GOV_DEMOCRACY,
		GOV_OLIGARCHY,
		GOV_ARISTOCRACY
	};

	enum Race
	{
		RACE_NONE,
		RACE_HUMAN,
		RACE_DARKELF,
		RACE_HIGHELF,
		RACE_GNOME,
		RACE_TROLL,
		RACE_DWARF,
		RACE_GOBLIN,
		RACE_ORC
	};



	
	
	std::map<std::string, double> m_AntiPairs;
	std::list<HistoricalEvent> m_History;
	
	std::list<Nation*> m_Neighbors;

	Race m_Race;
	GovernmentForm m_GovForm;
	std::string m_Language;

	int x_pos = 0;
	int y_pos = 0;


	int m_NationColor[3] = { 0, 0, 0 };


	std::list<Nation*> m_AlliedNations;
	std::list<Nation*> m_EnemyNations;


	double m_Population = 3.0;
	double m_Territory = 3.0;
};


/*
* TODO
* If this enum works ok, then
* add a stringifier to it!
*/
enum class HistoricalEvent
{
	HIST_GOV_EST = 0,
	HIST_GOV_DESTROY,

	HIST_ECONOMIC_UPRISE,
	HIST_ECONOMIC_DOWN,

	HIST_RELIGIOUS_UPRISE,
	HIST_RELIGOUS_DOWN,

	HIST_LAND_EXPANSION,
	HIST_LAND_SHRINK,

	HIST_TECH_UPRISE,
	HIST_TECH_DOWN,

	HIST_INDUSTRY_UPRISE,
	HIST_INDUSTRY_DOWN,

	HIST_AGRI_UPRISE,
	HIST_AGRI_DOWN,

	HIST_WAR,
	HIST_PEACE,

	HIST_FRIENDLY_NEIGHBOR,
	HIST_NEIGHBOR_FEUD,

	HIST_POPULATION_BOOST,
	HIST_POPULATION_DOWN,




	HIST_NOTHING,
	HIST_MAX_SIZE = HIST_NOTHING
};

std::map < Nation::Race, std::vector<std::string>> g_RacialTraitsArchetypes;
std::map<std::string, Nation*> g_NationArchetypes;
std::map<std::string, std::string> g_AntiPairs;

std::string historicalEventString(HistoricalEvent e);
std::string raceToString(Nation::Race r);
std::string pickRandomTrait(std::vector<std::string> vec);
Nation::Race pickRandomRace(); // We have 8 Races.
std::string pickRandomLanguage(std::vector<std::string> vec);
void pickRandomColor(int v[3]);
bool isNationInNeighbors(Nation* n, std::list<Nation*> list);
std::vector<std::string> m_HabitsTemplate;
std::vector<std::string> m_BeliefsTemplate;
std::vector<std::string> m_NormsTemplate;
std::vector<std::string> m_HistoryEventsTemplate;
std::vector<std::string> m_LanguageTemplate;








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
static const int g_VecSize = 100;

std::vector<Nation*> g_Nations; // Nations storage.
std::vector<std::vector<Nation*>> g_NationsMap; // Nations storage with direct point access.
std::vector<std::vector<std::string>> g_CultureDistribution; // Defines culture archetype at a given point x, y.
std::vector<std::vector<Nation::Race>> g_RacialDistribution; // Defines racial archetype at a given point x, y.
std::vector<std::string> g_NationLanguages;
static const int g_NationCount = 100;


/*
* Generate cultural distribution across the map.
*/
class TierZeroNationGenerator : public BTAction
{
public:
	TierZeroNationGenerator(std::string name) : BTAction(name)
	{
		g_CultureDistribution.resize(g_VecSize);
		for (auto& v : g_CultureDistribution)
		{
			v.resize(g_VecSize);
		}
	}


	BTNodeResult command() override final
	{
		using namespace std;
		
		// First step.
		// Create culture distributions map with perlin noise.
		for (int i = 0; i < g_VecSize; i++)
		{
			for (int j = 0; j < g_VecSize; j++)
			{
				double x = (double)j / ((double)g_VecSize) - 0.5;
				double y = (double)i / ((double)g_VecSize) - 0.5;

				double d = 0.0;

				// culture distribution is similar to moisture map.
				// Generate perlin moisture map.
				double wavelength = g_VecSize / perlin_xy_scalar;
				d = perlin_polar * g_Noise->noise(wavelength * x, wavelength * y, wavelength * x * y) +
					0.5 * perlin_polar * g_Noise->noise(2 * wavelength * x, 2 * wavelength * y, 2 * wavelength * x * y) +
					0.25 * perlin_polar * g_Noise->noise(4 * wavelength * x, 4 * wavelength * y, 4 * wavelength * x * y);


				// Manhatten.
				double dist = std::abs(x) + std::abs(y);
				d = perlin_polar * (0.5 + d - dist) / 2;

				d = std::clamp(d, 0.0, 1.0);


				// we have 12 possible culture archetypes.
				if (d < 0.1)
				{
					g_CultureDistribution[i][j] = "Primitive";
				}
				else if (d >= 0.1 && d < 0.2)
				{
					g_CultureDistribution[i][j] = "Materialistic";
				}
				else if (d >= 0.1 && d < 0.2)
				{
					g_CultureDistribution[i][j] = "Non-Economic";
				}
				else if (d >= 0.2 && d < 0.3)
				{
					g_CultureDistribution[i][j] = "Warfaring";
				}
				else if (d >= 0.3 && d < 0.4)
				{
					g_CultureDistribution[i][j] = "Isolationist";
				}
				else if (d >= 0.4 && d < 0.5)
				{
					g_CultureDistribution[i][j] = "Pacifist";
				}
				else if (d >= 0.5 && d < 0.6)
				{
					g_CultureDistribution[i][j] = "Expansive";
				}
				else if (d >= 0.6 && d < 0.7)
				{
					g_CultureDistribution[i][j] = "Economic";
				}
				else if (d >= 0.7 && d < 0.8)
				{
					g_CultureDistribution[i][j] = "Technologic";
				}
				else if (d >= 0.8 && d < 0.85)
				{
					g_CultureDistribution[i][j] = "Industrial";
				}
				else if (d >= 0.85 && d < 0.9)
				{
					g_CultureDistribution[i][j] = "Agricultural";
				}
				else if (d >= 0.9 && d <= 1.0)
				{
					g_CultureDistribution[i][j] = "Spiritual";
				}

			}
		}


		/*
		* Move to Tier Three Nation Generator!
		* 
		// Place initial nations.
		int nations_created = 0;
		for (int i = 1; i < g_BiomeMap.size() - 1; i++)
		{
			for (int j = 1; j < g_BiomeMap[i].size() - 1; j++)
			{

				std::mt19937 generator(g_VecSize); // Seed.
				std::uniform_real_distribution<> dis(0.0, 1.0);

				// Get neighbors.
				int neighbors = _neighbors(i, j);

				// Generate if "not that many neighbors".
				if (g_NationCount < nations_created) continue;

				// ... and if random value is correct.
				if (dis(generator) >= 0.7)
				{

					Nation* n = new Nation();
					if (g_CultureDistribution[i][j].compare("Materialistic") == 0)
					{
						n = g_NationArchetypes["Materialistic"];
					}
					else if (g_CultureDistribution[i][j].compare("Primitive") == 0)
					{
						n = g_NationArchetypes["Primitive"];
					}
					else if (g_CultureDistribution[i][j].compare("Industrial") == 0)
					{
						n = g_NationArchetypes["Industrial"];
					}
					else if (g_CultureDistribution[i][j].compare("Non-Economic") == 0)
					{
						n = g_NationArchetypes["Non-Economic"];
					}
					else if (g_CultureDistribution[i][j].compare("Agricultural") == 0)
					{
						n = g_NationArchetypes["Agricultural"];
					}
					else if (g_CultureDistribution[i][j].compare("Expansive") == 0)
					{
						n = g_NationArchetypes["Expansive"];
					}
					else if (g_CultureDistribution[i][j].compare("Isolationist") == 0)
					{
						n = g_NationArchetypes["Isolationist"];
					}
					else if (g_CultureDistribution[i][j].compare("Pacifist") == 0)
					{
						n = g_NationArchetypes["Pacifist"];
					}
					else if (g_CultureDistribution[i][j].compare("Warfaring") == 0)
					{
						n = g_NationArchetypes["Warfaring"];
					}
					else if (g_CultureDistribution[i][j].compare("Spiritual") == 0)
					{
						n = g_NationArchetypes["Spiritual"];
					}
					else if (g_CultureDistribution[i][j].compare("Economic") == 0)
					{
						n = g_NationArchetypes["Economic"];
					}
					else if (g_CultureDistribution[i][j].compare("Technologic") == 0)
					{
						n = g_NationArchetypes["Technologic"];
					}



					g_Nations.push_back(n);
					nations_created++;
				}


			}
		}
		*/


#ifdef _DEBUG_OUT
		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;
#endif

		return BTNodeResult::SUCCESS;
	}


private:


	int _neighbors(int x, int y)
	{

		// We have to check 8 neighbors.
		int neighbors = 0;
	
		
		if (g_HeightMap[x - 1][y - 1] != 0) neighbors++;

		if (g_HeightMap[x][y - 1] != 0) neighbors++;

		if (g_HeightMap[x + 1][y - 1] != 0) neighbors++;

		if (g_HeightMap[x - 1][y] != 0) neighbors++;

		if (g_HeightMap[x + 1][y] != 0) neighbors++;

		if (g_HeightMap[x - 1][y + 1] != 0) neighbors++;

		if (g_HeightMap[x][y + 1] != 0) neighbors++;

		if (g_HeightMap[x + 1][y + 1] != 0) neighbors++;
		

		return neighbors;
	}
};



/*
* Generate racial distribution across the map.
*/
class TierOneNationGenerator : public BTAction
{
public:
	TierOneNationGenerator(const std::string& name) : BTAction(name)
	{
		g_RacialDistribution.resize(g_VecSize);
		for (auto& v : g_RacialDistribution)
		{
			v.resize(g_VecSize);
		}
	}



	BTNodeResult command() override final
	{

		for (int i = 0; i < g_VecSize; i++)
		{
			for (int j = 0; j < g_VecSize; j++)
			{
				double x = (double)j / ((double)g_VecSize) - 0.5;
				double y = (double)i / ((double)g_VecSize) - 0.5;

				double d = 0.0;

				// Racial distribution is similar to moisture map.
				// Generate perlin Racial map.
				double wavelength = g_VecSize / perlin_xy_scalar;
				
				/*
				d = perlin_polar * g_Noise->noise(wavelength * x, wavelength * y, wavelength * x * y) +
					0.5 * perlin_polar* g_Noise->noise(2 * wavelength * x, 2 * wavelength * y, 2 * wavelength * x * y) +
					0.25 * perlin_polar* g_Noise->noise(4 * wavelength * x, 4 * wavelength * y, 4 * wavelength * x * y);
				*/
				d = 1.0 * perlin_polar * g_Noise->noise(wavelength * x, wavelength * y, wavelength * x * y) +
					0.83 * perlin_polar * g_Noise->noise(2 * wavelength * x, 2 * wavelength * y, wavelength * x * y) +
					0.69 * perlin_polar * g_Noise->noise(4 * wavelength * x, 4 * wavelength * y, wavelength * x * y);
					0.14 * perlin_polar* g_Noise->noise(8 * wavelength * x, 8 * wavelength * y, wavelength * x * y) +
					0.06 * perlin_polar * g_Noise->noise(16 * wavelength * x, 16 * wavelength * y, wavelength * x * y) +
					0.03 * perlin_polar * g_Noise->noise(32 * wavelength * x, 32 * wavelength * y, wavelength * x * y);



				// Diag.
				//double dist = 2 * std::max(std::abs(x), std::abs(y));

				// Euclid.
				//double dist = std::sqrt(x * x + y * y) / std::sqrt(0.5);

				// Manhatten
				double dist = abs(x) + abs(y);
				//d = (0.5 + d - dist) / 2;

				//double dist = std::abs(x) + std::abs(y);
				d = perlin_polar * (0.7 + d - dist) / 2;

				d = std::clamp(d, 0.0, 1.0);
				
				
				/*
				d = g_Noise->noise(wavelength * x, wavelength * y, wavelength * x * y);

				// Manhatten.
				double dist = std::abs(x) + std::abs(y);
				d = perlin_polar * (0.5 + d - dist) / 2;

				d = std::clamp(d, 0.0, 1.0);
				*/
				// we have 8 possible races archetypes.
				// we have 12 possible culture archetypes.
				if (d < 0.2)
				{
					g_RacialDistribution[i][j] = Nation::Race::RACE_DWARF;
				}
				else if (d < 0.3)
				{
					g_RacialDistribution[i][j] = Nation::Race::RACE_HIGHELF;
				}
				else if (d < 0.4)
				{
					g_RacialDistribution[i][j] = Nation::Race::RACE_GNOME;
				}
				else if (d < 0.5)
				{
					g_RacialDistribution[i][j] = Nation::Race::RACE_DARKELF;
				}
				else if (d < 0.6)
				{
					g_RacialDistribution[i][j] = Nation::Race::RACE_GOBLIN;
				}
				else if (d < 0.7)
				{
					g_RacialDistribution[i][j] = Nation::Race::RACE_ORC;
				}
				else if (d < 0.8)
				{
					g_RacialDistribution[i][j] = Nation::Race::RACE_TROLL;
				}
				else if (d < 0.9)
				{
					g_RacialDistribution[i][j] = Nation::Race::RACE_HUMAN;
				}
				else
				{
					g_RacialDistribution[i][j] = pickRandomRace();
				}
			}
		}


		using namespace std;

#ifdef _DEBUG_OUT
		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;
#endif

		return BTNodeResult::SUCCESS;
	}

};





/*
* Generate initial nation placement across the map based on the output 
* of the map and the tier 0 and tier 1 generators.
*/
class TierTwoNationGenerator : public BTAction
{
public:
	TierTwoNationGenerator(const std::string& name) : BTAction(name)
	{

	}



	BTNodeResult command() override final
	{
		std::uniform_int_distribution<> dis(g_NeighborOffset + 1, g_VecSize - g_NeighborOffset - 1);


		// Place initial nations.
		int nations_created = 0;


		for (int i = 0; i < g_NationCount; i++)
		{
			// Generate if "not that many neighbors".
			if (g_NationCount < nations_created) break;;

			int x = dis(g_Generator);
			int y = dis(g_Generator);


			// Check whether tile is appropriate for a city.
			if (g_BiomeMap[x][y]->name.compare("Sand") == 0)
			{
				continue;
			}
			else if (g_BiomeMap[x][y]->name.compare("Ocean") == 0)
			{
				continue;
			}
			else if (g_BiomeMap[x][y]->name.compare("Sea") == 0)
			{
				continue;
			}
			else if (g_BiomeMap[x][y]->name.compare("Mountain") == 0)
			{
				continue;
			}



			if (_neighbors(x, y, g_NeighborOffset - 1) < 20)
			{
				Nation* n = new Nation();
				if (g_CultureDistribution[x][y].compare("Materialistic") == 0)
				{
					n->operator=(*g_NationArchetypes["Materialistic"]);
				}
				else if (g_CultureDistribution[x][y].compare("Primitive") == 0)
				{
					n->operator=(*g_NationArchetypes["Primitive"]);
				}
				else if (g_CultureDistribution[x][y].compare("Industrial") == 0)
				{
					n->operator=(*g_NationArchetypes["Industrial"]);
				}
				else if (g_CultureDistribution[x][y].compare("Non-Economic") == 0)
				{
					n->operator=(*g_NationArchetypes["Non-Economic"]);
				}
				else if (g_CultureDistribution[x][y].compare("Agricultural") == 0)
				{
					n->operator=(*g_NationArchetypes["Agricultural"]);
				}
				else if (g_CultureDistribution[x][y].compare("Expansive") == 0)
				{
					n->operator=(*g_NationArchetypes["Expansive"]);
				}
				else if (g_CultureDistribution[x][y].compare("Isolationist") == 0)
				{
					n->operator=(*g_NationArchetypes["Isolationist"]);
				}
				else if (g_CultureDistribution[x][y].compare("Pacifist") == 0)
				{
					n->operator=(*g_NationArchetypes["Pacifist"]);
				}
				else if (g_CultureDistribution[x][y].compare("Warfaring") == 0)
				{
					n->operator=(*g_NationArchetypes["Warfaring"]);
				}
				else if (g_CultureDistribution[x][y].compare("Spiritual") == 0)
				{
					n->operator=(*g_NationArchetypes["Spiritual"]);
				}
				else if (g_CultureDistribution[x][y].compare("Economic") == 0)
				{
					n->operator=(*g_NationArchetypes["Economic"]);
				}
				else if (g_CultureDistribution[x][y].compare("Technologic") == 0)
				{
					n->operator=(*g_NationArchetypes["Technologic"]);
				}


				// Pick "random" Race.
				if (dis(g_Generator) > g_VecSize / 2 - g_NeighborOffset)
				{
					n->m_Race = g_RacialDistribution[x][y];
				}
				else
				{
					n->m_Race = pickRandomRace();
				}

				// Give nation a language. For now purely random.
				// This could be moved to another generator...
				n->m_Language = pickRandomLanguage(g_NationLanguages);


				// Assign random color to nation.
				pickRandomColor(n->m_NationColor);


				// Lastly set position of nation.
				n->x_pos = x;
				n->y_pos = y;


				g_Nations.push_back(n);
				g_NationsMap[x][y] = n;
				nations_created++;
			}

		}



		using namespace std;
		if (nations_created > 0)
		{
#ifdef _DEBUG_OUT
			cout << color(colors::GREEN) << endl;
			cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;
#endif
			return BTNodeResult::SUCCESS;
		}
		else
		{
#ifdef _DEBUG_OUT
			cout << color(colors::RED) << endl;
			cout << "\"" << name() << "\"->command() = FAILURE" << white << endl;
#endif
			return BTNodeResult::FAILURE;
		}
	}

private:


	int _neighbors(int x, int y, int neighbors_offset)
	{

		int neighbors = 0;

		// Check if any neighbors withing 20 Tiles.
		for (int i = 0; i < neighbors_offset; i++)
		{
			for (int j = 0; j < neighbors_offset; j++)
			{
				if (g_NationsMap[x - i][y - j] != 0) neighbors++;

				if (g_NationsMap[x][y - j] != 0) neighbors++;

				if (g_NationsMap[x + i][y - j] != 0) neighbors++;

				if (g_NationsMap[x - i][y] != 0) neighbors++;

				if (g_NationsMap[x + i][y] != 0) neighbors++;

				if (g_NationsMap[x - i][y + j] != 0) neighbors++;

				if (g_NationsMap[x][y + j] != 0) neighbors++;

				if (g_NationsMap[x + i][y + j] != 0) neighbors++;
			}

		}


		return neighbors;
	}
};




/*
* Populates the vector "g_NationLanguages" from where nations can pick a
* random language as their own...
*/
class NationLanguageGenerator : public BTAction
{
public:
	NationLanguageGenerator(const std::string& name) : BTAction(name)
	{
		m_Languages.push_back("Smertigg");
		m_Languages.push_back("Blacragfell");
		m_Languages.push_back("Bedale");
		m_Languages.push_back("Soutatis");
		m_Languages.push_back("Intarannia");
		m_Languages.push_back("Nemausey");
		m_Languages.push_back("Wallio");
		m_Languages.push_back("Wallo");
		m_Languages.push_back("Rudianus");
		m_Languages.push_back("Rombothrill");
		m_Languages.push_back("Anknotts");
		m_Languages.push_back("Hindonnus");
		m_Languages.push_back("Stonnus");
		m_Languages.push_back("Kirsr");
		m_Languages.push_back("Kirks");
		m_Languages.push_back("Illead");
		m_Languages.push_back("Atend");
		m_Languages.push_back("Mealfrag");
		m_Languages.push_back("Aeris");
	}


	BTNodeResult command() override final
	{
		using namespace std;

		std::uniform_int_distribution<> dis(0, 100);



		// After names were generate, push them into the public language vec.
		for (auto& l : m_Languages)
		{
			g_NationLanguages.push_back(l);
		}


#ifdef _DEBUG_OUT
		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;
#endif

		return BTNodeResult::SUCCESS;
	}


	// For now just define several languages from which to pick.
	// Later Generator will generate them with options...
	std::vector<std::string> m_Languages;
};





/*
* Populate for each nation common data, like count of neighbors.
* Neighbors are defined in
*/
class TierThreeNationGenerator : public BTAction
{
public:
	TierThreeNationGenerator(const std::string& name) : BTAction(name)
	{
	}


	BTNodeResult command() override final
	{
		using namespace std;

		int neighbors_added = 0;
		for (auto& n : g_Nations)
		{
			for (auto& o : g_Nations)
			{
				// Dont add self as neighbor.
				if (n == o) continue;


				int n_x = n->x_pos, n_y = n->y_pos;
				int o_x = o->x_pos, o_y = o->y_pos;

				// Detect collision -> Neighbors.
				if(n_x - g_NeighborStandardDistance < o_x && 
					n_x + g_NeighborStandardDistance > o_x  &&
					
					n_y - g_NeighborStandardDistance < o_y && 
					n_y + g_NeighborStandardDistance > o_y)
				{
					// Check that we dont add duplicates.
					if (isNationInNeighbors(o, n->m_Neighbors) ||
						isNationInNeighbors(n, o->m_Neighbors))
					{
						continue;
					}


					n->m_Neighbors.push_back(o);
					o->m_Neighbors.push_back(n);

					neighbors_added++;
				}

			}
		}


		if (neighbors_added > 0)
		{
#ifdef _DEBUG_OUT
			cout << color(colors::GREEN) << endl;
			cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;
#endif

			return BTNodeResult::SUCCESS;
		}
		else
		{
#ifdef _DEBUG_OUT
			cout << color(colors::RED) << endl;
			cout << "\"" << name() << "\"->command() = FAILURE" << white << endl;
#endif

			return BTNodeResult::FAILURE;
		}
	}


};




class TierFourNationGenerator : public BTAction
{
public:
	TierFourNationGenerator(const std::string& name) : BTAction(name)
	{
	}


	BTNodeResult command() override final
	{
		using namespace std;

		// We do not allow 100% certain and 0% certain  events from the start.
		std::uniform_real_distribution<> dis(0.3, 0.7);




		// Simulate nations for defined steps.
		for (int i = 0; i < g_NationSimulationSteps; i++)
		{




			// Do for each nation.
			for (auto& nation : g_Nations)
			{


				std::map<HistoricalEvent, double> hist_event_probabilities;

				// Initialize.
				// Randomize probabilities at the start.
				for (int i = 0; i < (int)HistoricalEvent::HIST_MAX_SIZE; i++)
				{
					if (HistoricalEvent(i) == HistoricalEvent::HIST_NOTHING)
					{
						hist_event_probabilities[HistoricalEvent(i)] = 0.5; // Base value that nothing happens.
					}


					hist_event_probabilities[HistoricalEvent(i)] = dis(g_Generator);
				}

				/*
				* Firstly, anti-pairs influence the probability of certain events happening.
				*/
				for (auto& antipair : nation->m_AntiPairs)
				{
					// Define initial probability values according to nation template.


					double nation_tech = antipair.second;


					if (antipair.first.compare("Technologic") == 0)
					{
						// Define positive and neative influences on distinct hist. events.
						hist_event_probabilities[HistoricalEvent::HIST_TECH_UPRISE] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_TECH_DOWN] -= nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_RELIGIOUS_UPRISE] -= nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_RELIGOUS_DOWN] += nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_WAR] -= nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_PEACE] += nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_POPULATION_BOOST] -= nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_POPULATION_DOWN] += nation_tech * 0.25;

					}
					else if (antipair.first.compare("Primitive") == 0)
					{
						// Define positive and neative influences on distinct hist. events.
						hist_event_probabilities[HistoricalEvent::HIST_NOTHING] += nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_POPULATION_BOOST] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_POPULATION_DOWN] -= nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_NEIGHBOR_FEUD] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_FRIENDLY_NEIGHBOR] -= nation_tech * 0.25;
					}
					else if (antipair.first.compare("Warfaring") == 0)
					{
						// Define positive and neative influences on distinct hist. events.
						hist_event_probabilities[HistoricalEvent::HIST_WAR] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_PEACE] -= nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_NEIGHBOR_FEUD] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_FRIENDLY_NEIGHBOR] -= nation_tech * 0.25;
					}
					else if (antipair.first.compare("Pacifist") == 0)
					{
						// Define positive and neative influences on distinct hist. events.
						hist_event_probabilities[HistoricalEvent::HIST_PEACE] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_WAR] -= nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_FRIENDLY_NEIGHBOR] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_NEIGHBOR_FEUD] -= nation_tech * 0.25;
					}
					else if (antipair.first.compare("Industrial") == 0)
					{
						// Define positive and neative influences on distinct hist. events.
						hist_event_probabilities[HistoricalEvent::HIST_INDUSTRY_UPRISE] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_INDUSTRY_DOWN] -= nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_AGRI_DOWN] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_AGRI_UPRISE] -= nation_tech * 0.25;
					}
					else if (antipair.first.compare("Agricultural") == 0)
					{
						// Define positive and neative influences on distinct hist. events.
						hist_event_probabilities[HistoricalEvent::HIST_AGRI_UPRISE] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_AGRI_DOWN] -= nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_INDUSTRY_DOWN] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_INDUSTRY_UPRISE] -= nation_tech * 0.25;
					}
					else if (antipair.first.compare("Isolationist") == 0)
					{
						// Define positive and neative influences on distinct hist. events.
						hist_event_probabilities[HistoricalEvent::HIST_NEIGHBOR_FEUD] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_FRIENDLY_NEIGHBOR] -= nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_NOTHING] += nation_tech * 0.25;
					}
					else if (antipair.first.compare("Expansive") == 0)
					{
						// Define positive and neative influences on distinct hist. events.
						hist_event_probabilities[HistoricalEvent::HIST_LAND_EXPANSION] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_LAND_SHRINK] -= nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_NEIGHBOR_FEUD] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_FRIENDLY_NEIGHBOR] -= nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_TECH_DOWN] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_RELIGOUS_DOWN] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_ECONOMIC_DOWN] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_AGRI_DOWN] += nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_WAR] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_PEACE] -= nation_tech * 0.25;
					}
					else if (antipair.first.compare("Spiritual") == 0)
					{
						// Define positive and neative influences on distinct hist. events.
						hist_event_probabilities[HistoricalEvent::HIST_RELIGIOUS_UPRISE] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_RELIGOUS_DOWN] -= nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_TECH_DOWN] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_TECH_UPRISE] -= nation_tech * 0.25;
					}
					else if (antipair.first.compare("Materialistic") == 0)
					{
						// Define positive and neative influences on distinct hist. events.
						hist_event_probabilities[HistoricalEvent::HIST_GOV_EST] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_GOV_DESTROY] -= nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_ECONOMIC_UPRISE] += nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_RELIGOUS_DOWN] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_RELIGIOUS_UPRISE] -= nation_tech * 0.25;
					}
					else if (antipair.first.compare("Economic") == 0)
					{
						// Define positive and neative influences on distinct hist. events.
						hist_event_probabilities[HistoricalEvent::HIST_ECONOMIC_UPRISE] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_ECONOMIC_DOWN] -= nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_TECH_DOWN] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_TECH_UPRISE] -= nation_tech * 0.25;


						hist_event_probabilities[HistoricalEvent::HIST_RELIGOUS_DOWN] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_RELIGIOUS_UPRISE] -= nation_tech * 0.25;
					}
					else if (antipair.first.compare("Non-Economic") == 0)
					{
						// Define positive and neative influences on distinct hist. events.
						hist_event_probabilities[HistoricalEvent::HIST_ECONOMIC_DOWN] += nation_tech * 0.25;
						hist_event_probabilities[HistoricalEvent::HIST_ECONOMIC_UPRISE] -= nation_tech * 0.25;
					}

				}


				// Normalize probabilities to be in range 0.0 - 1.0.
				for (auto& pair : hist_event_probabilities)
				{
					if (pair.second > 1.0) pair.second = 1.0;
					if (pair.second < 0.0) pair.second = 0.0;
				}


#ifdef _DEBUG_OUT
				cout << color(colors::RED);
				cout << "Historical Event Probabilities for: \"" << nation->m_Race << "\"" << white << endl;
				for (auto& p : hist_event_probabilities)
				{
					cout << color(colors::YELLOW);
					cout << "\"" << historicalEventString(p.first) << "\":";

					cout << color(colors::CYAN);
					cout << p.second << white << endl;
				}
#endif




				/*
				* Secondly, past history events influence the probability of current events occuring.
				*/
				int hist_event_depth = 0;
				for (auto& past_event : nation->m_History)
				{
					hist_event_depth++;

					switch (past_event)
					{
					case HistoricalEvent::HIST_AGRI_DOWN:

						if (nation->operator()("Agricultural") > 0.5) // More probable to stop agri shrink...
						{
							hist_event_probabilities[HistoricalEvent::HIST_AGRI_DOWN] -= 0.3;
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_AGRI_DOWN] -= 0.1;
						}
						break;



					case HistoricalEvent::HIST_AGRI_UPRISE:

						if (nation->operator()("Agricultural") > 0.5) // More probable to expand agri...
						{
							hist_event_probabilities[HistoricalEvent::HIST_AGRI_UPRISE] -= 0.1;
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_AGRI_UPRISE] -= 0.3;
						}
						break;



					case HistoricalEvent::HIST_ECONOMIC_DOWN:

						if (nation->operator()("Economic") > 0.5) // More probable to stop economy shrink...
						{
							hist_event_probabilities[HistoricalEvent::HIST_ECONOMIC_DOWN] -= 0.3;
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_ECONOMIC_DOWN] -= 0.1;
						}
						break;



					case HistoricalEvent::HIST_ECONOMIC_UPRISE:

						if (nation->operator()("Economic") > 0.5) // More probable to expand economy...
						{
							hist_event_probabilities[HistoricalEvent::HIST_ECONOMIC_UPRISE] -= 0.1;
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_ECONOMIC_UPRISE] -= 0.3;
						}
						break;



					case HistoricalEvent::HIST_GOV_DESTROY:

						if (nation->operator()("Primitive") > 0.5 && nation->m_GovForm != Nation::GovernmentForm::GOV_NONE) // Primitives are more likely to destroy current governement...
						{
							hist_event_probabilities[HistoricalEvent::HIST_GOV_DESTROY] += 0.3 + hist_event_depth * 0.1; // ... even more likely, if they havent destroyed one in the last time...
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_GOV_DESTROY] += 0.1; // Else simulate tension in society...
						}
						break;




					case HistoricalEvent::HIST_GOV_EST:

						if (nation->operator()("Primitive") > 0.5) // Basically, if a government was established lastly, then dont do it again...
						{
							hist_event_probabilities[HistoricalEvent::HIST_GOV_EST] -= 0.5 - hist_event_depth * 0.1; // For primitives it is less possible to establish a government...
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_GOV_EST] -= 0.3;
						}
						break;




					case HistoricalEvent::HIST_INDUSTRY_DOWN:

						if (nation->operator()("Industrial") > 0.5) // More probable to stop industry shrink...
						{
							hist_event_probabilities[HistoricalEvent::HIST_INDUSTRY_DOWN] -= 0.3;
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_INDUSTRY_DOWN] -= 0.1;
						}
						break;



					case HistoricalEvent::HIST_INDUSTRY_UPRISE:

						if (nation->operator()("Industrial") > 0.5) // More probable to expand industry...
						{
							hist_event_probabilities[HistoricalEvent::HIST_INDUSTRY_UPRISE] -= 0.1;
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_INDUSTRY_UPRISE] -= 0.3;
						}
						break;



					case HistoricalEvent::HIST_LAND_EXPANSION:

						if (nation->operator()("Expansive") > 0.5 && nation->m_Neighbors.size() > 2) // More probable to expand if enough land...
						{
							hist_event_probabilities[HistoricalEvent::HIST_LAND_EXPANSION] -= 0.1;
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_LAND_EXPANSION] -= 0.3;
						}
						break;



					case HistoricalEvent::HIST_LAND_SHRINK:

						if (nation->operator()("Expansive") > 0.5) // More probable to stop shrink...
						{
							hist_event_probabilities[HistoricalEvent::HIST_LAND_SHRINK] -= 0.3;
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_LAND_SHRINK] -= 0.1;
						}
						break;




					case HistoricalEvent::HIST_FRIENDLY_NEIGHBOR:

						if (nation->operator()("Warfaring") > 0.5 && nation->m_Neighbors.size() > 0) // Warfaring nation and has neighbors... meaning conflict.
						{
							hist_event_probabilities[HistoricalEvent::HIST_FRIENDLY_NEIGHBOR] -= 0.3;
						}
						else // Here we could add a check for "Pacifist".
						{
							hist_event_probabilities[HistoricalEvent::HIST_FRIENDLY_NEIGHBOR] += 0.2; // Simulate building up a friendship...
						}
						break;



					case HistoricalEvent::HIST_NEIGHBOR_FEUD:

						if (nation->operator()("Warfaring") > 0.5 && nation->m_Neighbors.size() > 0) // Warfaring nation and has neighbors...
						{
							hist_event_probabilities[HistoricalEvent::HIST_NEIGHBOR_FEUD] += 0.3;
						}
						else // Here we could add a check for "Pacifist".
						{
							hist_event_probabilities[HistoricalEvent::HIST_NEIGHBOR_FEUD] -= 0.3;
						}
						break;



					case HistoricalEvent::HIST_NOTHING:
						hist_event_probabilities[HistoricalEvent::HIST_NOTHING] -= 0.1; // Constraint...
						break;



					case HistoricalEvent::HIST_POPULATION_BOOST:

						hist_event_probabilities[HistoricalEvent::HIST_POPULATION_BOOST] -= 0.1; // Constraint...
						break;



					case HistoricalEvent::HIST_POPULATION_DOWN:

						hist_event_probabilities[HistoricalEvent::HIST_POPULATION_DOWN] -= 0.1; // Constraint...
						break;



					case HistoricalEvent::HIST_RELIGIOUS_UPRISE:

						if (nation->operator()("Spiritual") > 0.5)
						{
							hist_event_probabilities[HistoricalEvent::HIST_RELIGIOUS_UPRISE] -= 0.1; // Constraint...
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_RELIGIOUS_UPRISE] -= 0.3;
						}
						break;



					case HistoricalEvent::HIST_RELIGOUS_DOWN:

						if (nation->operator()("Spiritual") > 0.5)
						{
							hist_event_probabilities[HistoricalEvent::HIST_RELIGOUS_DOWN] -= 0.2; // Spiritual nation suffer less from past spritiual downfalls...
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_RELIGOUS_DOWN] -= 0.1;
						}
						break;



					case HistoricalEvent::HIST_TECH_DOWN:

						if (nation->operator()("Technologic") > 0.5)
						{
							hist_event_probabilities[HistoricalEvent::HIST_TECH_DOWN] -= 0.2; // Techie nation suffer less from past tech downfalls...
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_TECH_DOWN] -= 0.1;
						}
						break;



					case HistoricalEvent::HIST_TECH_UPRISE:

						if (nation->operator()("Technologic") > 0.5)
						{
							hist_event_probabilities[HistoricalEvent::HIST_TECH_UPRISE] -= 0.1; // Constraint on tech uprise, lesser if nation specializes in tech...
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_TECH_UPRISE] -= 0.2;
						}
						break;



					case HistoricalEvent::HIST_PEACE:

						if (nation->operator()("Warfaring") > 0.5)
						{
							hist_event_probabilities[HistoricalEvent::HIST_PEACE] -= 0.3;
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_PEACE] -= 0.1;
						}
						break;



					case HistoricalEvent::HIST_WAR:

						if (nation->operator()("Warfaring") > 0.5)
						{
							hist_event_probabilities[HistoricalEvent::HIST_WAR] -= 0.1; // Simulate war weariness, which is lesser for nations specializing in war...
						}
						else
						{
							hist_event_probabilities[HistoricalEvent::HIST_WAR] -= 0.3; // Simulate war weariness.
						}
						break;

					}
				}




				// Normalize probabilities to be in range 0.0 - 1.0.
				for (auto& pair : hist_event_probabilities)
				{
					if (pair.second > 1.0) pair.second = 1.0;
					if (pair.second < 0.0) pair.second = 0.0;
				}



				/*
				* Lastly, choose most probable event to happen and simulate his occurence.
				*/
				HistoricalEvent most_prob_event;
				double prob = 0.0;
				for (auto& pair : hist_event_probabilities)
				{
					if (pair.second > prob)
					{
						// Update new most probable event.
						most_prob_event = pair.first;
						prob = pair.second;
					}
				}


				// Resolve what happens on current historical event.
				switch (most_prob_event)
				{
					case HistoricalEvent::HIST_AGRI_DOWN:
						nation->m_AntiPairs["Agricultural"] -= 0.1;
						nation->m_Population -= 0.2;
						break;
					case HistoricalEvent::HIST_AGRI_UPRISE:
						nation->m_AntiPairs["Agricultural"] += 0.1;
						nation->m_Population += 0.2;
						break;
					case HistoricalEvent::HIST_ECONOMIC_DOWN:
						nation->m_AntiPairs["Economic"] -= 0.1;
						nation->m_Population -= 0.1;
						break;
					case HistoricalEvent::HIST_ECONOMIC_UPRISE:
						nation->m_AntiPairs["Economic"] += 0.1;
						nation->m_Population += 0.2;
						break;
					case HistoricalEvent::HIST_GOV_DESTROY: // TODO
						break;
					case HistoricalEvent::HIST_GOV_EST: // TODO
						break;
					case HistoricalEvent::HIST_INDUSTRY_DOWN:
						nation->m_AntiPairs["Industrial"] -= 0.1;
						break;
					case HistoricalEvent::HIST_INDUSTRY_UPRISE:
						nation->m_AntiPairs["Industrial"] += 0.1;
						break;
					case HistoricalEvent::HIST_LAND_EXPANSION: // TODO
						nation->m_AntiPairs["Expansive"] += 0.1;
						nation->m_Territory += 1.0;
						break;
					case HistoricalEvent::HIST_LAND_SHRINK: // TODO
						nation->m_AntiPairs["Expansive"] -= 0.1;
						nation->m_Territory -= 1.0;
						break;
					case HistoricalEvent::HIST_FRIENDLY_NEIGHBOR:
						nation->m_AntiPairs["Pacifist"] += 0.1;
						break;
					case HistoricalEvent::HIST_NEIGHBOR_FEUD:
						nation->m_AntiPairs["Pacifist"] -= 0.1;
						break;
					case HistoricalEvent::HIST_NOTHING:
						nation->m_Population += 0.2;
						break;
					case HistoricalEvent::HIST_POPULATION_BOOST: // TODO
						nation->m_Population += 1.0;
						break;
					case HistoricalEvent::HIST_POPULATION_DOWN: // TODO
						nation->m_Population -= 1.0;
						break;
					case HistoricalEvent::HIST_RELIGIOUS_UPRISE:
						nation->m_AntiPairs["Spiritual"] += 0.1;
						break;
					case HistoricalEvent::HIST_RELIGOUS_DOWN:
						nation->m_AntiPairs["Spiritual"] -= 0.1;
						break;
					case HistoricalEvent::HIST_TECH_DOWN:
						nation->m_AntiPairs["Technologic"] -= 0.1;
						break;
					case HistoricalEvent::HIST_TECH_UPRISE:
						nation->m_AntiPairs["Technologic"] += 0.1;
						break;
					case HistoricalEvent::HIST_PEACE: // TODO
						nation->m_Population += 0.2;
						break;
					case HistoricalEvent::HIST_WAR: // TODO
						nation->m_Population -= 0.2;
						break;
				}


				// Define territory with influence of population...
				nation->m_Territory += nation->m_Population * 0.01;


				// Normalize the anti-pairs of a nation to be 1.0 if summed.
				for (auto& pair : nation->m_AntiPairs)
				{
					if (nation->operator()(pair.first) + nation->operator()(g_AntiPairs[pair.first]) > 1.0) // Sum greater than 1.0, disbalance...
					{
						// Policy, decrease the greater value...
						double first = nation->operator()(pair.first);
						double second = nation->operator()(g_AntiPairs[pair.first]);

						double decrease_value = (first + second) - 1.0;

						if (first > second)
						{
							nation->m_AntiPairs[pair.first] -= decrease_value; // Decrease the value of current trait.
						}
						else
						{
							nation->m_AntiPairs[g_AntiPairs[pair.first]] -= decrease_value; // Decrease the value of the anti-pair.
						}
					}
				}


				// Normalize anti-pair values a last time.
				for (auto& pair : nation->m_AntiPairs)
				{
					if (pair.second > 1.0) pair.second = 1.0;
					if (pair.second < 0.0) pair.second = 0.0;
				}

#ifdef _DEBUG_OUT
				cout << color(colors::RED);
				cout << "Historical Event for: \"" << nation->m_Race << "\"" << white << endl;

				cout << color(colors::YELLOW);
				cout << "Event - \"" << historicalEventString(most_prob_event) << "\" with Prob. - ";

				cout << color(colors::CYAN);
				cout << prob << white << endl;
#endif
			}






		}







		if (true)
		{
#ifdef _DEBUG_OUT
			cout << color(colors::GREEN) << endl;
			cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;
#endif

			return BTNodeResult::SUCCESS;
		}
		else
		{
#ifdef _DEBUG_OUT
			cout << color(colors::RED) << endl;
			cout << "\"" << name() << "\"->command() = FAILURE" << white << endl;
#endif

			return BTNodeResult::FAILURE;
		}
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

		g_NationsMap.resize(g_VecSize);
		for (auto& v : g_NationsMap)
		{
			v.resize(g_VecSize);
		}


		/*
		* Defining the anti pairs.
		*/
		g_AntiPairs.emplace("Spiritual", "Materialistic");
		g_AntiPairs.emplace("Technologic", "Primitive");
		g_AntiPairs.emplace("Economic", "Non-Economic");
		g_AntiPairs.emplace("Isolationist", "Expansive");
		g_AntiPairs.emplace("Industrial", "Agricultural");
		g_AntiPairs.emplace("Warfaring", "Pacifist");

		/*
		* Defining beginning racial traits from which to choose from.
		*/
		std::vector<std::string> race_traits;
		g_RacialTraitsArchetypes.emplace(Nation::Race::RACE_HUMAN, race_traits);



		/*
		* Defining archetypes of nations from which to choose from.
		*/
		Nation* technocrat = new Nation();
		Nation* trader = new Nation();
		Nation* non_trader = new Nation();
		Nation* religious = new Nation();
		Nation* atheist = new Nation();
		Nation* warfaring = new Nation();
		Nation* primitive = new Nation();
		Nation* pacifist = new Nation();
		Nation* idustrial = new Nation();
		Nation* isolationist = new Nation();
		Nation* expansive = new Nation();
		Nation* agricultural = new Nation();

		/*
		* Define anti-pair values for each nation archetype.
		*/
		technocrat->m_AntiPairs.emplace("Technologic", 0.7);
		technocrat->m_AntiPairs.emplace("Primitive", 0.3);
		technocrat->m_AntiPairs.emplace("Warfaring", 0.2);
		technocrat->m_AntiPairs.emplace("Pacifist", 0.8);
		technocrat->m_AntiPairs.emplace("Industrial", 0.6);
		technocrat->m_AntiPairs.emplace("Agricultural", 0.4);
		technocrat->m_AntiPairs.emplace("Isolationist", 0.4);
		technocrat->m_AntiPairs.emplace("Expansive", 0.6);
		technocrat->m_AntiPairs.emplace("Spiritual", 0.1);
		technocrat->m_AntiPairs.emplace("Materialistic", 0.9);
		technocrat->m_AntiPairs.emplace("Economic", 0.4);
		technocrat->m_AntiPairs.emplace("Non-Economic", 0.6);

		trader->m_AntiPairs.emplace("Technologic", 0.4);
		trader->m_AntiPairs.emplace("Primitive", 0.6);
		trader->m_AntiPairs.emplace("Warfaring", 0.2);
		trader->m_AntiPairs.emplace("Pacifist", 0.8);
		trader->m_AntiPairs.emplace("Industrial", 0.5);
		trader->m_AntiPairs.emplace("Agricultural", 0.5);
		trader->m_AntiPairs.emplace("Isolationist", 0.2);
		trader->m_AntiPairs.emplace("Expansive", 0.8);
		trader->m_AntiPairs.emplace("Spiritual", 0.2);
		trader->m_AntiPairs.emplace("Materialistic", 0.8);
		trader->m_AntiPairs.emplace("Economic", 0.9);
		trader->m_AntiPairs.emplace("Non-Economic", 0.1);

		non_trader->m_AntiPairs.emplace("Technologic", 0.5);
		non_trader->m_AntiPairs.emplace("Primitive", 0.5);
		non_trader->m_AntiPairs.emplace("Warfaring", 0.5);
		non_trader->m_AntiPairs.emplace("Pacifist", 0.5);
		non_trader->m_AntiPairs.emplace("Industrial", 0.5);
		non_trader->m_AntiPairs.emplace("Agricultural", 0.5);
		non_trader->m_AntiPairs.emplace("Isolationist", 0.5);
		non_trader->m_AntiPairs.emplace("Expansive", 0.5);
		non_trader->m_AntiPairs.emplace("Spiritual", 0.5);
		non_trader->m_AntiPairs.emplace("Materialistic", 0.5);
		non_trader->m_AntiPairs.emplace("Economic", 0.2);
		non_trader->m_AntiPairs.emplace("Non-Economic", 0.8);

		religious->m_AntiPairs.emplace("Technologic", 0.3);
		religious->m_AntiPairs.emplace("Primitive", 0.7);
		religious->m_AntiPairs.emplace("Warfaring", 0.5);
		religious->m_AntiPairs.emplace("Pacifist", 0.5);
		religious->m_AntiPairs.emplace("Industrial", 0.3);
		religious->m_AntiPairs.emplace("Agricultural", 0.7);
		religious->m_AntiPairs.emplace("Isolationist", 0.5);
		religious->m_AntiPairs.emplace("Expansive", 0.5);
		religious->m_AntiPairs.emplace("Spiritual", 0.8);
		religious->m_AntiPairs.emplace("Materialistic", 0.2);
		religious->m_AntiPairs.emplace("Economic", 0.3);
		religious->m_AntiPairs.emplace("Non-Economic", 0.7);

		atheist->m_AntiPairs.emplace("Technologic", 0.5);
		atheist->m_AntiPairs.emplace("Primitive", 0.5);
		atheist->m_AntiPairs.emplace("Warfaring", 0.5);
		atheist->m_AntiPairs.emplace("Pacifist", 0.5);
		atheist->m_AntiPairs.emplace("Industrial", 0.5);
		atheist->m_AntiPairs.emplace("Agricultural", 0.5);
		atheist->m_AntiPairs.emplace("Isolationist", 0.5);
		atheist->m_AntiPairs.emplace("Expansive", 0.5);
		atheist->m_AntiPairs.emplace("Spiritual", 0.1);
		atheist->m_AntiPairs.emplace("Materialistic", 0.9);
		atheist->m_AntiPairs.emplace("Economic", 0.5);
		atheist->m_AntiPairs.emplace("Non-Economic", 0.5);

		warfaring->m_AntiPairs.emplace("Technologic", 0.3);
		warfaring->m_AntiPairs.emplace("Primitive", 0.7);
		warfaring->m_AntiPairs.emplace("Warfaring", 0.9);
		warfaring->m_AntiPairs.emplace("Pacifist", 0.1);
		warfaring->m_AntiPairs.emplace("Industrial", 0.5);
		warfaring->m_AntiPairs.emplace("Agricultural", 0.5);
		warfaring->m_AntiPairs.emplace("Isolationist", 0.2);
		warfaring->m_AntiPairs.emplace("Expansive", 0.8);
		warfaring->m_AntiPairs.emplace("Spiritual", 0.5);
		warfaring->m_AntiPairs.emplace("Materialistic", 0.5);
		warfaring->m_AntiPairs.emplace("Economic", 0.3);
		warfaring->m_AntiPairs.emplace("Non-Economic", 0.7);

		primitive->m_AntiPairs.emplace("Technologic", 0.1);
		primitive->m_AntiPairs.emplace("Primitive", 0.9);
		primitive->m_AntiPairs.emplace("Warfaring", 0.5);
		primitive->m_AntiPairs.emplace("Pacifist", 0.5);
		primitive->m_AntiPairs.emplace("Industrial", 0.2);
		primitive->m_AntiPairs.emplace("Agricultural", 0.8);
		primitive->m_AntiPairs.emplace("Isolationist", 0.5);
		primitive->m_AntiPairs.emplace("Expansive", 0.5);
		primitive->m_AntiPairs.emplace("Spiritual", 0.5);
		primitive->m_AntiPairs.emplace("Materialistic", 0.5);
		primitive->m_AntiPairs.emplace("Economic", 0.3);
		primitive->m_AntiPairs.emplace("Non-Economic", 0.7);

		pacifist->m_AntiPairs.emplace("Technologic", 0.5);
		pacifist->m_AntiPairs.emplace("Primitive", 0.5);
		pacifist->m_AntiPairs.emplace("Warfaring", 0.1);
		pacifist->m_AntiPairs.emplace("Pacifist", 0.9);
		pacifist->m_AntiPairs.emplace("Industrial", 0.5);
		pacifist->m_AntiPairs.emplace("Agricultural", 0.5);
		pacifist->m_AntiPairs.emplace("Isolationist", 0.5);
		pacifist->m_AntiPairs.emplace("Expansive", 0.5);
		pacifist->m_AntiPairs.emplace("Spiritual", 0.5);
		pacifist->m_AntiPairs.emplace("Materialistic", 0.5);
		pacifist->m_AntiPairs.emplace("Economic", 0.5);
		pacifist->m_AntiPairs.emplace("Non-Economic", 0.5);

		idustrial->m_AntiPairs.emplace("Technologic", 0.5);
		idustrial->m_AntiPairs.emplace("Primitive", 0.5);
		idustrial->m_AntiPairs.emplace("Warfaring", 0.5);
		idustrial->m_AntiPairs.emplace("Pacifist", 0.5);
		idustrial->m_AntiPairs.emplace("Industrial", 0.8);
		idustrial->m_AntiPairs.emplace("Agricultural", 0.2);
		idustrial->m_AntiPairs.emplace("Isolationist", 0.5);
		idustrial->m_AntiPairs.emplace("Expansive", 0.5);
		idustrial->m_AntiPairs.emplace("Spiritual", 0.5);
		idustrial->m_AntiPairs.emplace("Materialistic", 0.5);
		idustrial->m_AntiPairs.emplace("Economic", 0.4);
		idustrial->m_AntiPairs.emplace("Non-Economic", 0.6);

		isolationist->m_AntiPairs.emplace("Technologic", 0.3);
		isolationist->m_AntiPairs.emplace("Primitive", 0.7);
		isolationist->m_AntiPairs.emplace("Warfaring", 0.5);
		isolationist->m_AntiPairs.emplace("Pacifist", 0.5);
		isolationist->m_AntiPairs.emplace("Industrial", 0.5);
		isolationist->m_AntiPairs.emplace("Agricultural", 0.5);
		isolationist->m_AntiPairs.emplace("Isolationist", 0.7);
		isolationist->m_AntiPairs.emplace("Expansive", 0.3);
		isolationist->m_AntiPairs.emplace("Spiritual", 0.5);
		isolationist->m_AntiPairs.emplace("Materialistic", 0.5);
		isolationist->m_AntiPairs.emplace("Economic", 0.2);
		isolationist->m_AntiPairs.emplace("Non-Economic", 0.8);

		expansive->m_AntiPairs.emplace("Technologic", 0.5);
		expansive->m_AntiPairs.emplace("Primitive", 0.5);
		expansive->m_AntiPairs.emplace("Warfaring", 0.6);
		expansive->m_AntiPairs.emplace("Pacifist", 0.4);
		expansive->m_AntiPairs.emplace("Industrial", 0.5);
		expansive->m_AntiPairs.emplace("Agricultural", 0.5);
		expansive->m_AntiPairs.emplace("Isolationist", 0.2);
		expansive->m_AntiPairs.emplace("Expansive", 0.8);
		expansive->m_AntiPairs.emplace("Spiritual", 0.5);
		expansive->m_AntiPairs.emplace("Materialistic", 0.5);
		expansive->m_AntiPairs.emplace("Economic", 0.5);
		expansive->m_AntiPairs.emplace("Non-Economic", 0.5);

		agricultural->m_AntiPairs.emplace("Technologic", 0.3);
		agricultural->m_AntiPairs.emplace("Primitive", 0.7);
		agricultural->m_AntiPairs.emplace("Warfaring", 0.4);
		agricultural->m_AntiPairs.emplace("Pacifist", 0.6);
		agricultural->m_AntiPairs.emplace("Industrial", 0.2);
		agricultural->m_AntiPairs.emplace("Agricultural", 0.8);
		agricultural->m_AntiPairs.emplace("Isolationist", 0.5);
		agricultural->m_AntiPairs.emplace("Expansive", 0.5);
		agricultural->m_AntiPairs.emplace("Spiritual", 0.5);
		agricultural->m_AntiPairs.emplace("Materialistic", 0.5);
		agricultural->m_AntiPairs.emplace("Economic", 0.5);
		agricultural->m_AntiPairs.emplace("Non-Economic", 0.5);

		atheist->m_AntiPairs.emplace("Technologic", 0.6);
		atheist->m_AntiPairs.emplace("Primitive", 0.4);
		atheist->m_AntiPairs.emplace("Warfaring", 0.5);
		atheist->m_AntiPairs.emplace("Pacifist", 0.5);
		atheist->m_AntiPairs.emplace("Industrial", 0.5);
		atheist->m_AntiPairs.emplace("Agricultural", 0.5);
		atheist->m_AntiPairs.emplace("Isolationist", 0.5);
		atheist->m_AntiPairs.emplace("Expansive", 0.5);
		atheist->m_AntiPairs.emplace("Spiritual", 0.1);
		atheist->m_AntiPairs.emplace("Materialistic", 0.9);
		atheist->m_AntiPairs.emplace("Economic", 0.5);
		atheist->m_AntiPairs.emplace("Non-Economic", 0.5);

		non_trader->m_AntiPairs.emplace("Technologic", 0.5);
		non_trader->m_AntiPairs.emplace("Primitive", 0.5);
		non_trader->m_AntiPairs.emplace("Warfaring", 0.5);
		non_trader->m_AntiPairs.emplace("Pacifist", 0.5);
		non_trader->m_AntiPairs.emplace("Industrial", 0.5);
		non_trader->m_AntiPairs.emplace("Agricultural", 0.5);
		non_trader->m_AntiPairs.emplace("Isolationist", 0.8);
		non_trader->m_AntiPairs.emplace("Expansive", 0.2);
		non_trader->m_AntiPairs.emplace("Spiritual", 0.5);
		non_trader->m_AntiPairs.emplace("Materialistic", 0.5);
		non_trader->m_AntiPairs.emplace("Economic", 0.1);
		non_trader->m_AntiPairs.emplace("Non-Economic", 0.9);


		g_NationArchetypes.emplace("Technologic", technocrat);
		g_NationArchetypes.emplace("Economic", trader);
		g_NationArchetypes.emplace("Spiritual", religious);
		g_NationArchetypes.emplace("Warfaring", warfaring);
		g_NationArchetypes.emplace("Primitive", primitive);
		g_NationArchetypes.emplace("Pacifist", pacifist);
		g_NationArchetypes.emplace("Industrial", idustrial);
		g_NationArchetypes.emplace("Isolationist", isolationist);
		g_NationArchetypes.emplace("Expansive", expansive);
		g_NationArchetypes.emplace("Agricultural", agricultural);
		g_NationArchetypes.emplace("Materialistic", atheist);
		g_NationArchetypes.emplace("Non-Economic", non_trader);

	}

public:
	bool OnUserCreate() override
	{
		m_AIEngine = new AIEngine();


		BehaviorTree* tree = new BehaviorTree("Nation Generator");

		BTSequence* seq = new BTSequence("Starting Sequence");
		BTTimer* timer = new BTTimer("Timer", BTTimer::Granularity::Seconds, BTTimer::Policy::Greater, 0.0);
		NationLanguageGenerator* langgen = new NationLanguageGenerator("Language Generator");
		TierZeroNationGenerator* zerogen = new TierZeroNationGenerator("Culture DIstribution Generator");
		TierOneNationGenerator* onegen = new TierOneNationGenerator("Racial Distribution Generator");
		TierTwoNationGenerator* twogen = new TierTwoNationGenerator("Nation Placement Generator");
		TierThreeNationGenerator* threegen = new TierThreeNationGenerator("Nation Neighbor Counter");
		TierFourNationGenerator* fourgen = new TierFourNationGenerator("Nation Historical Events Generator");

		seq->addChild(timer);
		seq->addChild(langgen);
		seq->addChild(zerogen);
		seq->addChild(onegen);
		seq->addChild(twogen);
		seq->addChild(threegen);
		seq->addChild(fourgen);


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

		seed = rand() % 250;
		g_Noise = new PerlinNoise(seed);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		using namespace std;

		Clear(olc::Pixel(1, 1, 1, 1));
		

		if (GetKey(olc::Key::BACK).bPressed)
		{
			std::mt19937 generator(seed); // Seed.
			std::uniform_int_distribution<> dis(0, 1000);
			seed = dis(generator);

			// Clear Nations before reconstructing everything.
			if (g_Nations.size() > 0)
			{
				g_Nations.clear();
				g_NationsMap.clear();

				g_NationsMap.resize(g_VecSize);
				for (auto& v : g_NationsMap)
				{
					v.resize(g_VecSize);
				}
			}



			m_AIEngine->update();
			g_NationsGenerated = true;
		}


		if (GetKey(olc::Key::SPACE).bPressed)
		{
			perlin_xy_scalar = 12.90;
			perlin_redistribution_scalar = 5.0;
			perlin_octave_scalar = 1.0;
			perlin_polar = 0.9;
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

				/*
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
				*/

				/*
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
				*/

				/*
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
				*/
				if (d < 0.2) // Ocean
				{					
					g_BiomeMap[i][j] = new Biome("Ocean", d, m);
				}
				else if (d >= 0.2 && d < 0.25) // Sea
				{
					g_BiomeMap[i][j] = new Biome("Sea", d, m);

				}
				else if (d >= 0.25 && d < 0.3) // Sand
				{
					g_BiomeMap[i][j] = new Biome("Sand", d, m);
				}
				else if (d >= 0.3 && d < 0.4) // Savannah
				{
					if (m < 0.5)
					{
						g_BiomeMap[i][j] = new Biome("Tundra", d, m);

					}
					else if (m >= 0.5)
					{
						g_BiomeMap[i][j] = new Biome("Savannah", d, m);

					}
				}
				else if (d >= 0.4 && d < 0.9) // Land / Jungle
				{
					if (m >= 0.3)
					{
						g_BiomeMap[i][j] = new Biome("Jungle", d, m);

					}
					else if (m < 0.3)
					{
						g_BiomeMap[i][j] = new Biome("Temperate", d, m);

					}
				}
				else // Mountains
				{
					g_BiomeMap[i][j] = new Biome("Mountain", d, m);
				}


				// Draw final map with trees.
				if (d < 0.2) // Ocean
				{
					Draw(i, j, olc::Pixel(0, 0, 139));
					if (g_TreeMap[i][j] != nullptr)
					{
						if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
					}
				}
				else if (d >= 0.2 && d < 0.25) // Sea
				{
					Draw(i, j, olc::Pixel(0, 178, 238));
					if (g_TreeMap[i][j] != nullptr)
					{
						if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
					}
				}
				else if (d >= 0.25 && d < 0.3) // Sand
				{
					Draw(i, j, olc::Pixel(245, 222, 179));
					if (g_TreeMap[i][j] != nullptr)
					{
						if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
					}
				}
				else if (d >= 0.3 && d < 0.4) // Savannah
				{
					if (m < 0.5)
					{
						Draw(i, j, olc::Pixel(0, 100, 0)); // Tundra Tile.
						if (g_TreeMap[i][j] != nullptr)
						{
							if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
						}
					}
					else if (m >= 0.5)
					{
						Draw(i, j, olc::Pixel(244, 164, 96)); // Savannah
						if (g_TreeMap[i][j] != nullptr)
						{
							if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
						}
					}
				}
				else if (d >= 0.4 && d < 0.9) // Land / Jungle
				{
					if (m >= 0.3)
					{
						Draw(i, j, olc::Pixel(75, 0, 130)); // Jungle Tile.
						if (g_TreeMap[i][j] != nullptr)
						{
							if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
						}
					}
					else if (m < 0.3)
					{
						Draw(i, j, olc::Pixel(34, 139, 34)); // Normal Temperate.
						if (g_TreeMap[i][j] != nullptr)
						{
							if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
							else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
							{
								FillCircle(i, j, 1, olc::BACK);
							}
						}
					}
				}
				else // Mountains
				{
					Draw(i, j, olc::Pixel(139, 69, 19));
					if (g_TreeMap[i][j] != nullptr)
					{
						if (g_TreeMap[i][j]->biome.compare("Temperate") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Jungle") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Tundra") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
						else if (g_TreeMap[i][j]->biome.compare("Savannah") == 0)
						{
							FillCircle(i, j, 1, olc::BACK);
						}
					}
				}

				/*
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
				*/


				// Draw cultural distribution.
				if (!g_NationsGenerated) continue;

				if (g_CultureDistribution[i][j].compare("Materialistic") == 0)
				{
					Draw(g_VecSize + i, j, olc::Pixel(128, 0, 0));

				}
				else if (g_CultureDistribution[i][j].compare("Primitive") == 0)
				{
					Draw(g_VecSize  + i, j, olc::Pixel(255, 0, 0));
				}
				else if (g_CultureDistribution[i][j].compare("Industrial") == 0)
				{
					Draw(g_VecSize  + i, j, olc::Pixel(255, 69, 0));
				}
				else if (g_CultureDistribution[i][j].compare("Non-Economic") == 0)
				{
					Draw(g_VecSize  + i, j, olc::Pixel(220, 20, 60));
				}
				else if (g_CultureDistribution[i][j].compare("Agricultural") == 0)
				{
					Draw(g_VecSize  + i, j, olc::Pixel(205, 92, 92));
				}
				else if (g_CultureDistribution[i][j].compare("Expansive") == 0)
				{
					Draw(g_VecSize  + i, j, olc::Pixel(240, 128, 128));
				}
				else if (g_CultureDistribution[i][j].compare("Isolationist") == 0)
				{
					Draw(g_VecSize  + i, j, olc::Pixel(233, 150, 122));
				}
				else if (g_CultureDistribution[i][j].compare("Pacifist") == 0)
				{
					Draw(g_VecSize  + i, j, olc::Pixel(250, 128, 114));
				}
				else if (g_CultureDistribution[i][j].compare("Warfaring") == 0)
				{
					Draw(g_VecSize  + i, j, olc::Pixel(255, 160, 122));
				}
				else if (g_CultureDistribution[i][j].compare("Spiritual") == 0)
				{
					Draw(g_VecSize  + i, j, olc::Pixel(240, 128, 128));
				}
				else if (g_CultureDistribution[i][j].compare("Economic") == 0)
				{
					Draw(g_VecSize  + i, j, olc::Pixel(240, 178, 128));
				}
				else if (g_CultureDistribution[i][j].compare("Technologic") == 0)
				{
					Draw(g_VecSize  + i, j, olc::Pixel(240, 230, 128));
				}



				// Draw Racial Distribution.
				if (g_RacialDistribution[i][j] == Nation::Race::RACE_DARKELF)
				{
					Draw(i, g_VecSize + j, olc::Pixel(128, 0, 128)); // Violet
				}
				else if (g_RacialDistribution[i][j] == Nation::Race::RACE_DWARF)
				{
					Draw(i, g_VecSize + j, olc::Pixel(139, 69, 19)); // Dark Brown
				}
				else if (g_RacialDistribution[i][j] == Nation::Race::RACE_GNOME)
				{
					Draw(i, g_VecSize + j, olc::Pixel(222, 184, 135)); // Light Brown
				}
				else if (g_RacialDistribution[i][j] == Nation::Race::RACE_GOBLIN)
				{
					Draw(i, g_VecSize + j, olc::Pixel(173, 255, 47)); // Light Green
				}
				else if (g_RacialDistribution[i][j] == Nation::Race::RACE_HIGHELF)
				{
					Draw(i, g_VecSize + j, olc::Pixel(255, 223, 0)); // Gold
				}
				else if (g_RacialDistribution[i][j] == Nation::Race::RACE_HUMAN)
				{
					Draw(i, g_VecSize + j, olc::Pixel(65, 105, 225)); // Light Blue
				}
				else if (g_RacialDistribution[i][j] == Nation::Race::RACE_ORC)
				{
					Draw(i, g_VecSize + j, olc::Pixel(0, 128, 0)); // Dark Green
				}
				else if (g_RacialDistribution[i][j] == Nation::Race::RACE_TROLL)
				{
					Draw(i, g_VecSize + j, olc::Pixel(0, 255, 255)); // Cyan
				}
				else
				{
					Draw(i, g_VecSize + j, olc::Pixel(255, 0, 0)); // Error Bit
				}

				

				// Draw map with Trees and nations.
				if (g_BiomeMap[i][j] != nullptr)
				{
					if (g_BiomeMap[i][j]->name.compare("Temperate") == 0)
					{
						Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(34, 139, 34));
					}
					else if (g_BiomeMap[i][j]->name.compare("Tundra") == 0)
					{
						Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(0, 100, 0));
					}
					else if (g_BiomeMap[i][j]->name.compare("Jungle") == 0)
					{
						Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(75, 0, 130));
					}
					else if (g_BiomeMap[i][j]->name.compare("Ocean") == 0)
					{
						Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(0, 0, 139));
					}
					else if (g_BiomeMap[i][j]->name.compare("Sea") == 0)
					{
						Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(0, 178, 238));
					}
					else if (g_BiomeMap[i][j]->name.compare("Sand") == 0)
					{
						Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(245, 222, 179));
					}
					else if (g_BiomeMap[i][j]->name.compare("Savannah") == 0)
					{
						Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(244, 164, 96));
					}
					else if (g_BiomeMap[i][j]->name.compare("Mountain") == 0)
					{
						Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(139, 69, 19));
					}



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
		

		// Draw nations with borders.
		for (auto& n : g_Nations)
		{
			Draw(g_VecSize + n->x_pos, g_VecSize + n->y_pos, olc::Pixel(255, 0, 0)); // Some Nation.

			// Draw national Border around self.
			olc::Pixel c = olc::Pixel(n->m_NationColor[0], n->m_NationColor[1], n->m_NationColor[2], 230);
			DrawCircle(g_VecSize + n->x_pos, g_VecSize + n->y_pos, n->m_Territory, c);
		}




		// Check whether hovered over a nation.
		int mousex = GetMouseX();
		int mousey = GetMouseY();
		for (int i = 0; i < g_NationsMap.size(); i++)
		{
			for (int j = 0; j < g_NationsMap[i].size(); j++)
			{

				if (g_NationsMap[i][j] != nullptr)
				{

					if (mousex == g_VecSize + i && mousey == g_VecSize + j)
					{

						// Draw circle around self.
						Nation* nation = g_NationsMap[i][j];
						olc::Pixel c = olc::Pixel(nation->m_NationColor[0], nation->m_NationColor[1], nation->m_NationColor[2], 255);
						DrawCircle(mousex, mousey, 3, c);

						if (g_ShowNeighbors)
						{
							for (auto& n : g_NationsMap[i][j]->m_Neighbors)
							{
								DrawCircle(n->x_pos + g_VecSize, n->y_pos + g_VecSize, 3, olc::GREEN);
							}
						}



						cout << endl;
						cout << endl;

						cout << color(colors::RED);
						cout << "Inst. Addr." << &g_NationsMap[i][j] << endl;
						cout << "Language: " << g_NationsMap[i][j]->m_Language << endl;
						cout << "Neighbors: " << std::to_string(g_NationsMap[i][j]->m_Neighbors.size()) << endl;
						cout << "Race: " << raceToString(g_NationsMap[i][j]->m_Race) << white << endl;


						cout << color(colors::CYAN);
						cout << "Spiritual: " << g_NationsMap[i][j]->operator()("Spiritual") << endl;
						cout << "Materialistic: " << g_NationsMap[i][j]->operator()("Materialistic") << endl;
						cout << "Technologic: " << g_NationsMap[i][j]->operator()("Technologic") << endl;
						cout << "Primitive: " << g_NationsMap[i][j]->operator()("Primitive") << endl;
						cout << "Economic: " << g_NationsMap[i][j]->operator()("Economic") << endl;
						cout << "Non-Economic: " << g_NationsMap[i][j]->operator()("Non-Economic") << endl;
						cout << "Isolationist: " << g_NationsMap[i][j]->operator()("Isolationist") << endl;
						cout << "Expansive: " << g_NationsMap[i][j]->operator()("Expansive") << endl;
						cout << "Industrial: " << g_NationsMap[i][j]->operator()("Industrial") << endl;
						cout << "Agricultural: " << g_NationsMap[i][j]->operator()("Agricultural") << endl;
						cout << "Warfaring: " << g_NationsMap[i][j]->operator()("Warfaring") << endl;
						cout << "Pacifist: " << g_NationsMap[i][j]->operator()("Pacifist") << white << endl;
					}
				}
			}
		}




		
		DrawString(0, 190, "Frequency:" + std::to_string(perlin_xy_scalar));
		DrawString(0, 200, "Octave:" + std::to_string(perlin_octave_scalar));
		DrawString(0, 210, "Redist:" + std::to_string(perlin_redistribution_scalar));
		DrawString(0, 220, "Polar:" + std::to_string(perlin_polar));
		DrawString(0, 230, "Equator:" + std::to_string(perlin_equator));
		
		return true;
	}



private:


	AIEngine* m_AIEngine = nullptr;
};



int main()
{
	AISandbox demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}




std::string pickRandomTrait(std::vector<std::string> vec)
{
	std::uniform_int_distribution<int> dis(0, vec.size() - 1);

	return vec[dis(g_Generator)]; // Pick random index.
}


void pickRandomColor(int v[3])
{
	std::uniform_int_distribution<int> dis(0, 10);

	switch (dis(g_Generator))
	{
	case 0:
		v[0] = 255;
		break;
	case 1:
		v[1] = 255;
		break;
	case 2:
		v[2] = 255;
		break;
	case 3:
		v[0] = 255;
		v[1] = 255;
		break;
	case 4:
		v[0] = 255;
		v[2] = 255;
		break;
	case 5:
		v[1] = 255;
		v[2] = 255;
		break;
	case 6:
		v[0] = 255;
		v[1] = 255;
		v[2] = 255;
		break;
	case 7:
		v[0] = 100;
		v[1] = 255;
		break;
	case 8:
		v[0] = 255;
		v[1] = 100;
		break;
	case 9:
		v[0] = 255;
		v[2] = 100;
		break;
	case 10:
		v[0] = 100;
		v[2] = 255;
		break;

	}
}

std::string pickRandomLanguage(std::vector<std::string> vec)
{
	std::uniform_int_distribution<int> dis(0, vec.size() - 1);

	return vec[dis(g_Generator)]; // Pick random index.
}


Nation::Race pickRandomRace()
{
	std::uniform_int_distribution<int> dis(0, 7);


	Nation::Race r = Nation::RACE_NONE;

	switch (dis(g_Generator))
	{
	case 0:
		r = Nation::Race::RACE_DARKELF;
		break;
	case 1:
		r = Nation::Race::RACE_DWARF;
		break;
	case 2:
		r = Nation::Race::RACE_GNOME;
		break;
	case 3:
		r = Nation::Race::RACE_GOBLIN;
		break;
	case 4:
		r = Nation::Race::RACE_HIGHELF;
		break;
	case 5:
		r = Nation::Race::RACE_HUMAN;
		break;
	case 6:
		r = Nation::Race::RACE_ORC;
		break;
	case 7:
		r = Nation::Race::RACE_TROLL;
		break;
	default:
		break;
	}


	return r;
}


std::string raceToString(Nation::Race r)
{
	switch (r)
	{
	case Nation::Race::RACE_DARKELF:
		return "DarkElf";
	case Nation::Race::RACE_DWARF:
		return "Dwarf";
	case Nation::Race::RACE_GNOME:
		return "Gnome";
	case Nation::Race::RACE_GOBLIN:
		return "Goblin";
	case Nation::Race::RACE_HIGHELF:
		return "HighElf";
	case Nation::Race::RACE_HUMAN:
		return "Human";
	case Nation::Race::RACE_ORC:
		return "Orc";
	case Nation::Race::RACE_TROLL:
		return "Troll";
	default:
		throw std::runtime_error("No Race!");
	}
}


bool isNationInNeighbors(Nation* n, std::list<Nation*> list)
{
	for (auto& o : list)
	{
		if (o == n)
		{
			return true;
		}
	}

	return false;
}


std::string historicalEventString(HistoricalEvent e)
{
	switch (e)
	{
	case HistoricalEvent::HIST_AGRI_DOWN:
		return "agri_down";
	case HistoricalEvent::HIST_AGRI_UPRISE:
		return "agri_uprise";
	case HistoricalEvent::HIST_ECONOMIC_DOWN:
		return "economic_down";
	case HistoricalEvent::HIST_ECONOMIC_UPRISE:
		return "economic_uprise";
	case HistoricalEvent::HIST_GOV_DESTROY:
		return "gov_destroy";
	case HistoricalEvent::HIST_GOV_EST:
		return "gov_est";
	case HistoricalEvent::HIST_INDUSTRY_DOWN:
		return "industry_down";
	case HistoricalEvent::HIST_INDUSTRY_UPRISE:
		return "industry_uprise";
	case HistoricalEvent::HIST_LAND_EXPANSION:
		return "land_expansion";
	case HistoricalEvent::HIST_LAND_SHRINK:
		return "land_shrink";
	case HistoricalEvent::HIST_FRIENDLY_NEIGHBOR:
		return "friendly_neighbor";
	case HistoricalEvent::HIST_NEIGHBOR_FEUD:
		return "neighbor_feud";
	case HistoricalEvent::HIST_NOTHING:
		return "hist_nothing";
	case HistoricalEvent::HIST_POPULATION_BOOST:
		return "pop_boost";
	case HistoricalEvent::HIST_POPULATION_DOWN:
		return "pop_down";
	case HistoricalEvent::HIST_RELIGIOUS_UPRISE:
		return "religious_uprise";
	case HistoricalEvent::HIST_RELIGOUS_DOWN:
		return "religious_down";
	case HistoricalEvent::HIST_TECH_DOWN:
		return "tech_down";
	case HistoricalEvent::HIST_TECH_UPRISE:
		return "tech_uprise";
	case HistoricalEvent::HIST_PEACE:
		return "peace";
	case HistoricalEvent::HIST_WAR:
		return "war";
	}
}