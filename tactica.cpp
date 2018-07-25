#include "tactica.h"
#include "Cluster.h"

using namespace std;

enum ZoneType getZoneType(Coordinate location)
{
	if(location.x<=210&&location.y>=129.2&&location.y<=516.8)
		return OWN_BOX;
	else if(location.y<=129.2)
		return LEFT;
	else if(location.x>=210&&location.x<=840&&location.y>=129.2&&location.y<=516.8)
		return MID;
	else if(location.y>=516.8)
		return RIGHT;
	else
		return BOX;
}

enum EventType setPassType(Coordinate p1,Coordinate p2)
{
	switch(getZoneType(p1))
	{
		case OWN_BOX:
			switch(getZoneType(p2))
			{
				case OWN_BOX:
					return PASS_OWNBOX_TO_OWNBOX;
					break;	
				case LEFT:
					return PASS_OWNBOX_TO_LEFT;
					break;
				case MID:
					return PASS_OWNBOX_TO_MID;
					break;
				case RIGHT:
					return PASS_OWNBOX_TO_RIGHT;
					break;
				case BOX:
					return PASS_OWN_BOX_TO_BOX;
					break;
			}
			break;
		case LEFT:
			switch(getZoneType(p2))
			{
				case OWN_BOX:
					return PASS_LEFT_TO_OWNBOX;
					break;	
				case LEFT:
					return PASS_LEFT_TO_LEFT;
					break;
				case MID:
					return PASS_LEFT_TO_MID;
					break;
				case RIGHT:
					return PASS_LEFT_TO_RIGHT;
					break;
				case BOX:
					return PASS_LEFT_TO_BOX;
					break;
			}
			break;
		case MID:
			switch(getZoneType(p2))
			{
				case OWN_BOX:
					return PASS_MID_TO_OWNBOX;
					break;	
				case LEFT:
					return PASS_MID_TO_LEFT;
					break;
				case MID:
					return PASS_MID_TO_MID;
					break;
				case RIGHT:
					return PASS_MID_TO_RIGHT;
					break;
				case BOX:
					return PASS_MID_TO_BOX;
					break;
			}
			break;
		case RIGHT:
			switch(getZoneType(p2))
			{
				case OWN_BOX:
					return PASS_RIGHT_TO_OWNBOX;
					break;	
				case LEFT:
					return PASS_RIGHT_TO_LEFT;
					break;
				case MID:
					return PASS_RIGHT_TO_MID;
					break;
				case RIGHT:
					return PASS_RIGHT_TO_RIGHT;
					break;
				case BOX:
					return PASS_RIGHT_TO_BOX;
					break;
			}
			break;
		case BOX:
			switch(getZoneType(p2))
			{
				case OWN_BOX:
					return PASS_BOX_TO_OWNBOX;
					break;	
				case LEFT:
					return PASS_BOX_TO_LEFT;
					break;
				case MID:
					return PASS_BOX_TO_MID;
					break;
				case RIGHT:
					return PASS_BOX_TO_RIGHT;
					break;
				case BOX:
					return PASS_BOX_TO_BOX;
					break;
			}
			break;
	}
}

bool readPassEvents(const char* filename,vector<Event> &event)
{
	FILE *fp;
	if((fp=fopen(filename,"r"))==NULL)
	{
		cout<<"Cannot Open "<<filename<<endl;
		return false;
	}
	while(fgetc(fp)!='\n')
		;
	while(!feof(fp))
	{
		int frame;
		char team[10];
		Coordinate p1,p2;
		char type[20];
		Event e;
		
		fscanf(fp,"%d%s%lf%lf%lf%lf%s",&frame,team,&p1.x,&p1.y,&p2.x,&p2.y,type);
		e.frame=frame;
		strcpy(e.team,team);
		e.location=p1;
		if(!strcmp(type,"pass"))
			e.event_type=setPassType(p1,p2);
		else
			e.event_type=INTERCEPTION;
		event.push_back(e);	
	}
	fclose(fp);
	return true;
}

double deltaFunc(Event e1,Event e2)
{
	return sqrt(pow(e1.location.x-e2.location.x,2)+pow(e1.location.y-e2.location.y,2));
}

double getDeltaDistance(vector<Event> &p1,vector<Event> &p2)
{
	const int M=p1.size();
	const int N=p2.size();
	double D[M][N];
	
	for(int i=0;i<M;i++)
		for(int j=0;j<N;j++)
		{
			if(i==0||j==0)
				D[i][j]=deltaFunc(p1[i],p2[j]);
			else
				D[i][j]=deltaFunc(p1[i],p2[j])+min(min(D[i-1][j],D[i][j-1]),D[i-1][j-1]);
		}
	return D[M-1][N-1];
}

int main()
{	
	int front,rear; 
	vector<Event> v_event;
	vector<Phase> v_phase;
	readPassEvents("pass.txt",v_event);
	vector<Event>::iterator itr;
	sort(v_event.begin(),v_event.end());
	// for(itr=v_event.begin();itr<v_event.end();itr++)
	// 	cout<<itr->frame<<"  "<<itr->team<<"  ("<<itr->location.x<<","<<itr->location.y<<")  "<<itr->event_type<<endl;

	front = 0, rear = 0;
	while (rear != v_event.size())
	{
		//findrear
		while (rear != v_event.size() - 1 && v_event[rear + 1].frame - v_event[rear].frame <= 250 && !strcmp(v_event[rear + 1].team, v_event[rear].team)) {
			rear++;
		}
		if (rear - front >= 2) {
			Phase temp;
			for (int i = front; i <= rear; i++)
				temp.push_back(v_event[i]);
			v_phase.push_back(temp);
		}

		front = rear + 1;
		rear = front;
	}

	cout << "***********" << endl;
	vector<Event>::iterator itr1;
	for (int i = 0; i < v_phase.size(); i++) {
		for (itr1 = v_phase[i].begin(); itr1 < v_phase[i].end(); itr1++) {
			cout << itr1->frame << "  " << itr1->team << "  (" << itr1->location.x << "," << itr1->location.y << ")  " << itr1->event_type << endl;
		}
		cout << "***********" << endl;
	}

	vector<Cluster> clusters;
	clusters = AHCluster(v_phase, 10);
	printClusters(clusters);	

	return 0;
}


