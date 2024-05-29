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
    friend void mux21_module(string);
};

Gate::Gate(void)
{
    name = "";
    type = "";
}

vector<Gate> gate_list;

void mux21_module(string line) {
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
    
    if(select_line=="") return;


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

    if (inputs.size() != 2)
        return;


    Gate gate;

    gate.type = "not";
    gate.name = name + "_1";
    gate.outputs.push_back(output + "_1");
    gate.inputs.push_back(select_line);
    gate_list.push_back(gate);
    gate.inputs.clear();
    gate.outputs.clear();

    gate.type = "and";
    gate.name = name + "_2";
    gate.outputs.push_back(output + "_2");
    gate.inputs.push_back(inputs[0]);
    gate.inputs.push_back(output + "_1");
    gate_list.push_back(gate);
    gate.inputs.clear();
    gate.outputs.clear();

    gate.type = "and";
    gate.name = name + "_3";
    gate.outputs.push_back(output + "_3");
    gate.inputs.push_back(inputs[1]);
    gate.inputs.push_back(select_line);
    gate_list.push_back(gate);
    gate.inputs.clear();
    gate.outputs.clear();

    gate.type = "or";
    gate.name = name + "_4";
    gate.outputs.push_back(output);
    gate.inputs.push_back(output + "_2");
    gate.inputs.push_back(output + "_3");
    gate_list.push_back(gate);
    gate.inputs.clear();
    gate.outputs.clear();
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

void parse_verilog_file(string filename)
{
    // regex norRegex("nor2s1\\s+U\\d+\\s*\\(\\s*\\.Q\\([^\\)]*\\)(?:,\\s*\\.DIN\\d+\\([^\\)]*\\)){1,}\\s*\\);");
    // regex nandRegex("nnd\\s+U\\d+\\s*\\(\\s*\\.Q\\([^\\)]*\\)(?:,\\s*\\.DIN\\d+\\([^\\)]*\\)){1,}\\s*\\);");
    // regex andRegex("and2s1\\s+U\\d+\\s*\\(\\s*\\.Q\\([^\\)]*\\)(?:,\\s*\\.DIN\\d+\\([^\\)]*\\)){1,}\\s*\\);");
    // regex orRegex("or2s1\\s+U\\d+\\s*\\(\\s*\\.Q\\([^\\)]*\\)(?:,\\s*\\.DIN\\d+\\([^\\)]*\\)){1,}\\s*\\);");
    // regex xorRegex("xor2s1\\s+U\\d+\\s*\\(\\s*\\.Q\\([^\\)]*\\)(?:,\\s*\\.DIN\\d+\\([^\\)]*\\)){1,}\\s*\\);");
    // regex xnorRegex("xnor2s1\\s+U\\d+\\s*\\(\\s*\\.Q\\([^\\)]*\\)(?:,\\s*\\.DIN\\d+\\([^\\)]*\\)){1,}\\s*\\);");
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
            gate.type = "dff";
        }

        if (gate.type == "")
            continue;
        else if (gate.type == "dff")
        {
            // DFF <name> Q QN CLK DIN EB
            regex instance_name_regex("\\s+([^\\s]+)\\s*\\(");
            smatch instance_name_match;
            // string name = "";
            if (regex_search(line, instance_name_match, instance_name_regex))
                gate.name = instance_name_match[1].str();

            if (gate.name == "")
                continue;

            regex output_regex_Q("\\.Q\\(([^\\)]*)\\)");
            smatch output_match_Q;
            string output_Q = "";
            if (regex_search(line, output_match_Q, output_regex_Q))
                output_Q = output_match_Q[1].str();

            if (output_Q.size()==0)
                output_Q = "**";
            gate.outputs.push_back(output_Q);

            regex output_regex_QN("\\.QN\\(([^\\)]*)\\)");
            smatch output_match_QN;
            string output_QN = "";
            if (regex_search(line, output_match_QN, output_regex_QN))
                output_QN = output_match_QN[1].str();

            if (output_QN.size()==0)
                output_QN = "**";
            gate.outputs.push_back(output_QN);

            regex input_regex_CLK("\\.CLK\\(([^\\)]*)\\)");
            smatch input_match_CLK;
            if (regex_search(line, input_match_CLK, input_regex_CLK))
                gate.inputs.push_back(input_match_CLK[1].str());

            if (gate.inputs.size() == 0)
                continue;

            regex input_regex("\\.DIN\\(([^\\)]*)\\)");
            smatch input_match;
            if (regex_search(line, input_match, input_regex))
                gate.inputs.push_back(input_match[1].str());

            if (gate.inputs.size() == 1)
                continue;

            regex input_regex_EB("\\.EB\\(([^\\)]*)\\)");
            smatch input_match_EB;
            if (regex_search(line, input_match_EB, input_regex_EB))
                gate.inputs.push_back(input_match_EB[1].str());

            // if (gate.inputs.size() == 2)
            //     gate.inputs.push_back("**");
        }
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

        // if(regex_search(line,match,ignorePattern))
        // {
        //     continue;
        // }
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

    string file_name = "dump1";
    // string file_name = "c2670_T093";
    // string file_name = "c2670_T093";
    string input_file = input_folder + file_name + ".v";
    string output_file = output_folder + file_name + ".txt";

    parse_verilog_file(input_file);
    generate_text_file(output_file);
    // display(output_file);

    return 0;
}
