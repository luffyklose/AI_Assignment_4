#include "CollisionManager.h"
#include "Util.h"
#include <algorithm>
#include <iso646.h>


int CollisionManager::squaredDistance(const glm::vec2 p1, const glm::vec2 p2)
{
	const int diffOfXs = p2.x - p1.x;
	const int diffOfYs = p2.y - p1.y;
	const auto result = diffOfXs * diffOfXs + diffOfYs * diffOfYs;

	return result;
}

bool CollisionManager::squaredRadiusCheck(GameObject* object1, GameObject* object2)
{
	glm::vec2 P1 = object1->getTransform()->position;
	glm::vec2 P2 = object2->getTransform()->position;
	const int halfHeights = (object1->getHeight() + object2->getHeight()) * 0.5f;

	//if (glm::distance(P1, P2) < halfHeights) {

	if (CollisionManager::squaredDistance(P1, P2) < (halfHeights * halfHeights)) {
		if (!object2->getRigidBody()->isColliding) {

			object2->getRigidBody()->isColliding = true;

			switch (object2->getType()) {
			case TARGET:
				std::cout << "Collision with Target!" << std::endl;
				SoundManager::Instance().playSound("yay", 0);

				
				break;
			default:
				
				break;
			}

			return true;
		}
		return false;
	}
	else
	{
		object2->getRigidBody()->isColliding = false;
		return false;
	}
}

bool CollisionManager::AABBCheck(GameObject* object1, GameObject* object2)
{
	// prepare relevant variables
	const auto p1 = object1->getTransform()->position;
	const auto p2 = object2->getTransform()->position;
	const float p1Width = object1->getWidth();
	const float p1Height = object1->getHeight();
	const float p2Width = object2->getWidth();
	const float p2Height = object2->getHeight();

	if (
		p1.x < p2.x + p2Width &&
		p1.x + p1Width > p2.x&&
		p1.y < p2.y + p2Height &&
		p1.y + p1Height > p2.y
		)
	{
		if (!object2->getRigidBody()->isColliding) {

			object2->getRigidBody()->isColliding = true;

			switch (object2->getType()) {
			case TARGET:
				std::cout << "Collision with Target!" << std::endl;
				SoundManager::Instance().playSound("yay", 0);
				break;
			default:
				
				break;
			}

			return true;
		}
		return false;
	}
	else
	{
		object2->getRigidBody()->isColliding = false;
		return false;
	}

	return false;
}

bool CollisionManager::AABBCheck(const SDL_FRect& object1, const SDL_FRect& object2)
{
	return (object1.x < object2.x + object2.w &&
		object1.x + object1.w > object2.x &&
		object1.y < object2.y + object2.h &&
		object1.y + object1.h > object2.y);
}

bool CollisionManager::AABBCheck(const SDL_Rect& object1, GameObject* object2)
{
	return (object1.x < object2->getTransform()->position.x + object2->getWidth() and
		object1.x + object1.w > object2->getTransform()->position.x and
		object1.y < object2->getTransform()->position.y + object2->getHeight() and
		object1.y + object1.h > object2->getTransform()->position.y);
}

bool CollisionManager::lineLineCheck(const glm::vec2 line1_start, const glm::vec2 line1_end, const glm::vec2 line2_start, const glm::vec2 line2_end)
{
	const auto x1 = line1_start.x;
	const auto x2 = line1_end.x;
	const auto x3 = line2_start.x;
	const auto x4 = line2_end.x;
	const auto y1 = line1_start.y;
	const auto y2 = line1_end.y;
	const auto y3 = line2_start.y;
	const auto y4 = line2_end.y;

	// calculate the distance to intersection point
	const auto uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	const auto uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{
		return true;
	}

	return false;
}

bool CollisionManager::lineRectCheck(const glm::vec2 line1_start, const glm::vec2 line1_end, const glm::vec2 rec_start, const float rect_width, const float rect_height)
{
	const auto x1 = line1_start.x;
	const auto x2 = line1_end.x;
	const auto y1 = line1_start.y;
	const auto y2 = line1_end.y;
	const auto rx = rec_start.x;
	const auto ry = rec_start.y;
	const auto rw = rect_width;
	const auto rh = rect_height;

	// check if the line has hit any of the rectangle's sides
	// uses the Line/Line function below
	const auto left = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry), glm::vec2(rx, ry + rh));
	const auto right = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx + rw, ry), glm::vec2(rx + rw, ry + rh));
	const auto top = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry), glm::vec2(rx + rw, ry));
	const auto bottom = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry + rh), glm::vec2(rx + rw, ry + rh));

	//std::cout << left << " " << right << " " << top << " " << bottom << std::endl;

	// if ANY of the above are true, the line
	// has hit the rectangle
	if (left || right || top || bottom) {
		return true;
	}

	return false;
}

int CollisionManager::minSquaredDistanceLineLine(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 line2_start, glm::vec2 line2_end)
{
	auto u = line1_end - line1_start;
	auto v = line2_end - line2_start;
	auto w = line1_start - line2_start;
	auto a = Util::dot(u, u);         // always >= 0
	auto b = Util::dot(u, v);
	auto c = Util::dot(v, v);         // always >= 0
	auto d = Util::dot(u, w);
	auto e = Util::dot(v, w);
	auto D = a * c - b * b;					// always >= 0
	float sc, tc;

	// compute the line parameters of the two closest points
	if (D < Util::EPSILON) {					// the lines are almost parallel
		sc = 0.0;
		tc = (b > c ? d / b : e / c);			// use the largest denominator
	}
	else {
		sc = (b * e - c * d) / D;
		tc = (a * e - b * d) / D;
	}

	// get the difference of the two closest points
	auto dP = w + (sc * u) - (tc * v);  // =  L1(sc) - L2(tc)

	auto norm = Util::dot(dP, dP);
	return norm;
}

bool CollisionManager::lineAABBCheck(Ship* object1, GameObject* object2)
{
	const auto lineStart = object1->getTransform()->position;
	const auto lineEnd = object1->getTransform()->position + object1->getCurrentDirection() * 100.0f;
	// aabb
	const auto boxWidth = object2->getWidth();
	const int halfBoxWidth = boxWidth * 0.5f;
	const auto boxHeight = object2->getHeight();
	const int halfBoxHeight = boxHeight * 0.5f;
	const auto boxStart = object2->getTransform()->position - glm::vec2(halfBoxWidth, halfBoxHeight);

	if (lineRectCheck(lineStart, lineEnd, boxStart, boxWidth, boxHeight))
	{
		switch (object2->getType()) {
		case TARGET:
			std::cout << "Collision with DestructibleObstacle!" << std::endl;
			SoundManager::Instance().playSound("yay", 0);

			break;
		default:
			
			break;
		}

		return true;
	}

	return false;
}

bool CollisionManager::LOSCheck(GameObject* from, GameObject* to, GameObject* obstacle)
{
	const auto lineStart = from->getTransform()->position;
	const auto lineEnd = to->getTransform()->position;
	// aabb
	const auto boxWidth = obstacle->getWidth();
	const int halfBoxWidth = boxWidth * 0.5f;
	const auto boxHeight = obstacle->getHeight();
	const int halfBoxHeight = boxHeight * 0.5f;
	const auto boxStart = obstacle->getTransform()->position - glm::vec2(halfBoxWidth, halfBoxHeight);

	if (lineRectCheck(lineStart, lineEnd, boxStart, boxWidth, boxHeight))
	{
		//std::cout << "No LOS - Collision with DestructibleObstacle!" << std::endl;
		
		return false;
	}

	return true;
}

bool CollisionManager::LOSCheck(GameObject* from, GameObject* to, Tile* obstacle)
{
	if(obstacle->IsObstacle()==false)
	{
		return false;
	}
	const auto lineStart = from->getTransform()->position;
	const auto lineEnd = to->getTransform()->position;
	// aabb
	const auto boxWidth = obstacle->getWidth();
	const int halfBoxWidth = boxWidth * 0.5f;
	const auto boxHeight = obstacle->getHeight();
	const int halfBoxHeight = boxHeight * 0.5f;
	const auto boxStart = obstacle->getTransform()->position;
	
	/*std::cout << "Start: " << lineStart.x << " " << lineStart.y << " End: " << lineEnd.x << " " << lineEnd.y << std::endl;
	std::cout << "DestructibleObstacle: " << obstacle->getTransform()->position.x << " " << obstacle->getTransform()->position.y << std::endl;
	std::cout << "Result: " << lineRectCheck(lineStart, lineEnd, boxStart, boxWidth, boxHeight) << std::endl;*/
	
	if (!lineRectCheck(lineStart, lineEnd, boxStart, boxWidth, boxHeight))
	{
		//std::cout << "No LOS - Collision with DestructibleObstacle!" << std::endl;

		return false;
	}
	//std::cout << "LOS - Collision with DestructibleObstacle " << obstacle->getTransform()->position.x << " " << obstacle->getTransform()->position.y << std::endl;
	return true;
}

int CollisionManager::circleAABBsquaredDistance(const glm::vec2 circle_centre, int circle_radius, const glm::vec2 box_start, const int box_width, const int box_height)
{
	auto dx = std::max(box_start.x - circle_centre.x, 0.0f);
	dx = std::max(dx, circle_centre.x - (box_start.x + box_width));
	auto dy = std::max(box_start.y - circle_centre.y, 0.0f);
	dy = std::max(dy, circle_centre.y - (box_start.y + box_height));

	return (dx * dx) + (dy * dy);
}

bool CollisionManager::circleAABBCheck(GameObject* object1, GameObject* object2)
{
	// circle
	const auto circleCentre = object1->getTransform()->position;
	const int circleRadius = std::max(object1->getWidth() * 0.5f, object1->getHeight() * 0.5f);
	// aabb
	const auto boxWidth = object2->getWidth();
	int halfBoxWidth = boxWidth * 0.5f;
	const auto boxHeight = object2->getHeight();
	int halfBoxHeight = boxHeight * 0.5f;

	const auto boxStart = object2->getTransform()->position - glm::vec2(boxWidth * 0.5f, boxHeight * 0.5f);

	if (circleAABBsquaredDistance(circleCentre, circleRadius, boxStart, boxWidth, boxHeight) <= (circleRadius * circleRadius))
	{
		if (!object2->getRigidBody()->isColliding) {

			object2->getRigidBody()->isColliding = true;

			const auto attackVector = object1->getTransform()->position - object2->getTransform()->position;
			const auto normal = glm::vec2(0.0f, -1.0f);

			const auto dot = Util::dot(attackVector, normal);
			const auto angle = acos(dot / Util::magnitude(attackVector)) * Util::Rad2Deg;

			switch (object2->getType()) {
			case TARGET:
				std::cout << "Collision with Planet!" << std::endl;
				SoundManager::Instance().playSound("yay", 0);
				break;
			case SHIP:
				{
					SoundManager::Instance().playSound("thunder", 0);
					auto velocityX = object1->getRigidBody()->velocity.x;
					auto velocityY = object1->getRigidBody()->velocity.y;

					if ((attackVector.x > 0 && attackVector.y < 0) || (attackVector.x < 0 && attackVector.y < 0))
						// top right or top left
					{
						
						if (angle <= 45)
						{
							object1->getRigidBody()->velocity = glm::vec2(velocityX, -velocityY);
						}
						else
						{
							object1->getRigidBody()->velocity = glm::vec2(-velocityX, velocityY);
						}
					}

					if ((attackVector.x > 0 && attackVector.y > 0) || (attackVector.x < 0 && attackVector.y > 0))
						// bottom right or bottom left
					{
						if (angle <= 135)
						{
							object1->getRigidBody()->velocity = glm::vec2(-velocityX, velocityY);
													}
						else
						{
							object1->getRigidBody()->velocity = glm::vec2(velocityX, -velocityY);
													}
					}
				}
				

				break;
			default:
				
				break;
			}

			return true;
		}
		return false;
	}
	else
	{
		object2->getRigidBody()->isColliding = false;
		return false;
	}

	return false;
}

bool CollisionManager::pointRectCheck(const glm::vec2 point, const glm::vec2 rect_start, const float rect_width, const float rect_height)
{
	const float topLeftX = rect_start.x - rect_width * 0.5;
	const float topLeftY = rect_start.y - rect_height * 0.5;
	const auto width = rect_width;
	const auto height = rect_height;

	if (point.x > topLeftX&&
		point.x < topLeftX + width &&
		point.y > topLeftY&&
		point.y < topLeftY + height)
	{
		return true;
	}
	return false;
}

bool CollisionManager::circleRectCheck(glm::vec2 circle_centre, float circle_radius, glm::vec2 box_start, float box_width,
	float box_height)
{
	float testX, testY;
	testX = circle_centre.x;
	testY = circle_centre.y;
	if(circle_centre.x < box_start.x)
	{
		testX = box_start.x;
	}
	else if(circle_centre.x > box_start.x + box_width)
	{
		testX = box_start.x + box_width;
	}

	if(circle_centre.y<box_start.y)
	{
		testY = box_start.y;
	}
	else if(circle_centre.y > box_start.y + box_height)
	{
		testY = box_start.y + box_height;
	}

	float distX = circle_centre.x - testX;
	float distY = circle_centre.y - testY;
	float distanceSquare = (distX * distX) + (distY * distY);

	if (distanceSquare <= (circle_radius * circle_radius))
	{
		return true;
	}
	else
		return false;
}

bool CollisionManager::lineCircleCheck(glm::vec2 line_start, glm::vec2 line_end, glm::vec2 circle_centre,
	int circle_radius)
{
	if ((circle_centre.x - line_start.x) * (circle_centre.x - line_start.x) + (circle_centre.y - line_start.y) * (circle_centre.y - line_start.y) < (circle_radius * circle_radius)
		|| (circle_centre.x - line_end.x) * (circle_centre.x - line_end.x) + (circle_centre.y - line_end.y) * (circle_centre.y - line_end.y) < (circle_radius * circle_radius))
	{
		//point is in the circle
		return true;
	}

	float distX = line_start.x-line_end.x;
	float distY = line_start.y - line_end.y;
	float len = sqrt((distX * distX) + (distY * distY));
	float dot = (((((circle_centre.x - line_start.x) * (line_end.x - line_start.x)) + ((circle_centre.y - line_start.y) * (line_end.y - line_start.y))) / len)/len);
	float closestX = line_start.x + (dot * (line_end.x - line_start.x));
	float closestY = line_start.y + (dot * (line_end.y - line_start.y));

	bool onLine;
	float distStart = sqrt((closestX - line_start.x) * (closestX - line_start.x) + (closestY - line_start.y) * (closestY - line_start.y));
	float distEnd= sqrt((closestX - line_end.x) * (closestX - line_end.x) + (closestY - line_end.y) * (closestY - line_end.y));
	float buffer = 0.1f;
	float lineLength= sqrt((line_end.x - line_start.x) * (line_end.x - line_start.x) + (line_end.y - line_start.y) * (line_end.y - line_start.y));
	if(distStart+distEnd<=lineLength-buffer || distStart + distEnd >= lineLength + buffer)
	{
		onLine = false;
	}
	else
	{
		onLine = true;
	}
	
	if (!onLine)
	{
		return false;
	}

	distX = closestX - circle_centre.x;
	distY = closestY - circle_centre.y;
	float distance = sqrt((distX * distX) + (distY * distY));

	if (distance <= circle_radius) {
		return true;
	}
	return false;
}

bool CollisionManager::PlayerCollision(Player* player, glm::vec2 velocity, std::vector<Tile*> obstacleVec)
{
	SDL_FRect tempRect1, tempRect2;
	tempRect1.x = player->getTransform()->position.x + velocity.x;
	tempRect1.y = player->getTransform()->position.y + velocity.y;
	tempRect1.w = player->getWidth();
	tempRect1.h = player->getHeight();
	
	for(auto obstacle:obstacleVec)
	{
		tempRect2.x = obstacle->getTransform()->position.x;
		tempRect2.y = obstacle->getTransform()->position.y;
		tempRect2.w = obstacle->getWidth();
		tempRect2.h = obstacle->getHeight();
		
		if(AABBCheck(tempRect1,tempRect2))
		{
			std::cout << "Collision!" << std::endl;
			return true;
		}
	}
	return false;
	
	//SDL_Rect p = { player->getTransform()->position.x + velocity.x + 10, player->getTransform()->position.y + velocity.y + 10, player->getWidth() - 20, player->getHeight() - 20 }; // Adjusted bounding box.
	//			   
	//for (int i = 0; i < 9; i++)
	//{
	//	std::cout << "Player: " << player->getTransform()->position.x << " " << player->getTransform()->position.y << " Tile"<<i<<": " << tiles[i]->getTransform()->position.x << " " << tiles[i]->getTransform()->position.y << std::endl;
	//	std::cout << tiles[i]->IsObstacle()<<" "<<AABBCheck(player, tiles[i]) << std::endl;
	//	//SDL_Rect t = MAMA::ConvertFRect2Rect(*(tiles[i]->GetDstP()));
	//	if (tiles[i]->IsObstacle() && AABBCheck(player,tiles[i]))
	//	{ // Collision!
	//		return true;
	//	}
	//}
	//return false;
}

void CollisionManager::CheckMapCollision(Player* player, std::vector<Tile*> obstacleVec)
{
	for(auto obstacle:obstacleVec)
	{
		if(AABBCheck(player,obstacle))
		{
			if(player->getTransform()->position.y + player->getRigidBody()->velocity.y <= obstacle->getTransform()->position.y + obstacle->getHeight())//from bottom
			{
				/*std::cout << "from bottom" << std::endl;*/
				player->getRigidBody()->velocity.y = 0;
				player->getTransform()->position.y = obstacle->getTransform()->position.y + obstacle->getHeight()+1;
				std::cout << "from top" << std::endl;
				//player->getRigidBody()->velocity.y = 0;
				//player->getTransform()->position.y = obstacle->getTransform()->position.y - player->getHeight() - 1;
			}
			else if(player->getTransform()->position.y + player->getHeight() + player->getRigidBody()->velocity.y >= obstacle->getTransform()->position.y)//from top
			{
				/*std::cout << "from top" << std::endl;*/
				player->getRigidBody()->velocity.y = 0;
				player->getTransform()->position.y = obstacle->getTransform()->position.y - player->getHeight()-1;
				//std::cout << "from bottom" << std::endl;
				/*player->getRigidBody()->velocity.y = 0;
				player->getTransform()->position.y = obstacle->getTransform()->position.y + obstacle->getHeight() + 1;*/
			}
			else if(player->getTransform()->position.x + player->getRigidBody()->velocity.x <= obstacle->getTransform()->position.x + obstacle->getWidth())//from right
			{
				//std::cout << "from right" << std::endl;
				player->getRigidBody()->velocity.x = 0;
				player->getTransform()->position.x = obstacle->getTransform()->position.x + obstacle->getWidth() + 1;
			}
			else if(player->getTransform()->position.x + player->getWidth() + player->getRigidBody()->velocity.x >= obstacle->getTransform()->position.x)//from left
			{
				//std::cout << "from left" << std::endl;
				player->getRigidBody()->velocity.x = 0;
				player->getTransform()->position.x = obstacle->getTransform()->position.x - player->getWidth() - 1;
			}
		}		
	}
}


CollisionManager::CollisionManager()
= default;


CollisionManager::~CollisionManager()
= default;
