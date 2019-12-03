#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <map>
#include <array>
#include <cmath>

using namespace std;
namespace {
	enum class Mov {
		U,
		D,
		L,
		R
	};

	struct Move {
		Mov direction;
		size_t length;
	};

	struct Point {
		int x;
		int y;
	};

	struct Segment {
		Point _start;
		Point _end;
	};

	const auto calculateDistance = [](int x, int y, int x1, int y1) {
		return std::abs(x1 - x) + std::abs(y1 - y);
	};

	std::vector<Segment> first;
	std::vector<Segment> second;

	const auto parseInput = [](int x, int y, Move& m) {
		Segment res = {x, y, x, y};

		switch (m.direction) {
			case Mov::D: { res._end.y -= m.length; break;}
			case Mov::U: { res._end.y += m.length; break;}
			case Mov::L: { res._end.x -= m.length; break;}
			case Mov::R: { res._end.x += m.length; break;}
		}
		return res;
	};

	bool onSegment(Point p, Point q, Point r)
	{
		return (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
			q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y));
	}

	int orientation(Point p, Point q, Point r)
	{
		int val = (q.y - p.y) * (r.x - q.x) -
				(q.x - p.x) * (r.y - q.y);
		if (val == 0) return 0;
		return (val > 0)? 1: 2;
	}

	bool doIntersect(Point p1, Point q1, Point p2, Point q2)
	{
		// Find the four orientations needed for general and
		// special cases
		int o1 = orientation(p1, q1, p2);
		int o2 = orientation(p1, q1, q2);
		int o3 = orientation(p2, q2, p1);
		int o4 = orientation(p2, q2, q1);

		//general case
		if (o1 != o2 && o3 != o4)
			return true;

		// Special Cases
		// p1, q1 and p2 are colinear and p2 lies on segment p1q1
		if (o1 == 0 && onSegment(p1, p2, q1)) return true;
		// p1, q1 and q2 are colinear and q2 lies on segment p1q1
		if (o2 == 0 && onSegment(p1, q2, q1)) return true;
		// p2, q2 and p1 are colinear and p1 lies on segment p2q2
		if (o3 == 0 && onSegment(p2, p1, q2)) return true;
		// p2, q2 and q1 are colinear and q1 lies on segment p2q2
		if (o4 == 0 && onSegment(p2, q1, q2)) return true;
		return false; // Doesn't fall in any of the above cases
	}

	const auto getCrossection = [](Point A, Point B, Point C, Point D) -> std::pair<bool, Point>
	{
		const auto a1 = B.y - A.y;
		const auto b1 = A.x - B.x;
		const auto c1 = a1*(A.x) + b1*(A.y);

		const auto a2 = D.y - C.y;
		const auto b2 = C.x - D.x;
		const auto c2 = a2*(C.x)+ b2*(C.y);

		const auto determinant = a1*b2 - a2*b1;
		if (determinant == 0) return std::make_pair(false, Point{0,0});

		const auto x = (b2*c1 - b1*c2)/determinant;
		const auto y = (a1*c2 - a2*c1)/determinant;
		return std::make_pair(true, Point{x, y});
	};
}

int main()
{
	//std::vector<Move> firstWire = {{Mov::R,999},{Mov::D,666},{Mov::L,86},{Mov::U, 464},{Mov::R,755},{Mov::U, 652},{Mov::R,883},{Mov::D,287},{Mov::L,244},{Mov::U, 308},{Mov::L,965},{Mov::U, 629},{Mov::R,813},{Mov::U, 985},{Mov::R,620},{Mov::D,153},{Mov::L,655},{Mov::D,110},{Mov::R,163},{Mov::D,81},{Mov::L,909},{Mov::D,108},{Mov::L,673},{Mov::D,165},{Mov::L,620},{Mov::U, 901},{Mov::R,601},{Mov::D,561},{Mov::L,490},{Mov::D,21},{Mov::R,223},{Mov::U, 478},{Mov::R,80},{Mov::U, 379},{Mov::R,873},{Mov::U, 61},{Mov::L,674},{Mov::D,732},{Mov::R,270},{Mov::U, 297},{Mov::L,354},{Mov::U, 264},{Mov::L,615},{Mov::D,2},{Mov::R,51},{Mov::D,582},{Mov::R,280},{Mov::U, 173},{Mov::R,624},{Mov::U, 644},{Mov::R,451},{Mov::D,97},{Mov::R,209},{Mov::U, 245},{Mov::R,32},{Mov::U, 185},{Mov::R,948},{Mov::D,947},{Mov::R,380},{Mov::D,945},{Mov::L,720},{Mov::U, 305},{Mov::R,911},{Mov::U, 614},{Mov::L,419},{Mov::D,751},{Mov::L,934},{Mov::U, 371},{Mov::R,291},{Mov::D,166},{Mov::L,137},{Mov::D,958},{Mov::R,368},{Mov::U, 441},{Mov::R,720},{Mov::U, 822},{Mov::R,961},{Mov::D,32},{Mov::R,242},{Mov::D,972},{Mov::L,782},{Mov::D,166},{Mov::L,680},{Mov::U, 111},{Mov::R,379},{Mov::D,155},{Mov::R,213},{Mov::U, 573},{Mov::R,761},{Mov::D,543},{Mov::R,762},{Mov::U, 953},{Mov::R,317},{Mov::U, 841},{Mov::L,38},{Mov::U, 900},{Mov::R,573},{Mov::U, 766},{Mov::R,807},{Mov::U, 950},{Mov::R,945},{Mov::D,705},{Mov::R,572},{Mov::D,994},{Mov::L,633},{Mov::U, 33},{Mov::L,173},{Mov::U, 482},{Mov::R,253},{Mov::D,835},{Mov::R,800},{Mov::U, 201},{Mov::L,167},{Mov::U, 97},{Mov::R,375},{Mov::D,813},{Mov::L,468},{Mov::D,924},{Mov::L,972},{Mov::U, 570},{Mov::R,975},{Mov::D,898},{Mov::L,195},{Mov::U, 757},{Mov::L,565},{Mov::D,378},{Mov::R,935},{Mov::U, 4},{Mov::L,334},{Mov::D,707},{Mov::R,958},{Mov::U, 742},{Mov::R,507},{Mov::U, 892},{Mov::R,174},{Mov::D,565},{Mov::L,862},{Mov::D,311},{Mov::L,770},{Mov::D,619},{Mov::L,319},{Mov::D,698},{Mov::L,169},{Mov::D,652},{Mov::L,761},{Mov::D,644},{Mov::R,837},{Mov::U, 43},{Mov::L,197},{Mov::D,11},{Mov::L,282},{Mov::D,345},{Mov::L,551},{Mov::U, 460},{Mov::R,90},{Mov::D,388},{Mov::R,911},{Mov::U, 602},{Mov::L,21},{Mov::D,275},{Mov::L,763},{Mov::U, 880},{Mov::R,604},{Mov::D,838},{Mov::R,146},{Mov::U, 993},{Mov::L,99},{Mov::U, 99},{Mov::R,928},{Mov::U, 54},{Mov::L,148},{Mov::D,863},{Mov::R,618},{Mov::U, 449},{Mov::R,549},{Mov::D,659},{Mov::R,449},{Mov::D,435},{Mov::L,978},{Mov::D,612},{Mov::L,645},{Mov::D,691},{Mov::R,190},{Mov::D,434},{Mov::L,841},{Mov::D,364},{Mov::L,634},{Mov::D,590},{Mov::R,962},{Mov::U, 15},{Mov::R,921},{Mov::D,442},{Mov::L,284},{Mov::U, 874},{Mov::R,475},{Mov::D,556},{Mov::L,135},{Mov::U, 376},{Mov::L,459},{Mov::D,673},{Mov::L,515},{Mov::U, 438},{Mov::L,736},{Mov::U, 266},{Mov::L,601},{Mov::U, 351},{Mov::R,496},{Mov::U, 891},{Mov::L,893},{Mov::D,597},{Mov::L,135},{Mov::D,966},{Mov::R,121},{Mov::U, 763},{Mov::R,46},{Mov::D,110},{Mov::R,830},{Mov::U, 644},{Mov::L,932},{Mov::D,122},{Mov::L,123},{Mov::U, 145},{Mov::R,273},{Mov::U, 690},{Mov::L,443},{Mov::D,372},{Mov::R,818},{Mov::D,259},{Mov::L,695},{Mov::U, 69},{Mov::R,73},{Mov::D,718},{Mov::R,106},{Mov::U, 929},{Mov::L,346},{Mov::D,291},{Mov::L,857},{Mov::D,341},{Mov::R,297},{Mov::D,823},{Mov::R,819},{Mov::U, 496},{Mov::L,958},{Mov::U, 394},{Mov::R,102},{Mov::D,763},{Mov::L,444},{Mov::D,835},{Mov::L,33},{Mov::U, 45},{Mov::R,812},{Mov::U, 845},{Mov::R,196},{Mov::U, 458},{Mov::R,231},{Mov::U, 637},{Mov::R,661},{Mov::D,983},{Mov::L,941},{Mov::U, 975},{Mov::L,353},{Mov::U, 609},{Mov::L,698},{Mov::U, 152},{Mov::R,122},{Mov::D,882},{Mov::R,682},{Mov::D,926},{Mov::R,729},{Mov::U, 429},{Mov::R,255},{Mov::D,227},{Mov::R,987},{Mov::D,547},{Mov::L,446},{Mov::U, 217},{Mov::R,678},{Mov::D,464},{Mov::R,849},{Mov::D,472},{Mov::L,406},{Mov::U, 940},{Mov::L,271},{Mov::D,779},{Mov::R,980},{Mov::D,751},{Mov::L,171},{Mov::D,420},{Mov::L,49},{Mov::D,271},{Mov::R,430},{Mov::D,530},{Mov::R,509},{Mov::U, 479},{Mov::R,135},{Mov::D,770},{Mov::R,85},{Mov::U, 815},{Mov::R,328},{Mov::U, 234},{Mov::R,83}};
	//std::vector<Move> secondWire = {{Mov::L,1008},{Mov::D,951},{Mov::L,618},{Mov::U, 727},{Mov::L,638},{Mov::D,21},{Mov::R, 804},{Mov::D,19},{Mov::L,246},{Mov::U, 356},{Mov::L,51},{Mov::U, 8},{Mov::L,627},{Mov::U, 229},{Mov::R, 719},{Mov::D,198},{Mov::L,342},{Mov::U, 240},{Mov::L,738},{Mov::D,393},{Mov::L,529},{Mov::D,22},{Mov::R, 648},{Mov::D,716},{Mov::L,485},{Mov::U, 972},{Mov::L,580},{Mov::U, 884},{Mov::R, 612},{Mov::D,211},{Mov::L,695},{Mov::U, 731},{Mov::R, 883},{Mov::U, 470},{Mov::R, 732},{Mov::U, 723},{Mov::R, 545},{Mov::D,944},{Mov::R, 18},{Mov::U, 554},{Mov::L,874},{Mov::D,112},{Mov::R, 782},{Mov::D,418},{Mov::R, 638},{Mov::D,296},{Mov::L,123},{Mov::U, 426},{Mov::L,479},{Mov::U, 746},{Mov::L,209},{Mov::D,328},{Mov::L,121},{Mov::D,496},{Mov::L,172},{Mov::D,228},{Mov::L,703},{Mov::D,389},{Mov::R, 919},{Mov::U, 976},{Mov::R, 364},{Mov::D,468},{Mov::L,234},{Mov::U, 318},{Mov::R, 912},{Mov::U, 236},{Mov::R, 148},{Mov::U, 21},{Mov::R, 26},{Mov::D,116},{Mov::L,269},{Mov::D,913},{Mov::L,949},{Mov::D,206},{Mov::L,348},{Mov::U, 496},{Mov::R, 208},{Mov::U, 706},{Mov::R, 450},{Mov::U, 472},{Mov::R, 637},{Mov::U, 884},{Mov::L,8},{Mov::U, 82},{Mov::L,77},{Mov::D,737},{Mov::L,677},{Mov::D,358},{Mov::L,351},{Mov::U, 719},{Mov::R, 154},{Mov::U, 339},{Mov::L,506},{Mov::U, 76},{Mov::L,952},{Mov::D,791},{Mov::L,64},{Mov::U, 879},{Mov::R, 332},{Mov::D,244},{Mov::R, 638},{Mov::D,453},{Mov::L,107},{Mov::D,908},{Mov::L,58},{Mov::D,188},{Mov::R, 440},{Mov::D,147},{Mov::R, 913},{Mov::U, 298},{Mov::L,681},{Mov::D,582},{Mov::L,943},{Mov::U, 503},{Mov::L,6},{Mov::U, 459},{Mov::L,289},{Mov::D,131},{Mov::L,739},{Mov::D,443},{Mov::R, 333},{Mov::D,138},{Mov::R, 553},{Mov::D,73},{Mov::L,475},{Mov::U, 930},{Mov::L,332},{Mov::U, 518},{Mov::R, 614},{Mov::D,553},{Mov::L,515},{Mov::U, 602},{Mov::R, 342},{Mov::U, 95},{Mov::R, 131},{Mov::D,98},{Mov::R, 351},{Mov::U, 921},{Mov::L,141},{Mov::U, 207},{Mov::R, 199},{Mov::U, 765},{Mov::R, 55},{Mov::U, 623},{Mov::R, 768},{Mov::D,620},{Mov::L,722},{Mov::U, 31},{Mov::L,891},{Mov::D,862},{Mov::R, 85},{Mov::U, 271},{Mov::R, 590},{Mov::D,184},{Mov::R, 960},{Mov::U, 149},{Mov::L,985},{Mov::U, 82},{Mov::R, 591},{Mov::D,384},{Mov::R, 942},{Mov::D,670},{Mov::R, 584},{Mov::D,637},{Mov::L,548},{Mov::U, 844},{Mov::R, 353},{Mov::U, 496},{Mov::L,504},{Mov::U, 3},{Mov::L,830},{Mov::U, 239},{Mov::R, 246},{Mov::U, 279},{Mov::L,146},{Mov::U, 965},{Mov::R, 784},{Mov::U, 448},{Mov::R, 60},{Mov::D,903},{Mov::R, 490},{Mov::D,831},{Mov::L,537},{Mov::U, 109},{Mov::R, 271},{Mov::U, 306},{Mov::L,342},{Mov::D,99},{Mov::L,234},{Mov::D,936},{Mov::R, 621},{Mov::U, 870},{Mov::R, 56},{Mov::D,29},{Mov::R, 366},{Mov::D,562},{Mov::R, 276},{Mov::D,134},{Mov::L,289},{Mov::D,425},{Mov::R, 597},{Mov::D,102},{Mov::R, 276},{Mov::D,600},{Mov::R, 1},{Mov::U, 322},{Mov::L,526},{Mov::D,744},{Mov::L,259},{Mov::D,111},{Mov::R, 994},{Mov::D,581},{Mov::L,973},{Mov::D,871},{Mov::R, 173},{Mov::D,924},{Mov::R, 294},{Mov::U, 478},{Mov::R, 384},{Mov::D,242},{Mov::R, 606},{Mov::U, 629},{Mov::R, 472},{Mov::D,651},{Mov::R, 526},{Mov::U, 55},{Mov::R, 885},{Mov::U, 637},{Mov::R, 186},{Mov::U, 299},{Mov::R, 812},{Mov::D,95},{Mov::R, 390},{Mov::D,689},{Mov::R, 514},{Mov::U, 483},{Mov::R, 471},{Mov::D,591},{Mov::L,610},{Mov::D,955},{Mov::L,599},{Mov::D,674},{Mov::R, 766},{Mov::U, 834},{Mov::L,417},{Mov::U, 625},{Mov::R, 903},{Mov::U, 376},{Mov::R, 991},{Mov::U, 175},{Mov::R, 477},{Mov::U, 524},{Mov::L,453},{Mov::D,407},{Mov::R, 72},{Mov::D,217},{Mov::L,968},{Mov::D,892},{Mov::L,806},{Mov::D,589},{Mov::R, 603},{Mov::U, 938},{Mov::L,942},{Mov::D,940},{Mov::R, 578},{Mov::U, 820},{Mov::L,888},{Mov::U, 232},{Mov::L,740},{Mov::D,348},{Mov::R, 445},{Mov::U, 269},{Mov::L,170},{Mov::U, 979},{Mov::L,159},{Mov::U, 433},{Mov::L,31},{Mov::D,818},{Mov::L,914},{Mov::U, 600},{Mov::L,33},{Mov::U, 159},{Mov::R, 974},{Mov::D,983},{Mov::L,922},{Mov::U, 807},{Mov::R, 682},{Mov::U, 525},{Mov::L,234},{Mov::U, 624},{Mov::L,973},{Mov::U, 123},{Mov::L,875},{Mov::D,64},{Mov::L,579},{Mov::U, 885},{Mov::L,911},{Mov::D,578},{Mov::R, 17},{Mov::D,293},{Mov::L,211}};

	std::vector<Move> firstWire = {{Mov::R, 8}, {Mov::U, 5}, {Mov::L, 5}, {Mov::D, 3}};
	std::vector<Move> secondWire = {{Mov::U, 7}, {Mov::R, 6}, {Mov::D, 4}, {Mov::L, 4}};

	std::vector<Point> intersections;
	int x = 0;
	int y = 0;

	for (auto& e: firstWire) {
		first.push_back(parseInput(x, y, e));
		//std::cout << "First: (" << x << ", " << y << ") -> (" << first.back()._end.x << ", " << first.back()._end.y << ")\n";
		x = first.back()._end.x;
		y = first.back()._end.y;
	}


	x = 0;
	y = 0;
	auto firstSum = 0;
	for (auto& e: secondWire) {
		const auto s = parseInput(x, y, e);
		firstSum += e.length;
		//std::cout << "Second: (" << x << ", " << y << ") -> (" << s._end.x << ", " << s._end.y << ")\n";
		x = s._end.x;
		y = s._end.y;
		for (auto& other: first) {
			if (doIntersect(s._start, s._end, other._start, other._end)) {
				const auto [_, point] = getCrossection(s._start, s._end, other._start, other._end);
				if (point.x == 0 || point.y == 0) continue;
				intersections.push_back(point);
				firstSum -= calculateDistance(s._start.x, s._start.y, point.x, point.y);
				std::cout << "(" << point.x << ", " << point.y << "): "
					<< calculateDistance(0, 0, point.x, point.y)
					<< " - " <<  firstSum << '\n';
			}
		}
	}

/*
	first.clear();
	first.shrink_to_fit();
	x = 0;
	y = 0;
	std::cout << "second...\n";

	for (auto& i :intersections) {
		std::cout << i.x << " - " << i.y << '\n';
	}

	auto firstSum = 0;
	for (auto& e: firstWire) {
		firstSum += e.length;
		const auto s = parseInput(x, y, e);
		x = s._end.x;
		y = s._end.y;
		//std::cout << "First: (" << x << ", " << y << ")\n";
		Point p{x, y};
		const auto l = [p](auto& el) {
			return el.x == p.x || el.y == p.y;
		};
		const auto pos = std::find_if(intersections.cbegin(), intersections.cend(), l);
		if (pos != intersections.cend()) {
			std::cout << "First: (" << x << ", " << y << "): (" <<  firstSum <<  ")\n";
		}
	}

	firstSum = 0;
	x = 0;
	y = 0;
	for (auto& e: secondWire) {
		firstSum += e.length;
		const auto s = parseInput(x, y, e);
		//std::cout << "First: (" << x << ", " << y << ") -> (" << s._end.x << ", " << s._end.y << ")\n";
		x = s._end.x;
		y = s._end.y;
		//std::cout << "Second: (" << x << ", " << y << ")\n";
		Point p{x, y};
		const auto l = [p](auto& el) {
			return el.x == p.x || el.y == p.y;
		};
		const auto pos = std::find_if(intersections.cbegin(), intersections.cend(), l);
		if (pos != intersections.cend()) {
			std::cout << "Second: (" << x << ", " << y << "): (" <<  firstSum <<  ")\n";
		}
	}
*/
	return 0;
}
