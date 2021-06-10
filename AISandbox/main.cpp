
#define _DEBUG_OUT
#include "BTNodes.h"
#include "AIEngine.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <vector>
#include <random>
#include <list>

#include "PerlinNoise.h"

static int seed = 0;
static int x_ = 0;
static int y_ = 0;
static int size_ = 50;
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

















std::vector<std::vector<int>> g_Forests;
static const int g_VecSize = 40;



class SpawnForests : public BTAction
{
public:
	SpawnForests(std::string name) : BTAction(name) 
	{
		// Initialize empty forests...
		g_Forests.resize(g_VecSize);
		for (auto& v : g_Forests)
		{
			v.resize(g_VecSize);
		}
	}


	BTNodeResult command() override final
	{
		using namespace std;

		// Create a new forest matrix.
		std::vector<std::vector<int>> new_forests;
		new_forests.resize(g_VecSize);
		for (auto& v : new_forests)
		{
			v.resize(g_VecSize);
		}
		
		for (int i = 0; i < g_Forests.size(); i++)
		{
			for (int j = 0; j < g_Forests[i].size(); j++)
			{
				// Ignore the 0 and last indices.
				if (i == 0 || j == 0 || i == g_Forests.size() - 1 || j == g_Forests[i].size() - 1) continue;


				// Get neighbors.
				int neighbors = _neighbors(i, j);


				// Check conway stuff.
				// Here it always can be just one, either on or off.
				if (neighbors >= 2 && neighbors <= 3)
				{
					new_forests[i][j] = 1;
				}
				else if(neighbors > 3 && neighbors <= 4)
				{
					new_forests[i][j] = 2;
				}
				else if (neighbors == 1)
				{
					new_forests[i][j] = 3;
				}
				else
				{
					new_forests[i][j] = 0;
				}

			}
		}


		// Replace the old one with the new one.
		g_Forests = new_forests;

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
		// On second thought, we define neighbors as not vertical, so only 4.
		int neighbors = 0;
		
		if (g_Forests[x - 1][y - 1] != 0) neighbors++;

		if (g_Forests[x][y - 1] != 0) neighbors++;

		if (g_Forests[x + 1][y - 1] != 0) neighbors++;

		if (g_Forests[x - 1][y] != 0) neighbors++;

		if (g_Forests[x + 1][y] != 0) neighbors++;

		if (g_Forests[x - 1][y + 1] != 0) neighbors++;

		if (g_Forests[x][y + 1] != 0) neighbors++;

		if (g_Forests[x + 1][y + 1] != 0) neighbors++;
		



		return neighbors;
	}
};


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
		BTTimer* timer = new BTTimer("Timer", BTTimer::Granularity::Seconds, BTTimer::Policy::Greater, 3.0);
		SpawnForests* forests = new SpawnForests("Forest Spawner");



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
				g_Forests[i][j] = rand() % 2;
			}

		}

		g_Noise = new PerlinNoise(236);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		using namespace std;

		Clear(olc::Pixel(1, 1, 1, 1));
		
		m_AIEngine->update();
		
		for (int i = 0; i < g_Forests.size(); i++)
		{
			for (int j = 0; j < g_Forests[i].size(); j++)
			{
				if (g_Forests[i][j] == 1)
				{
					Draw(i, j, olc::Pixel(0.299 * 10, 0.587 * 230, 0.114 * 100));
				}
				else if (g_Forests[i][j] == 2)
				{
					Draw(i, j, olc::Pixel(0.299 * 5, 0.587 * 135, 0.114 * 10));
				}
				else if (g_Forests[i][j] == 3)
				{
					Draw(i, j, olc::Pixel(0.299 * 5, 0.587 * 255, 0.114 * 150));
				}
			}
		}


		if (GetKey(olc::Key::R).bPressed)
		{
			for (int i = 0; i < g_VecSize; i++)
			{
				for (int j = 0; j < g_VecSize; j++)
				{
					g_Forests[i][j] = rand() % 2;
				}

			}
		}
		

		
		if (GetKey(olc::Key::R).bPressed)
		{
			delete g_Noise;
			seed = rand();
			x_ = rand();
			y_ = rand();

			g_Noise = new PerlinNoise(seed);
		}


		for (int i = 0; i < size_; i++)
		{
			for (int j = 0; j < size_; j++)
			{
				double x = (double)j / ((double)x_);
				double y = (double)i / ((double)y_);

				double d = 20.0 * g_Noise->noise(i, j, 0.8);
				d = d - int(d);

				int r = d * 255;
				int g = d * 255;
				int b = d * 255;

				Draw(g_VecSize + i, g_VecSize + j, olc::Pixel(r, g, b));
			}
		}
		

		return true;
	}



private:


	AIEngine* m_AIEngine = nullptr;
};



int main()
{
	AISandbox demo;
	if (demo.Construct(128, 128, 8, 8))
		demo.Start();
	return 0;
}