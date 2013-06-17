#include "modules.h"
#include "../lintypes.h"
#include "../lctypes.h"
#include "../transport.h"

class Transport;



class TransportConstructionGroup: public ConstructionGroup {
public:
    TransportConstructionGroup(
        const char *name,
        unsigned short no_credit,
        unsigned short group,
        unsigned short size, int colour,
        int cost_mul, int bul_cost, int fire_chance, int cost, int tech
    ): ConstructionGroup(
        name, no_credit, group, size, colour, cost_mul, bul_cost, fire_chance, cost, tech
    )
    {      
        switch (group)
        {
            case GROUP_TRACK:
            case GROUP_TRACK_BRIDGE:            
                commodityRuleCount[Construction::STUFF_FOOD].maxload = MAX_FOOD_ON_TRACK;
                commodityRuleCount[Construction::STUFF_JOBS].maxload = MAX_JOBS_ON_TRACK;
                commodityRuleCount[Construction::STUFF_COAL].maxload = MAX_COAL_ON_TRACK;
                commodityRuleCount[Construction::STUFF_GOODS].maxload = MAX_GOODS_ON_TRACK;
                commodityRuleCount[Construction::STUFF_ORE].maxload = MAX_ORE_ON_TRACK;
                commodityRuleCount[Construction::STUFF_STEEL].maxload = MAX_STEEL_ON_TRACK;
                commodityRuleCount[Construction::STUFF_WASTE].maxload = MAX_WASTE_ON_TRACK;
                commodityRuleCount[Construction::STUFF_KWH].maxload = MAX_KWH_ON_TRACK;
                commodityRuleCount[Construction::STUFF_WATER].maxload = MAX_WATER_ON_TRACK;
            break;            
            case GROUP_ROAD:
            case GROUP_ROAD_BRIDGE:            
                commodityRuleCount[Construction::STUFF_FOOD].maxload = MAX_FOOD_ON_ROAD;
                commodityRuleCount[Construction::STUFF_JOBS].maxload = MAX_JOBS_ON_ROAD;
                commodityRuleCount[Construction::STUFF_COAL].maxload = MAX_COAL_ON_ROAD;
                commodityRuleCount[Construction::STUFF_GOODS].maxload = MAX_GOODS_ON_ROAD;
                commodityRuleCount[Construction::STUFF_ORE].maxload = MAX_ORE_ON_ROAD;
                commodityRuleCount[Construction::STUFF_STEEL].maxload = MAX_STEEL_ON_ROAD;
                commodityRuleCount[Construction::STUFF_WASTE].maxload = MAX_WASTE_ON_ROAD;
                commodityRuleCount[Construction::STUFF_KWH].maxload = MAX_KWH_ON_ROAD;
                commodityRuleCount[Construction::STUFF_WATER].maxload = MAX_WATER_ON_ROAD;
            break;
            case GROUP_RAIL:
            case GROUP_RAIL_BRIDGE:            
                commodityRuleCount[Construction::STUFF_FOOD].maxload = MAX_FOOD_ON_RAIL;
                commodityRuleCount[Construction::STUFF_JOBS].maxload = MAX_JOBS_ON_RAIL;
                commodityRuleCount[Construction::STUFF_COAL].maxload = MAX_COAL_ON_RAIL;
                commodityRuleCount[Construction::STUFF_GOODS].maxload = MAX_GOODS_ON_RAIL;
                commodityRuleCount[Construction::STUFF_ORE].maxload = MAX_ORE_ON_RAIL;
                commodityRuleCount[Construction::STUFF_STEEL].maxload = MAX_STEEL_ON_RAIL;
                commodityRuleCount[Construction::STUFF_WASTE].maxload = MAX_WASTE_ON_RAIL;
                commodityRuleCount[Construction::STUFF_KWH].maxload = MAX_KWH_ON_RAIL;
                commodityRuleCount[Construction::STUFF_WATER].maxload = MAX_WATER_ON_RAIL;
            break;
        } // end switch group
        commodityRuleCount[Construction::STUFF_FOOD].take = true;
        commodityRuleCount[Construction::STUFF_FOOD].give = true;
        commodityRuleCount[Construction::STUFF_JOBS].take = true;
        commodityRuleCount[Construction::STUFF_JOBS].give = true;
        commodityRuleCount[Construction::STUFF_COAL].take = true;
        commodityRuleCount[Construction::STUFF_COAL].give = true;
        commodityRuleCount[Construction::STUFF_GOODS].take = true;
        commodityRuleCount[Construction::STUFF_GOODS].give = true;
        commodityRuleCount[Construction::STUFF_ORE].take = true;
        commodityRuleCount[Construction::STUFF_ORE].give = true;        
        commodityRuleCount[Construction::STUFF_STEEL].take = true;
        commodityRuleCount[Construction::STUFF_STEEL].give = true;
        commodityRuleCount[Construction::STUFF_WASTE].take = true;
        commodityRuleCount[Construction::STUFF_WASTE].give = true;
        commodityRuleCount[Construction::STUFF_KWH].take = true;
        commodityRuleCount[Construction::STUFF_KWH].give = true;
        commodityRuleCount[Construction::STUFF_WATER].take = true;
        commodityRuleCount[Construction::STUFF_WATER].give = true;  
    }
    // overriding method that creates a transport tile
    virtual Construction *createConstruction(int x, int y, unsigned short type);
};

extern TransportConstructionGroup trackConstructionGroup, roadConstructionGroup, railConstructionGroup;
extern TransportConstructionGroup trackbridgeConstructionGroup, roadbridgeConstructionGroup, railbridgeConstructionGroup;

//Dummies for counting SubTypes of Transport
class Track{};
class Road{};
class Rail{};

class Transport : public CountedConstruction<Transport> { // Transport inherits from countedConstruction  
public:
	Transport(int x, int y, unsigned short type): CountedConstruction<Transport>(x, y, type) 
    {
        unsigned short group = get_group_of_type(type);         
        this->anim = 0;
        this->burning_waste = false;
        this->old_type = type;
        setMemberSaved(&(this->old_type),"type");                  
        // register the construction as transport tile
        // disable evacuation
        this->flags |= (FLAG_IS_TRANSPORT | FLAG_NEVER_EVACUATE);        
        if (world(x,y)->is_water())//we build bridges on water
        {
            switch (group)
            {
                case GROUP_TRACK:
                    group = GROUP_TRACK_BRIDGE;
                break;
                case GROUP_ROAD:
                    group = GROUP_ROAD_BRIDGE;
                break;
                case GROUP_RAIL:
                    group = GROUP_RAIL_BRIDGE;
                break;
            }
        } 
        else // we dont build bridges anywhere else
        {
            switch (group)
            {
                case GROUP_TRACK_BRIDGE:
                    group = GROUP_TRACK;
                break;
                case GROUP_ROAD_BRIDGE:
                    group = GROUP_ROAD;
                break;
                case GROUP_RAIL_BRIDGE:
                    group = GROUP_RAIL;
                break;
            }
        }

      switch (group)  //here we build accoding to terrain
        {
            case GROUP_TRACK:
                constructionGroup = &trackConstructionGroup;
                countedTrack = new Counted<Track>();
                this->subgroupID = countedTrack->getNextId();                
            break;
            case GROUP_TRACK_BRIDGE:
                constructionGroup = &trackbridgeConstructionGroup;
                countedTrack = new Counted<Track>();
                this->subgroupID = countedTrack->getNextId();         
            break;
            case GROUP_ROAD:
                constructionGroup = &roadConstructionGroup;
                countedRoad = new Counted<Road>();
                this->subgroupID = countedRoad->getNextId();         
            break;
            case GROUP_ROAD_BRIDGE:
                constructionGroup = &roadbridgeConstructionGroup;
                countedRoad = new Counted<Road>();
                this->subgroupID = countedRoad->getNextId(); 
            break;
            case GROUP_RAIL:
                constructionGroup = &railConstructionGroup;
                countedRail = new Counted<Rail>();
                this->subgroupID = countedRail->getNextId(); 
            break;
            case GROUP_RAIL_BRIDGE:
                constructionGroup = &railbridgeConstructionGroup;
                countedRail = new Counted<Rail>();
                this->subgroupID = countedRail->getNextId(); 
            break; 
        }

        initialize_commodities();
        this->trafficCount = this->commodityCount;      
    }
	~Transport()
    {        
        switch (constructionGroup->group)
        {
            case GROUP_TRACK:
            case GROUP_TRACK_BRIDGE:
                delete countedTrack;
            break;
            case GROUP_ROAD:
            case GROUP_ROAD_BRIDGE:
                delete countedRoad;
            break;
            case GROUP_RAIL:
            case GROUP_RAIL_BRIDGE:
                delete countedRail;
            break;
            default:
                std::cout << "counting error in Transport IDs" << std::endl;
            break;
        }    
    }
    Counted<Track> *countedTrack;
    Counted<Road> *countedRoad;
    Counted<Rail> *countedRail;
	void update();
	void report();
    void stuff_flow();
    std::map<Commodities, int> trafficCount;
    void list_traffic( int* i);        
    int subgroupID;
    int anim;
    bool burning_waste;
    int old_type;
};


 




/*
class Track: public Transport, CountedConstruction<Track>
{
public:
    Track(int x, int y, unsigned short type): Transport(x, y, type), CountedConstruction<Track>(x, y, type) 
    {
        if (world(x,y)->is_water())
        {   constructionGroup = &trackbridgeConstructionGroup;} 
        else
        {   constructionGroup = &trackConstructionGroup;}
        setMemberSaved(&(this->old_type),"type");                  
        this->flags |= FLAG_IS_TRANSPORT;    
        initialize_commodities();
        this->trafficCount = this->commodityCount;
    }
    virtual ~Track(){}
    std::map<Commodities, int> trafficCount;   
};
*/
/*
class Road: public Transport//CountedConstruction<Road>,
{
public:
    Road(int x, int y, unsigned short type): Transport(x, y, type) //CountedConstruction<Road>(x, y, type), 
    {
        if (world(x,y)->is_water())
        {   constructionGroup = &roadbridgeConstructionGroup;} 
        else
        {   constructionGroup = &roadConstructionGroup;}        
    }
    virtual ~Road(){}
};

class Rail: public Transport //CountedConstruction<Rail>,
{
    public:
    Rail(int x, int y, unsigned short type): Transport(x, y, type)//CountedConstruction<Rail>(x, y, type), 
    {
        if (world(x,y)->is_water())
        {   constructionGroup = &railbridgeConstructionGroup;} 
        else
        {   constructionGroup = &railConstructionGroup;}
    }
    virtual ~Rail(){}
};
*/