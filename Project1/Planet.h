#pragma once
#include <iomanip>
struct Vei3
{
	int x;
	int y;
	int z;
	bool operator==(Vei3& rhs)
	{
		return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
	}
};

class Planet
{
public:
	Planet(int x, int y, int z)
		:
		pos({x,y,z}),
		vel({0,0,0})
	{}
	void Print()
	{
		std::cout << std::setw(6)<< "Pos=<x=" << pos.x << ", y= " << pos.y << ">, z=" << pos.z << ">, ";
		std::cout << std::setw(6)<< "Vel=<x=" << vel.x << ", y= " << vel.y << ">, z=" << vel.z << ">\n";
	}
	void Gravitate(Planet& p2)
	{
		if (pos.x < p2.pos.x)
		{
			vel.x += 1;
			p2.vel.x -= 1;
		}
		else if (pos.x > p2.pos.x)
		{
			vel.x -= 1;
			p2.vel.x += 1;
		}
		if (pos.y < p2.pos.y)
		{
			vel.y += 1;
			p2.vel.y -= 1;
		}
		else if (pos.y > p2.pos.y)
		{
			vel.y -= 1;
			p2.vel.y += 1;

		}
		if (pos.z < p2.pos.z)
		{
			vel.z += 1;
			p2.vel.z -= 1;
		}
		else if (pos.z > p2.pos.z)
		{
			vel.z -= 1;
			p2.vel.z += 1;
		}
	}
	void Move()
	{
		pos.x += vel.x;
		pos.y += vel.y;
		pos.z += vel.z;
	}
	int TotalEnergy()
	{
		int pot = std::abs(pos.x) + std::abs(pos.y) + std::abs(pos.z);
		int kin = std::abs(vel.x) + std::abs(vel.y) + std::abs(vel.z);
		return (pot * kin);
	}
	std::string Serialize() const
	{
		return	std::to_string(pos.x)+ std::to_string(pos.y)+ std::to_string(pos.z)+
				std::to_string(vel.x)+ std::to_string(vel.y)+ std::to_string(vel.z);
	}
	bool operator==(Planet& rhs)
	{
		return ((this->pos == rhs.pos) && (this->vel == rhs.vel));
	}
public:
	Vei3 pos;
	Vei3 vel;
};