#ifndef FUNCTION_MAXIMA_H
#define FUNCTION_MAXIMA_H

/**
Języki i Narzędzia Programowania I, Zadanie 4
Autorzy:
Monika Michaluk
Antoni Koszowski
**/

#include <set>
#include <map>
#include <utility>
#include <vector>
#include <memory>

class InvalidArg : public std::exception {
public:
    const char *what() const noexcept override {
        return "invalid argument value";
    }
};

template<typename A, typename V>
class FunctionMaxima {
public:
    class point;

    using size_type = size_t;
    using point_type = point;
private:
    struct max_cmp;
    struct arg_cmp;
    using multiset_t = std::multiset<point_type, arg_cmp>;
    using max_multiset_t = std::multiset<point_type, max_cmp>;
public:
    using iterator = typename multiset_t::iterator;
    using mx_iterator = typename max_multiset_t::iterator;

    FunctionMaxima() = default;

    ~FunctionMaxima() noexcept = default;

    FunctionMaxima(FunctionMaxima<A, V> const &fm);

    FunctionMaxima &operator=(FunctionMaxima<A, V> fm) noexcept;

    /* Zwraca wartość w punkcie [a], rzuca wyjątek InvalidArg, jeśli [a] nie
     * należy do dziedziny funkcji. Złożoność O(log n). */
    V const &value_at(A const &a) const;

    /* Zmienia funkcję tak, żeby zachodziło f(a) = v. Jeśli [a] nie należy do
     * obecnej dziedziny funkcji, jest do niej dodawany. Złożoność O(log n). */
    void set_value(A const &a, V const &v);

    /* Usuwa a z dziedziny funkcji. Jeśli [a] nie należało do dziedziny funkcji,
     * nie dzieje się nic. Złożoność O(log n). */
    void erase(A const &a);

    /* Funkcja zwracająca rozmiar dziedziny. */
    size_type size() const { return values.size(); }

    /* Iterator wskazujący na pierwszy punkt. */
    iterator begin() const { return values.begin(); }

    /* Iterator wskazujący za ostatni punkt. */
    iterator end() const { return values.end(); }

    /* Iterator, który wskazuje na punkt funkcji o argumencie [a] lub end(),
     * jeśli takiego argumentu nie ma w dziedzinie funkcji. Złożoność O(logn).*/
    iterator find(A const &a) const;

    /* Iterator wskazujący na pierwsze lokalne maksimum. */
    mx_iterator mx_begin() const { return maxima.begin(); }

    /* Iterator wskazujący za ostatnie lokalne maksimum. */
    mx_iterator mx_end() const { return maxima.end(); }

private:
    multiset_t values; // struktura na punkty (x, f(x))
    max_multiset_t maxima; // struktura na maksima lokalne funkcji
    template<typename set_t>
    class Guard;

    struct it_adjacent;

    using A_ptr_t = std::shared_ptr<A>;
    using V_ptr_t = std::shared_ptr<V>;
    using vector_t = std::vector<mx_iterator>;

    void max_check(it_adjacent &i, Guard<max_multiset_t> &g,
                   vector_t &to_erase);

    bool is_maximum(it_adjacent &i);

    void init_info(iterator it, it_adjacent &info);

    void init_adjacent(iterator it, it_adjacent &mid,
                       it_adjacent &left,
                       it_adjacent &right);
};

/* Definicja klasy Guard */
template<typename A, typename V>
template<typename set_t>
class FunctionMaxima<A, V>::Guard {
public:
    using set_iterator = typename set_t::iterator;

    Guard() : rollback(false) {}

    ~Guard() noexcept;

    Guard &operator=(const Guard &) = delete;

    Guard(const Guard &) = delete;

    Guard(Guard &&g) = delete;

    Guard &operator=(Guard &&g) = delete;

    void drop_rollback() noexcept { rollback = false; }

    set_iterator insert(set_t *set, const point_type &p);

private:
    set_t *ptr_set;
    set_iterator it_new;
    bool rollback;
};

template<typename A, typename V>
template<typename set_t>
typename FunctionMaxima<A, V>::template Guard<set_t>::set_iterator
FunctionMaxima<A, V>::Guard<set_t>::insert(set_t *set, const point_type &p) {
    ptr_set = set;
    it_new = set->insert(p);
    rollback = true;
    return it_new;
}

template<typename A, typename V>
template<typename set_t>
FunctionMaxima<A, V>::Guard<set_t>::~Guard() noexcept {
    if (rollback)
        ptr_set->erase(it_new);
}
/* Koniec definicji klasy Guard */

/* Definicja klasy point */
template<typename A, typename V>
class FunctionMaxima<A, V>::point {
public:
    point() = delete;

    ~point() noexcept = default;

    point(const point_type &p) = default;

    point_type &operator=(const point_type &p) = default;

    point(point_type &&p) = default;

    point_type &operator=(point_type &&p) = default;

    /* Zwraca argument funkcji. */
    A const &arg() const { return arg_ == nullptr ? *temp : *arg_; }

    /* Zwraca wartość funkcji w tym punkcie. */
    V const &value() const { return *value_; }

private:
    friend class FunctionMaxima;

    point(A_ptr_t arg, V_ptr_t value) : arg_(arg), value_(value) {}

    /* Konstruktor przyjmujący oryginalny obiekt użytkownika i zapisujący 
     * wskaźnik do niego w atrybucie [temp]. Jest wykorzystywany do tworzenia
     * tymczasowego punktu przy wyszukiwaniu wartości funkcji dla zadanego
     * argumentu, bez konieczności kopiowania [arg].
     * */
    explicit point(const A *arg) : temp(arg), arg_(nullptr) {}

    const A *temp;
    A_ptr_t arg_;
    V_ptr_t value_;
};
/* Koniec definicji klasy point */


/* definicja publicznych składowych z klasy FunctionMaxima */

template<typename A, typename V>
FunctionMaxima<A, V>::FunctionMaxima(FunctionMaxima<A, V> const &fm)
        : values(fm.values), maxima(fm.maxima) {}

template<typename A, typename V>
FunctionMaxima<A, V> &
FunctionMaxima<A, V>::operator=(FunctionMaxima<A, V> fm) noexcept {
    values.swap(fm.values);
    maxima.swap(fm.maxima);
    return *this;
}

template<typename A, typename V>
V const &FunctionMaxima<A, V>::value_at(A const &a) const {
    iterator it = find(a);
    if (it == end())
        throw InvalidArg();
    return it->value();
}

template<typename A, typename V>
void FunctionMaxima<A, V>::set_value(const A &a, const V &v) {
    auto it = find(a);
    if (it != end() && !(it->arg() < a) && !(a < it->arg()) &&
        !(it->value() < v) && !(v < it->value()))
        return;

    A_ptr_t a_ptr = std::make_shared<A>(a);
    V_ptr_t v_ptr = std::make_shared<V>(v);
    point_type p(a_ptr, v_ptr);

    // Przechowuje iteratory do maksimów w zbiorze maxima, które trzeba usunąć.
    vector_t to_erase;
    Guard<multiset_t> vals_guard;
    bool found = true;
    if (it == end()) {
        it = vals_guard.insert(&values, p);
        found = false;
    } else { // Jeśli stary element był maksimum, to dodaję go do usunięcia potem.
        auto it_max = maxima.find(*it);
        if (it_max != maxima.end()) {
            to_erase.push_back(it_max);
        }
    }
    it_adjacent mid, left, right;
    init_adjacent(it, mid, left, right);

    auto it_new = it;
    if (found) {
        it_new = vals_guard.insert(&values, p);
        // Trzeba poprawić wartości w strukturach - środkowym elementem powinien
        // być nowo dodany element.
        mid.mid = it_new;
        left.right = it_new;
        right.left = it_new;
    }

    Guard<max_multiset_t> mid_guard, left_guard, right_guard;
    max_check(mid, mid_guard, to_erase);
    if (mid.has_left) max_check(left, left_guard, to_erase);
    if (mid.has_right) max_check(right, right_guard, to_erase);

    // Metoda erase(iterator) dla setu jest no-throw - żadna z poniższych
    // operacji nie rzuca wyjątku, więc nie musimy się martwić o ich odwracanie.
    if (found) values.erase(it);
    for (auto i: to_erase) maxima.erase(i);

    vals_guard.drop_rollback();
    mid_guard.drop_rollback();
    left_guard.drop_rollback();
    right_guard.drop_rollback();
}

template<typename A, typename V>
void FunctionMaxima<A, V>::erase(const A &a) {
    auto it = find(a);
    if (it == end()) return;

    // Przechowuje iteratory do maksimów w zbiorze maxima, które trzeba usunąć.
    vector_t to_erase;
    auto it_max = maxima.find(*it);
    if (it_max != maxima.end()) to_erase.push_back(it_max);

    it_adjacent mid, left, right;
    init_adjacent(it, mid, left, right);

    // Trzeba poprawić wartości sąsiadów w left i right w strukturach przez to,
    // że środkowy element zostanie usunięty.
    right.has_left = mid.has_left;
    left.has_right = mid.has_right;
    if (right.has_left) right.left = mid.left;
    if (left.has_right) left.right = mid.right;

    Guard<max_multiset_t> left_guard, right_guard;
    if (mid.has_left) max_check(left, left_guard, to_erase);
    if (mid.has_right) max_check(right, right_guard, to_erase);

    // Metoda erase(iterator) dla setu jest no-throw - żadna z poniższych
    // operacji nie rzuca wyjątku, więc nie musimy się martwić o ich odwracanie.
    for (auto i: to_erase) maxima.erase(i);
    values.erase(it);
    left_guard.drop_rollback();
    right_guard.drop_rollback();
}

template<typename A, typename V>
typename FunctionMaxima<A, V>::iterator
FunctionMaxima<A, V>::find(const A &a) const {
    return values.find(point_type(&a));
}

/* Prywatne składowe klasy: */

/* Comparator do zbioru maksimów - zbiór jest sortowany malejąco po wartościach,
 * a jeśli wartości są równe, to rosnąco po argumentach */
template<typename A, typename V>
struct FunctionMaxima<A, V>::max_cmp {
    bool operator()(FunctionMaxima::point_type const &p1,
                    FunctionMaxima::point_type const &p2) const {
        if (p2.value() < p1.value()) return true;
        if (!(p1.value() < p2.value()) && p1.arg() < p2.arg()) return true;
        return false;
    }
};

/* Comparator do zbioru punktów funkcji - zbiór jest sortowany rosnąco po
 * argumentach */
template<typename A, typename V>
struct FunctionMaxima<A, V>::arg_cmp {
    bool operator()(FunctionMaxima::point_type const &p1,
                    FunctionMaxima::point_type const &p2) const {
        return p1.arg() < p2.arg();
    }
};

/* Struktura przechowująca informacje o 'sąsiadach' iteratora mid w strukturze
 * przechowującej punkty funkcji (multiset_t values) */
template<typename A, typename V>
struct FunctionMaxima<A, V>::it_adjacent {
    iterator mid;
    iterator left;
    iterator right;
    bool has_left = false;
    bool has_right = false;
};

/* Inicjalizuje wartości struktur mid, left, right, przechowujących informację
 * o sąsiadach odpowiednio: iteratora it, jego lewego sąsiada oraz jego prawego
 * sąsiada. */
template<typename A, typename V>
void FunctionMaxima<A, V>::init_adjacent
        (FunctionMaxima::iterator it,
         FunctionMaxima::it_adjacent &mid,
         FunctionMaxima::it_adjacent &left,
         FunctionMaxima::it_adjacent &right) {
    init_info(it, mid);
    if (mid.has_left) init_info(mid.left, left);
    if (mid.has_right) init_info(mid.right, right);
}

/* Ustawia domyślne (takie, które można aktualnie odczytać ze zbioru values)
 * wartości w strukturze it_adjacent, przechowującej iterator it
 * (wskazujący na 'środkowy' element) i informacje o jego sąsiadach w zbiorze
 * punktów funkcji */
template<typename A, typename V>
void FunctionMaxima<A, V>::init_info(FunctionMaxima::iterator it,
                                     FunctionMaxima::it_adjacent &info) {
    info.mid = it;
    if (it != begin()) {
        info.has_left = true;
        info.left = std::prev(it);
    }
    if (std::next(it) != end()) {
        info.has_right = true;
        info.right = std::next(it);
    }
}

/* W zmiennej i trzymany jest element środkowy i.mid i informacje o jego
 * sąsiadach. Funkcja sprawdza czy ten element jest maksimum lokalnym. Jeśli
 * jest i nie znajduje się w zbiorze maksimum, to jest do niego dodawany za
 * pośrednictwem strażnika. Jeśli nie jest lokalnym maksimum, to sprawdzane
 * jest czy ten element znajdował się wcześniej w zbiorze maksimów - jeśli tak,
 * to jest dodawany do wektora iteratorów do późniejszego usunięcia ze zbioru
 * maksimów. */
template<typename A, typename V>
void FunctionMaxima<A, V>::max_check(FunctionMaxima::it_adjacent &i,
                                     FunctionMaxima::Guard<max_multiset_t> &g,
                                     FunctionMaxima::vector_t &to_erase) {
    if (is_maximum(i)) {
        if (maxima.find(*(i.mid)) == maxima.end())
            g.insert(&maxima, *(i.mid));
    } else {
        auto it_max = maxima.find(*(i.mid));
        if (it_max != maxima.end()) {
            to_erase.push_back(it_max);
        }
    }
}

/* Funkcja sprawdzająca warunek na bycie maksimum lokalnym.*/
template<typename A, typename V>
bool FunctionMaxima<A, V>::is_maximum(FunctionMaxima::it_adjacent &i) {
    return (!i.has_left || !((i.mid)->value() < (i.left)->value())) &&
           (!i.has_right || !((i.mid)->value() < (i.right)->value()));
}

#endif /* FUNCTION_MAXIMA_H */
