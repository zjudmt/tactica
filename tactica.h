#ifndef __TACTICA_H__
#define __TACTICA_H__

#include<stdio.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <string.h>
#include <vector>
using namespace std;

enum EventType
{	
	// 阶段开始
	// event.csv
	CORNER=1,
	KICK_OFF,
	FREE_KICK,
	INTERCEPTION,

	// 阶段结束
	// event.csv
	FOUL,
	GOAL,
	INJURY,
	OUTSIDE,
	OFFSIDE,

	// 阶段中间
	// pass.csv
	PASS_OWNBOX_TO_OWNBOX,
	PASS_OWNBOX_TO_LEFT,
	PASS_OWNBOX_TO_MID,
	PASS_OWNBOX_TO_RIGHT,
	PASS_OWN_BOX_TO_BOX,

	PASS_LEFT_TO_OWNBOX,
	PASS_LEFT_TO_LEFT,
	PASS_LEFT_TO_MID,
	PASS_LEFT_TO_RIGHT,
	PASS_LEFT_TO_BOX,

	PASS_MID_TO_OWNBOX,
	PASS_MID_TO_LEFT,
	PASS_MID_TO_MID,
	PASS_MID_TO_RIGHT,
	PASS_MID_TO_BOX,
	
	PASS_RIGHT_TO_OWNBOX,
	PASS_RIGHT_TO_LEFT,
	PASS_RIGHT_TO_MID,
	PASS_RIGHT_TO_RIGHT,
	PASS_RIGHT_TO_BOX,
	
	PASS_BOX_TO_OWNBOX,
	PASS_BOX_TO_LEFT,
	PASS_BOX_TO_MID,
	PASS_BOX_TO_RIGHT,
	PASS_BOX_TO_BOX,
	
	// shot.csv
	SHOT_ON_TARGET,
	SHOT_OFF_TARGET
};

enum ZoneType{OWN_BOX,LEFT,MID,RIGHT,BOX};

struct Coordinate{
	double x;
	double y;
};
 
struct Event{
	int frame;
	char team[10];
	Coordinate location;
	enum EventType event_type;
	
	bool operator<(const Event &e) const
	{
		return this->frame<e.frame;
	}
};
typedef vector<Event> Phase;

enum ZoneType getZoneType(Coordinate location);

enum EventType setPassType(Coordinate p1,Coordinate p2);

double deltaFunc(Event e1,Event e2);
double getDeltaDistance(vector<Event> &p1,vector<Event> &p2);


#endif