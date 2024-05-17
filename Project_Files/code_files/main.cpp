#include <bits/stdc++.h>
using namespace std;

class Node;
class Gate;

class Circuit {
    int no_of_pi, no_of_po, no_of_gates, no_of_nodes;
    vector<Node*>primary_inputs;
    vector<Node*>primary_outputs;
    vector<Gate*>gate_list;

    public:
        Circuit();
    
    friend void read_file();
    friend void display_gate_structure();
};

Circuit ::Circuit(void) {
    no_of_pi=0;
    no_of_po=0;
    no_of_gates=0;
    no_of_nodes=0;
}

Circuit *circuit = new Circuit();

class Gate {
    string name;
    string type;
    int level;
    vector<string>inputs;
    vector<string>outputs;
    public:
        Gate();
    
    friend void read_file();
    friend void display_gate_structure();
};

Gate ::Gate(void) {
    name = "";
    type = "";
    level = 0;
}


void display_gate_structure() {

    cout << "Gate Details:\n";
    for (const auto &gate : circuit->gate_list)
    {
        cout << "Name: " << gate->name << endl;
        cout << "Type: " << gate->type << endl;
        cout << "Inputs:";
        for (const auto &input : gate->inputs)
        {
            cout << " " << input;
        }
        cout << endl;
        cout << "Outputs:";
        for (const auto &output : gate->outputs)
        {
            cout << " " << output;
        }
        cout << endl;
        cout << endl;
    }
    return;
}

void read_file() {
    string filename;
    cout<<"Enter Input Text File: ";
    cin>>filename;
    // filename += "a2";
    filename = "./example_input_files/" + filename + ".txt";
    ifstream file(filename);

    string line = "";
    int ind = 1;
    while(getline(file, line)) {
        vector<string>tmp;
        for(int i=0;i<line.length();i++) {
            string s = "";
            while(i<line.length() && line[i]!= ' ') {
                s+=line[i];
                i++;
            }
            tmp.push_back(s);
        }

        // prepare gate structure
        Gate *gate = new Gate();
        gate->name = "G" + to_string(ind++);
        gate->type = tmp[0];
        if(tmp[0]!="FANOUT") {
            gate->outputs.push_back(tmp[1]);
            for(int i=2;i<tmp.size();i++) {
                gate->inputs.push_back(tmp[i]);
            }
        } else {
            gate->inputs.push_back(tmp[1]);
            for(int i=2;i<tmp.size();i++) {
                gate->outputs.push_back(tmp[i]);
            }
        }
        circuit->gate_list.push_back(gate);
        circuit->no_of_gates++;
    }

    file.close();
    
    return ;
}


int main() {
    
    read_file();
    display_gate_structure();

    return 0;
}