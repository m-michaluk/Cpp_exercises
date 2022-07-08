/**
	Pierwsze zadanie punktowane - Języki i Narzędzia Programowania 1
	Autorzy:
	Stanisław Durka
	Monika Michaluk
	29.10.2020
	grupa nr. 2
**/

#include <iostream>
#include <regex>
#include <string>
#include <map>
#include <cstdlib>
#include <cassert>

using std::string;
using std::pair;
using std::map;
using std::vector;
using std::map;
using std::tuple;
using std::to_string;
using std::make_pair;
using std::make_tuple;
using std::istringstream;
using std::regex;
using std::regex_match;
using std::smatch;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;


// entrance_info_t = <uint32_t numer drogi, char typ drogi, uint32_t kilometraż, string linijka błędu>
using entrance_info_t = tuple<uint32_t, char, uint32_t, string>;

// zawiera pary typu <'S', łączna droga przebyta przez pojazd drogami typu S> oraz <'A', droga przebyta drogami typu A>
using distances_vector_t = vector<pair<char, int>>;

// klucz: rejestracja pojazdu, wartość: typ przechowujący informacje o łącznych dystansach tego pojazdu na drogach S i A
using vehicles_map_t = map<string, distances_vector_t>;


// klucz: rejestracja pojazdu, wartość: typ przechowujący informacje o tym pojeździe
using entrances_map_t = map<string, entrance_info_t>;

// klucz: pair<numer drogi, char typ drogi>; wartość: liczba kilometrów przejechanych daną drogą
using roads_map_t = map<pair<int, char>, int>;

string convert_int_to_string_float(uint32_t number) {

    uint32_t integer_part = number / 1000;
    uint32_t decimal_part = number % 1000;
    string integer_part_str = to_string(integer_part);
    string decimal_part_str = to_string(decimal_part).substr(0, 1);
    return integer_part_str + "," + decimal_part_str;
}

void print_vehicle_distances(const string &registration, distances_vector_t &v) {
    cout << registration << " ";

    for (size_t i = 0; i < v.size(); i++) {
        char road_type = v[i].first;
        uint32_t dist = v[i].second;

        string number = convert_int_to_string_float(dist);
        cout << road_type << " " << number;

        if (i != v.size() - 1) cout << " ";
    }
    cout << endl;
}

void print_road_dist(int road_nr, char road_name, uint32_t dist) {
    string dist_str = convert_int_to_string_float(dist);
    cout << road_name << road_nr << " " << dist_str << endl;
}

void print_roads_map(roads_map_t &r_map) {
    roads_map_t::iterator it;
    for (it = r_map.begin(); it != r_map.end(); it++) {
        print_road_dist(it->first.first, it->first.second, it->second);
    }
}

void print_vehicles_map(vehicles_map_t &v_map) {
    vehicles_map_t::iterator it;
    for (it = v_map.begin(); it != v_map.end(); it++) {
        print_vehicle_distances(it->first, it->second);
    }
}

void add_distance_traveled(distances_vector_t &v, char road_name, uint32_t dist) {
    bool found = false;
    for (size_t i = 0; i < v.size() && !found; i++) {
        if (v[i].first == road_name) {
            v[i].second += dist;
            found = true;
        }
    }
    if (!found) {
        pair<char, int> p = make_pair(road_name, dist);
        if (road_name == 'A') {
            v.insert(v.begin(), p);
        } else {
            assert(road_name == 'S');
            v.insert(v.end(), p);
        }
    }
}

void
register_entrance(entrances_map_t &map, string &registration, uint32_t road_number, char road_type, uint32_t mileage,
                  string &possible_error_message) {

    auto it_vehicle = map.find(registration);

    if (it_vehicle != map.end())
        map.erase(registration);

    entrance_info_t new_entrance = make_tuple(road_number, road_type, mileage, possible_error_message);
    map.insert(make_pair(registration, new_entrance));
}

uint32_t convert_string_float_to_int(const string &number, bool &err) {
    istringstream ss(number);

    string integer_part_str, decimal_part_str;
    uint32_t integer_part, decimal_part;

    getline(ss, integer_part_str, ',');
    getline(ss, decimal_part_str);

    decimal_part = strtoul(decimal_part_str.c_str(), nullptr, 10);
    decimal_part *= 100;
    integer_part = strtoul(integer_part_str.c_str(), nullptr, 10);

    if (errno == ERANGE) {
        err = true;
        return 0;
    }

    if (integer_part < UINT32_MAX / 1000 && integer_part < UINT32_MAX / 1000 - decimal_part) {
        return integer_part * 1000 + decimal_part;
    }

    err = true;
    return 0;
}

void update_roads_map(roads_map_t &roads, uint32_t road_number, char road_type, uint32_t distance_travelled) {
    pair<uint32_t, char> road_name = make_pair(road_number, road_type);
    roads_map_t::iterator it_road;
    it_road = roads.find(road_name);
    if (it_road == roads.end()) {
        it_road = roads.insert(it_road, make_pair(road_name, 0));
    }
    it_road->second += distance_travelled;
}

void
register_exit(entrance_info_t &entr_info, entrances_map_t &entrances_map, roads_map_t &roads, vehicles_map_t &vehicles,
              string &registration, uint32_t road_number, uint32_t mileage, char road_type) {
    uint32_t road_number_in;
    char road_type_in;
    uint32_t mileage_in;
    string error_message;
    vehicles_map_t::iterator it_vehicle;

    tie(road_number_in, road_type_in, mileage_in, error_message) = entr_info;

    uint32_t distance_travelled;
    if (mileage > mileage_in)
        distance_travelled = (mileage - mileage_in);
    else
        distance_travelled = mileage_in - mileage;

    it_vehicle = vehicles.find(registration);

    if (it_vehicle == vehicles.end()) { // nie znaleziono w mapie pojazdów pojazdu o danej rejestracji

        vector<pair<char, int>> new_distance = {make_pair(road_type, 0)};
        it_vehicle = vehicles.insert(it_vehicle, make_pair(registration, new_distance));
    }

    // dodaj pare do wektora lub zwieksza wartosc elementu w wektorze
    add_distance_traveled(it_vehicle->second, road_type, distance_travelled);

    //dodaj przejechany dystans do drogi w mapie dróg:

    update_roads_map(roads, road_number, road_type, distance_travelled);

    // na koniec, skoro sparowaliśmy wjazd więc usuwamy go z mapy
    entrances_map.erase(registration);
}

void print_full_info(vehicles_map_t & vehicles, roads_map_t &roads) {
    print_vehicles_map(vehicles);
    print_roads_map(roads);
}

void print_vehicle_info(const string &registration, vehicles_map_t &vehicles) {
    auto it_vehicle = vehicles.find(registration);

    if (it_vehicle != vehicles.end()) {
        print_vehicle_distances(registration, it_vehicle->second);
    }
}

void print_road_info(const string &road_name, roads_map_t &roads) {

    string road_number_str = road_name.substr(1, road_name.length() - 1);

    char road_type = road_name[0];
    uint32_t road_number = strtoul(road_number_str.c_str(), nullptr, 10);

    auto it_road = roads.find(make_pair(road_number, road_type));
    if (it_road != roads.end())
        print_road_dist(road_number, road_type, it_road->second);
}

tuple<string, char, uint32_t, uint32_t> process_arguments(smatch &line_match, bool &err) {
    string registration = line_match[1].str();

    string road_str, road_number_str;
    road_str = line_match[2].str();
    road_number_str = road_str.substr(1, road_str.length() - 1);

    char road_type = road_str[0];
    uint32_t road_number = strtoul(road_number_str.c_str(), nullptr, 10);

    uint32_t mileage = convert_string_float_to_int(line_match[3].str(), err);
    return make_tuple(registration, road_type, road_number, mileage);
}

int main() {

    regex ENTRY_OR_EXIT("[\\s]*"
                        "([a-zA-Z0-9]{3,11})[\\s]+"
                        "([AS][1-9][0-9]{0,2})[\\s]+"
                        "(0,[0-9]|[1-9][0-9]*[,][0-9])"
                        "[\\s]*");


    regex PRINT_FULL_INFO("[\\s]*[?][\\s]*");

    regex PRINT_VEHICLE_INFO("[\\s]*[?][\\s]*"
                             "([a-zA-Z0-9]{3,11})"
                             "[\\s]*");

    regex PRINT_ROAD_INFO("[\\s]*[?][\\s]*"
                          "([AS][1-9][0-9]{0,2})"
                          "[\\s]*");

    regex COMMAND("[\\s]*[?][\\s]*|"
                  "[\\s]*[?][\\s]*[a-zA-Z0-9]{3,11}[\\s]*|"
                  "[\\s]*[?][\\s]*[AS][1-9][0-9]{0,2}[\\s]*");

    string line;
    smatch line_match;
    unsigned long int line_nr = 1;

    vehicles_map_t vehicles;
    roads_map_t roads;
    entrances_map_t entrances;

    vehicles_map_t::iterator it_vehicle;
    roads_map_t::iterator it_road;
    entrances_map_t::iterator it_entrance;

    char road_type;
    uint32_t road_number, mileage;
    string registration;


    while (getline(cin, line)) {

        if (!line.empty()) { // puste linie ignorujemy

            string possible_error_message = "Error in line " + to_string(line_nr) + ": " + line;

            if (regex_match(line, line_match, ENTRY_OR_EXIT)) {
                bool err = false;
                tie(registration, road_type, road_number, mileage) = process_arguments(line_match, err);

                if (err) {
                    cerr << possible_error_message << endl;
                    line_nr++;
                    continue;
                }

                it_entrance = entrances.find(registration);
                if (it_entrance != entrances.end()) { // pojazd wjechał wcześniej na jakąś drogę

                    uint32_t road_number_in, mileage_in;
                    char road_type_in;
                    string error_message;
                    tie(road_number_in, road_type_in, mileage_in, error_message) = it_entrance->second;

                    if (road_number_in == road_number &&
                        road_type_in == road_type) { // aktualne info jest zjazdem samochodu z zdrogi
                        register_exit(it_entrance->second, entrances, roads, vehicles, registration, road_number,
                                      mileage, road_type);
                    } else {
                        //jest błąd, aktualnie wczytana linia jest potencjalnie poprawną informacją o wjeździe pojazdu na daną drogę.
                        cerr << error_message << endl;
                        register_entrance(entrances, registration, road_number, road_type, mileage,
                                          possible_error_message);
                    }
                } else {
                    // dany pojazd nie jest na żadnej drodze, więc aktualnie wczytana linia jest informacją o wjeździe pojazdu na daną drogę
                    // dodajemy do mapy węzeł jako węzeł wjazdu na drogę
                    register_entrance(entrances, registration, road_number, road_type, mileage, possible_error_message);
                }

            } else if (regex_match(line, line_match, COMMAND)) {

                if (regex_match(line, line_match, PRINT_FULL_INFO)) {
                    print_full_info(vehicles, roads);
                } else {
                    if (regex_match(line, line_match, PRINT_VEHICLE_INFO)) {
                        print_vehicle_info(line_match[1].str(), vehicles);
                    }
                    if (regex_match(line, line_match, PRINT_ROAD_INFO)) {
                        print_road_info(line_match[1].str(), roads);
                    }
                }
            } else {
                cerr << possible_error_message << endl;
            }

        }
        line_nr++;
    }

    return 0;
}
