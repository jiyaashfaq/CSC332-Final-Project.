#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

struct Process {
    int pid = 0;
    double arrival = 0.0;
    int priority = 0;
    double burst_time = 0.0;
    double remaining_time = 0.0;
    double start_time = -1.0;
    double completion_time = 0.0;
    double tat = 0.0;
    bool is_completed = false;
};

void printResults(const std::vector<Process>& joblist, const char* title) {
    std::cout << "\n===== " << title << " =====\n"
        << std::left << std::setw(8) << "PID"
        << std::setw(12) << "Start"
        << std::setw(18) << "Completion Time"
        << "Turnaround Time\n";

    double total_tat = 0.0;
    for (const Process& job : joblist) {
        std::cout << std::setw(8) << job.pid
            << std::setw(12) << job.start_time
            << std::setw(18) << job.completion_time
            << job.tat << '\n';
        total_tat += job.tat;
    }

    std::cout << "Average turnaround time: "
        << total_tat / joblist.size() << '\n';
}


double nextArrival(const std::vector<Process>& joblist) {
    double arrival = std::numeric_limits<double>::infinity();
    for (const Process& job : joblist) {
        if (!job.is_completed) {
            arrival = std::min(arrival, job.arrival);
        }
    }
    return arrival;
}

void fcfs(const std::vector<Process>& jobs,
    const std::vector<int>& arrival_order) {
    std::vector<Process> joblist;
    for (int index : arrival_order) {
        joblist.push_back(jobs[index]);
    }
    const int numjobs = static_cast<int>(joblist.size());

    int jobs_completed = 0;
    double systemtime = 0.0;

    while (jobs_completed < numjobs) {
        bool ran_job = false;

        for (int i = 0; i < numjobs; i++) {
            if (joblist[i].arrival <= systemtime && !joblist[i].is_completed) {
                // Run this job to completion.
                joblist[i].start_time = systemtime;
                systemtime += joblist[i].burst_time;
                joblist[i].completion_time = systemtime;
                joblist[i].tat =
                    joblist[i].completion_time - joblist[i].arrival;

                joblist[i].is_completed = true;
                jobs_completed++;
                ran_job = true;
            }
        }

        if (!ran_job) {
            systemtime += 1.0;
        }
    }

    std::sort(joblist.begin(), joblist.end(),
        [](const Process& a, const Process& b) {
            return a.pid < b.pid;
        });
    printResults(joblist, "First Come, First Served");
}

int main() {
    
        std::ifstream input("input.txt");
        if (!input) {
            std::cerr << "Could not open input.txt.\n";
            return 1;
        }

        int numjobs;
        if (!(input >> numjobs) || numjobs <= 0) {
            std::cerr << "Enter a positive process count in input.txt.\n";
            return 1;
        }

        std::vector<Process> joblist(numjobs);
        std::vector<int> arrival_order(numjobs);

        for (int i = 0; i < numjobs; ++i) {
            Process& job = joblist[i];
            arrival_order[i] = i;

            if (!(input >> job.pid >> job.arrival >> job.burst_time) ||
                !std::isfinite(job.arrival) || !std::isfinite(job.burst_time) ||
                job.arrival < 0.0 || job.burst_time <= 0.0) {
                std::cerr << "Invalid process data on line " << i + 2 << ".\n";
                return 1;
            }

            job.remaining_time = job.burst_time;
        }

        std::stable_sort(arrival_order.begin(), arrival_order.end(),
            [&joblist](int a, int b) {
                return joblist[a].arrival < joblist[b].arrival;
            });

        std::cout << std::fixed << std::setprecision(2);
        fcfs(joblist, arrival_order);
    


}