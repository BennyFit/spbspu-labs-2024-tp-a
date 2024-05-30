#include "polygon.hpp"
#include <string>
#include <interface.hpp>

std::istream& chernikova::operator>>(std::istream& in, Point& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char symbol = 0;
  in >> ExactSymbolSavedI{ ' ', symbol };
  in >> ExactSymbolSavedI{ '(', symbol };
  in >> dest.x;
  in >> ExactSymbolSavedI{ ';', symbol };
  in >> dest.y;
  in >> ExactSymbolSavedI{ ')', symbol };

  if (!in && symbol != '\n')
  {
    fixStream(in);
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& chernikova::operator>>(std::istream& in, Polygon& dest)
{
  StreamGuard streamGuard(in);
  in.unsetf(std::ios_base::skipws);
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  size_t count = 0;
  in >> count;

  if (!in)
  {
    fixStream(in);
    in.setstate(std::ios::failbit);
    return in;
  }

  using iter = std::istream_iterator< Point >;
  dest.points.clear();
  dest.points.reserve(count);
  std::copy(iter(in), iter(), std::back_inserter(dest.points));
  in.clear();

  if (count < 3 || dest.points.size() != count)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  return in;
}

bool chernikova::isEven(const Polygon& polygon)
{
  return polygon.points.size() % 2 == 0;
}

bool chernikova::isOdd(const Polygon& polygon)
{
  return !isEven(polygon);
}

double chernikova::calcArea(const Point& left, const Point& right)
{
  return 0.5 * (left.x * right.y - right.x * left.y);
}

double chernikova::getArea(const Polygon& polygon)
{
  double res = 0.0;
  for (size_t i = 0; i < polygon.points.size() - 1; ++i)
  {
    res += chernikova::calcArea(polygon.points[i], polygon.points[i + 1]);
  }
  res += chernikova::calcArea(polygon.points.back(), polygon.points.front());
  return std::abs(res);
}

double chernikova::sumArea(double cur, const Polygon& polygon)
{
  return cur + chernikova::getArea(polygon);
}

bool chernikova::isNecessaryVertex(const Polygon& polygon, size_t count)
{
  return polygon.points.size() == count;
}

double chernikova::chooseGreaterArea(double cur, const Polygon& polygon)
{
  double area = chernikova::getArea(polygon);
  return (cur > area) ? cur : area;
}

size_t chernikova::chooseGreaterVertexes(double cur, const Polygon& polygon)
{
  size_t count = polygon.points.size();
  return (cur > count) ? cur : count;
}

double chernikova::chooseLessArea(double cur, const Polygon& polygon)
{
  double area = chernikova::getArea(polygon);
  return (cur < area) ? cur : area;
}

size_t chernikova::chooseLessVertexes(double cur, const Polygon& polygon)
{
  size_t count = polygon.points.size();
  return (cur < count) ? cur : count;
}

void chernikova::getAreaByPredicat(const std::vector< Polygon >& polygons, std::ostream& out, Predicat predicat)
{
  std::vector< Polygon > some_polygons;
  some_polygons.reserve(polygons.size());
  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(some_polygons), predicat);
  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  out << std::accumulate(some_polygons.begin(), some_polygons.end(), 0.0, sumArea) << "\n";
}

void chernikova::getAreaEven(const std::vector< Polygon >& polygons, std::ostream& out)
{
  getAreaByPredicat(polygons, out, isEven);
}

void chernikova::getAreaOdd(const std::vector< Polygon >& polygons, std::ostream& out)
{
  getAreaByPredicat(polygons, out, isOdd);
}

void chernikova::getAreaMean(const std::vector< Polygon >& polygons, std::ostream& out)
{
  size_t count = polygons.size();
  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  out << std::accumulate(polygons.begin(), polygons.end(), 0.0, sumArea) / count << "\n";
}

void chernikova::getAreaVertexes(const std::vector< Polygon >& polygons, size_t count, std::ostream& out)
{
  std::vector< Polygon > vertexesPolygons;
  vertexesPolygons.reserve(polygons.size());
  using namespace std::placeholders;
  auto pred = std::bind(isNecessaryVertex, _1, count);
  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(vertexesPolygons), pred);
  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  out << std::accumulate(vertexesPolygons.begin(), vertexesPolygons.end(), 0.0, sumArea) << "\n";
}

void chernikova::getExtremumArea(const std::vector< Polygon >& polygons, std::ostream& out, ComparatorArea comparator)
{
  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  out << std::accumulate(polygons.begin(), polygons.end(), 0.0, comparator) << "\n";
}

void chernikova::getMaxArea(const std::vector< Polygon >& polygons, std::ostream& out)
{
  getExtremumArea(polygons, out, chooseGreaterArea);
}

void chernikova::getMinArea(const std::vector< Polygon >& polygons, std::ostream& out)
{
  getExtremumArea(polygons, out, chooseLessArea);
}

void chernikova::getExtremumVertexes(const std::vector< Polygon >& polygons, std::ostream& out, ComparatorVertexes comparator)
{
  StreamGuard streamGuard(out);
  out << std::accumulate(polygons.begin(), polygons.end(), 0, comparator) << "\n";
}

void chernikova::getMaxVertexes(const std::vector< Polygon >& polygons, std::ostream& out)
{
  getExtremumVertexes(polygons, out, chooseGreaterVertexes);
}

void chernikova::getMinVertexes(const std::vector< Polygon >& polygons, std::ostream& out)
{
  getExtremumVertexes(polygons, out, chooseLessVertexes);
}

void chernikova::getCountByPredicat(const std::vector< Polygon >& polygons, std::ostream& out, Predicat predicat)
{
  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  out << std::count_if(polygons.begin(), polygons.end(), predicat) << "\n";
}

void chernikova::getCountEven(const std::vector< Polygon >& polygons, std::ostream& out)
{
  getCountByPredicat(polygons, out, isEven);
}

void chernikova::getCountOdd(const std::vector< Polygon >& polygons, std::ostream& out)
{
  getCountByPredicat(polygons, out, isOdd);
}

void chernikova::getCountVertexes(const std::vector< Polygon >& polygons, size_t count, std::ostream& out)
{
  using namespace std::placeholders;
  auto pred = std::bind(chernikova::isNecessaryVertex, _1, count);
  StreamGuard streamGuard(out);
  out << count_if(polygons.begin(), polygons.end(), pred) << "\n";
}

bool chernikova::isEqualPolygon(const Polygon& lhs, const Polygon& rhs)
{
  return (rhs.points == lhs.points);
}

void chernikova::echo(std::vector< Polygon >& polygons, const Polygon& polygon, std::ostream& out)
{
  using namespace std::placeholders;

  auto equal = std::bind(isEqualPolygon, _1, polygon);
  std::vector< Polygon > copies;
  copies.reserve(polygons.size());
  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(copies), equal);

  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  out << copies.size() << "\n";

  std::copy(copies.begin(), copies.end(), std::back_inserter(polygons));
}

bool chernikova::hasIntersection(const Polygon& lhs, const Polygon& rhs)
{
  Point minLhs = *std::min_element(lhs.points.begin(), lhs.points.end());
  Point minRhs = *std::min_element(rhs.points.begin(), rhs.points.end());
  Point maxLhs = *std::max_element(lhs.points.begin(), lhs.points.end());
  Point maxRhs = *std::max_element(rhs.points.begin(), rhs.points.end());

  return ((minLhs <= maxRhs) && (maxLhs >= minRhs)) || ((minRhs <= maxLhs) && (maxRhs >= minLhs));
}

void chernikova::intersections(const std::vector< Polygon >& polygons, const Polygon& polygon, std::ostream& out)
{
  using namespace std::placeholders;
  auto pred = std::bind(hasIntersection, _1, polygon);
  out << std::count_if(polygons.begin(), polygons.end(), pred) << "\n";
}

bool chernikova::isRightAngleByThreePoints(const Point* p1, const Point* p2, const Point* p3)
{
  long long int scalarMultiply = (p3->x - p2->x) * (p1->x - p2->x) + (p3->y - p2->y) * (p1->y - p2->y);
  return scalarMultiply == 0;
}

bool chernikova::isRightAngle(const Point& p)
{
  const Point* p1 = &p;
  const Point* p2 = &p - 1;
  const Point* p3 = &p - 2;

  return isRightAngleByThreePoints(p1, p2, p3);
}

bool chernikova::hasRightAngle(const Polygon& polygon)
{
  if (polygon.points.size() < 3)
  {
    return false;
  }

  auto it = std::find_if(polygon.points.begin() + 2, polygon.points.end(), isRightAngle);

  if (it != polygon.points.end())
  {
    return true;
  }
  if (isRightAngleByThreePoints(&polygon.points.back(), &polygon.points.front(), &polygon.points.front() + 1))
  {
    return true;
  }
  if (isRightAngleByThreePoints(&polygon.points.back() - 1, &polygon.points.back(), &polygon.points.front()))
  {
    return true;
  }

  return false;
}

void chernikova::rightShapes(const std::vector< Polygon >& polygons, std::ostream& out)
{
  size_t count = std::count_if(polygons.begin(), polygons.end(), hasRightAngle);
  out << count << "\n";
}
