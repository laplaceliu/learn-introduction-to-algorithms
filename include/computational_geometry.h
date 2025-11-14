#ifndef COMPUTATIONAL_GEOMETRY_H
#define COMPUTATIONAL_GEOMETRY_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <stack>
#include <vector>

namespace algorithms {

// 点结构体
struct Point {
  double x, y;

  Point(double x = 0, double y = 0) : x(x), y(y) {}

  bool operator==(const Point &other) const {
    return std::abs(x - other.x) < 1e-9 && std::abs(y - other.y) < 1e-9;
  }

  bool operator!=(const Point &other) const { return !(*this == other); }

  // 计算两点间距离
  double distance(const Point &other) const {
    return std::sqrt((x - other.x) * (x - other.x) +
                     (y - other.y) * (y - other.y));
  }
};

// 线段结构体
struct Segment {
  Point p1, p2;

  Segment(const Point &p1, const Point &p2) : p1(p1), p2(p2) {}
};

// 33.1 线段性质
class SegmentProperties {
public:
  // 计算叉积 (p1-p0) × (p2-p0)
  static double cross_product(const Point &p0, const Point &p1,
                              const Point &p2) {
    return (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
  }

  // 判断点p是否在线段s上
  static bool on_segment(const Point &p, const Segment &s) {
    // 检查点p是否在线段的边界框内
    if (p.x <= std::max(s.p1.x, s.p2.x) && p.x >= std::min(s.p1.x, s.p2.x) &&
        p.y <= std::max(s.p1.y, s.p2.y) && p.y >= std::min(s.p1.y, s.p2.y)) {
      // 检查点p是否在直线上（叉积为0）
      return std::abs(cross_product(s.p1, s.p2, p)) < 1e-9;
    }
    return false;
  }

  // 判断两条线段是否相交
  static bool segments_intersect(const Segment &s1, const Segment &s2) {
    double d1 = cross_product(s1.p1, s1.p2, s2.p1);
    double d2 = cross_product(s1.p1, s1.p2, s2.p2);
    double d3 = cross_product(s2.p1, s2.p2, s1.p1);
    double d4 = cross_product(s2.p1, s2.p2, s1.p2);

    // 检查一般情况
    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
      return true;
    }

    // 检查特殊情况（共线情况）
    if (std::abs(d1) < 1e-9 && on_segment(s2.p1, s1))
      return true;
    if (std::abs(d2) < 1e-9 && on_segment(s2.p2, s1))
      return true;
    if (std::abs(d3) < 1e-9 && on_segment(s1.p1, s2))
      return true;
    if (std::abs(d4) < 1e-9 && on_segment(s1.p2, s2))
      return true;

    return false;
  }
};

// 33.2 判断点是否在多边形内
class PointInPolygon {
public:
  // 射线法判断点是否在多边形内
  static bool is_point_in_polygon(const Point &p,
                                  const std::vector<Point> &polygon) {
    if (polygon.size() < 3)
      return false;

    bool inside = false;
    int n = polygon.size();

    for (int i = 0, j = n - 1; i < n; j = i++) {
      if (((polygon[i].y > p.y) != (polygon[j].y > p.y)) &&
          (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) /
                         (polygon[j].y - polygon[i].y) +
                     polygon[i].x)) {
        inside = !inside;
      }
    }

    return inside;
  }

  // 改进的射线法（处理边界情况）
  static bool is_point_in_polygon_robust(const Point &p,
                                         const std::vector<Point> &polygon) {
    if (polygon.size() < 3)
      return false;

    // 检查点是否在多边形的顶点上
    for (const auto &vertex : polygon) {
      if (p == vertex)
        return true;
    }

    // 检查点是否在多边形的边上
    for (int i = 0; i < polygon.size(); i++) {
      int j = (i + 1) % polygon.size();
      if (SegmentProperties::on_segment(p, Segment(polygon[i], polygon[j]))) {
        return true;
      }
    }

    // 使用射线法
    return is_point_in_polygon(p, polygon);
  }
};

// 33.3 凸包算法
class ConvexHull {
public:
  // Graham扫描算法
  static std::vector<Point> graham_scan(std::vector<Point> points) {
    if (points.size() < 3)
      return points;

    // 找到最左下角的点
    int min_idx = 0;
    for (int i = 1; i < points.size(); i++) {
      if (points[i].y < points[min_idx].y ||
          (points[i].y == points[min_idx].y &&
           points[i].x < points[min_idx].x)) {
        min_idx = i;
      }
    }
    std::swap(points[0], points[min_idx]);

    // 按极角排序
    Point p0 = points[0];
    std::sort(points.begin() + 1, points.end(),
              [&p0](const Point &a, const Point &b) {
                double cross = SegmentProperties::cross_product(p0, a, b);
                if (std::abs(cross) < 1e-9) {
                  return p0.distance(a) < p0.distance(b);
                }
                return cross > 0;
              });

    // 构建凸包
    std::vector<Point> hull;
    hull.push_back(points[0]);
    hull.push_back(points[1]);

    for (int i = 2; i < points.size(); i++) {
      while (hull.size() > 1 && SegmentProperties::cross_product(
                                    hull[hull.size() - 2],
                                    hull[hull.size() - 1], points[i]) <= 0) {
        hull.pop_back();
      }
      hull.push_back(points[i]);
    }

    return hull;
  }

  // Jarvis步进算法（礼品包装算法）
  static std::vector<Point> jarvis_march(const std::vector<Point> &points) {
    if (points.size() < 3)
      return points;

    // 找到最左边的点
    int leftmost = 0;
    for (int i = 1; i < points.size(); i++) {
      if (points[i].x < points[leftmost].x ||
          (points[i].x == points[leftmost].x &&
           points[i].y < points[leftmost].y)) {
        leftmost = i;
      }
    }

    std::vector<Point> hull;
    int p = leftmost, q;

    do {
      hull.push_back(points[p]);
      q = (p + 1) % points.size();

      for (int i = 0; i < points.size(); i++) {
        if (SegmentProperties::cross_product(points[p], points[i], points[q]) >
            0) {
          q = i;
        }
      }

      p = q;
    } while (p != leftmost);

    return hull;
  }
};

// 33.4 寻找最近点对
class ClosestPair {
public:
  // 分治法寻找最近点对
  static std::pair<Point, Point> closest_pair(std::vector<Point> points) {
    if (points.size() < 2) {
      throw std::invalid_argument("至少需要两个点");
    }

    // 按x坐标排序
    std::sort(points.begin(), points.end(),
              [](const Point &a, const Point &b) { return a.x < b.x; });

    return closest_pair_recursive(points, 0, points.size() - 1);
  }

private:
  static std::pair<Point, Point>
  closest_pair_recursive(const std::vector<Point> &points, int left,
                         int right) {
    if (right - left + 1 <= 3) {
      // 暴力求解
      return brute_force_closest(points, left, right);
    }

    int mid = left + (right - left) / 2;
    auto left_pair = closest_pair_recursive(points, left, mid);
    auto right_pair = closest_pair_recursive(points, mid + 1, right);

    double left_dist = left_pair.first.distance(left_pair.second);
    double right_dist = right_pair.first.distance(right_pair.second);

    double min_dist = std::min(left_dist, right_dist);
    auto min_pair = (left_dist < right_dist) ? left_pair : right_pair;

    // 检查跨越中线的点对
    double mid_x = points[mid].x;
    std::vector<Point> strip;
    for (int i = left; i <= right; i++) {
      if (std::abs(points[i].x - mid_x) < min_dist) {
        strip.push_back(points[i]);
      }
    }

    // 按y坐标排序
    std::sort(strip.begin(), strip.end(),
              [](const Point &a, const Point &b) { return a.y < b.y; });

    for (int i = 0; i < strip.size(); i++) {
      for (int j = i + 1;
           j < strip.size() && (strip[j].y - strip[i].y) < min_dist; j++) {
        double dist = strip[i].distance(strip[j]);
        if (dist < min_dist) {
          min_dist = dist;
          min_pair = {strip[i], strip[j]};
        }
      }
    }

    return min_pair;
  }

  static std::pair<Point, Point>
  brute_force_closest(const std::vector<Point> &points, int left, int right) {
    double min_dist = std::numeric_limits<double>::max();
    std::pair<Point, Point> closest_pair;

    for (int i = left; i <= right; i++) {
      for (int j = i + 1; j <= right; j++) {
        double dist = points[i].distance(points[j]);
        if (dist < min_dist) {
          min_dist = dist;
          closest_pair = {points[i], points[j]};
        }
      }
    }

    return closest_pair;
  }
};

// 33.5 线段相交检测
class SegmentIntersection {
public:
  // 扫描线算法检测线段相交
  static std::vector<std::pair<Segment, Segment>>
  find_intersections(const std::vector<Segment> &segments) {
    std::vector<std::pair<Segment, Segment>> intersections;

    for (int i = 0; i < segments.size(); i++) {
      for (int j = i + 1; j < segments.size(); j++) {
        if (SegmentProperties::segments_intersect(segments[i], segments[j])) {
          intersections.push_back({segments[i], segments[j]});
        }
      }
    }

    return intersections;
  }
};

} // namespace algorithms

#endif // COMPUTATIONAL_GEOMETRY_H
