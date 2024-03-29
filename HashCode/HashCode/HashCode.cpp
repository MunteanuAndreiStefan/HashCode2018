#include <bits/stdc++.h>
#define in f
#define out g

using namespace std;

ifstream f("e_high_bonus.in");
ofstream g("e.out");

const int INF = 1e9 + 5e8;
const int MAX_VEHICLES = 1e4 + 10;

struct Ride {
	int start_x;
	int start_y;
	int end_x;
	int end_y;
	int earliest;
	int latest;
	int id;
};

int n, m;
int vehicles;
int rides_count;
int bonus;
int steps;
vector<Ride> rides;
unordered_map<int, int> will_be_ready;
unordered_map<int, pair<int, int>> where_is;
int vehicles_score[MAX_VEHICLES];
unordered_map<int, vector<int>> assign;

double distance(int a, int b, int x, int y) {
	return abs(a - x) + abs(b - y);
}

void read() {
	in >> n;
	in >> m;
	in >> vehicles;
	in >> rides_count;
	in >> bonus;
	in >> steps;
	for (int i = 0; i < rides_count; i++) {
		Ride r;
		in >> r.start_x;
		in >> r.start_y;
		in >> r.end_x;
		in >> r.end_y;
		in >> r.earliest;
		in >> r.latest;
		r.id = i;
		rides.push_back(r);
	}
}

double C_DIST = 0.5;
double C_TIME = 0.5;
double C_READY = 0.5;
double C_BONUS = -0.5;

int main() {
	read();
	for (int i = 0; i < vehicles; i++) {
		will_be_ready[i] = 0;
		where_is[i] = { 0, 0 };
	}

	// sort the rides
	sort(rides.begin(), rides.end(), [](Ride a, Ride b) {
		if (a.earliest == b.earliest) {
			return a.latest < b.latest;
		}
		return a.earliest < b.earliest;
	});


	for (auto ride : rides) {
		// we find the vehicle with the best score for the ride
		double min_score = INF;
		// best vehicle index starting from 0...vehicles-1
		int best_vehicle = 0;
		// for each vehicle
		for (int vehicle = 0; vehicle < vehicles; vehicle++) {
			double distance_to_ride = distance(where_is[vehicle].first, where_is[vehicle].second, ride.start_x, ride.start_y);
			double ride_length = distance(ride.start_x, ride.start_y, ride.end_x, ride.end_y);
			double will_be_ready_delta = (will_be_ready[vehicle] - ride.earliest);
			double has_bonus = will_be_ready[vehicle] + distance_to_ride <= ride.earliest ? 1 : 0;

			if (will_be_ready_delta + distance_to_ride + ride_length > ride.latest) {
				continue;
			}

			double vehicle_score =
				distance_to_ride * C_DIST +
				will_be_ready[vehicle] * C_TIME +
				has_bonus * bonus * C_BONUS;

			if (vehicle_score < min_score) {
				best_vehicle = vehicle;
				min_score = vehicle_score;
			}
		}

		// now assign the vehicle
		will_be_ready[best_vehicle] += distance(where_is[best_vehicle].first, where_is[best_vehicle].second, ride.start_x, ride.start_y)
			+ distance(ride.start_x, ride.start_y, ride.end_x, ride.end_y);
		where_is[best_vehicle] = { ride.end_x, ride.end_y };
		assign[best_vehicle].push_back(ride.id);
	}

	for (int vehicle = 0; vehicle < vehicles; vehicle++) {
		out << assign[vehicle].size() << " ";
		for (auto ride_id : assign[vehicle]) {
			out << ride_id << " ";
		}

		out << "\n";
	}

	COUT << "DONE";

	return 0;
}
