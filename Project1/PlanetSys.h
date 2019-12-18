#pragma once
#include "Planet.h"
#include <vector>
#include <string>

class PMap
{
public:
	PMap(int width,int height)
		:
		fieldWidth(width),
		fieldHeight(height),
		nPanels(width*height),
		origin({ width / 2, height / 2 }),
		emptyField(nPanels, -1),
		field(nPanels, -1)
	{}
	void Clear()
	{
		field = emptyField;
	}
	void Print2DMap(const std::vector<Planet> planets)
	{
		ClearScreen();
		int count = 0; std::vector<int> offsets{ 0,10,100,1000 };
		for (auto p : planets)
		{
			std::pair<int, int> pos = p.Get2DPos(); pos.first += origin.first; pos.second += origin.second;
			if ((pos.second * fieldWidth + pos.first) < nPanels && (pos.second * fieldWidth + pos.first) > 0)
			{
				field[pos.second * fieldWidth + pos.first] = trailLength + offsets[count++];
			}
		}
		for (int y = 0; y < fieldHeight; y++)
		{
			for (int x = 0; x < fieldWidth; x++)
			{
				int val = field[y * fieldWidth + x];
				if (val <= 0)					std::cout << " ";//keep empty
				else if (val > 0 && val < 10)		std::cout << "O";
				else if (val > 10 && val < 100)		std::cout << "X";
				else if (val > 100 && val < 1000)	std::cout << "#";
				else if (val > 1000)				std::cout << ".";
			}
			std::cout << std::endl;
		}
		for (int y = 0; y < fieldHeight; y++)
		{
			for (int x = 0; x < fieldWidth; x++)
			{
				int val = field[y * fieldWidth + x];
				if (val == 0 || val == 10 || val == 100 || val == 1000) field[y * fieldWidth + x] = -1;
				else field[y * fieldWidth + x] = val - 1;
			}
			std::cout << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
private:
	const int fieldWidth;
	const int fieldHeight;
	int nPanels;
	const std::pair<int, int> origin;
	const std::vector<int> emptyField;
	std::vector<int> field;
	const int trailLength = 5;

};

class PlanetSys
{
public:
	PlanetSys() = default;
	void Reset()
	{
		planets = planets_orig;
		timestep = 0;
		pmap.Clear();
	}
	void AddPlanet(Vei3<int> pos)
	{
		planets.push_back(pos);
		nPlanets++;
		pairs = GetAllBilaterals();
		planets_orig = planets;
	}
	void Update(int n=1)
	{
		for (int i = 0; i < n; i++)
		{
			//apply gravity
			for (auto& p : pairs)
			{
				planets[p.first].Gravitate(planets[p.second]);
			}
			// apply velocity
			for (Planet& p : planets)
			{
				p.Move();
			}
			timestep++;
		}
	}
	void Print2D()
	{
		pmap.Print2DMap(planets);
	}
	void PrintCoordinates() const
	{
		std::cout << "=======================================================================" << std::endl;
		std::cout << "Timestep: " << timestep << ". Total energy: " << TotalEnergy()<<"."<< std::endl;
		std::cout << "-----------------------------------------------------------------------" << std::endl; 
		int count = 0;
		for (auto p : planets)
		{
			int strw = 6;
			std::cout << "Planet " << count<< ": ";
			std::cout << "POS (" << std::setw(strw) << p.pos.x << "," << std::setw(strw) << p.pos.y << "," << std::setw(strw) << p.pos.z << "); ";
			std::cout << "VEL (" << std::setw(strw) << p.vel.x << "," << std::setw(strw) << p.vel.y << "," << std::setw(strw) << p.vel.z << "); "<<std::endl;
			count++;
		}
	}
	int TotalEnergy() const
	{
		int energyTotal = 0;
		for (auto p : planets)
		{
			int pot = std::abs(p.pos.x) + std::abs(p.pos.y) + std::abs(p.pos.z);
			int kin = std::abs(p.vel.x) + std::abs(p.vel.y) + std::abs(p.vel.z);
			energyTotal += (pot * kin);
		}
		return energyTotal;
	}
	std::vector < long long int> GetOrbitalResonancePeriods(int starttime = 0)
	{
		Reset();
		for (int i = 0; i < starttime; i++)
		{
			Update();
		}
		PrintCoordinates();
		std::string startPosX;
		std::string startPosY;
		std::string startPosZ;
		for (auto p : planets)
		{
			startPosX+=p.SerializeX();
			startPosY+=p.SerializeY();
			startPosZ+=p.SerializeZ();
		}
		
		std::vector < long long int> orbResPeriods(3, 0);

		int count = 0;
		while (true)
		{
			Update();
			std::string posX;
			std::string posY;
			std::string posZ;
			for (auto p : planets)
			{
				posX += p.SerializeX();
				posY += p.SerializeY();
				posZ += p.SerializeZ();
			}
			{
				if (orbResPeriods[0] == 0 && posX == startPosX)
				{
					orbResPeriods[0] = timestep;
					count++;
					PrintCoordinates();
				}
				if (orbResPeriods[1] == 0 && posY == startPosY)
				{
					orbResPeriods[1] = timestep;
					count++;
					PrintCoordinates();
				}
				if (orbResPeriods[2] == 0 && posZ == startPosZ)
				{
					orbResPeriods[2] = timestep;
					count++;
					PrintCoordinates();
				}

			}
			if (count == 3) break;
		}
		return orbResPeriods;
	}	
private:
	std::vector<std::pair<int, int>> GetAllBilaterals()
	{
		std::string str;
		for (int i = 0; i < nPlanets; i++) str += std::to_string(i);
		std::vector<std::pair<int, int>> p;
		CreateSubsets(str, "", p);
		return p;
	}
	void CreateSubsets(std::string str, std::string sofar, std::vector<std::pair<int, int>>& pairs)
	{
		if (sofar.size() == 2)
		{
			pairs.push_back({ (int)(sofar[0] - '0'), (int)(sofar[1] - '0') });
			return;
		}
		else if (str.size() == 0)
		{
			return;
		}
		else
		{
			//for (int i = 0; i < str.size(); i++)
			{
				char ch = str[0];
				//sofar += ch;
				str.erase(0, 1);
				CreateSubsets(str, sofar, pairs);
				CreateSubsets(str, sofar + ch, pairs);
			}
		}
	}

private:

	std::vector<Planet> planets;
	std::vector<Planet> planets_orig;
	int nPlanets = 0;
	std::vector<std::pair<int, int>> pairs;
	long long int timestep = 0;
	PMap pmap = PMap(160,70);
};
