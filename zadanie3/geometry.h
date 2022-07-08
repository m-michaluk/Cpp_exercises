/*
Autorzy:
Monika Michaluk
Ignacy Makowski
*/
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cstdint>
#include <initializer_list>
#include <vector>

using std::vector;

class Vector;

class Position;

class Rectangle;

class Rectangles;

class Vector {
public:
    Vector() = delete;

    ~Vector() = default;

    Vector(int a, int b) : x_co(a), y_co(b) {}

    Vector(const Vector &v) = default;

    Vector(Vector &&v) = default;

    Vector &operator=(const Vector &v) = default;

    Vector &operator=(Vector &&v) = default;

    explicit Vector(const Position &p);

    explicit operator Position() const;

    Vector &operator+=(const Vector &v);

    bool operator==(const Vector &v) const;

    int32_t x() const { return x_co; };

    int32_t y() const { return y_co; };

    Vector reflection() const;

private:
    int32_t x_co;
    int32_t y_co;
};

class Position {
public:
    Position() = delete;

    ~Position() = default;

    Position(int32_t a, int32_t b) : x_co(a), y_co(b) {};

    Position(const Position &p) = default;

    Position(Position &&p) = default;

    Position &operator=(const Position &p) = default;

    Position &operator=(Position &&p) = default;

    explicit Position(const Vector &v);

    explicit operator Vector() const;

    bool operator==(const Position &p) const;

    Position &operator+=(const Vector &v);

    Position reflection() const;

    int32_t x() const { return x_co; };

    int32_t y() const { return y_co; };

    const static Position &origin();

private:
    int32_t x_co;
    int32_t y_co;
};

class Rectangle {
public:
    Rectangle() = delete;

    ~Rectangle() = default;

    Rectangle(const Rectangle &r) = default;

    Rectangle(Rectangle &&r) = default;

    Rectangle &operator=(const Rectangle &r) = default;

    Rectangle &operator=(Rectangle &&r) = default;

    Rectangle(int32_t width, int32_t height, const Position &p = {0, 0});

    bool operator==(const Rectangle &v) const;

    Rectangle &operator+=(const Vector &v);

    Rectangle reflection() const;

    int32_t width() const { return vec.x(); }

    int32_t height() const { return vec.y(); }

    int32_t area() const { return width() * height(); }

    const Position &pos() const { return p; }

private:
    Position p;
    Vector vec;
};

class Rectangles {
public:
    ~Rectangles() = default;

    Rectangles() = default;

    Rectangles(const Rectangles &r) = default;

    Rectangles(Rectangles &&r) = default;

    Rectangles &operator=(const Rectangles &r) = default;

    Rectangles &operator=(Rectangles &&r) = default;

    Rectangles(std::initializer_list<Rectangle> recs) : recs(recs) {}

    Rectangles &operator+=(const Vector &v);

    bool operator==(const Rectangles &v) const { return recs == v.recs; }

    const Rectangle &operator[](size_t i) const { return recs.at(i); }

    Rectangle &operator[](size_t i) { return recs.at(i); }

    size_t size() const { return recs.size(); }

private:
    vector<Rectangle> recs;
};

/* Dodatkowe operacje */

Rectangle merge_horizontally(const Rectangle &rect1, const Rectangle &rect2);

Rectangle merge_vertically(const Rectangle &rect1, const Rectangle &rect2);

Rectangle merge_all(const Rectangles &rectangles);

Position operator+(const Position &p, const Vector &v);

Position operator+(const Vector &v, const Position &p);

Vector operator+(const Vector &v1, const Vector &v2);

Rectangles operator+(Rectangles recs, const Vector &v);

Rectangles operator+(const Vector &v, Rectangles recs);

Rectangle operator+(const Rectangle &r, const Vector &v);

Rectangle operator+(const Vector &v, const Rectangle &r);

#endif /* GEOMETRY_H */
