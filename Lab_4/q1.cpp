#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <cmath>
#include <bits/stdc++.h>
using namespace std;

// website class
class Website
{
public:
    int website_id;
    string owner_id;
    int allocated_bandwidth;
    int allocated_processing_power;
    queue<int> request_queue;// request queue

    Website(int id, const string &owner, int bandwidth, int processing_power)
        : website_id(id), owner_id(owner), allocated_bandwidth(bandwidth), allocated_processing_power(processing_power) {}

    int weight()
    {
        return allocated_processing_power * allocated_bandwidth;
    }
};

class HttpRequest
{
public:
    int request_id;
    int website_id;
    int processing_time;

    HttpRequest(int id, int site_id, int time)
        : request_id(id), website_id(site_id), processing_time(time) {}
};

class LoadBalancer
{
public:
    vector<Website> websites;
    int totalWeight = 0;
    LoadBalancer() {}
    map<int,bool>mp;
    void add_website(int website_id, const string &owner_id, int bandwidth, int processing_power)
    {
        websites.emplace_back(website_id, owner_id, bandwidth, processing_power);
        mp[website_id]=1;
        totalWeight += bandwidth * processing_power;
        // cout << "no of websites : " << websites.size() << ", total weights : " << totalWeight << endl;
    }

    void enqueue_request(const HttpRequest &request)
    {
        if (request.website_id >= 0 && mp.find(request.website_id)!=mp.end() )
        {
            // cout<<request.processing_time<<endl;
            for (int i = 0; i < request.processing_time; i++)
            {
                websites[request.website_id - 1].request_queue.push(request.request_id);
            }
        }
        else
        {
            cout << "no website with that website id..\n";
        }
    }

    int dequeue_request()
    {
        if (websites.empty())
        {
            cout << "the websites are empty." << endl;
            return -1;
        }

        vector<pair<int,double>> divWeights;
        double minWeight = 10000;
        cout<<endl;
        for (auto &website : websites)
        {
            divWeights.push_back({website.website_id,website.weight() / (double)totalWeight});
            minWeight = min(minWeight, divWeights.back().second);
            cout << "weight of website_id " << website.website_id << ": " << divWeights.back().second << endl;
        }

        // Calculate the minimum weight from the divWeights vector.
        cout<<endl;
        cout<<"normalised weight\n";
        vector<int> normalised_weight(divWeights.size());

        // Calculate normalized weights for each website based on the minimum weight.
        for (int i = 0; i < divWeights.size(); i++)
        {
            normalised_weight[i] = round(divWeights[i].second / minWeight);
            cout << "Normalised weight of website_id " <<divWeights[i].first<<": "<<  normalised_weight[i] << " " << endl;
        }
        cout<<endl;
        // Now, there are two parts to dequeueing requests. The code attempts to dequeue requests
        // from websites based on certain conditions.

        int no_of_reponse_sent = 0;
        for (int i = 0; i < websites.size(); i++)
        {
            int n = min((int)websites[i].request_queue.size(), normalised_weight[i]);
            for (int j = 0; j < n; j++)
            {
                cout << "Response sent for request_id " << websites[i].request_queue.front() << " from website_id " << websites[i].website_id << endl;
                no_of_reponse_sent++;
                websites[i].request_queue.pop();
            }
        }
        cout<<endl;
        // The function does not return any request ID.

        return no_of_reponse_sent;
    }
};

int main()
{
    LoadBalancer load_balancer;

    while (true)
    {
        int choice;
        cout << "Select an option:" << endl;
        cout << "1. Add a website" << endl;
        cout << "2. Enqueue a request" << endl;
        cout << "3. Dequeue a request" << endl;
        cout << "4. Exit" << endl;

        cin >> choice;

        if (choice == 1)
        {
            // Adding a website
            int website_id, bandwidth, processing_power;
            string owner_id;

            cout << "Enter Website ID: ";
            cin >> website_id;

            cout << "Enter Owner ID: ";
            cin >> owner_id;

            cout << "Enter Allocated Bandwidth: ";
            cin >> bandwidth;

            cout << "Enter Allocated Processing Power: ";
            cin >> processing_power;

            load_balancer.add_website(website_id, owner_id, bandwidth, processing_power);
        }
        else if (choice == 2)
        {
            // Enqueuing a request
            int request_id, website_id, processing_time;

            cout << "Enter Request ID: ";
            cin >> request_id;

            cout << "Enter Website ID: ";
            cin >> website_id;

            cout << "Enter Processing Time: ";
            cin >> processing_time;

            load_balancer.enqueue_request(HttpRequest(request_id, website_id, processing_time));
        }
        else if (choice == 3)
        {
            // Dequeuing a request
            int request = load_balancer.dequeue_request();
            if (request != -1)
            {
                cout << "No of processed request: " << request << endl;
            }
            else
            {
                cout << "No requests to dequeue." << endl;
            }
        }
        else if (choice == 4)
        {
            // Exit the program
            break;
        }
        else
        {
            cout << "Invalid choice. Please select a valid option." << endl;
        }

        cout << "Number of websites: " << load_balancer.websites.size() << endl;
        cout<<"------------------------------------------------------------------------------------------------\n";
    }
    return 0;
}