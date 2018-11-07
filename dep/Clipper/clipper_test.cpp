#include <iostream>
#include <clipper.h>
#include <string>

using namespace std;
using namespace clipperlib;

void PathFromStr(const string &s, Path &path);

int main() { 

    ClipType clip_type = ctIntersection;
    FillRule fill_rule = frNonZero;
    Paths subject, clip, solution;
    
    subject.resize(1);
    clip.resize(1);

    PathFromStr("643,281 791,365 1094,295 1064,155 618,365 54,153 1031,232 669,639 413,177 770,522", subject[0]);
    PathFromStr("200,200, 400,200 400,400, 200,400", clip[0]);

    Clipper clipper;
    clipper.AddPaths(subject, ptSubject);
    clipper.AddPaths(clip, ptClip);
    clipper.Execute(clip_type, solution, fill_rule);
    for(auto&& p : subject ) cout << p;
    for(auto&& p : clip    ) cout << p;
    for(auto&& p : solution) cout << p;
    // OffsetPaths(solution, solution, 50, kSquare, kPolygon);

}

void PathFromStr(const string &s, Path &path) {
  path.resize(0);
  size_t len = s.size(), i = 0, j;
  if (len == 0) return;
  while (i < len) {
    int64_t y = 0, x = 0;
    bool isNeg;
    while ((int)s[i] < 33 && i < len) i++;
    if (i >= len) break;
    //get x ...
    isNeg = (int)s[i] == 45;
    if (isNeg) i++;
    if (i >= len || (int)s[i] < 48 || (int)s[i] > 57) break;
    j = i;
    while (j < len && (int)s[j] > 47 && (int)s[j] < 58) {
      x = x * 10 + ((int)s[j] - 48);
      j++;
    }
    if (j == i) break;
    if (isNeg) x = -x;
    //skip space or comma between x & y ...
    i = j;
    while (i < len && ((int)s[i] == 32 || (int)s[i] == 44)) i++;
    //get y ...
    if (i >= len) break;
    isNeg = (int)s[i] == 45;
    if (isNeg) i++;
    if (i >= len || (int)s[i] < 48 || (int)s[i] > 57) break;
    j = i;
    while (j < len && (int)s[j] > 47 && (int)s[j] < 58) {
      y = y * 10 + ((int)s[j] - 48);
      j++;
    }
    if (j == i) break;
    if (isNeg) y = -y;
    path.push_back(Point64(x, y));
    //skip trailing space, comma ...
    i = j;
    while (i < len && ((int)s[i] == 32 || (int)s[i] == 44)) i++;
  }
}
