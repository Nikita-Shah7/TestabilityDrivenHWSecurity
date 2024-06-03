#include <bits/stdc++.h>
#include <dirent.h>
using namespace std;

class Gate
{
    string type;
    string name;
    vector<string> inputs;
    vector<string> outputs;

public:
    Gate();
    friend void parse_verilog_file(string);
    friend void generate_text_file(string);
    friend void oai321s1_module(string);
    friend void mux21_module_util(string, string, string, string, string);
    friend void mux21_module(string);
    friend void dff_module_util(string, string, string, string, string, string);
    friend void dff_module(string);
    friend void sdffs1_module(string);
};

Gate::Gate(void)
{
    name = "";
    type = "";
}

vector<Gate> gate_list;

void mux21_module_util(string name, string A, string B, string S, string Y)
{
    Gate gate;

    gate.type = "not";
    gate.name = name + "-NOT";
    gate.outputs.push_back(name + "-s-bar");
    gate.inputs.push_back(S);
    gate_list.push_back(gate);
    gate.inputs.clear();
    gate.outputs.clear();

    gate.type = "and";
    gate.name = name + "-AND-A";
    gate.outputs.push_back(name + "-output-A");
    gate.inputs.push_back(A);
    gate.inputs.push_back(name + "-s-bar");
    gate_list.push_back(gate);
    gate.inputs.clear();
    gate.outputs.clear();

    gate.type = "and";
    gate.name = name + "-AND-B";
    gate.outputs.push_back(name + "-output-B");
    gate.inputs.push_back(B);
    gate.inputs.push_back(S);
    gate_list.push_back(gate);
    gate.inputs.clear();
    gate.outputs.clear();

    gate.type = "or";
    gate.name = name + "-OR";
    gate.outputs.push_back(Y);
    gate.inputs.push_back(name + "-output-A");
    gate.inputs.push_back(name + "-output-B");
    gate_list.push_back(gate);
    gate.inputs.clear();
    gate.outputs.clear();
}

void mux21_module(string line)
{
    regex instance_name_regex("\\s+([^\\s]+)\\s*\\(");
    smatch instance_name_match;
    string name = "";
    if (regex_search(line, instance_name_match, instance_name_regex))
        name = instance_name_match[1].str();

    if (name == "")
        return;

    regex output_regex("\\.Q\\(([^\\)]*)\\)");
    smatch output_match;
    string output = "";
    if (regex_search(line, output_match, output_regex))
        output = output_match[1].str();

    if (output == "")
        return;

    regex select_line_regex("\\.SIN\\(([^\\)]*)\\)");
    smatch select_line_match;
    string select_line = "";
    if (regex_search(line, select_line_match, select_line_regex))
        select_line = select_line_match[1].str();

    if (select_line == "")
        return;

    regex inputA_regex("\\.DIN1\\(([^\\)]*)\\)");
    smatch inputA_match;
    string inputA = "";
    if (regex_search(line, inputA_match, inputA_regex))
        inputA = inputA_match[1].str();

    if (inputA == "")
        return;

    regex inputB_regex("\\.DIN2\\(([^\\)]*)\\)");
    smatch inputB_match;
    string inputB = "";
    if (regex_search(line, inputB_match, inputB_regex))
        inputB = inputB_match[1].str();

    if (inputB == "")
        return;

    mux21_module_util(name, inputA, inputB, select_line, output);
}

void oai321s1_module(string line)
{
    regex instance_name_regex("\\s+([^\\s]+)\\s*\\(");
    smatch instance_name_match;
    string name = "";
    if (regex_search(line, instance_name_match, instance_name_regex))
        name = instance_name_match[1].str();

    if (name == "")
        return;

    regex output_regex("\\.Q\\(([^\\)]*)\\)");
    smatch output_match;
    string output = "";
    if (regex_search(line, output_match, output_regex))
        output = output_match[1].str();

    if (output == "")
        return;

    vector<string> inputs;
    regex input_regex("\\.DIN\\d+\\(([^\\)]*)\\)");
    sregex_iterator iter(line.begin(), line.end(), input_regex);
    sregex_iterator end;
    while (iter != end)
    {
        smatch match = *iter;
        if (match[1].matched)
            inputs.push_back(match[1].str());
        if (match[2].matched)
            inputs.push_back(match[2].str());
        ++iter;
    }

    if (inputs.size() != 6)
        return;

    Gate gate;

    gate.type = "or";
    gate.name = name + "_1";
    gate.outputs.push_back(output + "_1");
    for (int i = 0; i < 3; i++)
        gate.inputs.push_back(inputs[i]);
    gate_list.push_back(gate);
    gate.inputs.clear();
    gate.outputs.clear();

    gate.type = "or";
    gate.name = name + "_2";
    gate.outputs.push_back(output + "_2");
    for (int i = 3; i < 6; i++)
        gate.inputs.push_back(inputs[i]);
    gate_list.push_back(gate);
    gate.inputs.clear();
    gate.outputs.clear();

    gate.type = "and";
    gate.name = name + "_3";
    gate.outputs.push_back(output + "_3");
    gate.inputs.push_back(output + "_1");
    gate.inputs.push_back(output + "_2");
    gate_list.push_back(gate);
    gate.inputs.clear();
    gate.outputs.clear();

    gate.type = "not";
    gate.name = name + "_4";
    gate.outputs.push_back(output);
    gate.inputs.push_back(output + "_3");
    gate_list.push_back(gate);
    gate.inputs.clear();
    gate.outputs.clear();
}

void dff_module_util(string name, string D, string EB, string CLK, string Q, string QN)
{
    Gate gate;
    gate.name = name;
    gate.type = "dff";
    gate.inputs.push_back(CLK);
    gate.inputs.push_back(D);
    gate.inputs.push_back(EB);
    gate.outputs.push_back(Q);
    gate.outputs.push_back(QN);
    gate_list.push_back(gate);
}

void dff_module(string line)
{
    // DFF <name> Q QN CLK DIN EB
    regex instance_name_regex("\\s+([^\\s]+)\\s*\\(");
    smatch instance_name_match;
    string name = "";
    if (regex_search(line, instance_name_match, instance_name_regex))
        name = instance_name_match[1].str();

    if (name == "")
        return;

    regex output_regex_Q("\\.Q\\(([^\\)]*)\\)");
    smatch output_match_Q;
    string output_Q = "";
    if (regex_search(line, output_match_Q, output_regex_Q))
        output_Q = output_match_Q[1].str();

    if (output_Q.length() == 0)
        output_Q = "**";

    regex output_regex_QN("\\.QN\\(([^\\)]*)\\)");
    smatch output_match_QN;
    string output_QN = "";
    if (regex_search(line, output_match_QN, output_regex_QN))
        output_QN = output_match_QN[1].str();

    if (output_QN.length() == 0)
        output_QN = "**";

    if (output_Q == "**" && output_QN == "**")
        return;

    regex input_regex_CLK("\\.CLK\\(([^\\)]*)\\)");
    smatch input_match_CLK;
    string clk = "";
    if (regex_search(line, input_match_CLK, input_regex_CLK))
        clk = input_match_CLK[1].str();

    if (clk == "")
        return;

    regex input_regex("\\.DIN\\(([^\\)]*)\\)");
    smatch input_match;
    string d = "";
    if (regex_search(line, input_match, input_regex))
        d = input_match[1].str();

    if (d == "")
        return;

    regex input_regex_EB("\\.EB\\(([^\\)]*)\\)");
    smatch input_match_EB;
    string eb = "";
    if (regex_search(line, input_match_EB, input_regex_EB))
        eb = input_match_EB[1].str();

    if (eb == "")
        eb = "**";

    dff_module_util(name, d, eb, clk, output_Q, output_QN);
}

void sdffs1_module(string line)
{
    // MUX2_1 <name-SDFF> 
    // DFF <name-SDFF> 
    regex instance_name_regex("\\s+([^\\s]+)\\s*\\(");
    smatch instance_name_match;
    string name = "";
    if (regex_search(line, instance_name_match, instance_name_regex))
        name = instance_name_match[1].str();

    if (name == "")
        return;

    regex input_regex_D("\\.DIN\\(([^\\)]*)\\)");
    smatch input_match_D;
    string d = "";
    if (regex_search(line, input_match_D, input_regex_D))
        d = input_match_D[1].str();

    if (d == "")
        return;

    regex input_regex_SDIN("\\.SDIN\\(([^\\)]*)\\)");
    smatch input_match_SDIN;
    string sd = "";
    if (regex_search(line, input_match_SDIN, input_regex_SDIN))
        sd = input_match_SDIN[1].str();

    if (sd == "")
        return;

    regex input_regex_SSEL("\\.SSEL\\(([^\\)]*)\\)");
    smatch input_match_SSEL;
    string ssel = "";
    if (regex_search(line, input_match_SSEL, input_regex_SSEL))
        ssel = input_match_SSEL[1].str();

    if (ssel == "")
        return;

    regex input_regex_CLK("\\.CLK\\(([^\\)]*)\\)");
    smatch input_match_CLK;
    string clk = "";
    if (regex_search(line, input_match_CLK, input_regex_CLK))
        clk = input_match_CLK[1].str();

    if (clk == "")
        return;

    regex output_regex_Q("\\.Q\\(([^\\)]*)\\)");
    smatch output_match_Q;
    string output_Q = "";
    if (regex_search(line, output_match_Q, output_regex_Q))
        output_Q = output_match_Q[1].str();

    if (output_Q.length() == 0)
        output_Q = "**";

    regex output_regex_QN("\\.QN\\(([^\\)]*)\\)");
    smatch output_match_QN;
    string output_QN = "";
    if (regex_search(line, output_match_QN, output_regex_QN))
        output_QN = output_match_QN[1].str();

    if (output_QN.length() == 0)
        output_QN = "**";


    if (output_Q == "**" && output_QN == "**")
        return;

    mux21_module_util(name + "-SDFF", d, sd, ssel, name + "-SDFF-MUX21");
    dff_module_util(name + "-SDFF", name + "-SDFF-MUX21", "**", clk, output_Q, output_QN);
}

void parse_verilog_file(string filename)
{
    regex nandRegex("^\\s*nnd");
    regex norRegex("^\\s*nor");
    regex andRegex("^\\s*and");
    regex orRegex("^\\s*or");
    regex xorRegex("^\\s*xor");
    regex xnorRegex("^\\s*xnr");
    regex mux21Regex("^\\s*mxi21");
    regex buf1Regex("^\\s*hi");
    regex buf2Regex("^\\s*nb");
    regex buf3Regex("^\\s*i");
    regex buf4Regex("^\\s*ib");
    regex oai321s1Regex("^\\s*oai321s1");
    regex dffRegex("^\\s*dff");
    regex sdffRegex("^\\s*sdff");

    ifstream file(filename);
    string line = "";
    while (getline(file, line))
    {
        // cout << line << endl;
        smatch match;
        Gate gate;

        if (regex_search(line, match, nandRegex))
        {
            gate.type = "nand";
        }
        else if (regex_search(line, match, andRegex))
        {
            gate.type = "and";
        }
        else if (regex_search(line, match, xnorRegex))
        {
            gate.type = "xnor";
        }
        else if (regex_search(line, match, xorRegex))
        {
            gate.type = "xor";
        }
        else if (regex_search(line, match, norRegex))
        {
            gate.type = "nor";
        }
        else if (regex_search(line, match, orRegex))
        {
            gate.type = "or";
        }
        else if (regex_search(line, match, mux21Regex))
        {
            mux21_module(line);
            continue;
        }
        else if (regex_search(line, match, buf1Regex) || regex_search(line, match, buf2Regex) || regex_search(line, match, buf3Regex) || regex_search(line, match, buf4Regex))
        {
            gate.type = "buf";
        }
        else if (regex_search(line, match, oai321s1Regex))
        {
            oai321s1_module(line);
            continue;
        }
        else if (regex_search(line, match, dffRegex))
        {
            dff_module(line);
            continue;
        }
        else if (regex_search(line, match, sdffRegex))
        {
            sdffs1_module(line);
            continue;
        }

        if (gate.type == "")
            continue;
        else if (gate.type == "buf")
        {
            regex instance_name_regex("\\s+([^\\s]+)\\s*\\(");
            smatch instance_name_match;
            // string name = "";
            if (regex_search(line, instance_name_match, instance_name_regex))
                gate.name = instance_name_match[1].str();

            if (gate.name == "")
                continue;

            regex output_regex("\\.Q\\(([^\\)]*)\\)");
            smatch output_match;
            // string output = "";
            if (regex_search(line, output_match, output_regex))
                gate.outputs.push_back(output_match[1].str());

            if (gate.outputs.size() == 0)
                continue;

            regex input_regex("\\.DIN\\(([^\\)]*)\\)");
            smatch input_match;
            if (regex_search(line, input_match, input_regex))
                gate.inputs.push_back(input_match[1].str());

            if (gate.inputs.size() == 0)
                continue;
        }
        else
        {
            regex instance_name_regex("\\s+([^\\s]+)\\s*\\(");
            smatch instance_name_match;
            // string name = "";
            if (regex_search(line, instance_name_match, instance_name_regex))
                gate.name = instance_name_match[1].str();

            if (gate.name == "")
                continue;

            regex output_regex("\\.Q\\(([^\\)]*)\\)");
            smatch output_match;
            // string output = "";
            if (regex_search(line, output_match, output_regex))
                gate.outputs.push_back(output_match[1].str());

            if (gate.outputs.size() == 0)
                continue;

            regex input_regex("\\.DIN\\d+\\(([^\\)]*)\\)");
            sregex_iterator iter(line.begin(), line.end(), input_regex);
            sregex_iterator end;
            while (iter != end)
            {
                smatch match = *iter;
                if (match[1].matched)
                    gate.inputs.push_back(match[1].str());
                if (match[2].matched)
                    gate.inputs.push_back(match[2].str());
                ++iter;
            }

            if (gate.inputs.size() == 0)
                continue;
        }

        gate_list.push_back(gate);
    }
}

void generate_text_file(string filename)
{
    ofstream output_file(filename);
    for (size_t i = 0; i < gate_list.size(); ++i)
    {
        auto gate = gate_list[i];
        string type = gate.type;
        transform(type.begin(), type.end(), type.begin(), ::toupper);
        output_file << type << " ";
        string name = gate.name;
        output_file << name << " ";
        for (auto output : gate.outputs)
        {
            output_file << output << " ";
        }
        for (auto input : gate.inputs)
        {
            output_file << input << " ";
        }
        if (i != gate_list.size() - 1)
            output_file << endl;
    }
    output_file.close();
}

void display(string filename)
{
    cout << "\nOUTPUT_FILE::\n";
    ifstream file(filename);
    string line = "";
    while (getline(file, line))
        cout << line << endl;

    file.close();
}

int main()
{
    string input_folder = "../verilog_files/";
    string output_folder = "../input_text_files/";

    // string file_name = "dump1";
    // string file_name = "c2670";
    // string file_name = "c2670_T093";
    string file_name = "s1423scan";
    string input_file = input_folder + file_name + ".v";
    string output_file = output_folder + file_name + ".txt";

    parse_verilog_file(input_file);
    generate_text_file(output_file);
    // display(output_file);

    return 0;
}
