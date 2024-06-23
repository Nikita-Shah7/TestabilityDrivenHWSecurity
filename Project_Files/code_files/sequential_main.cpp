#include <bits/stdc++.h>
#include <dirent.h>
using namespace std;

#define ERR 100
#define INF 1e8

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
    int no_of_pi, no_of_po, no_of_gates, no_of_nodes;
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
    ~Circuit();

    friend void read_file(string);
    friend void display_circuit_details();
    friend void display_gate_structure();
    friend void display_dff_structure();
    friend void display_node_structure();
    friend void gates_nodes_levelization();
    friend void traverse_circuit();
    friend void assign_scoap();
    friend void assign_combinational_controllability();
    friend void assign_sequential_controllability();
    friend void initialization();
    friend int xor_cc0_helper(Gate *);
    friend int xor_cc1_helper(Gate *);
    friend int find_cc0(Gate *);
    friend int find_cc1(Gate *);
    friend int xor_sc0_helper(Gate *);
    friend int xor_sc1_helper(Gate *);
    friend int find_sc0(Gate *);
    friend int find_sc1(Gate *);
    friend void display_scoap_values();
    friend void generate_output_file(string, int);
    friend void clear_circuit_details();
};

Circuit ::Circuit(void)
{
    no_of_pi = 0;
    no_of_po = 0;
    no_of_gates = 0;
    no_of_nodes = 0;
    no_of_dffs = 0;
    no_of_levels = 0;
}

Circuit ::~Circuit()
{
    no_of_pi = 0;
    no_of_po = 0;
    no_of_gates = 0;
    no_of_dffs = 0;
    no_of_nodes = 0;
    no_of_levels = 0;
    primary_inputs.clear();
    primary_outputs.clear();
    gate_list.clear();
    dff_list.clear();
    node_list.clear();
    gate_node_list.clear();
    dff_node_list.clear();
    levelized_circuit.clear();
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

    friend void read_file(string);
    friend void display_circuit_details();
    friend void display_gate_structure();
    friend void display_dff_structure();
    friend void display_node_structure();
    friend void gates_nodes_levelization();
    friend void traverse_circuit();
    friend void assign_scoap();
    friend void assign_combinational_controllability();
    friend void assign_sequential_controllability();
    friend void initialization();
    friend int xor_cc0_helper(Gate *);
    friend int xor_cc1_helper(Gate *);
    friend int find_cc0(Gate *);
    friend int find_cc1(Gate *);
    friend int xor_sc0_helper(Gate *);
    friend int xor_sc1_helper(Gate *);
    friend int find_sc0(Gate *);
    friend int find_sc1(Gate *);
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
    string CLK, D, Q, QN, EB;

public:
    DFF();
    DFF(string, string, string, string, string, string);

    friend void read_file(string);
    friend void display_circuit_details();
    friend void display_gate_structure();
    friend void display_dff_structure();
    friend void display_node_structure();
    friend void gates_nodes_levelization();
    friend void traverse_circuit();
    friend void assign_scoap();
    friend void assign_combinational_controllability();
    friend void assign_sequential_controllability();
    friend void initialization();
    friend int xor_cc0_helper(Gate *);
    friend int xor_cc1_helper(Gate *);
    friend int find_cc0(Gate *);
    friend int find_cc1(Gate *);
    friend int xor_sc0_helper(Gate *);
    friend int xor_sc1_helper(Gate *);
    friend int find_sc0(Gate *);
    friend int find_sc1(Gate *);
    friend void display_scoap_values();
};

DFF ::DFF(void)
{
    name = "";
    level = 0;
    CLK = "";
    D = "";
    Q = "";
    QN = "";
    EB = "";
}

DFF ::DFF(string nme, string q, string qin, string c, string d, string eb)
{
    name = nme;
    level = 0;
    Q = q;
    QN = qin;
    CLK = c;
    D = d;
    EB = eb;
}

class Node
{
    string name;
    string type;
    int level;
    vector<Node *> next;
    int indeg, outdeg;
    int CC0, CC1;
    int SC0, SC1;

public:
    Node();
    Node(string, string);

    friend void read_file(string);
    friend void display_circuit_details();
    friend void display_gate_structure();
    friend void display_dff_structure();
    friend void display_node_structure();
    friend void gates_nodes_levelization();
    friend void traverse_circuit();
    friend void assign_scoap();
    friend void assign_combinational_controllability();
    friend void assign_sequential_controllability();
    friend void initialization();
    friend int xor_cc0_helper(Gate *);
    friend int xor_cc1_helper(Gate *);
    friend int find_cc0(Gate *);
    friend int find_cc1(Gate *);
    friend int xor_sc0_helper(Gate *);
    friend int xor_sc1_helper(Gate *);
    friend int find_sc0(Gate *);
    friend int find_sc1(Gate *);
    friend void display_scoap_values();
    friend void generate_output_file(string, int);
};

Node ::Node()
{
    name = "";
    type = "";
    level = 0;
    indeg = 0;
    outdeg = 0;
    CC0 = INF - 2;
    CC1 = INF - 2;
    SC0 = INF - 2;
    SC1 = INF - 2;
}

Node ::Node(string nme, string ty)
{
    name = nme;
    type = ty;
    level = 0;
    indeg = 0;
    outdeg = 0;
    CC0 = INF - 2;
    CC1 = INF - 2;
    SC0 = INF - 2;
    SC1 = INF - 2;
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
        cout << "Clock: " << dff->CLK << endl;
        cout << "EB: " << dff->EB << endl;
        cout << "D: " << dff->D << endl;
        cout << "Q: " << dff->Q << endl;
        cout << "QN: " << dff->QN << endl;
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

    for (auto &dff : circuit->dff_list)
    {
        Node *dff_node = circuit->node_list[dff->name];
        if (dff->Q != "**")
        {
            Node *dff_output_node_Q = circuit->node_list[dff->Q];
            dff_output_node_Q->indeg = 0;
            dff_node->outdeg--;
            q.push(dff_output_node_Q);
        }
        if (dff->QN != "**")
        {
            Node *dff_output_node_QN = circuit->node_list[dff->QN];
            dff_output_node_QN->indeg = 0;
            dff_node->outdeg--;
            q.push(dff_output_node_QN);
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
                if (circuit->dff_node_list[curr])
                {
                    DFF *dff = circuit->dff_node_list[curr];
                    dff->level = lvl;
                }
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
    cout << "Signal\tCC0\tCC1\tSC0\tSC1";
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

int xor_sc0_helper(Gate *gate)
{
    vector<int> sc0, sc1;
    for (auto input : gate->inputs)
    {
        sc0.push_back(circuit->node_list[input]->SC0);
        sc1.push_back(circuit->node_list[input]->SC1);
    }
    sort(sc0.begin(), sc0.end());
    sort(sc1.begin(), sc1.end());
    int n = gate->inputs.size();
    for (int i = 1; i < n; i++)
        sc0[i] += sc0[i - 1];
    for (int i = 1; i < n; i++)
        sc1[i] += sc1[i - 1];
    int p1 = -1, p0 = n - 1;
    int sc0_input = sc0[p0]; // taking all zeros
    p1 += 2;
    p0 -= 2;
    while (p1 < n && p0 >= 0)
    {
        sc0_input = min(sc0_input, sc1[p1] + sc0[p0]);
        p1 += 2;
        p0 -= 2;
    }
    return sc0_input;
}

int xor_sc1_helper(Gate *gate)
{
    vector<int> sc0, sc1;
    for (auto input : gate->inputs)
    {
        sc0.push_back(circuit->node_list[input]->SC0);
        sc1.push_back(circuit->node_list[input]->SC1);
    }
    sort(sc0.begin(), sc0.end());
    sort(sc1.begin(), sc1.end());
    int n = gate->inputs.size();
    for (int i = 1; i < n; i++)
        sc0[i] += sc0[i - 1];
    for (int i = 1; i < n; i++)
        sc1[i] += sc1[i - 1];
    int p1 = 0, p0 = n - 2;
    int sc1_input = sc1[p1] + sc0[p0]; // taking one 1 and rest 0s
    p1 += 2;
    p0 -= 2;
    while (p1 < n && p0 >= 0)
    {
        sc1_input = min(sc1_input, sc1[p1] + sc0[p0]);
        p1 += 2;
        p0 -= 2;
    }
    return sc1_input;
}

int find_cc0(Gate *gate)
{
    int cc0 = -1;
    string gate_type = gate->type;
    if (gate_type == "AND")
    {
        cc0 = INF - 2;
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
            if (cc0_input > INF - ERR)
            {
                cc0 = INF - 2;
                break;
            }
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
            if (cc1_input > INF - ERR)
            {
                cc0 = INF - 2;
                break;
            }
            cc0 += cc1_input;
        }
        cc0 += 1;
    }
    else if (gate_type == "NOR")
    {
        cc0 = INF - 2;
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
    else if (gate_type == "DFF")
    {
        int cc0_clk = circuit->node_list[gate->inputs[0]]->CC0;
        int cc1_clk = circuit->node_list[gate->inputs[0]]->CC1;
        int cc0_d = circuit->node_list[gate->inputs[1]]->CC0;
        if (cc0_d > INF - 4)
            cc0 = INF;
        else
            cc0 = cc0_clk + cc1_clk + cc0_d;
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
            if (cc1_input > INF - 4)
            {
                cc1 = INF - 2;
                break;
            }
            cc1 += cc1_input;
        }
        cc1 += 1;
    }
    else if (gate_type == "OR")
    {
        cc1 = INF - 2;
        for (auto input : gate->inputs)
        {
            int cc1_input = circuit->node_list[input]->CC1;
            cc1 = min(cc1, cc1_input);
        }
        cc1 += 1;
    }
    else if (gate_type == "NAND")
    {
        cc1 = INF - 2;
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
            if (cc0_input > INF - 4)
            {
                cc1 = INF - 2;
                break;
            }
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
    else if (gate_type == "DFF")
    {
        int cc0_clk = circuit->node_list[gate->inputs[0]]->CC0;
        int cc1_clk = circuit->node_list[gate->inputs[0]]->CC1;
        int cc1_d = circuit->node_list[gate->inputs[1]]->CC1;
        if (cc1_d > INF - 4)
            cc1 = INF;
        else
            cc1 = cc0_clk + cc1_clk + cc1_d;
    }
    return cc1;
}

int find_sc0(Gate *gate)
{
    int sc0 = -1;
    string gate_type = gate->type;
    if (gate_type == "AND")
    {
        sc0 = INF - 2;
        for (auto input : gate->inputs)
        {
            int sc0_input = circuit->node_list[input]->SC0;
            sc0 = min(sc0, sc0_input);
        }
    }
    else if (gate_type == "OR")
    {
        sc0 = 0;
        for (auto input : gate->inputs)
        {
            int sc0_input = circuit->node_list[input]->SC0;
            if (sc0_input > INF - 4)
            {
                sc0 = INF - 2;
                break;
            }
            sc0 += sc0_input;
        }
    }
    else if (gate_type == "NAND")
    {
        sc0 = 0;
        for (auto input : gate->inputs)
        {
            int sc1_input = circuit->node_list[input]->SC1;
            if (sc1_input > INF - 4)
            {
                sc0 = INF - 2;
                break;
            }
            sc0 += sc1_input;
        }
    }
    else if (gate_type == "NOR")
    {
        sc0 = INF - 2;
        for (auto input : gate->inputs)
        {
            int sc1_input = circuit->node_list[input]->SC1;
            sc0 = min(sc0, sc1_input);
        }
    }
    else if (gate_type == "XOR")
    {
        sc0 = xor_sc0_helper(gate);
    }
    else if (gate_type == "XNOR")
    {
        sc0 = xor_sc1_helper(gate);
    }
    else if (gate_type == "NOT")
    {
        for (auto input : gate->inputs)
        {
            int sc1_input = circuit->node_list[input]->SC1;
            sc0 = sc1_input;
        }
    }
    else if (gate_type == "BUF")
    {
        for (auto input : gate->inputs)
        {
            int sc0_input = circuit->node_list[input]->SC0;
            sc0 = sc0_input;
        }
    }
    else if (gate_type == "DFF")
    {
        int sc0_clk = circuit->node_list[gate->inputs[0]]->SC0;
        int sc1_clk = circuit->node_list[gate->inputs[0]]->SC1;
        int sc0_d = circuit->node_list[gate->inputs[1]]->SC0;
        if (sc0_d > INF - 4)
            sc0 = INF;
        else
            sc0 = sc0_clk + sc1_clk + sc0_d + 1;
    }
    return sc0;
}

int find_sc1(Gate *gate)
{
    int sc1 = -1;
    string gate_type = gate->type;
    if (gate_type == "AND")
    {
        sc1 = 0;
        for (auto input : gate->inputs)
        {
            int sc1_input = circuit->node_list[input]->SC1;
            if (sc1_input > INF - 4)
            {
                sc1 = INF - 2;
                break;
            }
            sc1 += sc1_input;
        }
    }
    else if (gate_type == "OR")
    {
        sc1 = INF - 2;
        for (auto input : gate->inputs)
        {
            int sc1_input = circuit->node_list[input]->SC1;
            sc1 = min(sc1, sc1_input);
        }
    }
    else if (gate_type == "NAND")
    {
        sc1 = INF - 2;
        for (auto input : gate->inputs)
        {
            int sc0_input = circuit->node_list[input]->SC0;
            sc1 = min(sc1, sc0_input);
        }
    }
    else if (gate_type == "NOR")
    {
        sc1 = 0;
        for (auto input : gate->inputs)
        {
            int sc0_input = circuit->node_list[input]->SC0;
            if (sc0_input > INF - 4)
            {
                sc1 = INF - 2;
                break;
            }
            sc1 += sc0_input;
        }
    }
    else if (gate_type == "XOR")
    {
        sc1 = xor_sc1_helper(gate);
    }
    else if (gate_type == "XNOR")
    {
        sc1 = xor_sc0_helper(gate);
    }
    else if (gate_type == "NOT")
    {
        for (auto input : gate->inputs)
        {
            int sc1_input = circuit->node_list[input]->SC0;
            sc1 = sc1_input;
        }
    }
    else if (gate_type == "BUF")
    {
        for (auto input : gate->inputs)
        {
            int sc1_input = circuit->node_list[input]->SC1;
            sc1 = sc1_input;
        }
    }
    else if (gate_type == "DFF")
    {
        int sc0_clk = circuit->node_list[gate->inputs[0]]->SC0;
        int sc1_clk = circuit->node_list[gate->inputs[0]]->SC1;
        int sc1_d = circuit->node_list[gate->inputs[1]]->SC1;
        if (sc1_d > INF - 4)
            sc1 = INF;
        else
            sc1 = sc0_clk + sc1_clk + sc1_d + 1;
    }
    return sc1;
}

void initialization()
{
    // level = 1 gates i/p
    for (auto pi : circuit->primary_inputs)
    {
        Node *node = circuit->node_list[pi->name];
        node->CC0 = 1;
        node->CC1 = 1;
        node->SC0 = 0;
        node->SC1 = 0;
    }

    // DFF o/p
    for (auto &dff : circuit->dff_list)
    {
        if (dff->Q != "**")
        {
            Node *dff_Q = circuit->node_list[dff->Q];
            dff_Q->CC0 = INF - 2;
            dff_Q->CC1 = INF - 2;
            dff_Q->SC0 = INF - 2;
            dff_Q->SC1 = INF - 2;
        }
        if (dff->QN != "**")
        {
            Node *dff_QN = circuit->node_list[dff->QN];
            dff_QN->CC0 = INF - 2;
            dff_QN->CC1 = INF - 2;
            dff_QN->SC0 = INF - 2;
            dff_QN->SC1 = INF - 2;
        }
    }
    return;
}

void assign_combinational_controllability()
{
    int iteration = 1;

    while (iteration)
    {
        bool flag = 0;
        for (int i = 2; i <= circuit->no_of_levels; i++)
        {
            for (auto node : circuit->levelized_circuit[i])
            {
                if (node->type == "NON-WIRE")
                {
                    Gate *gate = circuit->gate_node_list[node];
                    int cc0 = find_cc0(gate);
                    int cc1 = find_cc1(gate);
                    if (cc0 > INF - ERR)
                        cc0 = INF - 2;
                    if (cc1 > INF - ERR)
                        cc1 = INF - 2;
                    for (auto output : gate->outputs)
                    {
                        if (output == "**")
                            continue;
                        circuit->node_list[output]->CC0 = cc0;
                        circuit->node_list[output]->CC1 = cc1;
                    }
                    if (cc0 > INF - ERR)
                        flag = 1;
                    if (cc1 > INF - ERR)
                        flag = 1;
                }
            }
        }
        // cout << "\n\nITERATION " << iteration << "::";
        // display_scoap_values();

        if (!flag)
            iteration = 0;
        else
            iteration++;
    }
}

void assign_sequential_controllability()
{
    int iteration = 1;

    while (iteration)
    {
        bool flag = 0;
        for (int i = 2; i <= circuit->no_of_levels; i++)
        {
            for (auto node : circuit->levelized_circuit[i])
            {
                if (node->type == "NON-WIRE")
                {
                    Gate *gate = circuit->gate_node_list[node];
                    int sc0 = find_sc0(gate);
                    int sc1 = find_sc1(gate);
                    if (sc0 > INF - ERR)
                        sc0 = INF - 2;
                    if (sc1 > INF - ERR)
                        sc1 = INF - 2;
                    for (auto output : gate->outputs)
                    {
                        if (output == "**")
                            continue;
                        circuit->node_list[output]->SC0 = sc0;
                        circuit->node_list[output]->SC1 = sc1;
                    }
                    if (sc0 > INF - ERR)
                        flag = 1;
                    if (sc1 > INF - ERR)
                        flag = 1;
                }
            }
        }
        // cout << "\n\nITERATION " << iteration << "::";
        // display_scoap_values();

        if (!flag)
            iteration = 0;
        else
            iteration++;
    }
}

void assign_scoap()
{
    // cout << "\n\nINITIALIZATION ::";
    initialization();
    // display_scoap_values();

    assign_combinational_controllability();
    // display_scoap_values();

    assign_sequential_controllability();
    // display_scoap_values();
}

void generate_output_file(string filename, int duration)
{
    ofstream output_file(filename);

    // 1. CIRCUIT DETAILS::
    output_file << "\n--------------------------------------------------\n";
    output_file << "Circuit Details:";
    output_file << "\n--------------------------------------------------\n";
    output_file << "No. of Primary Inputs: " << circuit->no_of_pi << endl;
    output_file << "No. of Primary Outputs: " << circuit->no_of_po << endl;
    output_file << "No. of Gates: " << circuit->no_of_gates << endl;
    output_file << "No. of DFFs: " << circuit->no_of_dffs << endl;
    output_file << "No. of Nodes: " << circuit->no_of_nodes << endl;
    // Time taken to calculate the SCOAP Values::
    output_file << "Execution Time: " << duration << " ns" << endl;
    output_file << "Primary Inputs:";
    for (auto pi : circuit->primary_inputs)
    {
        output_file << " " << pi->name;
    }
    output_file << endl;
    output_file << "Outputs:";
    for (auto po : circuit->primary_outputs)
    {
        output_file << " " << po->name;
    }
    output_file << endl;
    output_file << endl;

    // 2. SCOAP VALUES::
    output_file << "\n----------------------------------------\n";
    output_file << "Signal\tCC0\tCC1\tSC0\tSC1";
    output_file << "\n----------------------------------------\n";
    for (int i = 1; i <= circuit->no_of_levels; i++)
    {
        for (auto node : circuit->levelized_circuit[i])
        {
            if (node->type == "WIRE")
            {
                output_file << node->name << "\t" << node->CC0 << "\t" << node->CC1 << "\t" << node->SC0 << "\t" << node->SC1 << endl;
            }
        }
    }
    output_file.close();
}

void clear_circuit_details()
{
    circuit->no_of_pi = 0;
    circuit->no_of_po = 0;
    circuit->no_of_gates = 0;
    circuit->no_of_dffs = 0;
    circuit->no_of_nodes = 0;
    circuit->no_of_levels = 0;
    circuit->primary_inputs.clear();
    circuit->primary_outputs.clear();
    circuit->gate_list.clear();
    circuit->dff_list.clear();
    circuit->node_list.clear();
    circuit->gate_node_list.clear();
    circuit->dff_node_list.clear();
    circuit->levelized_circuit.clear();
}

void read_file(string filename)
{
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
        if (tmp[0] == "DFF")
        {
            // Q QN CLK D EB
            DFF *dff = new DFF(gate->name, tmp[2], tmp[3], tmp[4], tmp[5], tmp[6]);
            circuit->dff_list.push_back(dff);
            circuit->dff_node_list[gateNode] = dff;
            circuit->no_of_dffs++;

            for (int i = 2; i <= 3; i++)
            {
                gate->outputs.push_back(tmp[i]);
                if (tmp[i] == "**")
                    continue;
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

            for (int i = 4; i <= 6; i++)
            {
                gate->inputs.push_back(tmp[i]);
                if (tmp[i] == "**")
                    continue;
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
        else
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

void read_files()
{
    string input_folder = "../input_text_files/sequential/";
    string output_folder = "../output_text_files/sequential/";

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(input_folder.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            string sub_dir_name = ent->d_name;
            if (sub_dir_name == "." || sub_dir_name == "..")
                continue;
            cout << sub_dir_name << endl;

            string input_sub_folder = input_folder + sub_dir_name + "/";
            string output_sub_folder = output_folder + sub_dir_name + "/";
            DIR *sub_dir = opendir(input_sub_folder.c_str());
            if (sub_dir != NULL)
            {
                struct dirent *sub_dir_ent;
                while ((sub_dir_ent = readdir(sub_dir)) != NULL)
                {
                    string sub_dir_ent_name = sub_dir_ent->d_name;
                    if (sub_dir_ent_name == "." || sub_dir_ent_name == "..")
                        continue;
                    cout << " - " << sub_dir_ent_name << endl;

                    string file_name = sub_dir_ent_name;
                    string input_file = input_sub_folder + file_name;
                    string output_file = output_sub_folder + file_name;

                    // cout<<input_file<<endl<<output_file<<endl;

                    read_file(input_file);
                    // display_gate_structure();
                    // display_dff_structure();
                    // display_node_structure();

                    gates_nodes_levelization();
                    // display_circuit_details();
                    // display_gate_structure();
                    // display_dff_structure();
                    // display_node_structure();
                    // traverse_circuit();

                    auto start = chrono::high_resolution_clock::now();
                    assign_scoap();
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                    // display_scoap_values();
                    generate_output_file(output_file, duration);
                    clear_circuit_details();
                }
                closedir(sub_dir);
            }
            else
            {
                cerr << "Error opening sub_directory " << input_sub_folder << endl;
                return;
            }
        }
        closedir(dir);
    }
    else
    {
        cerr << "Error opening directory " << input_folder << endl;
        return;
    }
}

int main()
{

    read_files();

    // string filename;
    // // cout << "Enter Input Text File: ";
    // // cin >> filename;
    // // filename += "b1";
    // filename += "s1423_T400";

    // string input_folder = "../input_text_files/sequential/s1423/";
    // string output_folder = "../output_text_files/sequential/s1423/";

    // // string input_folder = "../input_text_files/";
    // // string output_folder = "../output_text_files/";

    // string input_file = input_folder + filename + ".txt";
    // string output_file = output_folder + filename + ".txt";

    // read_file(input_file);
    // // display_gate_structure();
    // // display_dff_structure();
    // // display_node_structure();

    // gates_nodes_levelization();
    // // display_circuit_details();
    // // display_gate_structure();
    // // display_dff_structure();
    // // display_node_structure();
    // // traverse_circuit();

    // auto start = chrono::high_resolution_clock::now();
    // assign_scoap();
    // auto end = chrono::high_resolution_clock::now();
    // auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    // // display_scoap_values();
    // generate_output_file(output_file, duration);

    // // cout << "Time taken to calculate the SCOAP Values: " << duration << " ns" << endl;

    return 0;
}
