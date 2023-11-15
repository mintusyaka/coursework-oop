#include <iostream>
#include <chrono>
#include <array>
#include <vector>
#include <queue>


#define MAP_SIZE								20
#define ROAD_WIDTH								4

#define _POSITION(x, y)							(std::pair<int, int>(x, y))
#define _X										m_Position.first
#define _Y										m_Position.second

#define _CREATING_MAP_COND1_(i, j)				((i < (MAP_SIZE - ROAD_WIDTH) / 2) && (j < (MAP_SIZE - ROAD_WIDTH) / 2))
#define _CREATING_MAP_COND2_(i, j)				((i < (MAP_SIZE - ROAD_WIDTH) / 2) && (j >= (MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH))
#define _CREATING_MAP_COND3_(i, j)				((i >= (MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH) && (j < (MAP_SIZE - ROAD_WIDTH) / 2))
#define _CREATING_MAP_COND4_(i, j)				((i >= (MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH) && (j >= (MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH))
#define _CREATING_MAP_CONDITION_(i,j)			_CREATING_MAP_COND1_(i, j) || _CREATING_MAP_COND2_(i, j) || _CREATING_MAP_COND3_(i, j) || _CREATING_MAP_COND4_(i, j)

#define _IS_POSITION_FOR_CROSSWALK_1_(i, j)		((i == (MAP_SIZE - ROAD_WIDTH) / 2 - 1) && (j > (MAP_SIZE - ROAD_WIDTH) / 2 - 1) && (j < (MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH))
#define _IS_POSITION_FOR_CROSSWALK_2_(i, j)		((i == (MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH) && (j > (MAP_SIZE - ROAD_WIDTH) / 2 - 1) && (j < (MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH))
#define _IS_POSITION_FOR_CROSSWALK_3_(i, j)		((j == (MAP_SIZE - ROAD_WIDTH) / 2 - 1) && (i > (MAP_SIZE - ROAD_WIDTH) / 2 - 1) && (i < (MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH))
#define _IS_POSITION_FOR_CROSSWALK_4_(i, j)		((j == (MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH) && (i > (MAP_SIZE - ROAD_WIDTH) / 2 - 1) && (i < (MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH))
#define _IS_POSITION_FOR_CROSSWALK_(i, j)		(_IS_POSITION_FOR_CROSSWALK_1_(i,j) || _IS_POSITION_FOR_CROSSWALK_2_(i,j) || _IS_POSITION_FOR_CROSSWALK_3_(i,j) || _IS_POSITION_FOR_CROSSWALK_4_(i,j))

#define _MOVE_UP_RIGHT_CONDITION_(mp)			(mp->getPix(_X, _Y - 1)	== '.' && _Y != ((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH / 2))
#define _MOVE_UP_LEFT_CONDITION_(mp)			(mp->getPix(_X, _Y - 1)	== '.' && _Y != ((MAP_SIZE - ROAD_WIDTH) / 2 - 1 + ROAD_WIDTH / 2))

#define _MOVE_DOWN_LEFT_CONDITION_(mp)			(mp->getPix(_X, _Y + 1)	== '.' && _Y != ((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH / 2))
#define _MOVE_DOWN_RIGHT_CONDITION_(mp)			(mp->getPix(_X, _Y + 1)	== '.' && _Y != (MAP_SIZE - ROAD_WIDTH) / 2)

#define _MOVE_RIGHT_DOWN_CONDITION_(mp)			(mp->getPix(_X + 1, _Y) == '.' && _X != (MAP_SIZE - ROAD_WIDTH) / 2)
#define _MOVE_RIGHT_UP_CONDITION_(mp)			(mp->getPix(_X + 1, _Y)	== '.' && _X != ((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH / 2))

#define _MOVE_LEFT_DOWN_CONDITION_(mp)			(mp->getPix(_X - 1, _Y)	== '.' && _X != ((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH / 2 - 1))
#define _MOVE_LEFT_UP_CONDITION_(mp)			(mp->getPix(_X - 1, _Y)	== '.' && _X != ((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH / 2 + 1))

#define _IS_CROSSWALK_UP_(mp)					(mp->getPix(_X, _Y - 1) == '=')
#define _IS_CROSSWALK_DOWN_(mp)					(mp->getPix(_X, _Y + 1) == '=')
#define _IS_CROSSWALK_LEFT_(mp)					(mp->getPix(_X - 1, _Y) == '=')
#define _IS_CROSSWALK_RIGHT_(mp)				(mp->getPix(_X + 1, _Y) == '=')

#define _IS_CAR_CROSS_CROSSROAD_				((_X >= ((MAP_SIZE - ROAD_WIDTH) / 2 - 1)) && (_X <= ((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH)) && (_Y >= ((MAP_SIZE - ROAD_WIDTH) / 2 - 1)) && (_Y <= ((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH)))

#define _IS_EMPTY_PIX_UP_(mp)					(mp->getPix(_X, _Y - 1)	== '.' || _IS_CROSSWALK_UP_(mp))
#define _IS_EMPTY_PIX_DOWN_(mp)					(mp->getPix(_X, _Y + 1)	== '.' || _IS_CROSSWALK_DOWN_(mp))
#define _IS_EMPTY_PIX_RIGHT_(mp)				(mp->getPix(_X + 1, _Y)	== '.' || _IS_CROSSWALK_RIGHT_(mp))
#define _IS_EMPTY_PIX_LEFT_(mp)					(mp->getPix(_X - 1, _Y)	== '.' || _IS_CROSSWALK_LEFT_(mp))

#define _TRAFFIC_LIGHT_IS_GREEN_(tl)			(tl == Green)

#define _MOVE_RIGHT_CONDITION_(mp)				(_IS_EMPTY_PIX_RIGHT_(mp) && _Y > ((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH / 2 - 1) && _Y < ((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH))
#define _MOVE_LEFT_CONDITION_(mp)				(_IS_EMPTY_PIX_LEFT_(mp)  && _Y > ((MAP_SIZE - ROAD_WIDTH) / 2 - 1) && _Y < ((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH / 2))
#define _MOVE_UP_CONDITION_(mp)					(_IS_EMPTY_PIX_UP_(mp)	  && _X > ((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH / 2 - 1) && _X < ((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH))
#define _MOVE_DOWN_CONDITION_(mp)				(_IS_EMPTY_PIX_DOWN_(mp)  && _X > ((MAP_SIZE - ROAD_WIDTH) / 2 - 1) && _X < ((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH / 2))

#define CAR_POS									(m_RoadLines[roadline_idx][car_idx]->getPos())
#define _CAR_OUT_OF_MAP_						(CAR_POS.second < 0 || CAR_POS.second > 19 || CAR_POS.first < 0 || CAR_POS.first > 19)
#define CAR_TYPE								(m_RoadLines[roadline_idx][car_idx]->getType())
#define _IS_TURN_(type)							(type == Turn)
#define LAST_CAR_ON_THE_ROAD(roadline_idx)		m_RoadLines[roadline_idx][m_RoadLines[roadline_idx].size() - 1]
#define CAR_IN_QUEUE							(m_CarQueue[roadline_idx].front())

#define _RANDOM_GOAL_							(rand() % 10 / 4)
#define _RANDOM_DIRECTION_						(rand() % 10 / 5)
#define _RANDOM_ROADLINE_						(rand() % 10 / 5)
#define _RANDOM_CARSIZE_						(rand() % 10 / 5 + 1)
#define _IS_HORIZONTAL_ROAD_					(m_RoadDirection == Horizontal)

#define CUR_TIME								(std::chrono::high_resolution_clock::now())
#define DURATION(a, b)							(std::chrono::duration_cast<std::chrono::seconds>(a - b).count())

/*
ÐÓÕ ÂÏÅÐÅÄ:
ò³ëüêè âïåðåä:
ÿêùî
	_MOVE_FORWARD_CONDITION_ == 1 - ðóõàºòüñÿ âïåðåä, ³íàêøå - ñòî¿òü íà ì³ñö³

+ ïðàâîðó÷:
ÿêùî
	_MOVE_RIGHT_CONDITION_ == 1 - ðóõàºòüñÿ ïðàâîðó÷,
³íàêøå:
	_MOVE_FORWARD_RIGHT_CONDITION_ == 1 - ðóõàºòüñÿ ïðÿìî, ³íàêøå - ñòî¿òü íà ì³ñö³.

+ ë³âîðó÷:
ÿêùî
	_MOVE_LEFT_CONDITION_ == 1 - ðóõàºòüñÿ ë³âîðó÷
³íàêøå:
	_MOVE_FORWARD_LEFT_CONDITION_ == 1 - ðóõàºòüñÿ ïðÿìî, ³íàêøå - ñòî¿òü íà ì³ñö³

ÐÓÕ ÂÍÈÇ:
ò³ëüêè âíèç:
	
*/

//=============================================================================
//ENUMS
//-----------------------------------------------------------------------------

enum Light {
	Red,
	Green,
	Yellow
};

enum RoadDirection {
	Vertical,
	Horizontal
};

enum Goal {
	Forward,
	Right,
	Left
};

enum TypeOfRoadline {
	Main,
	Turn
};

//-----------------------------------------------------------------------------
//=============================================================================



class Map {
public:
	Map() {
		for (int i{ 0 }; i < MAP_SIZE; ++i) {
			for (int j{ 0 }; j < MAP_SIZE; ++j) {
				if (_IS_POSITION_FOR_CROSSWALK_(i, j)) {
					m_Map[i][j] = '=';
				}
				else if (_CREATING_MAP_CONDITION_(i, j))
				{
					m_Map[i][j] = '0';
				}
				else
				{
					m_Map[i][j] = '.';
				}
			}
		}

	}

	inline void setPix(int x, int y, char data) {
		m_Map[y][x] = data;
	}

	inline void delPix(int x, int y) {
		m_Map[y][x] = '.';	
	}

	char getPix(int x, int y) {
		if (x < 0 || x > 19 || y < 0 || y > 19) {
			return '.';
		}
		else {
			return m_Map[y][x];
		}
		// maybe catch exception
	}

	void showMap() {
		for (int i{ 0 }; i < MAP_SIZE; ++i) {
			for (int j{ 0 }; j < MAP_SIZE; ++j) {
				std::cout << m_Map[i][j] << " ";
			}
			std::cout << "\n";
		}
	}

private:
	std::array<std::array<char, MAP_SIZE>, MAP_SIZE> m_Map;
	//TrafficLight
};

class TrafficLight {
public:
	TrafficLight() : m_Light(Red) {}

	void switchLight() {
		switch (m_Light) {
		case Red:
			m_LastLight = m_Light;
			m_Light = Yellow;
			break;
		case Green:
			m_LastLight = m_Light;
			m_Light = Yellow;
			break;
		case Yellow:
			if (m_LastLight == Red)
				m_Light = Green;
			if (m_LastLight == Green)
				m_Light = Red;
			break;
		}
	}

	void setLight(Light& light) { m_Light = light; }

	Light getLight() { return m_Light; }
private:
	Light m_Light;
	Light m_LastLight;
};

class Car {
public:
	Car(int size, Map* mp, const Goal goal, const TypeOfRoadline roadtype)
		: m_Size(size), m_Position(_POSITION(-1, -1)), m_Map(mp), m_Goal(goal), m_curTLight(), m_RoadType(roadtype) {}

	void setPos(int x, int y) {
		if (x < 0 || x > 19 || y < 0 || y > 19) {
			if (_IS_POSITION_FOR_CROSSWALK_(_X, _Y)) {
				m_Map->setPix(_X, _Y, '=');
			}
			else m_Map->setPix(_X, _Y, '.');

			m_Position.first = x;
			m_Position.second = y;
		}
		else {
			if (_X != -1 && _Y != -1) {
				if (_IS_POSITION_FOR_CROSSWALK_(_X, _Y)) {
					m_Map->setPix(_X, _Y, '=');
				}
				else m_Map->setPix(_X, _Y, '.');
			}

			m_Position.first = x;
			m_Position.second = y;

			m_Map->setPix(_X, _Y, 'X');
		}		
	}

	inline std::pair<int, int> getPos() { return m_Position; }

	void move(const RoadDirection& direction, const int& roadline, const Light& light) {
		m_curTLight = light;
		switch (direction)
		{
		case Vertical:
			switch (roadline) {
			case 0:
				moveUp();
				break;
			case 1:
				moveDown();
				break;
			}
			break;
		case Horizontal:
			switch (roadline) {
			case 0:
				moveRight();
				break;
			case 1:
				moveLeft();
				break;
			}
			break;
		}
	}

	inline TypeOfRoadline getType() { return m_RoadType; }

private:
	int m_Size;
	Goal m_Goal;
	std::pair<int, int> m_Position;
	Map* m_Map;
	Light m_curTLight;
	TypeOfRoadline m_RoadType;

	void moveUp() {
		switch (m_Goal) {
		case Forward:
			if (!_IS_CROSSWALK_UP_(m_Map) || _IS_CAR_CROSS_CROSSROAD_) {
				if (_MOVE_UP_CONDITION_(m_Map)) {
					setPos(_X, _Y - 1);
				}
			}
			else {
				if (_TRAFFIC_LIGHT_IS_GREEN_(m_curTLight)) {
					setPos(_X, _Y - 1);
				}
			}
			break;
		case Right:
			moveUpRight();
			break;
		case Left:
			moveUpLeft();
			break;
		}
	}

	void moveUpRight() {
		if (!_IS_CROSSWALK_UP_(m_Map) || _IS_CAR_CROSS_CROSSROAD_) {
			if (_MOVE_RIGHT_CONDITION_(m_Map) == 1) {
				setPos(_X + 1, _Y);
			}
			else {
				if (_MOVE_UP_RIGHT_CONDITION_(m_Map) == 1) {
					setPos(_X, _Y - 1);
				}
			}
		}
		else {
			if (_TRAFFIC_LIGHT_IS_GREEN_(m_curTLight)) {
				setPos(_X, _Y - 1);
			}
		}
		
	}

	void moveUpLeft() {
		if (!_IS_CROSSWALK_UP_(m_Map) || _IS_CAR_CROSS_CROSSROAD_) {
			if (_MOVE_LEFT_CONDITION_(m_Map) == 1) {
				setPos(_X - 1, _Y);
			}
			else {
				if (_MOVE_UP_LEFT_CONDITION_(m_Map) == 1) {
					setPos(_X, _Y - 1);
				}
			}
		}
		else {
			if (_TRAFFIC_LIGHT_IS_GREEN_(m_curTLight)) {
				setPos(_X, _Y - 1);
			}
		}
	}

	void moveDown() {
		switch (m_Goal) {
		case Forward:
			if (!_IS_CROSSWALK_DOWN_(m_Map) || _IS_CAR_CROSS_CROSSROAD_) {
				if (_MOVE_DOWN_CONDITION_(m_Map)) {
					setPos(_X, _Y + 1);
				}
			}
			else {
				if (_TRAFFIC_LIGHT_IS_GREEN_(m_curTLight)) {
					setPos(_X, _Y + 1);
				}
			}
			break;
		case Right:
			moveDownRight();
			break;
		case Left:
			moveDownLeft();
			break;
		}
	}

	void moveDownRight() {
		if (!_IS_CROSSWALK_DOWN_(m_Map) || _IS_CAR_CROSS_CROSSROAD_) {
			if (_MOVE_LEFT_CONDITION_(m_Map) == 1) {
				setPos(_X - 1, _Y);
			}
			else {
				if (_MOVE_DOWN_RIGHT_CONDITION_(m_Map) == 1) {
					setPos(_X, _Y + 1);
				}
			}
		}
		else {
			if (_TRAFFIC_LIGHT_IS_GREEN_(m_curTLight)) {
				setPos(_X, _Y + 1);
			}
		}
	}

	void moveDownLeft() {
		if (!_IS_CROSSWALK_DOWN_(m_Map) || _IS_CAR_CROSS_CROSSROAD_) {
			if (_MOVE_RIGHT_CONDITION_(m_Map) == 1) {
				setPos(_X + 1, _Y);
			}
			else if (_MOVE_DOWN_LEFT_CONDITION_(m_Map) == 1) {
				setPos(_X, _Y + 1);
			}
		}
		else {
			if (_TRAFFIC_LIGHT_IS_GREEN_(m_curTLight)) {
				setPos(_X, _Y + 1);
			}
		}
	}

	void moveRight() {
		switch (m_Goal) {
		case Forward:
			if (!_IS_CROSSWALK_RIGHT_(m_Map) || _IS_CAR_CROSS_CROSSROAD_) {
				if (_MOVE_RIGHT_CONDITION_(m_Map)) {
					setPos(_X + 1, _Y);
				}
			}
			else {
				if (_TRAFFIC_LIGHT_IS_GREEN_(m_curTLight)) {
					setPos(_X + 1, _Y);
				}
			}
			break;
		case Right:
			moveRightDown();
			break;
		case Left:
			moveRightUp();
			break;
		}
	}

	void moveRightDown() {
		if (!_IS_CROSSWALK_RIGHT_(m_Map) || _IS_CAR_CROSS_CROSSROAD_) {
			if (_MOVE_DOWN_CONDITION_(m_Map) == 1) {
				setPos(_X, _Y + 1);
			}
			else {
				if (_MOVE_RIGHT_DOWN_CONDITION_(m_Map) == 1) {
					setPos(_X + 1, _Y);
				}
			}
		}
		else {
			if (_TRAFFIC_LIGHT_IS_GREEN_(m_curTLight)) {
				setPos(_X + 1, _Y);
			}
		}
	}

	void moveRightUp() {
		if (!_IS_CROSSWALK_RIGHT_(m_Map) || _IS_CAR_CROSS_CROSSROAD_) {
			if (_MOVE_UP_CONDITION_(m_Map) == 1) {
				setPos(_X, _Y - 1);
			}
			else {
				if (_MOVE_RIGHT_UP_CONDITION_(m_Map) == 1) {
					setPos(_X + 1, _Y);
				}
			}
		}
		else {
			if (_TRAFFIC_LIGHT_IS_GREEN_(m_curTLight)) {
				setPos(_X + 1, _Y);
			}
		}
	}

	void moveLeft() {
		switch (m_Goal) {
		case Forward:
			if (!_IS_CROSSWALK_LEFT_(m_Map) || _IS_CAR_CROSS_CROSSROAD_) {
				if (_MOVE_LEFT_CONDITION_(m_Map)) {
					setPos(_X - 1, _Y);
				}
			}
			else {
				if (_TRAFFIC_LIGHT_IS_GREEN_(m_curTLight)) {
					setPos(_X - 1, _Y);
				}
			}
			break;
		case Right:
			moveLeftUp();
			break;
		case Left:
			moveLeftDown();
			break;
		}
	}

	void moveLeftDown() {
		if (!_IS_CROSSWALK_LEFT_(m_Map) || _IS_CAR_CROSS_CROSSROAD_) {
			if (_MOVE_DOWN_CONDITION_(m_Map) == 1) {
				setPos(_X, _Y + 1);
			}
			else {
				if (_MOVE_LEFT_DOWN_CONDITION_(m_Map) == 1) {
					setPos(_X - 1, _Y);
				}
			}
		}
		else {
			if (_TRAFFIC_LIGHT_IS_GREEN_(m_curTLight)) {
				setPos(_X - 1, _Y);
			}
		}
	}

	void moveLeftUp() {
		if (!_IS_CROSSWALK_LEFT_(m_Map) || _IS_CAR_CROSS_CROSSROAD_) {
			if (_MOVE_UP_CONDITION_(m_Map) == 1 ) {
				setPos(_X, _Y - 1);
			}
			else {
				if (_MOVE_LEFT_UP_CONDITION_(m_Map) == 1) {
					setPos(_X - 1, _Y);
				}
			}
		}
		else {
			if (_TRAFFIC_LIGHT_IS_GREEN_(m_curTLight)) {
				setPos(_X - 1, _Y);
			}
		}
	}
};

class Road {
public:
	Road(const RoadDirection& direction, Map* map) 
		: m_RoadDirection(direction), m_Map(map) {}

	RoadDirection& getDirection() { return m_RoadDirection; }

	Light getLightOnRoad(const TypeOfRoadline& roadtype) { return m_LightsOnRoad[roadtype].getLight(); }

	void switchLightOnMainRoad() {
		m_LightsOnRoad[0].switchLight();
	}

	void switchLightOnTurnRoad() {
		m_LightsOnRoad[1].switchLight();
	}

	void addCar() {
		int roadline_idx = _RANDOM_ROADLINE_;
		int size = 1;
		Car* car = nullptr;
		if (_IS_HORIZONTAL_ROAD_) {
			switch (_RANDOM_GOAL_){
			case Forward:
				createNewCar(car, size, Forward, roadline_idx, Main);
				break;
			case Right:
				createNewCar(car, size, Right, roadline_idx, Main);
				break;
			case Left:
				createNewCar(car, size, Left, roadline_idx, Turn);
				break;
			}
		}
		else {
			switch (_RANDOM_GOAL_) {
			case Forward:
				createNewCar(car, size, Forward, roadline_idx, Main);
				break;
			case Right:
				createNewCar(car, size, Right, roadline_idx, Main);
				break;
			case Left:
				createNewCar(car, size, Left, roadline_idx, Turn);
				break;
			}
		}
	} // RANDOM CAR GENERATE

	void addCar(const RoadDirection& dir) {
		int size = 1;
		Car* car = nullptr;
		int roadline_idx = 1;
		if (dir) {
			switch (_RANDOM_GOAL_) {
			case Forward:
				createNewCar(car, size, Forward, roadline_idx, Main);
				break;
			case Right:
				createNewCar(car, size, Forward, roadline_idx, Main);
				break;
			case Left:
				createNewCar(car, size, Forward, roadline_idx, Turn);
				break;
			}
		}
	}

	void moveCars() {
		for (int roadline_idx = 0; roadline_idx < m_RoadLines.size(); ++roadline_idx) {
			for (int car_idx = 0; car_idx < m_RoadLines[roadline_idx].size(); car_idx++) {
				if (CAR_TYPE == Main) {
					m_RoadLines[roadline_idx][car_idx]->move(m_RoadDirection, roadline_idx, m_LightsOnRoad[0].getLight());
				}
				else {
					m_RoadLines[roadline_idx][car_idx]->move(m_RoadDirection, roadline_idx, m_LightsOnRoad[1].getLight());
				}			

				if (_CAR_OUT_OF_MAP_) {
					delete m_RoadLines[roadline_idx][car_idx];
					m_RoadLines[roadline_idx].erase(m_RoadLines[roadline_idx].begin() + car_idx);
				}
			}
		}
		for (int roadline_idx = 0; roadline_idx < m_RoadLines.size(); ++roadline_idx) {
			for (int car_idx = 0; car_idx < m_RoadLines[roadline_idx].size(); car_idx++) {
				if (!m_CarQueue[roadline_idx].empty()) {
					if (!isLineFull(roadline_idx, CAR_IN_QUEUE->getType())) {
						addCarOnTheRoad(CAR_IN_QUEUE, roadline_idx, CAR_IN_QUEUE->getType());
						m_CarQueue[roadline_idx].pop();
					}
				}
			}
		}

		//logic of moving cars
	}


private:
	RoadDirection m_RoadDirection;
	std::array<TrafficLight, 2> m_LightsOnRoad;				// [0] - main road; [1] - turn road
	std::array<std::vector<Car*>, ROAD_WIDTH / 2> m_RoadLines;	// [0] - up/right; [1] - down/left
	std::array<std::queue<Car*>, ROAD_WIDTH / 2> m_CarQueue;

	void addCarOnTheRoad(Car* car, const int& roadline_idx, const TypeOfRoadline& type) {
		m_RoadLines[roadline_idx].push_back(car);
		if (m_RoadDirection == Horizontal)
			switch (roadline_idx) {
			case 0:
				LAST_CAR_ON_THE_ROAD(roadline_idx)->setPos(
					0,
					(MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH - 1 - 1 * _IS_TURN_(type)
				);
				break;
			case 1:
				LAST_CAR_ON_THE_ROAD(roadline_idx)->setPos(
					(MAP_SIZE - 1),
					((MAP_SIZE - ROAD_WIDTH) / 2 + (1 * _IS_TURN_(type)))
				);
				break;
			}
			
		else
			switch (roadline_idx) {
			case 0:
				LAST_CAR_ON_THE_ROAD(roadline_idx)->setPos(
					((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH - 1 - 1 * _IS_TURN_(type)),
					(MAP_SIZE - 1)
				);
				break;
			case 1:
				LAST_CAR_ON_THE_ROAD(roadline_idx)->setPos(
					((MAP_SIZE - ROAD_WIDTH) / 2 + 1 * _IS_TURN_(type)),
					0
				);
				break;
			}
	}

	void addCarToQueue(Car* car, const int& roadline_idx) {
		m_CarQueue[roadline_idx].push(car);
	}

	void createNewCar(Car* car, const int& size, const Goal& goal, const int& roadline_idx, const TypeOfRoadline& type) {
		car = new Car(1, m_Map, goal, type);
		if (!isLineFull(roadline_idx, type)) {
			addCarOnTheRoad(car, roadline_idx, type);
		}
		else {
			addCarToQueue(car, roadline_idx);
		}
	}

	bool isLineFull(const int& roadline_idx, const TypeOfRoadline roadLineType) {
		int x{ 0 };
		int y{ 0 };

		switch (m_RoadDirection) {
		case Horizontal:
			switch (roadline_idx) {
			case 0:
				x = 0;
				y = (MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH - 1 - 1 * _IS_TURN_(roadLineType);
				break;
			case 1:
				x = (MAP_SIZE - 1);
				y = ((MAP_SIZE - ROAD_WIDTH) / 2 + (1 * _IS_TURN_(roadLineType)));
				break;
			}
			break;
		case Vertical:
			switch (roadline_idx) {
			case 0:
				x = ((MAP_SIZE - ROAD_WIDTH) / 2 + ROAD_WIDTH - 1 - 1 * _IS_TURN_(roadLineType));
				y = (MAP_SIZE - 1);
				break;
			case 1:
				x = ((MAP_SIZE - ROAD_WIDTH) / 2 + 1 * _IS_TURN_(roadLineType));
				y = 0;
				break;
			}
			break;
		}

		if (m_Map->getPix(x, y) == '.')
			return false;

		return true;
	}

	Map* m_Map;
};

class RoadsSystem {
public:
	RoadsSystem(Map* map) : m_Map(map) {}

	void changeHorizontalLight() {
		m_Horizontal_Road.switchLightOnMainRoad();
	}

	void changeVerticalLight() {
		m_Vertical_Road.switchLightOnMainRoad();
	}

	void changeHorizontalLightTurn() {
		m_Horizontal_Road.switchLightOnTurnRoad();
	}

	void changeVerticalLightTurn() {
		m_Vertical_Road.switchLightOnTurnRoad();
	}

	void addHorizontalCars() {
		m_Horizontal_Road.addCar(Horizontal);
		m_Horizontal_Road.addCar(Horizontal);
		m_Horizontal_Road.addCar(Horizontal);
		m_Horizontal_Road.addCar(Horizontal);
	}

	void addCar() {
		switch (_RANDOM_DIRECTION_) {
		case Vertical:
			m_Vertical_Road.addCar();
			break;
		case Horizontal:
			m_Horizontal_Road.addCar();
			break;
		}
	}

	void moveCars() {
		m_Horizontal_Road.moveCars();
		m_Vertical_Road.moveCars();
	}

	Light getLight(const RoadDirection& direction, const TypeOfRoadline& roadtype) {
		switch (direction) {
		case Horizontal:
			return m_Horizontal_Road.getLightOnRoad(roadtype);
			break;
		case Vertical:
			return m_Vertical_Road.getLightOnRoad(roadtype);
			break;
		}
	}
private:
	Map* m_Map;
	Road m_Horizontal_Road{ Horizontal, m_Map };
	Road m_Vertical_Road{ Vertical, m_Map };
};

class System {
public:
	System() {
		
	}

	void switchLight(const RoadDirection& rd, const TypeOfRoadline& roadtype) {
		switch (rd) {
		case Horizontal:
			switch (roadtype) {
			case Main:
				m_RoadsSystem.changeHorizontalLight();
				break;
			case Turn:
				m_RoadsSystem.changeHorizontalLightTurn();
				break;
			}
			break;
		case Vertical:
			switch (roadtype) {
			case Main:
				m_RoadsSystem.changeVerticalLight();
				break;
			case Turn:
				m_RoadsSystem.changeVerticalLightTurn();
				break;
			}
			break;
		}
	}

	void newRandomCar() {
		m_RoadsSystem.addCar();
	}

	void move() {
		m_RoadsSystem.moveCars();
	}

	void refresh() {
		system("cls");
		m_Map.showMap();

		std::cout <<
			"\n\n" <<
			"Vertical road: " << getStrLight(m_RoadsSystem.getLight(Vertical, Main)) << "\n" <<
			"\tturn road: " << getStrLight(m_RoadsSystem.getLight(Vertical, Turn)) << "\n" <<
			"Horizontal road: " << getStrLight(m_RoadsSystem.getLight(Horizontal, Main)) << "\n" <<
			"\tturn road: " << getStrLight(m_RoadsSystem.getLight(Horizontal, Turn));
	}

	void createCarsHorizontal() {
		m_RoadsSystem.addHorizontalCars();
	}
private:
	Map m_Map;
	RoadsSystem m_RoadsSystem{ &m_Map };

	std::string getStrLight(const Light& light) {
		switch (light) {
		case 0:
			return "Red";
		case 1:
			return "Green";
		case 2:
			return "Yellow";
		}
	}
};



int main(void) {
	srand((unsigned)time(NULL));

	System model;

	// âñå â ñåêóíäàõ
	
	int CAR_APPEAR_FREQUENCY{ 2 };
	double CAR_MOVE_FREQUENCY{ 0.3 };
	double TRAFFICLIGHTS_FREQUENCY{ 20 };
	double YELLOW_LIGHT_DURATION{ TRAFFICLIGHTS_FREQUENCY * 2 / 10 };
	double RED_LIGHT_DURATION{ TRAFFICLIGHTS_MAIN_FREQUENCY * 3 / 10 };
	double GREEN_LIGHT_DURATION{ TRAFFICLIGHTS_MAIN_FREQUENCY * 5 / 10 };
	double YELLOW_LIGHT_TURN_DURATION{ TRAFFICLIGHTS_TURN_FREQUENCY * 2 / 10 };
	double RED_LIGHT_TURN_DURATION{ TRAFFICLIGHTS_TURN_FREQUENCY * 3 / 10 };
	double GREEN_LIGHT_TURN_DURATION{ TRAFFICLIGHTS_TURN_FREQUENCY * 5 / 10 };

	int TIME_OF_MODELLING{ 120 };

	bool isMainRoadSig{ true };
	bool isYellow{ false };
	RoadDirection activeRoad{ Horizontal };

	auto TIMER_CAR_APPEAR_START{ CUR_TIME };
	auto TIMER_CAR_MOVE_START{ CUR_TIME };
	auto TIMER_TRAFFICLIGHTS_MAIN_START{ CUR_TIME };
	auto TIMER_TRAFFICLIGHTS_TURN_START{ CUR_TIME };

	auto TIMER_START_MODELLING{ CUR_TIME };

	double REFRESH_FREQ{ 1 };
	auto TIMER_REFRESH{ CUR_TIME };

	// ÇÐÎÁÈÒÈ ÏÅÐÅÂ²ÐÊÓ ÍÀ ÊÎÆÍÈÉ ÊÎË²Ð ÑÂ²ÒËÎÔÎÐÈ ÒÀ Ì²ÍßÒÈ ÊÎÆÅÍ ÊÎË²Ð ÎÊÐÅÌÎ ÇÀËÅÆÍÎ Â²Ä ÓÌÎÂ?

	while (DURATION(CUR_TIME, TIMER_START_MODELLING) <= TIME_OF_MODELLING) {
		if (DURATION(CUR_TIME, TIMER_CAR_APPEAR_START) >= CAR_APPEAR_FREQUENCY) {
			TIMER_CAR_APPEAR_START = CUR_TIME;
			model.newRandomCar();
		}

		if (DURATION(CUR_TIME, TIMER_CAR_MOVE_START) >= CAR_MOVE_FREQUENCY) {
			TIMER_CAR_MOVE_START = CUR_TIME;
			model.move();
		}

		

		if (DURATION(CUR_TIME, TIMER_REFRESH) >= REFRESH_FREQ) {
			model.refresh();
			TIMER_REFRESH = CUR_TIME;
		}
	}

	return 0;
}