#ifndef RANKING_HPP
#define RANKING_HPP

#include <string>

#include "ClearSurface.hpp"

struct RankingData
{
	std::string name;
	unsigned int points;
	
	RankingData(const std::string& name = "", unsigned int points = 0);
	~RankingData();
	
	unsigned int save(
		const std::string& filename,
		unsigned int max_slots
	) const;
};

class Ranking
{
private:
	ClearSurface surface;
public:
	Ranking(
		const std::string& filename, Text* text, unsigned int max_points,
		int spacing = 0, bool center = true,
		int left = 0, int top = 0, int right = 0, int bottom = 0
	);
	~Ranking();
	
	void render();
};

#endif
