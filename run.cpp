#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
class Run_Entry
{
public:
    int distance;
    int pace;
    int calories;
    int avg_heart_rate;
    int time;
    int day;
    int month;

    //All the information needed for a run entry
    Run_Entry(int dist, int p, int cal, int hr, int t)
    {
        distance = dist;
        pace = p;
        calories = cal;
        avg_heart_rate = hr;
        time = t;

        auto now = chrono::system_clock::now();
        std::time_t now_c = chrono::system_clock::to_time_t(now);
        std::tm local_tm = *std::localtime(&now_c);
        day = local_tm.tm_mday;
        month = local_tm.tm_mon + 1;
    }
    void display_entry()
    {
        cout << "Run Entry Details:" << endl;

        cout << "Date: " << day << "/" << month << endl;
        cout << "Distance: " << distance << " kilometers" << endl;
        cout << "Pace: " << pace << " seconds per kilometer" << endl;
        cout << "Calories burned: " << calories << endl;
        cout << "Average heart rate: " << avg_heart_rate << " bpm" << endl;
        cout << "Time: " << time << " minutes" << endl;
    }
    //Takes the filename as a parameter and appends the run entry to the CSV file
    void save_to_csv(const string& filename)
    {
        bool file_exists = false;

    
    ifstream infile(filename);
    if (infile.good() && infile.peek() != ifstream::traits_type::eof()) {
        file_exists = true;
    }
    infile.close();

    ofstream file(filename, ios::app);

    
    if (!file_exists) {
        file << "Date,Distance (km),Pace (s/km),Calories (kcal),Avg Heart Rate (bpm),Time (min)\n";
    }
        file << day << "/" << month << ","
             << distance << ","
             << pace << ","
             << calories << ","
             << avg_heart_rate << ","
             << time << endl;

        file.close();
    }
};

// Class to summarize all runs from the CSV file
class Run_Summary {
private:
    long total_distance = 0;
    long total_calories = 0;
    long total_time = 0;
    int total_runs = 0;
    int longest_run = 0;

public:
    void load_from_csv(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Could not open " << filename << endl;
            return;
        }

        string line;
        getline(file, line); 

        while (getline(file, line)) {
            stringstream ss(line);
            string value;
            int colIndex = 0;

            int daymonth, distance, pace, calories, avg_heart_rate, time;

            while (getline(ss, value, ',')) {
                switch (colIndex) {
                    case 0: daymonth = stoi(value); break;
                    case 1: distance = stoi(value); break;
                    case 2: pace = stoi(value); break;
                    case 3: calories = stoi(value); break;
                    case 4: avg_heart_rate = stoi(value); break;
                    case 5: time = stoi(value); break;
                }
                colIndex++;
            }

            total_distance += distance;
            total_calories += calories;
            total_time += time;
            if (distance > longest_run) longest_run = distance;
            total_runs++;
        }

        file.close();
    }

    void display_summary() {
        cout << "\n=== Run Summary ===" << endl;
        cout << "Total runs: " << total_runs << endl;
        cout << "Total distance: " << total_distance << " km" << endl;
        cout << "Total calories burned: " << total_calories << " kcal" << endl;
        cout << "Total time: " << total_time << " minutes" << endl;

        if (total_runs > 0) {
            cout << "Average distance per run: " 
                 << (double)total_distance / total_runs << " km" << endl;
            cout << "Longest run: " << longest_run << " km" << endl;
        }
    }
};

int main()
{
    //Main menu for user interaction. There are two options: enter a new run or view the summary of all runs
    cout << "Welcome to the Run Tracker!" << endl;
    int userChoice;
    cout << "What would you like to do? " << endl;
    cout << "1. Enter a new run" << endl;
    cout << "2. View run summary" << endl;
    cin >> userChoice;
    if (userChoice == 1)
    {
        int distance;
        int pace;
        int calories;
        int avg_heart_rate;
        int time;
        string reply;
        cout << "Enter distance (km): ";
        cin >> distance;
        cout << "Enter pace (m/km): ";
        cin >> pace;
        cout << "Enter calories burned (kcal): ";
        cin >> calories;
        cout << "Enter average heart rate (bpm): ";
        cin >> avg_heart_rate;
        cout << "Enter time (in minutes): ";
        cin >> time;
        Run_Entry entry(distance, pace, calories, avg_heart_rate, time);
        entry.display_entry();
        cout << "Would you like to save this entry? (yes/no): ";
        cin >> reply;
        if (reply == "yes")
        {
            entry.save_to_csv("run_entries.csv");
            cout << "Run saved to runs.csv ✅" << endl;
        }
        else
        {
            cout << "Run not saved ❌" << endl;
        }
    }
    
    if (userChoice == 2) {
        Run_Summary summary;
        summary.load_from_csv("run_entries.csv");
        summary.display_summary();
        return 0;
    }
    else {
        cout << "Invalid choice. Exiting." << endl;
    }
    
    


    

    return 0;
}