#include <bits/stdc++.h>
using namespace std;

void divider()
{
    cout << endl;
    for (int i = 0; i < 40; i++)
    {
        cout << "-";
    }
    cout << endl;
    return;
}

class Node;
class Gate;

class Circuit
{
    int no_of_pi, no_of_po, no_of_gates, no_of_nodes;
    int no_of_levels;
    vector<Node *> primary_inputs;
    vector<Node *> primary_outputs;
    vector<Gate *> gate_list;
    unordered_map<string, Node *> node_list; // <nodeName,Node-pointer>
    unordered_map<Node *, Gate *> gate_node_list;
    map<int, vector<Node *>> levelized_circuit;

public:
    Circuit();

    friend void read_file();
    friend void display_circuit_details();
    friend void display_gate_structure();
    friend void display_node_structure();
    friend void gates_nodes_levelization();
    friend void traverse_circuit();
    friend void assign_scoap();
    friend void assign_controllability();
    friend void assign_observability();
    friend int xor_cc0_helper(Gate *);
    friend int xor_cc1_helper(Gate *);
    friend int find_cc0(Gate *);
    friend int find_cc1(Gate *);
    friend int co_helper(Gate *, string);
    friend int find_co(Gate *);
    friend void display_scoap_values();
};

Circuit ::Circuit(void)
{
    no_of_pi = 0;
    no_of_po = 0;
    no_of_gates = 0;
    no_of_nodes = 0;
}

Circuit *circuit = new Circuit();

class Gate
{
    string name;
    string type;
    int level;
    vector<string> inputs;
    vector<string> outputs;

public:
    Gate();

    friend void read_file();
    friend void display_circuit_details();
    friend void display_gate_structure();
    friend void display_node_structure();
    friend void gates_nodes_levelization();
    friend void traverse_circuit();
    friend void assign_scoap();
    friend void assign_controllability();
    friend void assign_observability();
    friend int xor_cc0_helper(Gate *);
    friend int xor_cc1_helper(Gate *);
    friend int find_cc0(Gate *);
    friend int find_cc1(Gate *);
    friend int co_helper(Gate *, string);
    friend int find_co(Gate *);
    friend void display_scoap_values();
};

Gate ::Gate(void)
{
    name = "";
    type = "";
    level = 0;
}

class Node
{
    string name;
    string type;
    int level;
    vector<Node *> next;
    int indeg, outdeg;
    int CC0, CC1, CO;

public:
    Node();
    Node(string, string);

    friend void read_file();
    friend void display_circuit_details();
    friend void display_gate_structure();
    friend void display_node_structure();
    friend void gates_nodes_levelization();
    friend void traverse_circuit();
    friend void assign_scoap();
    friend void assign_controllability();
    friend void assign_observability();
    friend int xor_cc0_helper(Gate *);
    friend int xor_cc1_helper(Gate *);
    friend int find_cc0(Gate *);
    friend int find_cc1(Gate *);
    friend int co_helper(Gate *, string);
    friend int find_co(Gate *);
    friend void display_scoap_values();
};

Node ::Node()
{
    name = "";
    type = "";
    level = 0;
    indeg = 0;
    outdeg = 0;
    CC0 = INT_MAX - 2;
    CC1 = INT_MAX - 2;
    CO = INT_MAX - 2;
}

Node ::Node(string nme, string ty)
{
    name = nme;
    type = ty;
    level = 0;
    indeg = 0;
    outdeg = 0;
    CC0 = INT_MAX - 2;
    CC1 = INT_MAX - 2;
    CO = INT_MAX - 2;
}

void display_circuit_details()
{
    divider();
    cout << "Circuit Details:";
    divider();
    cout << "No. of Primary Inputs: " << circuit->no_of_pi << endl;
    cout << "No. of Primary Outputs: " << circuit->no_of_po << endl;
    cout << "No. of Gates: " << circuit->no_of_gates << endl;
    cout << "No. of Nodes: " << circuit->no_of_nodes << endl;
    cout << "Primary Inputs:";
    for (auto pi : circuit->primary_inputs)
    {
        cout << " " << pi->name;
    }
    cout << endl;
    cout << "Outputs:";
    for (auto po : circuit->primary_outputs)
    {
        cout << " " << po->name;
    }
    cout << endl;
    cout << endl;
    return;
}

void display_gate_structure()
{
    divider();
    cout << "Gate Details:";
    divider();
    for (const auto &gate : circuit->gate_list)
    {
        cout << "Name: " << gate->name << endl;
        cout << "Type: " << gate->type << endl;
        cout << "Level: " << gate->level << endl;
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

void display_node_structure()
{
    divider();
    cout << "Node Details:";
    divider();
    for (const auto &node_info : circuit->node_list)
    {
        string node_name = node_info.first;
        Node *node = node_info.second;
        cout << "Name: " << node->name << endl;
        cout << "Type: " << node->type << endl;
        cout << "Level: " << node->level << endl;
        cout << "Indeg: " << node->indeg << endl;
        cout << "Outdeg: " << node->outdeg << endl;
        cout << "Outputs:";
        for (const auto &next : node->next)
        {
            cout << " -> " << next->name;
        }
        cout << endl;
        cout << endl;
    }
    return;
}

void gates_nodes_levelization()
{
    queue<Node *> q;
    for (auto &node_info : circuit->node_list)
    {
        string node_name = node_info.first;
        Node *node = node_info.second;
        if (node->indeg == 0)
        {
            circuit->primary_inputs.push_back(node);
            circuit->no_of_pi++;
            q.push(node);
        }
        if (node->outdeg == 0)
        {
            circuit->primary_outputs.push_back(node);
            circuit->no_of_po++;
        }
    }

    int lvl = 0;
    while (!q.empty())
    {
        lvl++;
        int sz = q.size();
        while (sz--)
        {
            Node *curr = q.front();
            curr->level = lvl;
            circuit->no_of_levels = lvl;
            circuit->levelized_circuit[lvl].push_back(curr);
            if (curr->type == "NON-WIRE")
            {
                Gate *gate = circuit->gate_node_list[curr];
                gate->level = lvl;
            }
            q.pop();
            for (auto &next_node : curr->next)
            {
                next_node->indeg--;
                if (next_node->indeg == 0)
                {
                    q.push(next_node);
                }
            }
        }
    }

    return;
}

void traverse_circuit()
{
    divider();
    cout << "CIRCUIT TRAVERSAL::";
    divider();
    for (int i = 1; i <= circuit->no_of_levels; i++)
    {
        cout << "Level " << i << " :";
        for (auto node : circuit->levelized_circuit[i])
        {
            cout << " " << node->name;
        }
        cout << endl;
    }
}

void display_scoap_values()
{
    divider();
    cout << "Signal\tCC0\tCC1\tCO";
    divider();
    for (int i = 1; i <= circuit->no_of_levels; i++)
    {
        for (auto node : circuit->levelized_circuit[i])
        {
            if (node->type == "WIRE")
            {
                cout << node->name << "\t" << node->CC0 << "\t" << node->CC1 << "\t" << node->CO << endl;
            }
        }
    }
}

int xor_cc0_helper(Gate *gate)
{
    vector<int> cc0, cc1;
    for (auto input : gate->inputs)
    {
        cc0.push_back(circuit->node_list[input]->CC0);
        cc1.push_back(circuit->node_list[input]->CC1);
    }
    sort(cc0.begin(), cc0.end());
    sort(cc1.begin(), cc1.end());
    int n = gate->inputs.size();
    for (int i = 1; i < n; i++)
        cc0[i] += cc0[i - 1];
    for (int i = 1; i < n; i++)
        cc1[i] += cc1[i - 1];
    int p1 = -1, p0 = n - 1;
    int cc0_input = cc0[p0]; // taking all zeros
    p1 += 2;
    p0 -= 2;
    while (p1 < n && p0 >= 0)
    {
        cc0_input = min(cc0_input, cc1[p1] + cc0[p0]);
        p1 += 2;
        p0 -= 2;
    }
    return cc0_input;
}

int xor_cc1_helper(Gate *gate)
{
    vector<int> cc0, cc1;
    for (auto input : gate->inputs)
    {
        cc0.push_back(circuit->node_list[input]->CC0);
        cc1.push_back(circuit->node_list[input]->CC1);
    }
    sort(cc0.begin(), cc0.end());
    sort(cc1.begin(), cc1.end());
    int n = gate->inputs.size();
    for (int i = 1; i < n; i++)
        cc0[i] += cc0[i - 1];
    for (int i = 1; i < n; i++)
        cc1[i] += cc1[i - 1];
    int p1 = 0, p0 = n - 2;
    int cc1_input = cc1[p1] + cc0[p0]; // taking one 1 and rest 0s
    p1 += 2;
    p0 -= 2;
    while (p1 < n && p0 >= 0)
    {
        cc1_input = min(cc1_input, cc1[p1] + cc0[p0]);
        p1 += 2;
        p0 -= 2;
    }
    return cc1_input;
}

int find_cc0(Gate *gate)
{
    int cc0 = -1;
    string gate_type = gate->type;
    if (gate_type == "AND")
    {
        cc0 = INT_MAX - 2;
        for (auto input : gate->inputs)
        {
            int cc0_input = circuit->node_list[input]->CC0;
            cc0 = min(cc0, cc0_input);
        }
        cc0 += 1;
    }
    else if (gate_type == "OR")
    {
        cc0 = 0;
        for (auto input : gate->inputs)
        {
            int cc0_input = circuit->node_list[input]->CC0;
            cc0 += cc0_input;
        }
        cc0 += 1;
    }
    else if (gate_type == "NAND")
    {
        cc0 = 0;
        for (auto input : gate->inputs)
        {
            int cc1_input = circuit->node_list[input]->CC1;
            cc0 += cc1_input;
        }
        cc0 += 1;
    }
    else if (gate_type == "NOR")
    {
        cc0 = INT_MAX - 2;
        for (auto input : gate->inputs)
        {
            int cc1_input = circuit->node_list[input]->CC1;
            cc0 = min(cc0, cc1_input);
        }
        cc0 += 1;
    }
    else if (gate_type == "XOR")
    {
        cc0 = xor_cc0_helper(gate);
        cc0 += 1;
    }
    else if (gate_type == "XNOR")
    {
        cc0 = xor_cc1_helper(gate);
        cc0 += 1;
    }
    else if (gate_type == "NOT")
    {
        for (auto input : gate->inputs)
        {
            int cc1_input = circuit->node_list[input]->CC1;
            cc0 = cc1_input;
        }
        cc0 += 1;
    }
    else if (gate_type == "BUF")
    {
        for (auto input : gate->inputs)
        {
            int cc0_input = circuit->node_list[input]->CC0;
            cc0 = cc0_input;
        }
        cc0 += 1;
    }
    return cc0;
}

int find_cc1(Gate *gate)
{
    int cc1 = -1;
    string gate_type = gate->type;
    if (gate_type == "AND")
    {
        cc1 = 0;
        for (auto input : gate->inputs)
        {
            int cc1_input = circuit->node_list[input]->CC1;
            cc1 += cc1_input;
        }
        cc1 += 1;
    }
    else if (gate_type == "OR")
    {
        cc1 = INT_MAX - 2;
        for (auto input : gate->inputs)
        {
            int cc1_input = circuit->node_list[input]->CC1;
            cc1 = min(cc1, cc1_input);
        }
        cc1 += 1;
    }
    else if (gate_type == "NAND")
    {
        cc1 = INT_MAX - 2;
        for (auto input : gate->inputs)
        {
            int cc0_input = circuit->node_list[input]->CC0;
            cc1 = min(cc1, cc0_input);
        }
        cc1 += 1;
    }
    else if (gate_type == "NOR")
    {
        cc1 = 0;
        for (auto input : gate->inputs)
        {
            int cc0_input = circuit->node_list[input]->CC0;
            cc1 += cc0_input;
        }
        cc1 += 1;
    }
    else if (gate_type == "XOR")
    {
        cc1 = xor_cc1_helper(gate);
        cc1 += 1;
    }
    else if (gate_type == "XNOR")
    {
        cc1 = xor_cc0_helper(gate);
        cc1 += 1;
    }
    else if (gate_type == "NOT")
    {
        for (auto input : gate->inputs)
        {
            int cc1_input = circuit->node_list[input]->CC0;
            cc1 = cc1_input;
        }
        cc1 += 1;
    }
    else if (gate_type == "BUF")
    {
        for (auto input : gate->inputs)
        {
            int cc1_input = circuit->node_list[input]->CC1;
            cc1 = cc1_input;
        }
        cc1 += 1;
    }
    return cc1;
}

int co_helper(Gate *gate, string input)
{
    int co_ = 0;
    string gate_type = gate->type;
    if (gate_type == "AND")
    {
        co_ = -1 * circuit->node_list[input]->CC1;
    }
    else if (gate_type == "OR")
    {
        co_ = -1 * circuit->node_list[input]->CC0;
    }
    else if (gate_type == "NAND")
    {
        co_ = -1 * circuit->node_list[input]->CC1;
    }
    else if (gate_type == "NOR")
    {
        co_ = -1 * circuit->node_list[input]->CC0;
    }
    else if (gate_type == "XOR")
    {
        if (gate->inputs[0] == input)
        {
            int cc0_input = circuit->node_list[gate->inputs[1]]->CC0;
            int cc1_input = circuit->node_list[gate->inputs[1]]->CC1;
            co_ = min(cc0_input, cc1_input);
        }
        else
        {
            int cc0_input = circuit->node_list[gate->inputs[0]]->CC0;
            int cc1_input = circuit->node_list[gate->inputs[0]]->CC1;
            co_ = min(cc0_input, cc1_input);
        }
    }
    else if (gate_type == "XNOR")
    {
        if (gate->inputs[0] == input)
        {
            int cc0_input = circuit->node_list[gate->inputs[1]]->CC0;
            int cc1_input = circuit->node_list[gate->inputs[1]]->CC1;
            co_ = min(cc0_input, cc1_input);
        }
        else
        {
            int cc0_input = circuit->node_list[gate->inputs[0]]->CC0;
            int cc1_input = circuit->node_list[gate->inputs[0]]->CC1;
            co_ = min(cc0_input, cc1_input);
        }
    }
    else if (gate_type == "NOT")
    {
        ;
    }
    else if (gate_type == "BUF")
    {
        ;
    }
    return co_;
}

int find_co(Gate *gate)
{
    int co = circuit->node_list[gate->outputs[0]]->CO;
    string gate_type = gate->type;
    if (gate_type == "AND")
    {
        int X1 = 0;
        for (auto input : gate->inputs)
            X1 += circuit->node_list[input]->CC1;
        co += X1 + 1;
    }
    else if (gate_type == "OR")
    {
        int X0 = 0;
        for (auto input : gate->inputs)
            X0 += circuit->node_list[input]->CC0;
        co += X0 + 1;
    }
    else if (gate_type == "NAND")
    {
        int X1 = 0;
        for (auto input : gate->inputs)
            X1 += circuit->node_list[input]->CC1;
        co += X1 + 1;
    }
    else if (gate_type == "NOR")
    {
        int X0 = 0;
        for (auto input : gate->inputs)
            X0 += circuit->node_list[input]->CC0;
        co += X0 + 1;
    }
    else if (gate_type == "XOR")
    {
        // co = xor_co_helper(gate);
        co += 1;
    }
    else if (gate_type == "XNOR")
    {
        // co = xor_cc0_helper(gate);
        co += 1;
    }
    else if (gate_type == "NOT")
    {
        co += 1;
    }
    else if (gate_type == "BUF")
    {
        co += 1;
    }
    return co;
}

void assign_controllability()
{
    // level = 1
    for (auto pi : circuit->primary_inputs)
    {
        Node *node = circuit->node_list[pi->name];
        node->CC0 = 1;
        node->CC1 = 1;
    }

    for (int i = 2; i <= circuit->no_of_levels; i++)
    {
        for (auto node : circuit->levelized_circuit[i])
        {
            if (node->type == "NON-WIRE")
            {
                Gate *gate = circuit->gate_node_list[node];
                int cc0 = find_cc0(gate);
                int cc1 = find_cc1(gate);
                for (auto output : gate->outputs)
                {
                    circuit->node_list[output]->CC0 = cc0;
                    circuit->node_list[output]->CC1 = cc1;
                }
            }
        }
    }
}

void assign_observability()
{
    // level = last
    for (auto po : circuit->primary_outputs)
    {
        Node *node = circuit->node_list[po->name];
        node->CO = 0;
    }

    for (int i = circuit->no_of_levels; i > 0; i--)
    {
        for (auto node : circuit->levelized_circuit[i])
        {
            if (node->type == "NON-WIRE")
            {
                Gate *gate = circuit->gate_node_list[node];
                int co = find_co(gate);
                for (auto input : gate->inputs)
                {
                    int co_ = co_helper(gate, input);
                    circuit->node_list[input]->CO = min(circuit->node_list[input]->CO, co + co_);
                }
            }
        }
    }
    return;
}

void assign_scoap()
{
    assign_controllability();
    // display_scoap_values();

    assign_observability();
    // display_scoap_values();
}

void read_file()
{
    string filename;
    cout << "Enter Input Text File: ";
    cin >> filename;
    // filename += "c2670";
    // filename += "c2670_T093";
    // filename = "./example_input_files/" + filename + ".txt";
    filename = "../input_text_files/" + filename + ".txt";
    ifstream file(filename);

    string line = "";
    while (getline(file, line))
    {
        vector<string> tmp;
        for (int i = 0; i < line.length(); i++)
        {
            string s = "";
            while (i < line.length() && line[i] != ' ')
            {
                s += line[i];
                i++;
            }
            if (s.length())
                tmp.push_back(s);
        }

        // prepare gate structure and circuit graph
        Gate *gate = new Gate();
        gate->type = tmp[0];
        gate->name = tmp[1];

        Node *gateNode = new Node(gate->name, "NON-WIRE");
        circuit->no_of_nodes++;
        circuit->node_list[gate->name] = gateNode;
        circuit->gate_node_list[gateNode] = gate;

        // if...else for tmp[0]
        {
            circuit->no_of_gates++;
            gate->outputs.push_back(tmp[2]);
            gateNode->outdeg++;

            if (!circuit->node_list[tmp[2]])
            {
                Node *outputNode = new Node(tmp[2], "WIRE");
                circuit->no_of_nodes++;
                outputNode->indeg++;
                gateNode->next.push_back(outputNode);
                circuit->node_list[tmp[2]] = outputNode;
            }
            else
            {
                circuit->node_list[tmp[2]]->indeg++;
                gateNode->next.push_back(circuit->node_list[tmp[2]]);
            }

            for (int i = 3; i < tmp.size(); i++)
            {
                gate->inputs.push_back(tmp[i]);
                gateNode->indeg++;

                if (!circuit->node_list[tmp[i]])
                {
                    Node *inputNode = new Node(tmp[i], "WIRE");
                    circuit->no_of_nodes++;
                    inputNode->outdeg++;
                    inputNode->next.push_back(gateNode);
                    circuit->node_list[tmp[i]] = inputNode;
                }
                else
                {
                    circuit->node_list[tmp[i]]->outdeg++;
                    circuit->node_list[tmp[i]]->next.push_back(gateNode);
                }
            }
        }
        circuit->gate_list.push_back(gate);
    }

    file.close();

    return;
}

int main()
{
    read_file();
    // display_gate_structure();
    // display_node_structure();

    gates_nodes_levelization();
    display_circuit_details();
    // display_gate_structure();
    // display_node_structure();
    // traverse_circuit();

    auto start = chrono::high_resolution_clock::now();
    assign_scoap();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    display_scoap_values();

    cout << "Time taken to calculate the SCOAP Values: " << duration << " ns" << endl;

    return 0;
}
