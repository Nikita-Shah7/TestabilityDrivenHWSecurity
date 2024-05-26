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
class DFF;

class Circuit
{
    int no_of_pi, no_of_po, no_of_gates, no_of_nodes, no_of_fanouts;
    int no_of_dffs;
    int no_of_levels;
    vector<Node *> primary_inputs;
    vector<Node *> primary_outputs;
    vector<Gate *> gate_list;
    vector<DFF *> dff_list;
    unordered_map<string, Node *> node_list; // <nodeName,Node-pointer>
    unordered_map<Node *, Gate *> gate_node_list;
    unordered_map<Node *, DFF *> dff_node_list;
    map<int, vector<Node *>> levelized_circuit;

public:
    Circuit();

    friend void read_file();
    friend void display_circuit_details();
    friend void display_gate_structure();
    friend void display_dff_structure();
    friend void display_node_structure();
    friend void gates_nodes_levelization();
    friend void traverse_circuit();
    friend void assign_scoap();
    friend void assign_combinational_controllability();
    friend void assign_sequential_controllability();
    friend int xor_cc0_helper(Gate *);
    friend int xor_cc1_helper(Gate *);
    friend int find_cc0(Gate *);
    friend int find_cc1(Gate *);
    friend void display_scoap_values();
};

Circuit ::Circuit(void)
{
    no_of_pi = 0;
    no_of_po = 0;
    no_of_gates = 0;
    no_of_fanouts = 0;
    no_of_nodes = 0;
    no_of_dffs = 0;
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
    friend void display_dff_structure();
    friend void display_node_structure();
    friend void gates_nodes_levelization();
    friend void traverse_circuit();
    friend void assign_scoap();
    friend void assign_combinational_controllability();
    friend void assign_sequential_controllability();
    friend int xor_cc0_helper(Gate *);
    friend int xor_cc1_helper(Gate *);
    friend int find_cc0(Gate *);
    friend int find_cc1(Gate *);
    friend void display_scoap_values();
};

Gate ::Gate(void)
{
    name = "";
    type = "";
    level = 0;
}

class DFF
{
    string name;
    int level;
    string clk, D, Q;

public:
    DFF();
    DFF(string, string, string, string);

    friend void read_file();
    friend void display_circuit_details();
    friend void display_gate_structure();
    friend void display_dff_structure();
    friend void display_node_structure();
    friend void gates_nodes_levelization();
    friend void traverse_circuit();
    friend void assign_scoap();
    friend void assign_combinational_controllability();
    friend void assign_sequential_controllability();
    friend int xor_cc0_helper(Gate *);
    friend int xor_cc1_helper(Gate *);
    friend int find_cc0(Gate *);
    friend int find_cc1(Gate *);
    friend void display_scoap_values();
};

DFF ::DFF(void)
{
    name = "";
    level = 0;
    clk = "";
    D = "";
    Q = "";
}

DFF ::DFF(string nme, string c, string d, string q)
{
    name = nme;
    level = 0;
    clk = c;
    D = d;
    Q = q;
}

class Node
{
    string name;
    string type;
    int level;
    // Node *next;
    // vector just bcoz fanout gate will have may out-going Nodes
    // while other nodes will have only one out-going Node
    vector<Node *> next;
    int indeg, outdeg;
    int CC0, CC1;
    int SC0, SC1;

public:
    Node();
    Node(string, string);

    friend void read_file();
    friend void display_circuit_details();
    friend void display_gate_structure();
    friend void display_dff_structure();
    friend void display_node_structure();
    friend void gates_nodes_levelization();
    friend void traverse_circuit();
    friend void assign_scoap();
    friend void assign_combinational_controllability();
    friend void assign_sequential_controllability();
    friend int xor_cc0_helper(Gate *);
    friend int xor_cc1_helper(Gate *);
    friend int find_cc0(Gate *);
    friend int find_cc1(Gate *);
    friend void display_scoap_values();
};

Node ::Node()
{
    name = "";
    type = "";
    level = 0;
    indeg = 0;
    outdeg = 0;
    // next = NULL;
    CC0 = INT_MAX - 2;
    CC1 = INT_MAX - 2;
    SC0 = INT_MAX - 2;
    SC1 = INT_MAX - 2;
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
    SC0 = INT_MAX - 2;
    SC1 = INT_MAX - 2;
}

void display_circuit_details()
{
    divider();
    cout << "Circuit Details:";
    divider();
    cout << "No. of Primary Inputs: " << circuit->no_of_pi << endl;
    cout << "No. of Primary Outputs: " << circuit->no_of_po << endl;
    cout << "No. of Gates: " << circuit->no_of_gates << endl;
    cout << "No. of DFFs: " << circuit->no_of_dffs << endl;
    cout << "No. of Fanouts: " << circuit->no_of_fanouts << endl;
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

void display_dff_structure()
{
    divider();
    cout << "DFF Details:";
    divider();
    for (const auto &dff : circuit->dff_list)
    {
        cout << "Name: " << dff->name << endl;
        cout << "Level: " << dff->level << endl;
        cout << "Clock: " << dff->clk << endl;
        cout << "D: " << dff->D << endl;
        cout << "Q: " << dff->Q << endl;
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

    for (auto &gate : circuit->gate_list)
    {
        if (gate->type == "DFF") // to push DFF o/p
        {
            Node *dff_output_node = circuit->node_list[gate->outputs[0]];
            dff_output_node->indeg = 0;

            Node *dff_node = circuit->node_list[gate->name];
            dff_node->outdeg--;

            q.push(dff_output_node);
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
    cout << "Circuit Traversal:";
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
    cout << "Signal\tCC0\tCC1\tSC\tSC1";
    divider();
    for (int i = 1; i <= circuit->no_of_levels; i++)
    {
        for (auto node : circuit->levelized_circuit[i])
        {
            if (node->type == "WIRE")
            {
                cout << node->name << "\t" << node->CC0 << "\t" << node->CC1 << "\t" << node->SC0 << "\t" << node->SC1 << endl;
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
    else if (gate_type == "FANOUT")
    {
        for (auto input : gate->inputs)
        {
            int cc0_input = circuit->node_list[input]->CC0;
            cc0 = cc0_input;
        }
    }
    else if (gate_type == "MUX2_1")
    {
        cc0 = 0;
        int cc0_S = circuit->node_list[gate->inputs[0]]->CC0; // select-line
        int cc1_S = circuit->node_list[gate->inputs[0]]->CC1; // select-line
        int cc0_A = circuit->node_list[gate->inputs[1]]->CC0; // input A
        int cc1_A = circuit->node_list[gate->inputs[1]]->CC1; // input A
        int cc0_B = circuit->node_list[gate->inputs[2]]->CC0; // input B
        int cc1_B = circuit->node_list[gate->inputs[2]]->CC1; // input B
        int cc0_first = min(cc0_A, cc1_S + 1) + 1;
        int cc0_second = min(cc0_B, cc0_S) + 1;
        cc0 = cc0_first + cc0_second + 1;
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
            int cc1_input = circuit->node_list[input]->CC1;
            cc1 = min(cc1, cc1_input);
        }
        cc1 += 1;
    }
    else if (gate_type == "NOR")
    {
        cc1 = 0;
        for (auto input : gate->inputs)
        {
            int cc1_input = circuit->node_list[input]->CC1;
            cc1 += cc1_input;
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
    else if (gate_type == "FANOUT")
    {
        for (auto input : gate->inputs)
        {
            int cc1_input = circuit->node_list[input]->CC1;
            cc1 = cc1_input;
        }
    }
    else if (gate_type == "MUX2_1")
    {
        cc1 = 0;
        int cc0_S = circuit->node_list[gate->inputs[0]]->CC0; // select-line
        int cc1_S = circuit->node_list[gate->inputs[0]]->CC1; // select-line
        int cc0_A = circuit->node_list[gate->inputs[1]]->CC0; // input A
        int cc1_A = circuit->node_list[gate->inputs[1]]->CC1; // input A
        int cc0_B = circuit->node_list[gate->inputs[2]]->CC0; // input B
        int cc1_B = circuit->node_list[gate->inputs[2]]->CC1; // input B
        int cc1_first = cc1_A + cc0_S + 1 + 1;
        int cc1_second = cc1_B + cc1_S + 1;
        cc1 = min(cc1_first, cc1_second) + 1;
    }
    return cc1;
}

void assign_combinational_controllability()
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

void assign_sequential_controllability()
{
}

void assign_scoap()
{
    assign_combinational_controllability();
    // display_scoap_values();

    assign_sequential_controllability();
    // display_scoap_values();
}

void read_file()
{
    string filename;
    cout << "Enter Input Text File: ";
    // cin >> filename;
    filename += "a10";
    filename = "./example_input_files/" + filename + ".txt";
    ifstream file(filename);

    string line = "";
    int ind = 1;

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
            tmp.push_back(s);
        }

        // prepare gate structure and circuit graph
        Gate *gate = new Gate();
        gate->name = "G" + to_string(ind++);
        gate->type = tmp[0];

        Node *gateNode = new Node(gate->name, "NON-WIRE");
        circuit->no_of_nodes++;
        circuit->node_list[gate->name] = gateNode;
        circuit->gate_node_list[gateNode] = gate;

        if (tmp[0] == "FANOUT")
        {
            circuit->no_of_fanouts++;
            gate->inputs.push_back(tmp[1]);
            gateNode->indeg++;

            if (!circuit->node_list[tmp[1]])
            {
                Node *inputNode = new Node(tmp[1], "WIRE");
                circuit->no_of_nodes++;
                inputNode->outdeg++;
                inputNode->next.push_back(gateNode);
                circuit->node_list[tmp[1]] = inputNode;
            }
            else
            {
                circuit->node_list[tmp[1]]->outdeg++;
                circuit->node_list[tmp[1]]->next.push_back(gateNode);
            }

            for (int i = 2; i < tmp.size(); i++)
            {
                gate->outputs.push_back(tmp[i]);
                gateNode->outdeg++;

                if (!circuit->node_list[tmp[i]])
                {
                    Node *outputNode = new Node(tmp[i], "WIRE");
                    circuit->no_of_nodes++;
                    outputNode->indeg++;
                    gateNode->next.push_back(outputNode);
                    circuit->node_list[tmp[i]] = outputNode;
                }
                else
                {
                    circuit->node_list[tmp[i]]->indeg++;
                    gateNode->next.push_back(circuit->node_list[tmp[i]]);
                }
            }
        }
        else
        {
            gate->outputs.push_back(tmp[1]);
            gateNode->outdeg++;

            if (tmp[0] == "DFF")
            {
                DFF *dff = new DFF(gate->name, tmp[2], tmp[3], tmp[1]);
                circuit->dff_list.push_back(dff);
                circuit->dff_node_list[gateNode] = dff;
                circuit->no_of_dffs++;
            }
            else
            {
                circuit->no_of_gates++;
            }

            if (!circuit->node_list[tmp[1]])
            {
                Node *outputNode = new Node(tmp[1], "WIRE");
                circuit->no_of_nodes++;
                outputNode->indeg++;
                gateNode->next.push_back(outputNode);
                circuit->node_list[tmp[1]] = outputNode;
            }
            else
            {
                circuit->node_list[tmp[1]]->indeg++;
                gateNode->next.push_back(circuit->node_list[tmp[1]]);
            }

            for (int i = 2; i < tmp.size(); i++)
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
    display_circuit_details();
    display_gate_structure();
    display_dff_structure();
    display_node_structure();

    // gates_nodes_levelization();
    // display_circuit_details();
    // display_gate_structure();
    // display_node_structure();
    // traverse_circuit();

    // assign_scoap();
    // display_scoap_values();

    return 0;
}