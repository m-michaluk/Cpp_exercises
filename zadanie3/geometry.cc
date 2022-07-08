#include "geometry.h"
#include <cassert>

/**************  Implementacja funkcji z klasy Vector **************/
Vector &Vector::operator+=(const Vector &v) {
    x_co += v.x_co;
    y_co += v.y_co;
    return *this;
}

bool Vector::operator==(const Vector &v) const {
    return x_co == v.x_co && y_co == v.y_co;
}

Vector Vector::reflection() const {
    return Vector(y_co, x_co);
}

Vector::Vector(const Position &p) : x_co(p.x()), y_co(p.y()) {}

Vector::operator Position() const {
    return Position(x_co, y_co);
}
/**/


/************** Implementacja funkcji z klasy Position **************/
Position &Position::operator+=(const Vector &v) {
    x_co += v.x();
    y_co += v.y();
    return (*this);
}

bool Position::operator==(const Position &p) const {
    return x_co == p.x_co && y_co == p.y_co;
}

Position::Position(const Vector &v) : x_co(v.x()), y_co(v.y()) {}

Position::operator Vector() const { // operator rzutowania Position na Vector
    return Vector(x_co, y_co);
}

Position Position::reflection() const {
    return Position(y(), x());
}

const Position &Position::origin() {
    static const Position ORIGIN(0, 0);
    return ORIGIN;
}
/**/


/************** Implementacja funkcji z klasy Rectangle **************/
Rectangle::Rectangle(int32_t width, int32_t height, const Position &pos)
        : p(pos), vec(width, height) {
    assert(width > 0 && height > 0);
}

Rectangle &Rectangle::operator+=(const Vector &r) {
    p += r;
    return *this;
}

bool Rectangle::operator==(const Rectangle &r) const {
    return p == r.p && vec == r.vec;
}

Rectangle Rectangle::reflection() const {
    return Rectangle(height(), width(), p.reflection());
}
/**/


/************** Implementacja funkcji z klasy Rectangles **************/
Rectangles &Rectangles::operator+=(const Vector &v) {
    size_t length = recs.size();
    for (size_t i = 0; i < length; i++)
        recs[i] += v;
    return *this;
}
/**/

/* Funkcje pomocnicze do mergowania prostokątów */
namespace {
    /* Sprawdza czy da się zmergować wertykalnie rect1 i rect2 */
    bool
    possible_vertical_merge(const Rectangle &rect1, const Rectangle &rect2) {
        if (rect1.height() == rect2.height() &&
            rect1.pos().y() == rect2.pos().y() &&
            rect1.pos().x() + rect1.width() == rect2.pos().x())
            return true;
        return false;
    }

    /* Sprawdza czy da się zmergować horyzontalnie rect1 i rect2 */
    bool
    possible_horizontal_merge(const Rectangle &rect1, const Rectangle &rect2) {
        if (rect1.width() == rect2.width() &&
            rect1.pos().x() == rect2.pos().x() &&
            rect1.pos().y() + rect1.height() == rect2.pos().y())
            return true;
        return false;
    }

    /* Zwraca nowy prostokąt, który jest wynikiem operacji mergowania
     * wzdłuż osi X rect1 i rect2 -- nie sprawdza poprawności argumentów,
     * zakładamy, że są poprawne. */
    Rectangle
    merge_horizontally_helper(const Rectangle &rect1, const Rectangle &rect2) {
        return Rectangle(rect1.width(), rect1.height() + rect2.height(),
                         rect1.pos());
    }

    /* Zwraca nowy prostokąt, który jest wynikiem operacji mergowania
     * wzdłuż osi Y rect1 i rect2 -- nie sprawdza poprawności argumentów,
     * zakładamy, że są poprawne. */
    Rectangle
    merge_vertically_helper(const Rectangle &rect1, const Rectangle &rect2) {
        return Rectangle(rect1.width() + rect2.width(), rect1.height(),
                         rect1.pos());
    }
}

/* Dodatkowe operacje mergujące prostokąty*/

Rectangle merge_horizontally(const Rectangle &rect1, const Rectangle &rect2) {
    assert(possible_horizontal_merge(rect1, rect2));
    return merge_horizontally_helper(rect1, rect2);
}

Rectangle merge_vertically(const Rectangle &rect1, const Rectangle &rect2) {
    assert(possible_vertical_merge(rect1, rect2));
    return merge_vertically_helper(rect1, rect2);
}

Rectangle merge_all(const Rectangles &recs) {
    size_t length = recs.size();
    assert(length > 0);
    Rectangle result = recs[0];
    for (size_t i = 1; i < length; i++) {
        if (possible_vertical_merge(result, recs[i]))
            result = merge_vertically_helper(result, recs[i]);
        else if (possible_horizontal_merge(result, recs[i]))
            result = merge_horizontally_helper(result, recs[i]);
        else
            assert(false);
    }
    return result;
}

/*
 * Implementacja operacji:
 *   Position = Position + Vector
 *   Position = Vector + Position
 *   Vector = Vector + Vector
 *   Rectangle = Rectangle + Vector
 *   Rectangle = Vector + Rectangle
 *   Rectangles = Rectangles + Vector
 *   Rectangles = Vector + Rectangles
 */

Position operator+(const Position &p, const Vector &v) {
    Position result(p);
    result += v;
    return result;
}

Position operator+(const Vector &v, const Position &p) {
    return p + v;
}

Vector operator+(const Vector &v1, const Vector &v2) {
    Vector result(v1);
    result += v2;
    return result;
}

Rectangle operator+(const Rectangle &r, const Vector &v) {
    Rectangle result(r);
    result += v;
    return result;
}

Rectangle operator+(const Vector &v, const Rectangle &r) {
    return r + v;
}

Rectangles operator+(Rectangles recs, const Vector &v) {
    recs += v;
    return recs;
}

Rectangles operator+(const Vector &v, Rectangles recs) {
    return std::move(recs) + v;
}
