#include <unordered_map>
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <string>

using namespace std; // Stops use having to put std:: before a lot of things (some reason bugs out when doing function definitions)

/*
* Implements fundamental stable marriage algorithm
*/
std::vector<tuple <string,string> > StableMarriageAlgo(unordered_map<string, vector<string> >  prefA, unordered_map<string, vector<string>>  prefB) 
{
    // Initialisation
    int k = 0;
    int n = prefA.size();

    /*  
    * Variables that are needing to be predefined
    */
    vector<string> :: iterator itr;  // This is a general variable used later on to recover index positions in vectors
    vector<string> prefs;
    vector<tuple <string, string> > tempMatchings; // Store temp matchings as the function output type
    string womCurrMatch;

    // Create an empty list to store male suitors
    vector<string> men;
    men.reserve(prefA.size()); 

    // Add undersirable man to the fold and temp engage to women
    for (auto& [key, v] : prefB) {
        v.push_back("undesirable");
        tempMatchings.push_back(tuple <string, string>("undesirable", key));
    }
    
    // Assign Male keys to a vector to allow easier iteration
    for(auto kv : prefA) {
        men.push_back(kv.first);
    } 
    // Add undesirable man to the male preference list to avoid some errors of empty vectors we get later
    prefA["undesirable"] = {};
    men.push_back("undesirable");
    
    while (k<n) {
        // Assign next man in list as the suitor
        string X = men[k];
            
        // Get that mans preference list
        prefs = prefA[X];
        
        // While the suitor isn't the undesirable man
        while (X != "undesirable") {
            // Find best remaining choice on suitors list
            string x = prefs[0];

            // Get index of suitor (X) and fiance from womans (x) list
            for (auto& [m,w] : tempMatchings) {
                if (w==x) {
                    womCurrMatch = m;
                }
            }
            
            // Find new mans index in womans list
            itr = find(prefB[x].begin(), prefB[x].end(), womCurrMatch);
            int womCurrMatchIndex = distance(prefB[x].begin(), itr);
            
            // Find index of suitor
            itr = find(prefB[x].begin(), prefB[x].end(), X);
            int womSuitorIdx = distance(prefB[x].begin(), itr);
            
            // Compare positions
            if (womSuitorIdx < womCurrMatchIndex) {
                // call off womans (x) current marriage and match suitor (X) and woman
                auto it = remove_if(tempMatchings.begin(), tempMatchings.end(), [&](auto & tup){ return (get<0>(tup) == womCurrMatch) && (get<1>(tup) == x); }); 
                tempMatchings.erase(it, tempMatchings.end()); // resize the vector to only hold live elements
                
                tempMatchings.push_back(tuple <string, string>(X, x));
                
                // new suitor is abandoned man
                X = womCurrMatch;
                
                // Generate the new preference list
                prefs = prefA[X];
            }
            if (X != "undesirable") {
                // Remove x from suitors list
                prefs.erase(remove(prefs.begin(), prefs.end(), x), prefs.end());
            }
        }
        k+=1;
    }
    // Return sorted matchings
    sort(tempMatchings.begin(), tempMatchings.end());
    return tempMatchings;
}

int main() {
    vector<tuple <string, string> > matches; // Stores Matches tables
    unordered_map<string, vector<string> > prefA, prefB; // Stores preference tables

    // Populate Preference tables
    prefA["A"] = {"c","b","d","a"};
    prefA["B"] = {"b", "a", "c", "d"};
    prefA["C"] = {"b","d","a","c"};
    prefA["D"] = {"c", "a", "d", "b"};

    prefB["a"] = {"A", "B", "D", "C"};
    prefB["b"] = {"C", "A", "D", "B"};
    prefB["c"] = {"C","B","D","A"};
    prefB["d"] = {"B","A","C","D"};
    matches = StableMarriageAlgo(prefA, prefB);
    
    // Print matches
    for (int i = 0; i<=matches.size()-1; i++) {
        cout << get<0>(matches[i]) << " marries " << get<1>(matches[i]) << endl;
    }
    
    return 0;
}