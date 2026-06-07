class Room {
public:
  int x, y, width, height;
  const int MAX_MONSTERS;
  Room(int width, int height, int x, int y, int MAX_MONSTERS)
      : x(x), y(y), width(width), height(height), MAX_MONSTERS(MAX_MONSTERS) {}
  int center_x() const { return x + width / 2; }
  int center_y() const { return y + height / 2; }
  static Room generate(int node_width, int node_height, int node_x, int node_y);
};