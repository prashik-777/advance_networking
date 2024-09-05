#include <iostream>
#include <queue>
#include <random>
using namespace std;

class AirportSecuritySimulation
{
public:
    AirportSecuritySimulation(double arrival_rate, double service_rate, int buffer_size, int no_of_server)
        : arrival_rate_(arrival_rate), service_rate_(service_rate), buffer_size_(buffer_size), no_of_server_(no_of_server)
    {
        srand(time(NULL));
    }

    void run(int simulation_time)
    {
        queue<int> queue;
        int current_time = 0;
        int total_wait_time = 0;
        int total_service_time = 0;
        int passengers_processed = 0;

        vector<int> server_on_time(no_of_server_, -1);
        while (current_time < simulation_time)
        {
            // Generate passenger arrival time
            if (rand() / (double)RAND_MAX < arrival_rate_)
            {
                if (queue.size() < buffer_size_)
                    queue.push(current_time);
            }

            // Allocate a passenger to the server if it's idle and the queue is not empty
            for (int i = 0; i < no_of_server_; i++)
            {
                if (server_on_time[i] == -1 && !queue.empty())
                {
                    int arrival_time = queue.front();
                    server_on_time[i] = current_time;
                    total_wait_time += current_time - arrival_time;
                    passengers_processed++;
                    queue.pop();
                }
                else if (rand() / (double)RAND_MAX < service_rate_ && (server_on_time[i] != -1))
                {
                    total_service_time += (current_time - server_on_time[i]);
                    server_on_time[i] = -1;
                }
            }

            current_time++;
        }

        // Calculate statistics
        cout << "total_service_time : " << total_service_time << endl;
        cout << "total_wait_time : " << total_wait_time << endl;
        cout << "passengers_processed : " << passengers_processed << endl;
        cout << endl;
        double average_wait_time = static_cast<double>(total_wait_time) / passengers_processed;
        double average_queue_length = static_cast<double>(total_wait_time) / simulation_time;
        double system_utilization = static_cast<double>((double)total_service_time/no_of_server_) / simulation_time;

        // Print results
        cout << "Average Waiting Time: " << average_wait_time << endl;
        cout << "Average Queue Length: " << average_queue_length << endl;
        cout << "System Utilization: " << system_utilization << endl;
    }

private:
    double arrival_rate_;
    double service_rate_;
    int buffer_size_;
    int no_of_server_;
};

int main()
{
    double arrival_rate;
    double service_rate;
    int buffer_size, simulation_time,no_of_server_;

    cout << "Enter arrival rate (λ): ";
    cin >> arrival_rate;
    cout << "Enter service rate (μ): ";
    cin >> service_rate;
    cout << "Enter buffer size (K): ";
    cin >> buffer_size;
    cout << "Enter simulation time: ";
    cin >> simulation_time;
    cout << "Enter no of server : ";
    cin >> no_of_server_;

    AirportSecuritySimulation simulation(arrival_rate, service_rate, buffer_size,no_of_server_);
    simulation.run(simulation_time);

    return 0;
}
