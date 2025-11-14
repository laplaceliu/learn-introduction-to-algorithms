#include "computational_geometry.h"
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using namespace algorithms;

// 测试线段性质
void test_segment_properties() {
  std::cout << "=== 测试线段性质 (33.1) ===" << std::endl;

  // 测试叉积
  Point p0(0, 0), p1(1, 0), p2(0, 1);
  double cross = SegmentProperties::cross_product(p0, p1, p2);
  std::cout << "叉积测试: (" << p0.x << "," << p0.y << ") -> (" << p1.x << ","
            << p1.y << ") -> (" << p2.x << "," << p2.y << "): " << cross
            << " (应为正数)" << std::endl;

  // 测试点在线段上
  Point p3(0.5, 0.5);
  Segment s1(Point(0, 0), Point(1, 1));
  bool on_segment = SegmentProperties::on_segment(p3, s1);
  std::cout << "点(" << p3.x << "," << p3.y
            << ")在线段(0,0)-(1,1)上: " << (on_segment ? "是" : "否")
            << std::endl;

  // 测试线段相交
  Segment s2(Point(0, 1), Point(1, 0));
  Segment s3(Point(0, 0), Point(1, 1));
  bool intersect = SegmentProperties::segments_intersect(s2, s3);
  std::cout << "线段(0,1)-(1,0)与线段(0,0)-(1,1)相交: "
            << (intersect ? "是" : "否") << std::endl;

  // 测试不相交的线段
  Segment s4(Point(0, 0), Point(1, 0));
  Segment s5(Point(0, 2), Point(1, 2));
  bool not_intersect = SegmentProperties::segments_intersect(s4, s5);
  std::cout << "线段(0,0)-(1,0)与线段(0,2)-(1,2)相交: "
            << (not_intersect ? "是" : "否") << std::endl;

  std::cout << std::endl;
}

// 测试点是否在多边形内
void test_point_in_polygon() {
  std::cout << "=== 测试点是否在多边形内 (33.2) ===" << std::endl;

  // 创建一个正方形多边形
  std::vector<Point> square = {Point(0, 0), Point(4, 0), Point(4, 4),
                               Point(0, 4)};

  // 测试多边形内的点
  Point inside(2, 2);
  bool result1 = PointInPolygon::is_point_in_polygon_robust(inside, square);
  std::cout << "点(" << inside.x << "," << inside.y
            << ")在正方形内: " << (result1 ? "是" : "否") << std::endl;

  // 测试多边形外的点
  Point outside(5, 5);
  bool result2 = PointInPolygon::is_point_in_polygon_robust(outside, square);
  std::cout << "点(" << outside.x << "," << outside.y
            << ")在正方形内: " << (result2 ? "是" : "否") << std::endl;

  // 测试边界上的点
  Point on_edge(2, 0);
  bool result3 = PointInPolygon::is_point_in_polygon_robust(on_edge, square);
  std::cout << "点(" << on_edge.x << "," << on_edge.y
            << ")在正方形内: " << (result3 ? "是" : "否") << std::endl;

  // 测试顶点上的点
  Point on_vertex(0, 0);
  bool result4 = PointInPolygon::is_point_in_polygon_robust(on_vertex, square);
  std::cout << "点(" << on_vertex.x << "," << on_vertex.y
            << ")在正方形内: " << (result4 ? "是" : "否") << std::endl;

  // 创建一个复杂多边形
  std::vector<Point> complex_polygon = {Point(0, 0), Point(3, 0), Point(3, 2),
                                        Point(1, 2), Point(1, 4), Point(3, 4),
                                        Point(3, 6), Point(0, 6)};

  Point test_point(2, 3);
  bool result5 =
      PointInPolygon::is_point_in_polygon_robust(test_point, complex_polygon);
  std::cout << "点(" << test_point.x << "," << test_point.y
            << ")在复杂多边形内: " << (result5 ? "是" : "否") << std::endl;

  std::cout << std::endl;
}

// 测试凸包算法
void test_convex_hull() {
  std::cout << "=== 测试凸包算法 (33.3) ===" << std::endl;

  // 测试点集
  std::vector<Point> points = {
      Point(0, 0),  Point(1, 1),  Point(2, 2),  Point(3, 1), Point(4, 0),
      Point(3, -1), Point(2, -2), Point(1, -1), Point(2, 0) // 内部点
  };

  std::cout << "原始点集: ";
  for (const auto &p : points) {
    std::cout << "(" << p.x << "," << p.y << ") ";
  }
  std::cout << std::endl;

  // Graham扫描算法
  auto start = std::chrono::high_resolution_clock::now();
  auto graham_hull = ConvexHull::graham_scan(points);
  auto end = std::chrono::high_resolution_clock::now();
  auto graham_time =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();

  std::cout << "Graham扫描凸包点: ";
  for (const auto &p : graham_hull) {
    std::cout << "(" << p.x << "," << p.y << ") ";
  }
  std::cout << std::endl;
  std::cout << "Graham扫描时间: " << graham_time << " 微秒" << std::endl;

  // Jarvis步进算法
  start = std::chrono::high_resolution_clock::now();
  auto jarvis_hull = ConvexHull::jarvis_march(points);
  end = std::chrono::high_resolution_clock::now();
  auto jarvis_time =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();

  std::cout << "Jarvis步进凸包点: ";
  for (const auto &p : jarvis_hull) {
    std::cout << "(" << p.x << "," << p.y << ") ";
  }
  std::cout << std::endl;
  std::cout << "Jarvis步进时间: " << jarvis_time << " 微秒" << std::endl;

  // 验证两个算法结果是否一致
  bool same_hull = (graham_hull.size() == jarvis_hull.size());
  if (same_hull) {
    for (int i = 0; i < graham_hull.size(); i++) {
      if (graham_hull[i] != jarvis_hull[i]) {
        same_hull = false;
        break;
      }
    }
  }

  std::cout << "两个算法结果一致: " << (same_hull ? "是" : "否") << std::endl;
  std::cout << std::endl;
}

// 测试最近点对算法
void test_closest_pair() {
  std::cout << "=== 测试最近点对算法 (33.4) ===" << std::endl;

  // 测试点集1：明显的最远点对
  std::vector<Point> points1 = {Point(0, 0), Point(1, 1), Point(2, 2),
                                Point(3, 3)};

  auto closest1 = ClosestPair::closest_pair(points1);
  double dist1 = closest1.first.distance(closest1.second);

  std::cout << "点集1: ";
  for (const auto &p : points1) {
    std::cout << "(" << p.x << "," << p.y << ") ";
  }
  std::cout << std::endl;
  std::cout << "最近点对: (" << closest1.first.x << "," << closest1.first.y
            << ") - (" << closest1.second.x << "," << closest1.second.y << ")"
            << std::endl;
  std::cout << "距离: " << dist1 << std::endl;

  // 测试点集2：随机点
  std::vector<Point> points2 = {Point(1, 2), Point(3, 4), Point(5, 1),
                                Point(7, 3), Point(2, 5), Point(4, 6),
                                Point(6, 2), Point(8, 4)};

  auto closest2 = ClosestPair::closest_pair(points2);
  double dist2 = closest2.first.distance(closest2.second);

  std::cout << "点集2: ";
  for (const auto &p : points2) {
    std::cout << "(" << p.x << "," << p.y << ") ";
  }
  std::cout << std::endl;
  std::cout << "最近点对: (" << closest2.first.x << "," << closest2.first.y
            << ") - (" << closest2.second.x << "," << closest2.second.y << ")"
            << std::endl;
  std::cout << "距离: " << dist2 << std::endl;

  // 暴力验证
  double min_dist = std::numeric_limits<double>::max();
  Point p1, p2;
  for (int i = 0; i < points2.size(); i++) {
    for (int j = i + 1; j < points2.size(); j++) {
      double dist = points2[i].distance(points2[j]);
      if (dist < min_dist) {
        min_dist = dist;
        p1 = points2[i];
        p2 = points2[j];
      }
    }
  }

  std::cout << "暴力验证最近点对: (" << p1.x << "," << p1.y << ") - (" << p2.x
            << "," << p2.y << ")" << std::endl;
  std::cout << "暴力验证距离: " << min_dist << std::endl;
  std::cout << "算法正确性: "
            << (std::abs(dist2 - min_dist) < 1e-9 ? "通过" : "失败")
            << std::endl;

  std::cout << std::endl;
}

// 测试线段相交检测
void test_segment_intersection() {
  std::cout << "=== 测试线段相交检测 (33.5) ===" << std::endl;

  // 创建线段集合
  std::vector<Segment> segments = {
      Segment(Point(0, 0), Point(4, 4)), // s1
      Segment(Point(0, 4), Point(4, 0)), // s2 (与s1相交)
      Segment(Point(1, 1), Point(3, 3)), // s3 (与s1重叠)
      Segment(Point(5, 5), Point(7, 7)), // s4 (不相交)
      Segment(Point(0, 2), Point(4, 2)), // s5 (与s1相交)
      Segment(Point(2, 0), Point(2, 4))  // s6 (与s1相交)
  };

  auto intersections = SegmentIntersection::find_intersections(segments);

  std::cout << "线段集合:" << std::endl;
  for (int i = 0; i < segments.size(); i++) {
    std::cout << "  s" << i + 1 << ": (" << segments[i].p1.x << ","
              << segments[i].p1.y << ")-(" << segments[i].p2.x << ","
              << segments[i].p2.y << ")" << std::endl;
  }

  std::cout << "找到 " << intersections.size() << " 对相交线段:" << std::endl;
  for (const auto &pair : intersections) {
    std::cout << "  线段1: (" << pair.first.p1.x << "," << pair.first.p1.y
              << ")-(" << pair.first.p2.x << "," << pair.first.p2.y << ")"
              << std::endl;
    std::cout << "  线段2: (" << pair.second.p1.x << "," << pair.second.p1.y
              << ")-(" << pair.second.p2.x << "," << pair.second.p2.y << ")"
              << std::endl;
  }

  std::cout << std::endl;
}

// 性能测试
void performance_test() {
  std::cout << "=== 性能测试 ===" << std::endl;

  // 生成随机点集
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 100.0);

  std::vector<Point> large_points;
  for (int i = 0; i < 1000; i++) {
    large_points.push_back(Point(dis(gen), dis(gen)));
  }

  // 测试凸包算法性能
  auto start = std::chrono::high_resolution_clock::now();
  auto hull = ConvexHull::graham_scan(large_points);
  auto end = std::chrono::high_resolution_clock::now();
  auto graham_time =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();

  std::cout << "1000个点的Graham扫描凸包时间: " << graham_time << " 微秒"
            << std::endl;
  std::cout << "凸包点数: " << hull.size() << std::endl;

  // 测试最近点对算法性能
  start = std::chrono::high_resolution_clock::now();
  auto closest = ClosestPair::closest_pair(large_points);
  end = std::chrono::high_resolution_clock::now();
  auto closest_time =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();

  std::cout << "1000个点的最近点对查找时间: " << closest_time << " 微秒"
            << std::endl;
  std::cout << "最近点对距离: " << closest.first.distance(closest.second)
            << std::endl;

  std::cout << std::endl;
}

int main() {
  std::cout << "第33章 计算几何学算法测试" << std::endl;
  std::cout << "==========================" << std::endl;

  try {
    test_segment_properties();
    test_point_in_polygon();
    test_convex_hull();
    test_closest_pair();
    test_segment_intersection();
    performance_test();

    std::cout << "所有测试完成!" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "测试失败: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
